#include "test-utils.h"
#include "json/Unicode.h"

using uchar = unsigned char;

using namespace json::Unicode;

TEST_CASE( "getUnicode", "[unicode]" ) {

    std::string expected = "\t";
    CHECK( expected == getUTF8( "09" ) );

    expected = "\n";
    CHECK( expected == getUTF8( "000A" ) );

    expected = "\xC3\xBF";
    CHECK( expected == getUTF8( "ff" ) );

    expected = "\xEA\xAA\xAA";
    CHECK( expected == getUTF8( "AAAA" ) );

    expected = "\xE1\x88\xB4";
    CHECK( expected == getUTF8( "1234" ) );

    expected = "\xF0\x90\x90\xB7";
    CHECK( expected == getUTF8( "D801DC37" ) );
}

TEST_CASE( "getUnicode limit cases", "[unicode]" ) {
    std::string expected = "\x7f";
    CHECK( expected == getUTF8( "7f" ) );

    expected = "\xC2\x80";
    CHECK( expected == getUTF8( "80" ) );

    expected = "\xDF\xBF";
    CHECK( expected == getUTF8( "07ff" ) );

    expected = "\xE0\xA0\x80";
    CHECK( expected == getUTF8( "0800" ) );

    expected = "\xEF\xBF\xBF";
    CHECK( expected == getUTF8( "ffff" ) );
}

TEST_CASE( "toAscii string", "[unicode]" ) {
    std::string given = "text ";
    given += getUTF8( "ff" );
    CHECK( "text \\u00FF" == toAscii( given ) );

    given = getUTF8( "1f" );
    CHECK( "\\u001F" == toAscii( given ) );

    given = getUTF8( "aaaa" );
    CHECK( "\\uAAAA" == toAscii( given ) );

    given = getUTF8( "0a" );
    CHECK( "\\n" == toAscii( given ) );

    given = getUTF8( "D801DC37" );
    CHECK( "\\uD801\\uDC37" == toAscii( given ) );

}
