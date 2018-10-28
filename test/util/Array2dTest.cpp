#include "catch.hpp"

#include "util/Array2d.hpp"

#include <vector>

using namespace Util;


template<typename T, typename F>
void forEachCell(Array2d<T>& a, F f) {
	for (int i = 0; i < a.size0(); i++) {
		for (int j = 0; j < a.size1(); j++) {
				f(a,i,j);
		}
	}
}

TEST_CASE("Array2d") {

	struct Data {
		int a;
		int b;

	public:
		Data(int a, int b) : a(a), b(b) {}
		bool operator==(const Data& other) const {
			return a == other.a && b == other.b;
		}
	};

	SECTION("Constructor Empty") {
		Array2d<Data> a;

		REQUIRE(a.size0() == 0);
		REQUIRE(a.size1() == 0);
	}

	SECTION("Constructor Sized") {
		Array2d<int> a(1,2);

		REQUIRE(a.size0() == 1);
		REQUIRE(a.size1() == 2);
	}

	SECTION("Emplace every cell") {
		Array2d<Data> a;

		a.resizeAndEmplaceNewEntries(1,2, 1,2);

		REQUIRE(a.size0() == 1);
		REQUIRE(a.size1() == 2);

		forEachCell(a, [] (auto& a, int i, int j) {
			REQUIRE(a.at(i,j) == Data(1,2));
		});

		forEachCell(a, [] (auto& a, int i, int j) {
			a.emplace(i, j, i+1, j+1);
		});

		forEachCell(a, [] (auto& a, int i, int j) {
			REQUIRE(a.at(i,j) == Data(i+1,j+1));
		});
	}

	SECTION("Check new entries after resize") {
		Array2d<Data> a;

		a.resizeAndEmplaceNewEntries(1,2, 1,2);

		REQUIRE(a.size0() == 1);
		REQUIRE(a.size1() == 2);

		forEachCell(a, [] (auto& a, int i, int j) {
			a.emplace(i, j, i, j);
		});

		a.resizeAndEmplaceNewEntries(2,3, Data(-1,-2));

		forEachCell(a, [] (auto& a, int i, int j) {
			if (i < 1 && j < 2) {
				REQUIRE(a.at(i,j) == Data(i,j));
			} else {
				REQUIRE(a.at(i,j) == Data(-1,-2));
			}
		});
	}

}

