#include "test-utils.h"
#include "json/Unicode.h"

using uchar = unsigned char;

TEST_CASE( "encode character", "[unicode]" ) {

    std::string expected8 = "\t";
    CHECK( expected8 == json::Unicode::encode( "09" ) );

    expected8 = "\n";
    CHECK( expected8 == json::Unicode::encode( "0A" ) );

    std::string expected16 = "\xC3\xBF";
    CHECK( expected16 == json::Unicode::encode( "ff" ) );

    std::string expected24 = "\xEA\xAA\xAA";
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

    given = json::Unicode::encode( "0a" );
    CHECK( "\\n" == json::Unicode::decode( given ) );
}