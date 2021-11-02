#include <catch2/catch.hpp>

#include <edoren/StringView.hpp>

using namespace edoren;

TEST_CASE("StringView::find", "[StringView]") {
    // "水、火、地、風、空"
    StringView elements =
        u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A";

    SECTION("must be able to find any UTF-8 string") {
        size_t location = elements.find(u8"\U00003001\U000098A8\U00003001", 0);  // "、風、"
        REQUIRE(location == 5);
    }
    SECTION("if no start is specified it start from the beginning") {
        size_t location = elements.find(u8"\U00006C34");  // "水"
        REQUIRE(location == 0);
    }
    SECTION("it can start to search from any position") {
        size_t location1 = elements.find(u8"\U00005730", 2);  // "地"
        size_t location2 = elements.find(u8"\U00005730", 4);  // "地"
        REQUIRE(location1 == 4);
        REQUIRE(location1 == location2);
    }
    SECTION("if the string is not found it returns StringView::sInvalidPos") {
        size_t location1 = elements.find(u8"A", 0);
        size_t location2 = elements.find(u8"\U0000706B", 5);  // "火"
        REQUIRE(location1 == StringView::sInvalidPos);
        REQUIRE(location1 == location2);
    }
}

TEST_CASE("StringView::findFirstOf", "[StringView]") {
    // "水、火、地、風、空"
    StringView elements =
        u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A";

    SECTION("must be able to find any of the specified UTF-8 codepoints") {
        size_t location = elements.findFirstOf(u8"\U0000706Bñ\U00005730", 0);  // "火ñ地"
        REQUIRE(location == 2);
    }
    SECTION("if no start is specified it start from the beginning") {
        size_t location = elements.findFirstOf(u8"\U00005730\U0000706B\U00003001");  // "地火、"
        REQUIRE(location == 1);
    }
    SECTION("it can start to search from any position") {
        size_t location1 = elements.findFirstOf(u8"\U00003001\U00005730\U0000706B", 2);            // "、地火"
        size_t location2 = elements.findFirstOf(u8"\U00007A7A\U00003001\U00005730\U0000706B", 4);  // "空、地火"
        REQUIRE(location1 == 2);
        REQUIRE(location2 == 4);
    }
    SECTION("if the any of the UTF-8 characters are not found it returns StringView::sInvalidPos") {
        size_t location1 = elements.findFirstOf("A", 0);
        size_t location2 = elements.findFirstOf(u8"#\U00007A7Añ\U00003001\U000098A8");  // "#空ñ、風"
        REQUIRE(location1 == StringView::sInvalidPos);
        REQUIRE(location2 == 1);
    }
}

TEST_CASE("StringView::findLastOf", "[StringView]") {
    // "水、火、地、風、空"
    StringView elements =
        u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A";

    SECTION("must be able to find any of the specified UTF-8 codepoints") {
        size_t location = elements.findLastOf(u8"\U0000706B\U00003001\U00005730", elements.getSize() - 1);  // "火、地"
        REQUIRE(location == 7);
    }
    SECTION("if no start is specified it start from the end") {
        size_t location = elements.findLastOf(u8"\U000098A8\U00005730\U0000706B");  // "風地火"
        REQUIRE(location == 6);
    }
    SECTION("it can start to search from any position") {
        size_t location1 = elements.findLastOf(u8"\U00003001\U00005730\U0000706B", 2);  // "、地火"
        size_t location2 = elements.findLastOf(u8"\U00007A7A\U00003001\U0000706B", 4);  // "空、火"
        REQUIRE(location1 == 2);
        REQUIRE(location2 == 3);
    }
    SECTION("if the any of the UTF-8 characters are not found it returns StringView::sInvalidPos") {
        size_t location1 = elements.findLastOf("A");
        size_t location2 = elements.findLastOf(u8"#\U00007A7Añ\U00003001\U000098A8");  // "#空ñ、風"
        REQUIRE(location1 == StringView::sInvalidPos);
        REQUIRE(location2 == 8);
    }
}

