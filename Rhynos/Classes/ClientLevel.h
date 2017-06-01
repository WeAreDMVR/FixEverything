#ifndef _CLIENT_LEVEL_H_
#define _CLIENT_LEVEL_H_

#include "Client.h"
#include "Level.h"
#include "Player.h"

#include <string>

class ClientLevel : public Level {
 public:
  using Level::Level;

  static ClientLevel* createWithMap(const std::string& tmxFile);

  static ClientLevel* init(ClientLevel* client_level, const std::string& host);

 protected:
  void handleInput() override;
  void addPlayer(const std::string& className, Player* player) override;
  void extraUpdates() override;

 private:
  Client _client;
  int _localPlayerId;
  int _otherPlayerId;
};

#endif  // _CLIENT_LEVEL_H_
