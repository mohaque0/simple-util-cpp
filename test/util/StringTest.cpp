#include "catch.hpp"

#include "util/String.hpp"

using namespace Util;

TEST_CASE("String") {

	SECTION("Constructors") {
		{
			String a;
			REQUIRE(a.size() == 0);
			REQUIRE(a == "");
		}
		{
			String a('c');
			REQUIRE(a.size() == 1);
			REQUIRE(a.at(0) == 'c');
			REQUIRE(a == "c");
		}
		{
			String a(10);
			REQUIRE(a.size() == 2);
			REQUIRE(a == "10");
		}
		{
			String a(10l);
			REQUIRE(a.size() == 2);
			REQUIRE(a == "10");
		}
		{
			String a(10ul);
			REQUIRE(a.size() == 2);
			REQUIRE(a == "10");
		}
		{
			String a(1.2f);
			REQUIRE(a.size() == 3);
			REQUIRE(a == "1.2");
		}
		{
			String a(double(1.2));
			REQUIRE(a.size() == 3);
			REQUIRE(a == "1.2");
		}
		{
			String a("Hello, World!");
			REQUIRE(a.size() == 13);
			REQUIRE(a == "Hello, World!");
		}
	}

	SECTION("at") {
		const char* base = "Hello, World!";
		String s(base);

		REQUIRE(s.size() == 13);

		for (size_t i = 0; i < s.size(); i++) {
			REQUIRE(s.at(i) == base[i]);
			REQUIRE(s[i] == base[i]);
		}
	}

	SECTION("find") {
		String s("Hello, Hello, Hello!");

		REQUIRE(s.find("World") == String::NOTFOUND);
		REQUIRE(s.find("Hello") == 0);
		REQUIRE(s.find("Hello", 1) == 7);
		REQUIRE(s.find("Helmet", 1, 3) == 7);
	}

	SECTION("substr") {
		String s("Whirl");
		REQUIRE(s.substr(1,2) == "hi");
	}

	SECTION("addition") {
		String s = String("a") + "b";
		REQUIRE(s == "ab");
		s += "c";
		REQUIRE(s == "abc");
	}

	SECTION("equality") {
		String a0 = "a";
		String a1 = "a";
		String b = "b";

		REQUIRE(a0 == a1);
		REQUIRE(a0 != b);
		REQUIRE(b != a0);
		REQUIRE(b != 'c');
	}
}