TEST_CASE("StringView::iterator forward", "[StringView]") {
    StringView elements = u8"\U00006C34\U00003001\U0000706B\U00003001";  // "水、火、"
    auto it0 = elements.begin();

    SECTION("begin() must be able to get the first character of the UTF-8 string") {
        char32_t codePoint0 = it0->get().getCodePoint();
        REQUIRE(codePoint0 == 0x6C34);
    }
    SECTION("must be able to increment the UTF-8 iterator") {
        auto it1 = it0++;
        auto it2 = it0;
        auto it3 = ++it0;
        auto it4 = it0 + 1;
        auto it5 = it0;
        auto codeUnit1 = it1->get();
        auto codeUnit2 = it2->get();
        auto codeUnit3 = it3->get();
        auto codeUnit4 = it4->get();
        auto codeUnit5 = it5->get();
        REQUIRE(codeUnit1.getCodePoint() == 0x6C34);  // 水
        REQUIRE(codeUnit2.getCodePoint() == 0x3001);  // 、
        REQUIRE(codeUnit3.getCodePoint() == 0x706B);  // 火
        REQUIRE(codeUnit4.getCodePoint() == 0x3001);  // 、
        REQUIRE(codeUnit5.getCodePoint() == 0x706B);  // 火
    }
    SECTION("must be able to iterate correctly through the UTF-8 string") {
        size_t count = 0;
        for (const auto& utfIt : elements) {
            char32_t codePoint = utfIt.get().getCodePoint();
            if (count == 0) {
                REQUIRE(codePoint == 0x6C34);
            } else if (count == 1) {
                REQUIRE(codePoint == 0x3001);
            } else if (count == 2) {
                REQUIRE(codePoint == 0x706B);
            } else if (count == 3) {
                REQUIRE(codePoint == 0x3001);
            }
            count++;
        }
    }
}

TEST_CASE("StringView::iterator reverse", "[StringView]") {
    StringView elements = u8"\U00006C34\U00003001\U0000706B\U00003001";  // "水、火、"
    auto it0 = elements.end();

    SECTION("end() should return an invalid interator which will hold always null") {
        char32_t codePoint0 = it0->get().getCodePoint();
        REQUIRE(codePoint0 == 0x0);
    }
    SECTION("must be able to decrement the UTF-8 iterator") {
        auto it1 = --it0;
        auto it2 = it0--;
        auto it3 = it0;
        auto it4 = --it0;
        auto it5 = it0 - 1;
        auto it6 = it0;
        auto codeUnit1 = it1->get();
        auto codeUnit2 = it2->get();
        auto codeUnit3 = it3->get();
        auto codeUnit4 = it4->get();
        auto codeUnit5 = it5->get();
        auto codeUnit6 = it6->get();
        REQUIRE(codeUnit1.getCodePoint() == 0x3001);
        REQUIRE(codeUnit2.getCodePoint() == 0x3001);
        REQUIRE(codeUnit3.getCodePoint() == 0x706B);
        REQUIRE(codeUnit4.getCodePoint() == 0x3001);
        REQUIRE(codeUnit5.getCodePoint() == 0x6C34);
        REQUIRE(codeUnit6.getCodePoint() == 0x3001);
    }
    SECTION("must be able to iterate backwards correctly through the UTF-8 string") {
        size_t count = 0;
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            char32_t codePoint = (*it).get().getCodePoint();
            if (count == 0) {
                REQUIRE(codePoint == 0x3001);
            } else if (count == 1) {
                REQUIRE(codePoint == 0x706B);
            } else if (count == 2) {
                REQUIRE(codePoint == 0x3001);
            } else if (count == 3) {
                REQUIRE(codePoint == 0x6C34);
            }
            count++;
        }
    }
}

TEST_CASE("StringView::operator[]", "[StringView]") {
    StringView faces = u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"

    SECTION("must be able to access any code unit in the UTF-8 StringView") {
        REQUIRE(faces[0] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x80}));  // 😀
        REQUIRE(faces[1] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x83}));  // 😃
        REQUIRE(faces[2] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x84}));  // 😄
        REQUIRE(faces[3] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x81}));  // 😁
        REQUIRE(faces[4] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x86}));  // 😆
    }
}

TEST_CASE("StringView::subString", "[StringView]") {
    // "😀😃😄😁😆"
    StringView faces = u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";
    // "水、火、地、風、空"
    StringView elements(u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A");

    SECTION("Can get any sub-string from the initial string") {
        REQUIRE(faces.subString(1, 3) == u8"\U0001F603\U0001F604\U0001F601");  // "😃😄😁"
    }
    SECTION("If no length is specified it should return a sub string to the end") {
        REQUIRE(elements.subString(4) == u8"\U00005730\U00003001\U000098A8\U00003001\U00007A7A");  // "😃😄😁"
    }
    SECTION("StringView::sInvalidPos") {
        REQUIRE(faces.subString(5, StringView::sInvalidPos) == u8"\U0001F606");  // "😆"
    }
}
