#include <string>
#include <tuple>

#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "tuple.h"

namespace tuple {

TEST_CASE("TupleDef", "TupleDef") {
  SECTION("TupleDef") {
    static_assert(std::is_same<std::tuple<int, int>, TupleDef<int, 2>::type>::value, "FAIL");
  }
}

}  // namespace tuple
