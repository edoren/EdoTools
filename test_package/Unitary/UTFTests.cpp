#include <catch2/catch.hpp>

#include <edoren/UTF.hpp>

using namespace edoren;

TEST_CASE("Calling utf::GetEncodingSize", "[UTF]") {
    SECTION("Should return the appropiate values for each encoding") {
        REQUIRE(utf::GetEncodingSize(utf::UTF_8) == 1);
        REQUIRE(utf::GetEncodingSize(utf::UTF_16) == 2);
        REQUIRE(utf::GetEncodingSize(utf::UTF_32) == 4);
    }
}

TEST_CASE("Calling utf::UtfToUtf", "[UTF]") {
    std::basic_string<char8_t> smiley8 = u8"\U0001F600\U00005730\u00F1\u0041";   // "😀地ñA"
    std::basic_string<char16_t> smiley16 = u"\U0001F600\U00005730\u00F1\u0041";  // "😀地ñA"
    std::basic_string<char32_t> smiley32 = U"\U0001F600\U00005730\u00F1\u0041";  // "😀地ñA"

    std::basic_string<char8_t> output8;
    std::basic_string<char16_t> output16;
    std::basic_string<char32_t> output32;

    SECTION("Should convert to a valid encoding") {
        {
            output16.clear();
            output32.clear();
            utf::UtfToUtf<utf::UTF_8, utf::UTF_16>(smiley8.begin(), smiley8.end(), &output16);
            utf::UtfToUtf<utf::UTF_8, utf::UTF_32>(smiley8.data(), smiley8.data() + smiley8.size(), &output32);
            REQUIRE(output16 == smiley16);
            REQUIRE(output32 == smiley32);
        }
        {
            output8.clear();
            output32.clear();
            utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(smiley16.begin(), smiley16.end(), &output8);
            utf::UtfToUtf<utf::UTF_16, utf::UTF_32>(smiley16.data(), smiley16.data() + smiley16.size(), &output32);
            REQUIRE(output8 == smiley8);
            REQUIRE(output32 == smiley32);
        }
        {
            output8.clear();
            output16.clear();
            utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(smiley32.begin(), smiley32.end(), &output8);
            utf::UtfToUtf<utf::UTF_32, utf::UTF_16>(smiley32.data(), smiley32.data() + smiley32.size(), &output16);
            REQUIRE(output8 == smiley8);
            REQUIRE(output16 == smiley16);
        }
    }

    SECTION(
        "Should return a non-end iterator pointing to the invalid "
        "point on the string when the source has a bad encoding") {
        // This should invalidate the UTF-8 and UTF-16 string
        smiley8[1] = 'A';
        smiley16[1] = 'A';
        auto it0 = utf::UtfToUtf<utf::UTF_8, utf::UTF_16>(smiley8.begin(), smiley8.end(), &output16);
        auto it1 = utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(smiley16.begin(), smiley16.end(), &output8);
        REQUIRE(it0 >= smiley8.begin());
        REQUIRE(it0 < smiley8.end());
        REQUIRE(it1 >= smiley16.begin());
        REQUIRE(it1 < smiley16.end());
    }

    SECTION("Should return the end iterator when the string is valid") {
        auto it0 = utf::UtfToUtf<utf::UTF_8, utf::UTF_16>(smiley8.begin(), smiley8.end(), &output16);
        auto it1 = utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(smiley16.begin(), smiley16.end(), &output8);
        REQUIRE(it0 == smiley8.end());
        REQUIRE(it1 == smiley16.end());
    }
}

