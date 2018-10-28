#include "catch.hpp"

#include "util/Option.hpp"

using namespace Util;

TEST_CASE("Option") {

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

	SECTION("Default Constructor") {
		Option<Data> opt;
		bool flag = false;

		REQUIRE(!opt.isSome());
		REQUIRE(opt.isNone());

		opt.isSome([&] (Data& value) { flag = true; });
		REQUIRE(!flag);

		opt.isNone([&] { flag = true; });
		REQUIRE(flag);
	}

	SECTION("Construct with Data") {
		Option<Data> opt(Data(1,2,3));
		bool flag = false;

		REQUIRE(opt.isSome());
		REQUIRE(!opt.isNone());

		opt.isNone([&] { flag = true; });
		REQUIRE(!flag);

		opt.isSome([&] (Data& value) {
			flag = true;
			REQUIRE(value == Data(1,2,3));
		});
		REQUIRE(flag);
	}
}
