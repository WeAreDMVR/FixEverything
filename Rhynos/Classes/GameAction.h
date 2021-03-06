#ifndef _GAME_ACTION_H_
#define _GAME_ACTION_H_

#include "cereal/types/vector.hpp"
#include "cocos2d.h"

#include <vector>

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
      const std::vector<cocos2d::EventKeyboard::KeyCode> &keys_pressed,
      const int player_id) {
    return GameAction(Type::KEY_PRESSED, keys_pressed, player_id);
  }

  GameAction() {}

  template <class Archive>
  void serialize(Archive &ar) {
    ar(type, keys_pressed, player_id, other_player_id);
  }

  Type type;
  std::vector<cocos2d::EventKeyboard::KeyCode> keys_pressed;
  int player_id;
  int other_player_id;

 private:
  GameAction(const Type type) : type(type) {}
  GameAction(const Type type,
             const std::vector<cocos2d::EventKeyboard::KeyCode> &keys_pressed,
             const int player_id)
      : type(type), keys_pressed(keys_pressed), player_id(player_id) {}
  GameAction(const Type type, const int player_id,
             const int other_player_id = -1)
      : type(type), player_id(player_id), other_player_id(other_player_id) {}
};

#endif  // _GAME_ACTION_H_
