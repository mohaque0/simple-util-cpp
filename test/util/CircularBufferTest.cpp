#include "catch.hpp"

#include "util/CircularBuffer.hpp"

#include <vector>

using namespace Util;

TEST_CASE("Circular Buffer") {

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

	SECTION("Test Capacity 0") {
		CircularBuffer<int> buf(0);

		REQUIRE(buf.size() == 0);
		REQUIRE(buf.capacity() == 0);
	}

	SECTION("Test Capacity 1") {
		CircularBuffer<int> buf(1);

		REQUIRE(buf.size() == 0);
		REQUIRE(buf.capacity() == 1);

		buf.emplace_back(1);

		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == 1);

		buf.pop_front();

		REQUIRE(buf.size() == 0);
		REQUIRE(buf.capacity() == 1);
	}

	SECTION("Test Struct Type") {
		CircularBuffer<Data> buf(3);

		// Pushed entry is readable.
		buf.emplace_back(1,2,3);
		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == Data(1,2,3));

		// Check data is not corrupted.
		buf.emplace_back(2,3,4);
		REQUIRE(buf.size() == 2);
		REQUIRE(buf.peek_front() == Data(1,2,3));

		buf.pop_front();
		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == Data(2,3,4));

		buf.emplace_back(3,4,5);
		buf.emplace_back(4,5,6);
		REQUIRE(buf.size() == 3);
		REQUIRE(buf.peek_front() == Data(2,3,4));

		buf.pop_front();
		REQUIRE(buf.size() == 2);
		REQUIRE(buf.peek_front() == Data(3,4,5));

		buf.pop_front();
		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == Data(4,5,6));

		buf.pop_front();
		REQUIRE(buf.size() == 0);
	}

}
