#include "ClientLevel.h"

#include "cocos2d.h"

#include <stdexcept>
#include <string>
#include <vector>

using namespace cocos2d;

using std::runtime_error;
using std::string;
using std::to_string;
using std::vector;

ClientLevel* ClientLevel::createNetworkedWithMap(const string& tmxFile,
                                                 const string& host) {
  ClientLevel* ret = new ClientLevel(tmxFile);
  GameAction game_action;
  CCLOG("connecting to server");
  asio::error_code ec = ret->_client.connect(host);
  if (ec) {
    CCLOG(ec.message().c_str());
    throw runtime_error("Could not connect to " + host);
  }
  do {
    ret->_client.read(game_action);
  } while (game_action.type != GameAction::Type::CONNECTION_ESTABLISHED);
  CCLOG("Connected as player %d", game_action.player_id);
  ret->_localPlayerId = game_action.player_id;
  CCLOG("Waiting for another player");
  do {
    ret->_client.read(game_action);
  } while (game_action.type != GameAction::Type::GAME_START);
  ret->_otherPlayerId = game_action.player_id == ret->_localPlayerId
                            ? game_action.other_player_id
                            : game_action.player_id;
  CCLOG("Two players have connected");
  return ret;
}

void ClientLevel::handleInput() {
  vector<EventKeyboard::KeyCode> keys_pressed;
  // Arrows
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
    CCLOG("right");
    keys_pressed.push_back(EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
    CCLOG("left");
    keys_pressed.push_back(EventKeyboard::KeyCode::KEY_LEFT_ARROW);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE)) {
    CCLOG("up");
    keys_pressed.push_back(EventKeyboard::KeyCode::KEY_SPACE);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_1)) {
    CCLOG("1");
    keys_pressed.push_back(EventKeyboard::KeyCode::KEY_1);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_2)) {
    CCLOG("2");
    keys_pressed.push_back(EventKeyboard::KeyCode::KEY_2);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_3)) {
    CCLOG("3");
    keys_pressed.push_back(EventKeyboard::KeyCode::KEY_3);
  }

  if (!keys_pressed.empty()) {
    _client.write(keys_pressed);
  }
}

void ClientLevel::addPlayer(const std::string& className, Player* player) {
  if (this->_localPlayer == nullptr) {
    this->_players[to_string(_localPlayerId)] = player;
    this->_localPlayer = player;
  } else {
    this->_players[to_string(_otherPlayerId)] = player;
  }
}

void ClientLevel::extraUpdates() {
  vector<GameAction> game_actions;
  if (_client.available()) {
    _client.read(game_actions);
    for (const GameAction& action : game_actions) {
      if (action.type == GameAction::Type::KEY_PRESSED) {
        for (const EventKeyboard::KeyCode key : action.keys_pressed) {
          _players[to_string(action.player_id)]->applyInput(key);
        }
      }
    }
  }
}
