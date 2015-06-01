#include <string>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "constraints.h"
#include "list.h"

using std::pair;
using std::string;
using std::vector;

namespace constraints {

TEST_CASE("PairWithSum", "PairWithSum") {
  SECTION("PairWithSum") {
    vector<int> in = {1,2,3,4};
    vector<pair<int, int>> result = eval_plan(pair_with_sum(3), in);
    REQUIRE(result.size() == 2);
    REQUIRE(result[0].first == 1);
    REQUIRE(result[0].second == 2);
    REQUIRE(result[1].first == 2);
    REQUIRE(result[1].second == 1);
  }

  SECTION("PairWithSum_no_solution") {
    vector<int> in = {1,2,3,4};
    vector<pair<int, int>> result = eval_plan(pair_with_sum(10), in);
    REQUIRE(result.size() == 0);
  }
}

TEST_CASE("VectorWithSum", "VectorWithSum") {
  SECTION("VectorWithSum") {
    vector<int> in = {1,2,3,4,5,6,7,8};
    vector<std::vector<int>> result = eval_plan(vector_with_sum(3, 6), in);
    REQUIRE(result.size() == 6);
    for (const auto& r : result) {
      REQUIRE(sum(r) == 6);
      LOG(list::to_string(r).c_str());
    }
  }
}

}  // namespace constraints
