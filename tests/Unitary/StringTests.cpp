#include <catch2/catch.hpp>

#include <edoren/String.hpp>

using namespace edoren;

TEST_CASE("String from other encodings", "[String]") {
    SECTION("from Wide strings") {
        std::wstring smiley = L"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"
        String a = String(smiley.data());
        String b = String(smiley);
        String c = String::FromWide(smiley.begin(), smiley.end());
        REQUIRE(a.getSize() == 5);
        REQUIRE(a == L"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");  // "😀😃😄😁😆"
        REQUIRE(a == b);
        REQUIRE(b == c);
    }
    SECTION("from UTF-8 strings") {
        std::u8string smiley = u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"
        String a = String(smiley.data());
        String b = String(smiley);
        String c = String::FromUtf8(smiley.begin(), smiley.end());
        REQUIRE(a == u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");  // "😀😃😄😁😆"
        REQUIRE(a == b);
        REQUIRE(b == c);
    }
    SECTION("from UTF-16 strings") {
        std::u16string smiley = u"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"
        String a = String(smiley.data());
        String b = String(smiley);
        String c = String::FromUtf16(smiley.begin(), smiley.end());
        REQUIRE(a == u"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");  // "😀😃😄😁😆"
        REQUIRE(a == b);
        REQUIRE(b == c);
    }
    SECTION("from UTF-32 strings") {
        std::u32string smiley = U"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"
        String a = String(smiley.data());
        String b = String(smiley);
        String c = String::FromUtf32(smiley.begin(), smiley.end());
        REQUIRE(a == U"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");  // "😀😃😄😁😆"
        REQUIRE(a == b);
        REQUIRE(b == c);
    }
}

TEST_CASE("String to other encodings", "[String]") {
    String faces = u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";     // "😀😃😄😁😆"
    String elements = u8"\U00006C34\U0000706B\U00005730\U000098A8\U00007A7A";  // "水火地風空"

    SECTION("to Wide strings") {
        std::wstring facesWide = faces.toWide();
        REQUIRE(facesWide == L"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");
    }
    // SECTION("to UTF-8 strings") {
    //     const std::string& facesUtf8 = faces.toUtf8();
    //     const std::string& elementsUtf8 = elements.toUtf8();
    //     REQUIRE(facesUtf8 == u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");
    //     REQUIRE(elementsUtf8 == u8"\U00006C34\U0000706B\U00005730\U000098A8\U00007A7A");
    // }
    SECTION("to UTF-16 strings") {
        std::u16string facesUtf16 = faces.toUtf16();
        std::u16string elementsUtf16 = elements.toUtf16();
        REQUIRE(facesUtf16 == u"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");
        REQUIRE(elementsUtf16 == u"\U00006C34\U0000706B\U00005730\U000098A8\U00007A7A");
    }
    SECTION("to UTF-32 strings") {
        std::u32string facesUtf32 = faces.toUtf32();
        std::u32string elementsUtf32 = elements.toUtf32();
        REQUIRE(facesUtf32 == U"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");
        REQUIRE(elementsUtf32 == U"\U00006C34\U0000706B\U00005730\U000098A8\U00007A7A");
    }
}

TEST_CASE("String::find", "[String]") {
    // "水、火、地、風、空"
    String elements = u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A";

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
    SECTION("if the string is not found it returns String::sInvalidPos") {
        size_t location1 = elements.find(u8"A", 0);
        size_t location2 = elements.find(u8"\U0000706B", 5);  // "火"
        REQUIRE(location1 == String::sInvalidPos);
        REQUIRE(location1 == location2);
    }
}

TEST_CASE("String::findFirstOf", "[String]") {
    // "水、火、地、風、空"
    String elements = u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A";

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
    SECTION("if the any of the UTF-8 characters are not found it returns String::sInvalidPos") {
        size_t location1 = elements.findFirstOf("A", 0);
        size_t location2 = elements.findFirstOf(u8"#\U00007A7Añ\U00003001\U000098A8");  // "#空ñ、風"
        REQUIRE(location1 == String::sInvalidPos);
        REQUIRE(location2 == 1);
    }
}

TEST_CASE("String::findLastOf", "[String]") {
    // "水、火、地、風、空"
    String elements = u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A";

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
    SECTION("if the any of the UTF-8 characters are not found it returns String::sInvalidPos") {
        size_t location1 = elements.findLastOf("A");
        size_t location2 = elements.findLastOf(u8"#\U00007A7Añ\U00003001\U000098A8");  // "#空ñ、風"
        REQUIRE(location1 == String::sInvalidPos);
        REQUIRE(location2 == 8);
    }
}

TEST_CASE("String::replace", "[String]") {
    // "水、火、地、風、空"
    String elements = u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A";

    SECTION("must be able to replace any UTF-8 string") {
        elements.replace(u8"\U00006C34", "Water");  // "水", "Water"
        elements.replace(u8"\U00005730", "Earth");  // "地", "Earth"
        elements.replace(u8"\U00007A7A", "Void");   // "空", "Void"
        // // "Water、火、Earth、風、Void"
        REQUIRE(elements == u8"Water\U00003001\U0000706B\U00003001Earth\U00003001\U000098A8\U00003001Void");
    }
    SECTION("must replace all the ocurrences of the provided string") {
        elements.replace(u8"\U00003001", ", ");  // "、", ", "
        REQUIRE(elements == u8"\U00006C34, \U0000706B, \U00005730, \U000098A8, \U00007A7A");
    }
    SECTION("could replace any element given a range") {
        elements.replace(4, 1, "Earth");
        elements.replace(0, 1, "Water");
        elements.replace(16, 1, "Void");
        // "Water、火、Earth、風、Void"
        REQUIRE(elements == u8"Water\U00003001\U0000706B\U00003001Earth\U00003001\U000098A8\U00003001Void");
    }
    SECTION("could replace the whole string") {
        // "😀😃😄😁😆"
        elements.replace(0, elements.getSize(), u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");
        REQUIRE(elements == u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");
    }
    SECTION("must replace any Unicode code point with another") {
        // Replace U+3001 (、) with U+1F603 (😃)
        elements.replace(0x3001, 0x1F603);
        REQUIRE(elements ==
                u8"\U00006C34\U0001F603\U0000706B\U0001F603\U00005730\U0001F603\U000098A8\U0001F603\U00007A7A");
        // Replace U+1F603 (😃) with U+2D (-)
        elements.replace(0x1F603, '-');
        REQUIRE(elements == u8"\U00006C34-\U0000706B-\U00005730-\U000098A8-\U00007A7A");
        // Replace U+2D (-) with U+20 (Space)
        elements.replace('-', ' ');
        REQUIRE(elements == u8"\U00006C34 \U0000706B \U00005730 \U000098A8 \U00007A7A");
    }
}

TEST_CASE("String::iterator forward", "[String]") {
    String elements = u8"\U00006C34\U00003001\U0000706B\U00003001";  // "水、火、"
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

TEST_CASE("String::iterator reverse", "[String]") {
    String elements = u8"\U00006C34\U00003001\U0000706B\U00003001";  // "水、火、"
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

TEST_CASE("String::operator[]", "[String]") {
    String faces = u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"

    SECTION("must be able to access any code unit in the UTF-8 String") {
        REQUIRE(faces[0] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x80}));  // 😀
        REQUIRE(faces[1] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x83}));  // 😃
        REQUIRE(faces[2] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x84}));  // 😄
        REQUIRE(faces[3] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x81}));  // 😁
        REQUIRE(faces[4] == utf::CodeUnit<utf::UTF_8>({0xF0, 0x9F, 0x98, 0x86}));  // 😆
    }
}

