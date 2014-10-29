#include "test-utils.h"
#include "json/Unicode.h"

using uchar = unsigned char;

TEST_CASE( "encode character", "[unicode]" ) {

    char expected16[] = { char(0xc3), char(0xbf), 0 };
    CHECK( expected16 == json::Unicode::encode( "ff" ) );

    char expected24[] = {char(0xea), char(0xaa), char(0xaa), 0};
    CHECK( expected24 == json::Unicode::encode( "AAAA" ) );
}


TEST_CASE( "decode string", "[unicode]" ) {
    std::string given = "text ";
    given += json::Unicode::encode( "ff" );

    CHECK( "text \\u00FF" == json::Unicode::decode( given ) );

    given = json::Unicode::encode( "1f" );
    CHECK( "\\x1F" == json::Unicode::decode( given, true ) );

    given = json::Unicode::encode( "aaaa" );
    CHECK( "\\uAAAA" == json::Unicode::decode( given ) );
}