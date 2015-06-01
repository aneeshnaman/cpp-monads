#include "calc.h"

namespace calc {

Plan pop() {
  return [](State state) {
    if (state.empty()) throw "Empty state. Cannot pop";
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
  return monads::mbind(pop(), [](Item front) {
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
