#include "Server.h"

#include "GameAction.h"

#include "asio.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/vector.hpp"
#include "cocos2d.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

using namespace asio;
using namespace cereal;
using namespace cocos2d;

using std::error_code;
using std::make_pair;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::vector;

void session(connection_ptr conn, Server* server) {
  while (true) {
    vector<EventKeyboard::KeyCode> keys_pressed;
    conn->read(keys_pressed);

    for (const EventKeyboard::KeyCode code : keys_pressed) {
      switch (code) {
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
          CCLOG("right");
          break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
          CCLOG("left");
          break;
        case EventKeyboard::KeyCode::KEY_SPACE:
          CCLOG("up");
          break;
        case EventKeyboard::KeyCode::KEY_1:
          CCLOG("1");
          break;
        case EventKeyboard::KeyCode::KEY_2:
          CCLOG("2");
          break;
        case EventKeyboard::KeyCode::KEY_3:
          CCLOG("3");
          break;
        default:
          break;
      }
    }
    server->mut.lock();
    server->game_actions.push_back(
        GameAction::keyPressedAction(keys_pressed, conn->id()));
    server->mut.unlock();
  }
}

void Server::do_accept(Server* server) {
  static const int NUM_PLAYERS = 2;
  for (int i = 0; i < NUM_PLAYERS; i++) {
    connection_ptr new_conn(new Connection(server->acceptor.get_io_service()));
    error_code ec;
    server->acceptor.accept(new_conn->socket(), ec);
    CCLOG("Accepted new connection");
    if (ec) {
      CCLOG(ec.message().c_str());
      i--;
    } else {
      server->connections.push_back(new_conn);
      CCLOG("Sending identification to client %d", new_conn->id());
      new_conn->write(GameAction::connectionEstablishedAction(new_conn->id()));
      CCLOG("Finished sending identification to client %d", new_conn->id());
      std::thread(session, new_conn, server).detach();
    }
  }

  int player1_id = server->connections[0]->id();
  int player2_id = server->connections[1]->id();
  for (auto& connection : server->connections) {
    connection->write(GameAction::gameStartAction(player1_id, player2_id));
  }
  Server::do_game_loop(server);
}

void Server::do_game_loop(Server* server) {
  while (true) {
    vector<GameAction> actions;
    server->mut.lock();
    actions.swap(server->game_actions);
    server->mut.unlock();
    if (!actions.empty()) {
      for (auto connection : server->connections) {
        connection->write(actions);
      }
    }
  }
}

bool ServerScene::init() {
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }
  const auto& visibleSize = Director::getInstance()->getVisibleSize();
  const Vec2& origin = Director::getInstance()->getVisibleOrigin();

  // create and initialize a label
  auto label =
      Label::createWithTTF("Server is running", "fonts/Marker Felt.ttf", 24);

  // position the label on the center of the screen
  label->setPosition(
      Vec2(origin.x + visibleSize.width / 2,
           origin.y + visibleSize.height - label->getContentSize().height));

  // add the label as a child to this layer
  this->addChild(label, 1);

  return true;
}
