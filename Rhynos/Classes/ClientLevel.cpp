#include "ClientLevel.h"

#include "cocos2d.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "SimpleAudioEngine.h"

using namespace cocos2d;

using std::endl;
using std::ostringstream;
using std::runtime_error;
using std::string;
using std::to_string;
using std::vector;

ClientLevel* ClientLevel::createWithMap(const string& tmxFile) {
  return new ClientLevel(tmxFile);
}

ClientLevel* ClientLevel::connect(ClientLevel* client_level,
                                  const string& host) {
  GameAction game_action;
  CCLOG("connecting to server");
  asio::error_code ec = client_level->_client.connect(host);
  if (ec) {
    CCLOG(ec.message().c_str());
    throw runtime_error("Could not connect to " + host);
  }
  do {
    client_level->_client.read(game_action);
  } while (game_action.type != GameAction::Type::CONNECTION_ESTABLISHED);
  CCLOG("Connected as player %d", game_action.player_id);
  client_level->_localPlayerId = game_action.player_id;
  CCLOG("Waiting for another player");
  do {
    client_level->_client.read(game_action);
  } while (game_action.type != GameAction::Type::GAME_START);
  client_level->_otherPlayerId =
      game_action.player_id == client_level->_localPlayerId
          ? game_action.other_player_id
          : game_action.player_id;
  CCLOG("Two players have connected");
  return client_level;
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
          // apply layer switching
          switch (key) {
            case EventKeyboard::KeyCode::KEY_1:
              setActiveFGandBG(1);
              break;
            case EventKeyboard::KeyCode::KEY_2:
              setActiveFGandBG(2);
              break;
            case EventKeyboard::KeyCode::KEY_3:
              setActiveFGandBG(3);
              break;
            default:
              break;
          }
        }
      }
    }
  }
}

static string msg;

bool ClientLevel::didWin(float x, float y) {
  for (auto& player : this->_players) {
    if (player.second->checkWin(Point(2700, 500))) {
      ostringstream oss;
      oss << "Player " << player.first << " wins! " << endl
          << "Press Enter to go to the main menu.";
      msg = oss.str();
      this->over = true;
    }
  }

  if (this->over) {
    CCLOG(msg);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/fanfare.wav");
    auto label = Label::createWithTTF(msg, "fonts/Marker Felt.ttf", 48);
    // position the label on the center of the screen
    label->setPosition(Vec2(x, y));
    // Keeep the message in the middle of the screen
    label->setName("winning");
    this->addChild(label, 1, "winning");
  }
  return this->over;
}
