#pragma once

#include <cctype>

namespace json {

    class Unicode {
    public:
        using uchar = unsigned char;
        
        static bool fromHexToChar( char higher, char lower, char &out ) {

            uchar hi;
            uchar lo;

            bool result =
                hex( higher, hi ) &&
                hex( lower, lo );
            if ( result )
                out = char( (hi << 4) | lo );
            return result;
        }

        static void fromCharToHex( char c, char &higher, char &lower, bool uppercase = true ) {
            uchar hi = uchar( c ) >> 4;
            uchar lo = uchar( c );

            higher = hex( hi, uppercase );
            lower = hex( lo, uppercase );
        }

    private:

        static bool hex( char c, uchar &u ) {
            if ( std::isdigit( c ) ) {
                u = uchar( c - '0' );
                return true;
            }
            if ( std::isxdigit( c ) ) {
                u = std::islower( c ) ?
                    uchar( c - 'a' ) :
                    uchar( c - 'A' );
                return true;
            }
            return false;
        }

        static char hex( uchar c, bool uppercase ) {
            // allow just lower 4 bits
            c &= 15;

            if ( c < 10 )
                return '0' + c;

            return ( uppercase ? 'A' : 'a' ) - 10 + c;
        }
    };
}
