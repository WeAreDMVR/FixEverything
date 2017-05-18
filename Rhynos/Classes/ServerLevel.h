#ifndef _SERVER_LEVEL_H_
#define _SERVER_LEVEL_H_

#include "Level.h"
#include "NetworkingConstants.h"
#include "Server.h"

#include "asio.hpp"

#include <string>

class ServerLevel : public Level {
 public:
  static ServerLevel* createWithMap(const std::string& tmxFile);

  ServerLevel(const std::string& tmxFile)
      : Level(tmxFile, false /* displayObjects */),
        io_service_(),
        server_(io_service_, NetworkingConstants::PORT) {}

  virtual bool init() override;

  void update(float dt) override;

 protected:
  void handleInput() override;

 private:
  asio::io_service io_service_;
  Server server_;
};

#endif  // _SERVER_LEVEL_H_
