#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "NetworkingConstants.h"

#include "asio.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/vector.hpp"

#include <functional>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using asio::ip::tcp;

static int next_connection_id = 1;

class Connection {
 public:
  Connection(asio::io_service& io_service)
      : id_(next_connection_id++), socket_(io_service) {}

  tcp::socket& socket() { return socket_; }
  int id() { return id_; }

  template <typename T>
  void write(const T& t) {
    // Serialize the data first so we know how large it is.
    std::ostringstream archive_stream;
    {
      cereal::PortableBinaryOutputArchive archive(archive_stream);
      archive(t);
    }
    outbound_data_ = archive_stream.str();

    // Format the header.
    std::ostringstream header_stream;
    header_stream << std::setw(NetworkingConstants::HEADER_LENGTH) << std::hex
                  << outbound_data_.size();
    if (!header_stream ||
        header_stream.str().size() != NetworkingConstants::HEADER_LENGTH) {
      // Something went wrong, inform the caller.
      CCLOG("Networking error");
      return;
    }
    outbound_header_ = header_stream.str();

    // Write the serialized data to the socket. We use "gather-write" to send
    // both the header and the data in a single write operation.
    std::vector<asio::const_buffer> buffers;
    buffers.push_back(asio::buffer(outbound_header_));
    buffers.push_back(asio::buffer(outbound_data_));

    asio::error_code ec;
    asio::write(socket_, buffers, ec);
    if (ec) {
      CCLOG(ec.message().c_str());
    }
  }

  template <typename T>
  void read(T& t) {
    // Issue a read operation to read exactly the number of bytes in a header.
    asio::error_code ec;
    asio::read(socket_, asio::buffer(inbound_header_), ec);
    handle_read_header(ec, t);
  }

  template <typename T>
  void handle_read_header(const asio::error_code& e, T& t) {
    if (e) {
      CCLOG(e.message().c_str());
    } else {
      // Determine the length of the serialized data.
      std::istringstream is(
          std::string(inbound_header_, NetworkingConstants::HEADER_LENGTH));
      std::size_t inbound_data_size = 0;
      if (!(is >> std::hex >> inbound_data_size)) {
        // Header doesn't seem to be valid. Inform the caller.
        CCLOG("Invalid message header");
        return;
      }

      // Start an asynchronous call to receive the data.
      inbound_data_.resize(inbound_data_size);
      asio::error_code ec;
      asio::read(socket_, asio::buffer(inbound_data_), ec);
      handle_read_data(ec, t);
    }
  }

  template <typename T>
  void handle_read_data(const asio::error_code& e, T& t) {
    if (e) {
      CCLOG(e.message().c_str());
    } else {
      // Extract the data structure from the data just received.
      std::string archive_data(&inbound_data_[0], inbound_data_.size());
      std::istringstream archive_stream(archive_data);
      cereal::PortableBinaryInputArchive archive(archive_stream);
      archive(t);
    }
  }

 private:
  // The connection id
  int id_;

  // The underlying socket.
  tcp::socket socket_;

  // Holds an outbound header.
  std::string outbound_header_;

  // Holds the outbound data.
  std::string outbound_data_;

  // Holds an inbound header.
  char inbound_header_[NetworkingConstants::HEADER_LENGTH];

  // Holds the inbound data.
  std::vector<char> inbound_data_;
};

typedef std::shared_ptr<Connection> connection_ptr;

#endif  // _CONNECTION_H_
