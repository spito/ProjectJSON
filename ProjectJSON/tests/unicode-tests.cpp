#include "test-utils.h"
#include "json/Unicode.h"

using uchar = unsigned char;

using namespace json::Unicode;

TEST_CASE( "encode character", "[unicode]" ) {

    std::string expected = "\t";
    CHECK( expected == encode( "09" ) );

    expected = "\n";
    CHECK( expected == encode( "0A" ) );

    expected = "\xC3\xBF";
    CHECK( expected == encode( "ff" ) );

    expected = "\xEA\xAA\xAA";
    CHECK( expected == encode( "AAAA" ) );

    expected = "\xE1\x88\xB4";
    CHECK( expected == encode( "1234" ) );
}

TEST_CASE( "encoding limit cases", "[unicode]" ) {
    std::string expected = "\x7f";
    CHECK( expected == encode( "7f" ) );

    expected = "\xC2\x80";
    CHECK( expected == encode( "80" ) );

    expected = "\xDF\xBF";
    CHECK( expected == encode( "07ff" ) );

    expected = "\xE0\xA0\x80";
    CHECK( expected == encode( "0800" ) );

    expected = "\xEF\xBF\xBF";
    CHECK( expected == encode( "ffff" ) );
}

TEST_CASE( "decode string", "[unicode]" ) {
    std::string given = "text ";
    given += encode( "ff" );
    CHECK( "text \\u00FF" == decode( given ) );

    given = encode( "1f" );
    CHECK( "\\x1F" == decode( given, true ) );

    given = encode( "aaaa" );
    CHECK( "\\uAAAA" == decode( given ) );

    given = encode( "0a" );
    CHECK( "\\n" == decode( given ) );
}
