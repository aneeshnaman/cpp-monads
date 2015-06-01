#include <string>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "constraints.h"

using std::pair;
using std::string;
using std::vector;

namespace constraints {

TEST_CASE("PairWithSum", "PairWithSum") {
  SECTION("PairWithSum") {
    vector<int> in = {1,2,3,4};
    vector<pair<int, int>> expected = {{1,2}};
    vector<pair<int, int>> result = constraints::eval_plan(pair_with_sum(3), in);
    REQUIRE(result.size() == 2);
    REQUIRE(result[0].first == 1);
    REQUIRE(result[0].second == 2);
    REQUIRE(result[1].first == 2);
    REQUIRE(result[1].second == 1);
  }
}

}  // namespace constraints