TEST_CASE("Calling utf::GetCodePoint", "[UTF]") {
    std::array<uint8_t, 4> smile8 = {0xF0, 0x9F, 0x98, 0x80};  // 😀
    std::array<uint16_t, 2> smile16 = {0xD83D, 0xDE00};        // 😀
    std::array<uint8_t, 4> earth8 = {0xE5, 0x9C, 0xB0, 0x00};  // 地
    std::array<uint16_t, 2> earth16 = {0x5730, 0x0000};        // 地

    SECTION("Should return the right code point from any encoding") {
        auto val0 = utf::GetCodePoint<utf::UTF_8>(smile8.begin(), smile8.end());
        auto val1 = utf::GetCodePoint<utf::UTF_16>(smile16.begin(), smile16.end());
        auto val2 = utf::GetCodePoint<utf::UTF_8>(earth8.begin(), earth8.end());
        auto val3 = utf::GetCodePoint<utf::UTF_16>(earth16.begin(), earth16.end());
        REQUIRE(val0 == 0x1F600);
        REQUIRE(val1 == 0x1F600);
        REQUIRE(val2 == 0x5730);
        REQUIRE(val3 == 0x5730);
    }
}

TEST_CASE("Calling utf::Next", "[UTF]") {
    SECTION("Should return the next code unit") {
        {
            std::basic_string<char8_t> smiley = u8"\U0001F600\U0001F603\U00005730";  // "😀😃地"

            auto begin = smiley.begin();
            auto end = smiley.end();

            auto it0 = utf::Next<utf::UTF_8>(begin, end);
            auto it1 = utf::Next<utf::UTF_8>(smiley.data(), smiley.data() + smiley.size());
            auto it2 = utf::Next<utf::UTF_8>(it0, end);
            auto it3 = utf::Next<utf::UTF_8>(it2, end);  // This is the end

            REQUIRE(&(*it0) == &(*it1));
            REQUIRE(&(*(begin + 4)) == &(*it1));
            REQUIRE(&(*(begin + 8)) == &(*it2));
            REQUIRE(end == it3);
        }
        {
            std::basic_string<char16_t> smiley = u"\U0001F600\U0001F603\U00005730";  // "😀😃地"

            auto begin = smiley.begin();
            auto end = smiley.end();

            auto it0 = utf::Next<utf::UTF_16>(begin, end);
            auto* it1 = utf::Next<utf::UTF_16>(smiley.data(), smiley.data() + smiley.size());
            auto it2 = utf::Next<utf::UTF_16>(it0, end);
            auto it3 = utf::Next<utf::UTF_16>(it2, end);  // This is the end

            REQUIRE(&(*it0) == &(*it1));
            REQUIRE(&(*(begin + 2)) == &(*it1));
            REQUIRE(&(*(begin + 4)) == &(*it2));
            REQUIRE(end == it3);
        }
        {
            std::basic_string<char32_t> smiley = U"\U0001F600\U0001F603\U00005730";  // "😀😃地"

            auto begin = smiley.begin();
            auto end = smiley.end();

            auto it0 = utf::Next<utf::UTF_32>(begin, end);
            auto* it1 = utf::Next<utf::UTF_32>(smiley.data(), smiley.data() + smiley.size());
            auto it2 = utf::Next<utf::UTF_32>(it0, end);
            auto it3 = utf::Next<utf::UTF_32>(it2, end);  // This is the end

            REQUIRE(&(*it0) == &(*it1));
            REQUIRE(&(*(begin + 1)) == &(*it1));
            REQUIRE(&(*(begin + 2)) == &(*it2));
            REQUIRE(end == it3);
        }
    }

    SECTION("Should return end if the end of the string has been reached") {
        std::basic_string<char8_t> smiley8 = u8"\U0001F600";   // "😀"
        std::basic_string<char16_t> smiley16 = u"\U0001F600";  // "😀"
        std::basic_string<char32_t> smiley32 = U"\U0001F600";  // "😀"

        {
            auto begin = smiley8.begin();
            auto end = smiley8.end();

            auto it0 = utf::Next<utf::UTF_8>(begin, end);
            auto it1 = utf::Next<utf::UTF_8>(it0, end);
            auto it2 = utf::Next<utf::UTF_8>(it1, end);

            REQUIRE(it1 == end);
            REQUIRE(it2 == end);
        }
        {
            auto begin = smiley16.begin();
            auto end = smiley16.end();

            auto it0 = utf::Next<utf::UTF_16>(begin, end);
            auto it1 = utf::Next<utf::UTF_16>(it0, end);
            auto it2 = utf::Next<utf::UTF_16>(it1, end);

            REQUIRE(it1 == end);
            REQUIRE(it2 == end);
        }
        {
            auto begin = smiley32.begin();
            auto end = smiley32.end();

            auto it0 = utf::Next<utf::UTF_32>(begin, end);
            auto it1 = utf::Next<utf::UTF_32>(it0, end);
            auto it2 = utf::Next<utf::UTF_32>(it1, end);

            REQUIRE(it1 == end);
            REQUIRE(it2 == end);
        }
    }

    SECTION("Should return begin if an encoding error occured") {
        std::basic_string<char8_t> smiley8 = u8"\U0001F600\U0001F603";   // "😀😃"
        std::basic_string<char16_t> smiley16 = u"\U0001F600\U0001F603";  // "😀😃"

        {
            auto begin0 = smiley8.begin() + 1;
            auto it0 = utf::Next<utf::UTF_8>(begin0, smiley8.end());

            auto begin1 = smiley8.begin();
            auto it1 = utf::Next<utf::UTF_8>(begin1, smiley8.end());

            REQUIRE(it0 == begin0);
            REQUIRE(it1 == (begin1 + 4));
        }
        {
            auto begin0 = smiley16.begin() + 1;
            auto it0 = utf::Next<utf::UTF_16>(begin0, smiley16.end());

            auto begin1 = smiley16.begin();
            auto it1 = utf::Next<utf::UTF_16>(begin1, smiley16.end());

            REQUIRE(it0 == begin0);
            REQUIRE(it1 == (begin1 + 2));
        }
    }
}

