#include "catch.hpp"

#include "util/Array3d.hpp"

#include <vector>

using namespace Util;

template<typename T, typename F>
void forEachCell(Array3d<T>& a, F f) {
	for (int i = 0; i < a.size0(); i++) {
		for (int j = 0; j < a.size1(); j++) {
			for (int k = 0; k < a.size2(); k++) {
				f(a,i,j,k);
			}
		}
	}
}

TEST_CASE("Array3d") {

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

	SECTION("Constructor Empty") {
		Array3d<Data> a;

		REQUIRE(a.size0() == 0);
		REQUIRE(a.size1() == 0);
		REQUIRE(a.size2() == 0);
	}

	SECTION("Constructor Sized") {
		Array3d<int> a(1,2,3);

		REQUIRE(a.size0() == 1);
		REQUIRE(a.size1() == 2);
		REQUIRE(a.size2() == 3);
	}

	SECTION("Set every cell") {
		Array3d<Data> a;

		a.resizeAndSetNewEntries(1,2,3,Data(1,2,3));

		REQUIRE(a.size0() == 1);
		REQUIRE(a.size1() == 2);
		REQUIRE(a.size2() == 3);

		forEachCell(a, [] (auto& a, int i, int j, int k) {
			REQUIRE(a.at(i,j,k) == Data(1,2,3));
		});

		forEachCell(a, [] (auto& a, int i, int j, int k) {
			a.set(Data(i,j,k), i, j, k);
		});

		forEachCell(a, [] (auto& a, int i, int j, int k) {
			REQUIRE(a.at(i,j,k) == Data(i,j,k));
		});
	}

	SECTION("setAll") {
		Array3d<Data> a;
		const Data before = Data(1,2,3);
		const Data after = Data(-1,-2,-3);

		a.resizeAndSetNewEntries(1,2,3,before);

		REQUIRE(a.size0() == 1);
		REQUIRE(a.size1() == 2);
		REQUIRE(a.size2() == 3);

		//forEachCell(a, [&] (auto& a, int i, int j, int k) {
		//	REQUIRE(a.at(i,j,k) == before);
		//});

		a.setAll(after);

		forEachCell(a, [&] (auto& a, int i, int j, int k) {
			REQUIRE(a.at(i,j,k) == after);
		});
	}

	SECTION("Emplace every cell") {
		Array3d<Data> a;

		a.resizeAndSetNewEntries(1,2,3,Data(1,2,3));

		REQUIRE(a.size0() == 1);
		REQUIRE(a.size1() == 2);
		REQUIRE(a.size2() == 3);

		forEachCell(a, [] (auto& a, int i, int j, int k) {
			REQUIRE(a.at(i,j,k) == Data(1,2,3));
		});

		forEachCell(a, [] (auto& a, int i, int j, int k) {
			a.emplace(i, j, k, i+1, j+1, k+1);
		});

		forEachCell(a, [] (auto& a, int i, int j, int k) {
			REQUIRE(a.at(i,j,k) == Data(i+1,j+1,k+1));
		});
	}

	SECTION("Check new entries after resize") {
		Array3d<Data> a;

		a.resizeAndSetNewEntries(1,2,3,Data(1,2,3));

		REQUIRE(a.size0() == 1);
		REQUIRE(a.size1() == 2);
		REQUIRE(a.size2() == 3);

		forEachCell(a, [] (auto& a, int i, int j, int k) {
			a.set(Data(i,j,k), i, j, k);
		});

		a.resizeAndSetNewEntries(2,3,4, Data(-1,-2,-3));

		forEachCell(a, [] (auto& a, int i, int j, int k) {
			if (i < 1 && j < 2 && k < 3) {
				REQUIRE(a.at(i,j,k) == Data(i,j,k));
			} else {
				REQUIRE(a.at(i,j,k) == Data(-1,-2,-3));
			}
		});
	}

}
