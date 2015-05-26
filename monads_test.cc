// Tests for monad functions

#include <string>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "monads.h"

using std::pair;
using std::string;
using std::vector;

TEST_CASE("Concat test", "Concat") {
  SECTION("empty input") {
    vector<vector<int>> in = {};
    vector<int> out = {};
    REQUIRE(monads::concat_all(in) == out);
  }

  SECTION("simple test") {
    vector<vector<int>> in = {{1, 2}, {3, 4}};
    vector<int> out = {1, 2, 3, 4};
    REQUIRE(monads::concat_all(in) == out);
  }
}

TEST_CASE("ForEach", "ForEach") {
  SECTION("empty input") {
    vector<int> in = {};
    vector<int> expected = {};
    REQUIRE(monads::for_each(in, [](int x) {
      return vector<int>{x*x};
    }) == expected);
  }

  SECTION("simple test") {
    vector<int> in = {1,2,3};
    vector<int> expected = {1,4,9};
    REQUIRE(monads::for_each(in, [](int x) {
      return vector<int>{x*x};
    }) == expected);
  }

  SECTION("chess board") {
    vector<string> rows = {"a", "b"};
    vector<string> cols = {"1", "2"};
    vector<string> expected = {"a1", "a2", "b1", "b2"};
    REQUIRE(monads::for_each(rows, [&cols](string c1) {
      return monads::for_each(cols, [&c1](string c2) {
        return vector<string>{c1 + c2};
      });
    }) == expected);
  }
}

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
