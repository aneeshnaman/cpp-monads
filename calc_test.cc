// Tests for calculator

#include <string>
#include <vector>

#include "calc.h"

#define CATCH_CONFIG_MAIN
#include "catch.h"

namespace calc {

TEST_CASE("Calc test", "Calc") {
  SECTION("Plus") {
    State input = {Item(ItemType::kPlus), Item(2), Item(4)};
    Item result = monads::eval_plan(calc(), input);
    Item expected = Item(6);
    REQUIRE(result.type == expected.type);
    REQUIRE(result.num == expected.num);
  }

  SECTION("Minus") {
    State input = {Item(ItemType::kMinus), Item(2), Item(4)};
    Item result = monads::eval_plan(calc(), input);
    Item expected = Item(-2);
    REQUIRE(result.type == expected.type);
    REQUIRE(result.num == expected.num);
  }

  SECTION("PlusMinus") {
    State input = {
      Item(ItemType::kPlus), 
      Item(ItemType::kMinus), 
      Item(4), 
      Item(8), 
      Item(3)
    };
    Item result = monads::eval_plan(calc(), input);
    Item expected = Item(-1);
    REQUIRE(result.type == expected.type);
    REQUIRE(result.num == expected.num);
  }
}

}  // namespace calc
