#pragma once

#include <string>
namespace json {

    namespace Unicode {
        using uchar = unsigned char;

        // This functions can encode multi-byte character
        // from \uXXXX or \xXX into UTF-8 encoding
        std::string encode( const std::string & );

        // This function can decode whole text into \uXXXX escape sequence
        // If useExtension is set to true,
        //    the function is allowed to generate \xXX escape sequences
        //    if it is reliable
        std::string decode( const std::string &, bool useExtension = false );


        bool fromHexToChar( char higher, char lower, uchar &out );

        void fromCharToHex( uchar c, char &higher, char &lower, bool uppercase = true );

    };
}
