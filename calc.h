#include <vector>

#include "monads.h"

namespace calc {

enum class ItemType { kPlus, kMinus, kNum };

struct Item {
  ItemType type;
  int num;
  Item(int n) : type(ItemType::kNum), num(n) {}
  Item(ItemType type) : type(type), num(-1) {}
};

using State = std::vector<Item>;
using PlanResult = monads::PlanResult<Item, State>;
using Plan = monads::Plan<Item, State>;

Plan calc();

}  // namespace calc