TEST_CASE("String::subString", "[String]") {
    // "😀😃😄😁😆"
    String faces = u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";
    // "水、火、地、風、空"
    String elements(u8"\U00006C34\U00003001\U0000706B\U00003001\U00005730\U00003001\U000098A8\U00003001\U00007A7A");

    SECTION("Can get any sub-string from the initial string") {
        REQUIRE(faces.subString(1, 3) == u8"\U0001F603\U0001F604\U0001F601");  // "😃😄😁"
    }
    SECTION("If no length is specified it should return a sub string to the end") {
        REQUIRE(elements.subString(4) == u8"\U00005730\U00003001\U000098A8\U00003001\U00007A7A");  // "😃😄😁"
    }
    SECTION("String::sInvalidPos") {
        REQUIRE(faces.subString(5, String::sInvalidPos) == u8"\U0001F606");  // "😆"
    }
}

TEST_CASE("String::operator+=", "[String]") {
    String string;

    SECTION("must be able to append a ASCII value") {
        string += 'A';
        REQUIRE(string == u8"A");
    }
    SECTION("must be able to append an UTF-32 value") {
        string += char32_t(0x6C34);
        REQUIRE(string == u8"\U00006C34");  // "水"
    }
    SECTION("must be able to append another String") {
        string += String(u8"Love \U00005730");   // "Love 地"
        REQUIRE(string == u8"Love \U00005730");  // "Love 地"
    }
    SECTION("must be able to append a CodeUnit") {
        string += utf::CodeUnit<utf::UTF_8>({0xE6, 0xB0, 0xB4});
        REQUIRE(string == u8"\U00006C34");  // "水"
    }
    SECTION("must be able to append a raw UTF-8 string") {
        // "😀😃😄😁😆"
        string += u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";
        REQUIRE(string == u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606");
    }
}

TEST_CASE("String::operator<=>", "[String]") {
    String hola = "HOLA";
    String mundo = "MUNDO";
    String holaMundo = "HOLA MUNDO";

    SECTION("String is less") {
        auto ret = hola <=> mundo;
        REQUIRE(ret == std::strong_ordering::less);
    }
    SECTION("String is greater") {
        auto ret = mundo <=> hola;
        REQUIRE(ret == std::strong_ordering::greater);
    }
    SECTION("String is equal") {
        auto ret = (hola + " " + mundo) <=> holaMundo;
        REQUIRE(ret == std::strong_ordering::equal);
    }
}
