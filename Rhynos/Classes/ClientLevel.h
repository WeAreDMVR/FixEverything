#ifndef _CLIENT_LEVEL_H_
#define _CLIENT_LEVEL_H_

#include "Client.h"
#include "Level.h"

#include <string>

class ClientLevel : public Level {
 public:
  using Level::Level;

  static ClientLevel* createNetworkedWithMap(const std::string& tmxFile,
                                             const std::string& host);

 protected:
  void handleInput() override;

 private:
  Client _client;
};

#endif  // _CLIENT_LEVEL_H_
