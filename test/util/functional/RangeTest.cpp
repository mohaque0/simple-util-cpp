#include "catch.hpp"

#include "util/functional/Range.hpp"
#include <vector>


using namespace Util::Functional;

TEST_CASE("functional/Range") {

	std::vector<int> data = {1,2,3,4,5};

	SECTION("Sum integers") {
		int sum = iterate(data.begin(), data.end())
				.foldl([] (int& sum, int x) {
					return sum + x;
				}, 0);
		REQUIRE(sum == 15);
	}

	SECTION("Chain functions") {
		std::vector<int> acc;
		iterate(data)
				.map([] (int x) { return x + 1; })
				.filter([] (int x) { return x > 3; })
				.foldl(
					[] (std::vector<int>* acc, int x) -> std::vector<int>* {
						acc->push_back(x);
						return acc;
					},
					&acc
				);
		REQUIRE(acc == std::vector<int>{4,5,6});
	}
}
