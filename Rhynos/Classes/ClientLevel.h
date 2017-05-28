#ifndef _CLIENT_LEVEL_H_
#define _CLIENT_LEVEL_H_

#include "Client.h"
#include "Level.h"
#include "Player.h"

#include <string>

class ClientLevel : public Level {
 public:
  using Level::Level;

  static ClientLevel* createNetworkedWithMap(const std::string& tmxFile,
                                             const std::string& host);

 protected:
  void handleInput() override;
  void addPlayer(const std::string& className, Player* player) override;

 private:
  Client _client;
  int _localPlayerId;
  int _otherPlayerId;
};

#endif  // _CLIENT_LEVEL_H_
