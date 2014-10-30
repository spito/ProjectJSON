#include "Unicode.h"
#include "exceptions.h"

#include <cctype>

namespace json {
    namespace Unicode {

        enum class Mask : uchar {
            Carry = 0x03,
            Tail = 0x3f,
            U8 = 0x7f,
            U16 = 0x1f,
            U24 = 0x0f,
            U32 = 0x07,
            U40 = 0x03,
            U48 = 0x01
        };
        enum class Prefix : uchar {
            Tail = 0x80,
            U8 = 0,
            U16 = 0xc0,
            U24 = 0xe0,
            U32 = 0xf0,
            U40 = 0xf8,
            U48 = 0xfc
        };

        bool hex2char( char c, uchar &u ) {
            if ( std::isdigit( c ) ) {
                u = uchar( c - '0' );
                return true;
            }
            if ( std::isxdigit( c ) ) {
                u = std::islower( c ) ?
                    uchar( c - 'a' + 10 ) :
                    uchar( c - 'A' + 10 );
                return true;
            }
            return false;
        }

        char char2hex( uchar c, bool uppercase ) {
            // allow just lower 4 bits
            c &= 15;

            if ( c < 10 )
                return '0' + c;

            return ( uppercase ? 'A' : 'a' ) - 10 + c;
        }


        bool fromHexToChar( char higher, char lower, uchar &out ) {

            uchar hi{};
            uchar lo{};

            bool result =
                hex2char( higher, hi ) &&
                hex2char( lower, lo );
            if ( result )
                out = ( hi << 4 ) | lo;
            return result;
        }

        void fromCharToHex( uchar c, char &higher, char &lower, bool uppercase ) {
            uchar hi = uchar( c ) >> 4;
            uchar lo = uchar( c );

            higher = char2hex( hi, uppercase );
            lower = char2hex( lo, uppercase );
        }

        inline uchar modifyChar( Prefix prefix, Mask mask, uchar c ) {
            return uchar( prefix ) | ( uchar( mask ) & c );
        }
        inline uchar modifyChar( Mask mask, uchar c ) {
            return uchar( mask ) & c;
        }

        std::string encode( uchar c ) {
            std::string result;

            if ( c < 0x80 )
                result += modifyChar( Prefix::U8, Mask::U8, c );
            else {
                result += modifyChar( Prefix::U16, Mask::U16, c >> 6 );
                result += modifyChar( Prefix::Tail, Mask::Tail, c );
            }
            return result;
        }

        std::string encode( uchar c1, uchar c2 ) {
            std::string result;
            if ( c1 < 0x08 ) {
                result +=
                    modifyChar( Prefix::U16, Mask::U16, c1 << 2 ) |
                    modifyChar( Prefix::U16, Mask::U16, c2 >> 6 );
                result +=
                    modifyChar( Prefix::Tail, Mask::Tail, c2 );
            }
            else {
                result +=
                    modifyChar( Prefix::U24, Mask::U24, c1 >> 4 );
                result +=
                    modifyChar( Prefix::Tail, Mask::Tail, c1 << 2 ) |
                    modifyChar( Prefix::Tail, Mask::Tail, c2 >> 6 );
                result +=
                    modifyChar( Prefix::Tail, Mask::Tail, c2 );
            }
            return result;
        }

        std::string encode( const std::string &input ) {

            if ( input.size() == 2 ) {
                uchar c;
                if ( !fromHexToChar( input[ 0 ], input[ 1 ], c ) )
                    throw exception::Unicode( "Invalid input format" );
                return encode( c );
            }
            if ( input.size() == 4 ) {
                uchar c1, c2;
                if ( !fromHexToChar( input[ 0 ], input[ 1 ], c1 ) ||
                     !fromHexToChar( input[ 2 ], input[ 3 ], c2 ) )
                     throw exception::Unicode( "Invalid input format" );
                return encode( c1, c2 );
            }
            throw exception::Unicode( "Unsupported length of unicode character" );
        }