TEST_CASE("Calling utf::Prior", "[UTF]") {
    SECTION("Should return the prior UTF-8 code unit") {
        {
            std::basic_string<char8_t> smiley = u8"\U0001F600\U0001F603\U00005730";  // "😀😃地"

            auto begin = smiley.begin();
            auto end = smiley.end();

            auto it0 = utf::Prior<utf::UTF_8>(end, begin);
            auto* it1 = utf::Prior<utf::UTF_8>(smiley.data() + smiley.size(), smiley.data());
            auto it2 = utf::Prior<utf::UTF_8>(it0, begin);
            auto it3 = utf::Prior<utf::UTF_8>(it2, begin);

            REQUIRE(&(*it0) == &(*it1));
            REQUIRE(&(*(end - 3)) == &(*it1));
            REQUIRE(&(*(end - 7)) == &(*it2));
            REQUIRE(&(*(end - 11)) == &(*it3));
        }
        {
            std::basic_string<char16_t> smiley = u"\U0001F600\U0001F603\U00005730";  // "😀😃地"

            auto begin = smiley.begin();
            auto end = smiley.end();

            auto it0 = utf::Prior<utf::UTF_16>(end, begin);
            auto* it1 = utf::Prior<utf::UTF_16>(smiley.data() + smiley.size(), smiley.data());
            auto it2 = utf::Prior<utf::UTF_16>(it0, begin);
            auto it3 = utf::Prior<utf::UTF_16>(it2, begin);

            REQUIRE(&(*it0) == &(*it1));
            REQUIRE(&(*(end - 1)) == &(*it1));
            REQUIRE(&(*(end - 3)) == &(*it2));
            REQUIRE(&(*(end - 5)) == &(*it3));
        }
        {
            std::basic_string<char32_t> smiley = U"\U0001F600\U0001F603\U00005730";  // "😀😃地"

            auto begin = smiley.begin();
            auto end = smiley.end();

            auto it0 = utf::Prior<utf::UTF_32>(end, begin);
            auto* it1 = utf::Prior<utf::UTF_32>(smiley.data() + smiley.size(), smiley.data());
            auto it2 = utf::Prior<utf::UTF_32>(it0, begin);
            auto it3 = utf::Prior<utf::UTF_32>(it2, begin);

            REQUIRE(&(*it0) == &(*it1));
            REQUIRE(&(*(end - 1)) == &(*it1));
            REQUIRE(&(*(end - 2)) == &(*it2));
            REQUIRE(&(*(end - 3)) == &(*it3));
        }
    }

    SECTION("Should return begin if the start of the string has been reached") {
        std::basic_string<char8_t> smiley8 = u8"\U0001F600";   // "😀"
        std::basic_string<char16_t> smiley16 = u"\U0001F600";  // "😀"
        std::basic_string<char32_t> smiley32 = U"\U0001F600";  // "😀"

        {
            auto begin = smiley8.begin();
            auto end = smiley8.end();

            auto it0 = utf::Prior<utf::UTF_8>(end, begin);
            auto it1 = utf::Prior<utf::UTF_8>(it0, begin);
            auto it2 = utf::Prior<utf::UTF_8>(it1, begin);

            REQUIRE(it1 == begin);
            REQUIRE(it2 == begin);
        }
        {
            auto begin = smiley16.begin();
            auto end = smiley16.end();

            auto it0 = utf::Prior<utf::UTF_16>(end, begin);
            auto it1 = utf::Prior<utf::UTF_16>(it0, begin);
            auto it2 = utf::Prior<utf::UTF_16>(it1, begin);

            REQUIRE(it1 == begin);
            REQUIRE(it2 == begin);
        }
        {
            auto begin = smiley32.begin();
            auto end = smiley32.end();

            auto it0 = utf::Prior<utf::UTF_32>(end, begin);
            auto it1 = utf::Prior<utf::UTF_32>(it0, begin);
            auto it2 = utf::Prior<utf::UTF_32>(it1, begin);

            REQUIRE(it1 == begin);
            REQUIRE(it2 == begin);
        }
    }

    SECTION("Should return end if an encoding error occured") {
        std::basic_string<char8_t> smiley8 = u8"\U0001F600\U0001F603";   // "😀😃"
        std::basic_string<char16_t> smiley16 = u"\U0001F600\U0001F603";  // "😀😃"

        {
            auto end0 = smiley8.end() - 1;
            auto it0 = utf::Prior<utf::UTF_8>(end0, smiley8.begin());

            auto end1 = smiley8.end();
            auto it1 = utf::Prior<utf::UTF_8>(end1, smiley8.begin());

            REQUIRE(it0 == end0);
            REQUIRE(it1 == (end1 - 4));
        }
        {
            auto end0 = smiley16.end() - 1;
            auto it0 = utf::Prior<utf::UTF_16>(end0, smiley16.begin());

            auto end1 = smiley16.end();
            auto it1 = utf::Prior<utf::UTF_16>(end1, smiley16.begin());

            REQUIRE(it0 == end0);
            REQUIRE(it1 == (end1 - 2));
        }
    }
}

