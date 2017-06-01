#include "Client.h"

#include <cocos2d.h>
#include <ui/UITextField.h>
#include "asio.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/vector.hpp"

#include "GameAction.h"
#include "Level.h"
#include "NetworkingConstants.h"

#include <chrono>
#include <cstring>
#include <future>
#include <sstream>
#include <string>

using namespace asio;
using namespace cocos2d;

using std::string;
using std::to_string;

bool Client::is_open() const { return conn_->socket().is_open(); }

error_code Client::connect(const string& host) {
  tcp::resolver resolver(conn_->socket().get_io_service());
  error_code ec;
  asio::connect(conn_->socket(),
                resolver.resolve({host, to_string(NetworkingConstants::PORT)}),
                ec);
  return ec;
}

bool Client::available() {
  tcp::socket::bytes_readable command(true);
  conn_->socket().io_control(command);
  return command.get() > 0;
}
