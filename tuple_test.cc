#include <string>
#include <tuple>

#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "tuple.h"

namespace tuple {

TEST_CASE("TupleDef", "TupleDef") {
  SECTION("TupleDef") {
    static_assert(std::is_same<std::tuple<int, int>, TupleDef<int, 2>::type>::value, "FAIL");
    static_assert(std::is_same<std::tuple<>, TupleDef<int, 0>::type>::value, "FAIL");
  }
}

TEST_CASE("Sum", "Sum") {
  SECTION("Sum") {
    std::tuple<int, int, int> in = std::make_tuple(1, 2, 3);
    REQUIRE((tuple::sum<int, 3>(in)) == 6);
  }
}

TEST_CASE("ToString", "ToString") {
  SECTION("ToString") {
    std::tuple<int, int, int> in = std::make_tuple(1, 2, 3);
    REQUIRE((tuple::to_string<int, 3>(in)) == "1, 2, 3, ");
  }
}

}  // namespace tuple