TEST_CASE("Calling utf::ForEach", "[UTF]") {
    std::basic_string<char8_t> smiley8 = u8"\U0001F600\U00005730\u00F1\u0041";   // "😀地ñA"
    std::basic_string<char16_t> smiley16 = u"\U0001F600\U00005730\u00F1\u0041";  // "😀地ñA"
    std::basic_string<char32_t> smiley32 = U"\U0001F600\U00005730\u00F1\u0041";  // "😀地ñA"

    SECTION("Should able to iterate over any encoding") {
        size_t count = 0;
        auto checkValue = [&count](auto&& codePointRange) {
            if (count == 0) {
                REQUIRE(codePointRange.get().getCodePoint() == 0x1F600);  // 😀
            } else if (count == 1) {
                REQUIRE(codePointRange.get().getCodePoint() == 0x5730);  // 地
            } else if (count == 2) {
                REQUIRE(codePointRange.get().getCodePoint() == 0xF1);  // ñ
            } else if (count == 3) {
                REQUIRE(codePointRange.get().getCodePoint() == 0x41);  // A
            } else {
                FAIL("Iterated past the limit");
            }
            ++count;
        };
        utf::ForEach<utf::UTF_8>(smiley8.begin(), smiley8.end(), checkValue);
        count = 0;
        utf::ForEach<utf::UTF_16>(smiley16.begin(), smiley16.end(), checkValue);
        count = 0;
        utf::ForEach<utf::UTF_32>(smiley32.begin(), smiley32.end(), checkValue);
    }

    SECTION("Should fail and return the iterator to the fail point when the encoding is incorrect") {
        smiley8[5] = 'A';
        smiley16[1] = 'A';
        auto it0 = utf::ForEach<utf::UTF_8>(smiley8.begin(), smiley8.end(), [](auto&& /*unused*/) {});
        auto it1 = utf::ForEach<utf::UTF_16>(smiley16.begin(), smiley16.end(), [](auto&& /*unused*/) {});
        REQUIRE(it0 >= smiley8.begin());
        REQUIRE(it0 < smiley8.end());
        REQUIRE(it1 >= smiley16.begin());
        REQUIRE(it1 < smiley16.end());
        REQUIRE(it0 == smiley8.begin() + 4);
        REQUIRE(it1 == smiley16.begin());
    }

    SECTION("Should return the end iterator when succeeds") {
        auto it0 = utf::ForEach<utf::UTF_8>(smiley8.begin(), smiley8.end(), [](auto&& /*unused*/) {});
        auto it1 = utf::ForEach<utf::UTF_16>(smiley16.begin(), smiley16.end(), [](auto&& /*unused*/) {});
        auto it2 = utf::ForEach<utf::UTF_32>(smiley32.begin(), smiley32.end(), [](auto&& /*unused*/) {});
        REQUIRE(it0 == smiley8.end());
        REQUIRE(it1 == smiley16.end());
        REQUIRE(it2 == smiley32.end());
    }
}