        inline bool isUnicode16( uchar c ) {
            return ( ~uchar( Mask::U16 ) & c ) == uchar( Prefix::U16 );
        }

        inline bool isUnicode24( uchar c ) {
            return ( ~uchar( Mask::U24 ) & c ) == uchar( Prefix::U24 );
        }

        inline bool isUnicode32( uchar c ) {
            return ( ~uchar( Mask::U32 ) & c ) == uchar( Prefix::U32 );
        }

        inline bool isUnicode40( uchar c ) {
            return ( ~uchar( Mask::U40 ) & c ) == uchar( Prefix::U40 );
        }

        inline bool isUnicode48( uchar c ) {
            return ( ~uchar( Mask::U48 ) & c ) == uchar( Prefix::U48 );
        }

        std::string decodeUnicode8( const std::string &text, size_t &index ) {
            char hex[ 3 ] = { 0 };
            fromCharToHex( text[ index ], hex[ 0 ], hex[ 1 ] );
            return std::string( "\\u00" ) + hex;
        }

        std::string decodeUnicode16( const std::string &text, size_t &index ) {
            char hex[ 5 ] = { 0 };

            char c[ 2 ];
            c[ 0 ] =
                modifyChar( Mask::U16, text[ index ] ) >> 2;

            c[ 1 ] =
                ( modifyChar( Mask::Carry, text[ index ] ) << 6 ) |
                modifyChar( Mask::Tail, text[ index + 1 ] );

            fromCharToHex( c[ 0 ], hex[ 0 ], hex[ 1 ] );
            fromCharToHex( c[ 1 ], hex[ 2 ], hex[ 3 ] );

            ++index;

            return std::string( "\\u" ) + hex;
        }

        std::string decodeUnicode24( const std::string &text, size_t &index ) {
            char hex[ 5 ] = { 0 };

            char c[ 2 ];
            c[ 0 ] =
                ( modifyChar( Mask::U24, text[ index ] ) << 4 ) |
                ( modifyChar( Mask::Tail, text[ index + 1 ] ) >> 2 );

            c[ 1 ] =
                ( modifyChar( Mask::Carry, text[ index + 1 ] ) << 6 ) |
                modifyChar( Mask::Tail, text[ index + 2 ] );

            fromCharToHex( c[ 0 ], hex[ 0 ], hex[ 1 ] );
            fromCharToHex( c[ 1 ], hex[ 2 ], hex[ 3 ] );

            ++index;
            ++index;

            return std::string( "\\u" ) + hex;
        }

        std::string decodeByte( uchar c ) {
            char hex[ 3 ] = { 0 };
            fromCharToHex( c, hex[ 0 ], hex[ 1 ] );
            return std::string( "\\x" ) + hex;
        }


        std::string decode( const std::string &text, bool useExtension ) {
            std::string result;

            for ( size_t i = 0; i < text.size(); ++i ) {
                if ( isUnicode16( text[ i ] ) )
                    result += decodeUnicode16( text, i );
                else if ( isUnicode24( text[ i ] ) )
                    result += decodeUnicode24( text, i );
                else if ( isUnicode32( text[ i ] ) || isUnicode40( text[ i ] ) || isUnicode48( text[ i ] ) )
                    throw exception::Unicode( "Unsupported length of UTF-8 character" );
                else switch ( text[ i ] ) {
                case '"':
                case '\\':
                    result += '\\';
                    result += text[ i ];
                    break;
                case '\b':
                    result += "\\b";
                    break;
                case '\f':
                    result += "\\f";
                    break;
                case '\n':
                    result += "\\n";
                    break;
                case '\r':
                    result += "\\r";
                    break;
                case '\t':
                    result += "\\t";
                    break;
                default:
                    if ( uchar( text[ i ] ) < 0x20 ) {
                        if ( useExtension )
                            result += decodeByte( text[ i ] );
                        else
                            result += decodeUnicode8( text, i );
                    }
                    else
                        result += text[ i ];
                    break;
                }
            }
            return result;
        }
    }
}