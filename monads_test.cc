// Tests for monad functions

#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "monads.h"

using std::pair;
using std::vector;

TEST_CASE("SelectFirst", "SelectFirst") {
  SECTION("simple test") {
    vector<int> in = {1, 2, 3};
    vector<int> expected_new_state = {2, 3};
    auto result = monads::run_plan<int, vector<int>>(
        monads::select_first<int>, in);
    REQUIRE(result.first == 1);
    REQUIRE(result.second == expected_new_state);
  }
}

TEST_CASE("SelectTwo", "SelectTwo") {
  SECTION("simple test") {
    vector<int> in = {1, 2, 3, 4};
    pair<int, int> expected_result = {1, 2};
    vector<int> expected_new_state = {3, 4};
    auto result = monads::run_plan<pair<int, int>, vector<int>>(
        monads::select_two<int>, in);
    REQUIRE(result.first == expected_result);
    REQUIRE(result.second == expected_new_state);
  }
}
