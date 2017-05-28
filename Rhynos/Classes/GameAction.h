#ifndef _GAME_ACTION_H_
#define _GAME_ACTION_H_

#include "cereal/types/unordered_set.hpp"
#include "cocos2d.h"

#include <unordered_set>

struct GameAction {
  enum class Type { CONNECTION_ESTABLISHED, GAME_START, KEY_PRESSED };

  static GameAction gameStartAction(const int player_id,
                                    const int other_player_id) {
    return GameAction(Type::GAME_START, player_id, other_player_id);
  }

  static GameAction connectionEstablishedAction(const int player_id) {
    return GameAction(Type::CONNECTION_ESTABLISHED, player_id);
  }

  static GameAction keyPressedAction(
      const std::unordered_set<cocos2d::EventKeyboard::KeyCode> &keys_pressed) {
    return GameAction(Type::KEY_PRESSED, keys_pressed);
  }

  GameAction() {}

  template <class Archive>
  void serialize(Archive &ar) {
    ar(type, keys_pressed);
  }

  Type type;
  std::unordered_set<cocos2d::EventKeyboard::KeyCode> keys_pressed;
  int player_id;
  int other_player_id;

 private:
  GameAction(const Type type) : type(type) {}
  GameAction(
      const Type type,
      const std::unordered_set<cocos2d::EventKeyboard::KeyCode> &keys_pressed)
      : type(type), keys_pressed(keys_pressed) {}
  GameAction(const Type type, const int player_id,
             const int other_player_id = -1)
      : type(type), player_id(player_id), other_player_id(other_player_id) {}
};

#endif  // _GAME_ACTION_H_
