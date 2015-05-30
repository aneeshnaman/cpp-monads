#include "calc.h"

namespace calc {

std::string to_string(State state) {
  std::string s;
  for (auto item : state) { s += " " + std::to_string(item.num) + ", "; }
  return s;
}

Plan popped() {
  return [](State state) {
    return std::make_pair(state.front(), State(state.begin() + 1, state.end()));
  };
}

Plan add() {
  return monads::mbind(calc(), [](Item first) {
    return monads::mbind(calc(), [first](Item second) {
      return monads::mreturn<Item, State>(Item(first.num + second.num));
    });
  });
}

Plan subtract() {
  return monads::mbind(calc(), [](Item first) {
    return monads::mbind(calc(), [first](Item second) {
      return monads::mreturn<Item, State>(Item(first.num - second.num));
    });
  });
}

Plan calc() {
  return monads::mbind(popped(), [](Item front) {
    switch (front.type) {
      case ItemType::kPlus:
        return add();
        break;
      case ItemType::kMinus:
        return subtract();
        break;
      case ItemType::kNum:
        return monads::mreturn<Item, State>(front);
        break;
    }
  });
}

}  // namespace calc
