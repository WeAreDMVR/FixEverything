#include "ClientLevel.h"

#include "cocos2d.h"

#include <stdexcept>
#include <string>
#include <unordered_set>

using namespace cocos2d;

using std::runtime_error;
using std::string;
using std::to_string;
using std::unordered_set;

ClientLevel* ClientLevel::createNetworkedWithMap(const string& tmxFile,
                                                 const string& host) {
  ClientLevel* ret = new ClientLevel(tmxFile);
  GameAction game_action;
  if (!ret->_client.connect(host)) {
    throw runtime_error("Could not connect to " + host);
  }
  do {
    ret->_client.read(&game_action);
  } while (game_action.type != GameAction::Type::CONNECTION_ESTABLISHED);
  CCLOG("Connected as player %d", game_action.player_id);
  ret->_localPlayerId = game_action.player_id;
  CCLOG("Waiting for another player");
  do {
    ret->_client.read(&game_action);
  } while (game_action.type != GameAction::Type::GAME_START);
  ret->_otherPlayerId = game_action.player_id == ret->_localPlayerId
                            ? game_action.other_player_id
                            : game_action.player_id;
  CCLOG("Two players have connected");
  return ret;
}

void ClientLevel::handleInput() {
  unordered_set<EventKeyboard::KeyCode> keys_pressed;
  // Arrows
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
    CCLOG("right");
    keys_pressed.insert(EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
    CCLOG("left");
    keys_pressed.insert(EventKeyboard::KeyCode::KEY_LEFT_ARROW);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE)) {
    CCLOG("up");
    keys_pressed.insert(EventKeyboard::KeyCode::KEY_SPACE);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_1)) {
    CCLOG("1");
    keys_pressed.insert(EventKeyboard::KeyCode::KEY_1);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_2)) {
    CCLOG("2");
    keys_pressed.insert(EventKeyboard::KeyCode::KEY_2);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_3)) {
    CCLOG("3");
    keys_pressed.insert(EventKeyboard::KeyCode::KEY_3);
  }

  _client.write(keys_pressed);
}

void ClientLevel::addPlayer(const std::string& className, Player* player) {
  if (this->_localPlayer == nullptr) {
    this->_players[to_string(_localPlayerId)] = player;
    this->_localPlayer = player;
  } else {
    this->_players[to_string(_otherPlayerId)] = player;
  }
}
