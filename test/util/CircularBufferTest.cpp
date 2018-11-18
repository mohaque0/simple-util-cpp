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

		REQUIRE(buf.pop_front().isError());
		REQUIRE(buf.pop_front().error() == CircularBufferError::NoMoreElements);
	}

	SECTION("Test Capacity 1") {
		CircularBuffer<char> buf(1);

		REQUIRE(buf.size() == 0);
		REQUIRE(buf.capacity() == 1);

		REQUIRE(buf.emplace_back('a').isOk());
		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == 'a');

		REQUIRE(buf.emplace_back('b').isError());
		REQUIRE(buf.emplace_back('b').error() == CircularBufferError::OutOfSpace);
		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == 'a');

		REQUIRE(buf.pop_front().isOk());
		REQUIRE(buf.size() == 0);
		REQUIRE(buf.capacity() == 1);
	}

	SECTION("Test Struct Type") {
		CircularBuffer<Data> buf(3);

		// Pushed entry is readable.
		REQUIRE(buf.emplace_back(1,2,3).isOk());
		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == Data(1,2,3));

		// Check data is not corrupted.
		REQUIRE(buf.emplace_back(2,3,4).isOk());
		REQUIRE(buf.size() == 2);
		REQUIRE(buf.peek_front() == Data(1,2,3));

		REQUIRE(buf.pop_front().isOk());
		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == Data(2,3,4));

		REQUIRE(buf.emplace_back(3,4,5).isOk());
		REQUIRE(buf.emplace_back(4,5,6).isOk());
		REQUIRE(buf.size() == 3);
		REQUIRE(buf.peek_front() == Data(2,3,4));

		REQUIRE(buf.pop_front().isOk());
		REQUIRE(buf.size() == 2);
		REQUIRE(buf.peek_front() == Data(3,4,5));

		REQUIRE(buf.pop_front().isOk());
		REQUIRE(buf.size() == 1);
		REQUIRE(buf.peek_front() == Data(4,5,6));

		REQUIRE(buf.pop_front().isOk());
		REQUIRE(buf.size() == 0);

		REQUIRE(buf.pop_front().isError());
		REQUIRE(buf.pop_front().error() == CircularBufferError::NoMoreElements);
	}

}
