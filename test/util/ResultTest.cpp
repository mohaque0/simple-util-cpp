#include "catch.hpp"

#include "util/Result.hpp"

using namespace Util;

TEST_CASE("Result") {

	struct Data {
		int a;
		int b;
		int c;

	public:
		Data(int a, int b, int c) : a(a), b(b), c(c) {}
		bool operator==(const Data& other) const {
			return a == other.a && b == other.b && c == other.c;
		}
	};

	SECTION("Test Ok") {
		Data data(1,2,3);
		Result<Data, int> result(result_ok<Data, int>(std::move(data)));

		REQUIRE(bool(result) == true);
		REQUIRE(result.isOk());
		REQUIRE(!result.isError());
		REQUIRE(result.ok() == Data(1,2,3));
	}

	SECTION("Test Error") {
		Result<Data, int> result(result_err<Data, int>(3));

		REQUIRE(!result.isOk());
		REQUIRE(result.isError());
		REQUIRE(result.error() == 3);
	}
}