TEST_CASE("Calling utf::GetSize", "[UTF]") {
    std::basic_string<char8_t> smiley8 = u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"
    std::basic_string<char16_t> smiley16 = u"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"
    std::basic_string<char32_t> smiley32 = U"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"

    std::basic_string<char8_t> elements8 = u8"\U00006C34\U0000706B\U00005730\U000098A8\U00007A7A";  // "水火地風空"
    std::basic_string<char16_t> elements16 = u"\U00006C34\U0000706B\U00005730\U000098A8\U00007A7A";  // "水火地風空"
    std::basic_string<char32_t> elements32 = U"\U00006C34\U0000706B\U00005730\U000098A8\U00007A7A";  // "水火地風空"

    SECTION("Should return the correct size for the string") {
        REQUIRE(utf::GetSize<utf::UTF_8>(smiley8.begin(), smiley8.end()) == 5);
        REQUIRE(utf::GetSize<utf::UTF_16>(smiley16.begin(), smiley16.end()) == 5);
        REQUIRE(utf::GetSize<utf::UTF_32>(smiley32.begin(), smiley32.end()) == 5);
        REQUIRE(utf::GetSize<utf::UTF_8>(elements8.begin(), elements8.end()) == 5);
        REQUIRE(utf::GetSize<utf::UTF_16>(elements16.begin(), elements16.end()) == 5);
        REQUIRE(utf::GetSize<utf::UTF_32>(elements32.begin(), elements32.end()) == 5);
    }
}

TEST_CASE("Calling utf::IsValid", "[UTF]") {
    std::basic_string<char8_t> smiley8 = u8"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"
    std::basic_string<char16_t> smiley16 = u"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"
    std::basic_string<char32_t> smiley32 = U"\U0001F600\U0001F603\U0001F604\U0001F601\U0001F606";  // "😀😃😄😁😆"

    SECTION("Should return true if the string is valid") {
        REQUIRE(utf::IsValid<utf::UTF_8>(smiley8.begin(), smiley8.end()) == true);
        REQUIRE(utf::IsValid<utf::UTF_16>(smiley16.begin(), smiley16.end()) == true);
        REQUIRE(utf::IsValid<utf::UTF_32>(smiley32.begin(), smiley32.end()) == true);
    }

    SECTION("Should return false if the string is invalid") {
        smiley8[5] = 'A';
        smiley16[1] = 'A';
        REQUIRE(utf::IsValid<utf::UTF_8>(smiley8.begin(), smiley8.end()) == false);
        REQUIRE(utf::IsValid<utf::UTF_16>(smiley16.begin(), smiley16.end()) == false);
    }
}
