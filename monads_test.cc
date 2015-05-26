// Tests for monad functions

#include <string>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "monads.h"

using std::vector;
using std::string;

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
