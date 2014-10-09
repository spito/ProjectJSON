
#include "Tokenizer.h"
#include "../exceptions.h"

#include <string>
#include <sstream>

namespace json {
    namespace parser {

        bool Tokenizer::isTokenType( Token::Type type, Token &token ) {
            Position before = position();
            getToken( token );
            if ( token.type() == type )
                return true;

            position( before );
            return false;
        }

        void Tokenizer::readToken( Token &token ) {
            getToken( token );
        }

        void Tokenizer::lookAtToken( Token &token ) {
            Position before = position();
            getToken( token );
            position( before );
        }

        void Tokenizer::getToken( Token &token ) {
            _input.skipWhite();

            Position before = position();
            char c = _input.readChar();
            switch ( c ) {
            case ',':
                token = Token( Token::Type::Comma, before );
                return;
            case ':':
                token = Token( Token::Type::Colon, before );
                return;
            case '{':
                token = Token( Token::Type::LeftBrace, before );
                return;
            case '}':
                token = Token( Token::Type::RightBrace, before );
                return;
            case '[':
                token = Token( Token::Type::LeftBracket, before );
                return;
            case ']':
                token = Token( Token::Type::RightBracket, before );
                return;
            case 't':
                position( before );
                processWord( Token::Type::True, token );
                return;
            case 'f':
                position( before );
                processWord( Token::Type::False, token );
                return;
            case 'n':
                position( before );
                processWord( Token::Type::Null, token );
                return;
            case '"':
                position( before );
                processString( token );
                return;
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                position( before );
                processNumber( token );
                return;
            case char( -1 ) :
                token = Token( Token::Type::EndOfFile, before );
                return;
            default:
                throw exception::InvalidCharacter( c, ",:{}[]tfn\"-0123456789", before );
            }
        }

        void Tokenizer::processWord( Token::Type type, Token &token ) {

            const char *string = Token::value( type );
            size_t size = strlen( string );

            Position before = position();

            for ( int i = 0; i < int(size); ++i ) {
                Position p = position();
                char given = _input.read();
                if ( given != string[ i ] ) {
                    throw exception::InvalidCharacter( given, string[ i ], p );
                }
            }
            token = Token( type, before );
        }

        // according to the documentation these characters are not allowed
        // and have to escaped by using \uXXXX notation.
        bool Tokenizer::isBadChar( char c ) {
            return char( 0 ) <= c && c <= char( 0x1f );
        }

        void Tokenizer::processString( Token &token ) {
            std::string rawToken;
            bool special = false;

            Position before = position();
            _input.read();// read "

            while ( true ) {
                char c = _input.read();
                if ( _input.eof() ) {
                    throw exception::EndOfFile( "", position() );
                }

                if ( special ) {
                    switch ( c ) {
                    case '"':
                    case '\\':
                    case '/':
                        rawToken.push_back( c );
                        break;
                    case 'b':
                        rawToken.push_back( '\b' );
                        break;
                    case 'f':
                        rawToken.push_back( '\f' );
                        break;
                    case 'n':
                        rawToken.push_back( '\n' );
                        break;
                    case 'r':
                        rawToken.push_back( '\r' );
                        break;
                    case 't':
                        rawToken.push_back( '\t' );
                        break;
                    case 'u':
                        rawToken += processUnicode();
                    default:
                        throw exception::InvalidCharacter( c, "\"\\/bfnrtu", position() );
                    }
                    special = false;
                }
                else if ( c == '\\' )
                    special = true;
                else if ( c == '"' )
                    break;
                else if ( isBadChar( c ) )
                    throw exception::InvalidCharacter( c, position() );
                else
                    rawToken.push_back( c );
            }
            token = Token( Token::Type::String, rawToken, before );
        }

        // TODO: implement
        std::string Tokenizer::processUnicode() {
            throw exception::UnsupportedFeature( "process unicode", position() );
#if 0
            inline json_uchar SurrogatePair( const json_uchar hi, const json_uchar lo ) json_pure;
            inline json_uchar SurrogatePair( const json_uchar hi, const json_uchar lo ) json_nothrow{
                JSON_ASSERT( sizeof( unsigned int ) == 4, JSON_TEXT( "size of unsigned int is not 32-bit" ) );
                JSON_ASSERT( sizeof( json_uchar ) == 4, JSON_TEXT( "size of json_char is not 32-bit" ) );
                return ( ( ( hi << 10 ) & 0x1FFC00 ) + 0x10000 ) | lo & 0x3FF;
            }

                void JSONWorker::UTF( const json_char * & pos, json_string & result, const json_char * const end ) json_nothrow{
                JSON_ASSERT_SAFE( ( (long)end - (long)pos ) > 4, JSON_TEXT( "UTF will go out of bounds" ), return; );
                json_uchar first = UTF8( pos, end );
                if ( json_unlikely( ( first > 0xD800 ) && ( first < 0xDBFF ) &&
                    ( *( pos + 1 ) == '\\' ) && ( *( pos + 2 ) == 'u' ) ) ) {
                    const json_char * original_pos = pos;  //if the 2nd character is not correct I need to roll back the iterator
                    pos += 2;
                    json_uchar second = UTF8( pos, end );
                    //surrogate pair, not two characters
                    if ( json_unlikely( ( second > 0xDC00 ) && ( second < 0xDFFF ) ) ) {
                        result += SurrogatePair( first, second );
                    }
                    else {
                        pos = original_pos;
                    }
                }
                else {
                    result += first;
                }
            }
            json_uchar JSONWorker::UTF8( const json_char * & pos, const json_char * const end ) json_nothrow{
                JSON_ASSERT_SAFE( ( (long)end - (long)pos ) > 4, JSON_TEXT( "UTF will go out of bounds" ), return JSON_TEXT( '\0' ); );
#ifdef JSON_UNICODE
                ++pos;
                json_uchar temp = Hex( pos ) << 8;
                ++pos;
                return temp | Hex( pos );
#else
                JSON_ASSERT( *( pos + 1 ) == JSON_TEXT( '0' ), JSON_TEXT( "wide utf character (hihi)" ) );
                JSON_ASSERT( *( pos + 2 ) == JSON_TEXT( '0' ), JSON_TEXT( "wide utf character (hilo)" ) );
                pos += 3;
                return Hex( pos );
#endif
            }
            json_char JSONWorker::Hex( const json_char * & pos ) json_nothrow{
                /*
                takes the numeric value of the next two characters and convert them
                \u0058 becomes 0x58

                In case of \u, it's SpecialChar's responsibility to move past the first two chars
                as this method is also used for \x
                */
                //First character
                json_uchar hi = *pos++ - 48;
                if ( hi > 48 ) {  //A-F don't immediately follow 0-9, so have to pull them down a little
                    hi -= 39;
                }
                else if ( hi > 9 ) {  //neither do a-f
                    hi -= 7;
                }
                //second character
                json_uchar lo = *pos - 48;
                if ( lo > 48 ) {  //A-F don't immediately follow 0-9, so have to pull them down a little
                    lo -= 39;
                }
                else if ( lo > 9 ) {  //neither do a-f
                    lo -= 7;
                }
                //combine them
                return (json_char)( ( hi << 4 ) | lo );
            }
#endif
        }

        // TODO: check needed
        // TODO: refactoring needed
        void Tokenizer::processNumber( Token &token ) {

            enum class States {
                Init,
                Minus,
                Zero,
                Digits,
                Point,
                DecimalDigits,
                E,
                EPlusMinus,
                EDigits
            };

            bool isReal = false;
            bool isMinus = false;
            bool isE = false;
            bool isExponentMinus = false;
            bool quit = false;
            States state = States::Init;
            double denominator = 10.0;

            std::string rawToken;
            Position before = position();
            long long exponent = 0;
            long long integer = 0;
            long double real = 0.0;

            while ( !quit ) {

                char c = _input.read();
                switch ( state ) {
                case States::Init:
                    if ( c == '-' ) {
                        isMinus = true;
                        state = States::Minus;
                        rawToken.push_back( c );
                    }
                    else if ( c == '0' ) {
                        state = States::Zero;
                        rawToken.push_back( c );
                    }
                    else if ( std::isdigit( c ) ) {
                        state = States::Digits;
                        integer = c - '0';
                        rawToken.push_back( c );
                    }
                    else {
                        throw exception::InvalidCharacter( c, "-0123456789", position() );
                    }
                    break;
                case States::Minus:
                    if ( c == '0' ) {
                        state = States::Zero;
                        rawToken.push_back( c );
                    }
                    else if ( std::isdigit( c ) ) {
                        state = States::Digits;
                        integer = c - '0';
                        rawToken.push_back( c );
                    }
                    else {
                        throw exception::InvalidCharacter( c, "0123456789", position() );
                    }
                    break;
                case States::Digits:
                    if ( std::isdigit( c ) ) {
                        rawToken.push_back( c );
                        integer *= 10;
                        integer += c - '0';
                        break;
                    }
                    // fall down
                case States::Zero:
                    if ( c == '.' ) {
                        state = States::Point;
                        isReal = true;
                        real = static_cast<long double>( integer );
                        rawToken.push_back( c );
                    }
                    else if ( c == 'e' || c == 'E' ) {
                        state = States::E;
                        isE = true;
                        rawToken.push_back( c );
                    }
                    else {
                        quit = true;
                    }
                    break;
                case States::Point:
                    if ( std::isdigit( c ) ) {
                        state = States::DecimalDigits;
                        real += ( c - '0' ) / denominator;
                        denominator *= 10;
                        rawToken.push_back( c );
                    }
                    else {
                        throw exception::InvalidCharacter( c, "0123456789", position() );
                    }
                    break;
                case States::DecimalDigits:
                    if ( std::isdigit( c ) ) {
                        real += ( c - '0' ) / denominator;
                        denominator *= 10;
                        rawToken.push_back( c );
                    }
                    else if ( c == 'e' || c == 'E' ) {
                        state = States::E;
                        isE = true;
                        rawToken.push_back( c );
                    }
                    else {
                        quit = true;
                    }
                    break;
                case States::E:
                    if ( c == '-' ) {
                        isExponentMinus = true;
                        state = States::EPlusMinus;
                        rawToken.push_back( c );
                    }
                    else if ( c == '+' ) {
                        state = States::EPlusMinus;
                        rawToken.push_back( c );
                    }
                    else if ( std::isdigit( c ) ) {
                        state = States::EDigits;
                        exponent *= 10;
                        exponent += c - '0';
                        rawToken.push_back( c );
                    }
                    else {
                        throw exception::InvalidCharacter( c, "+-0123456789", position() );
                    }
                    break;
                case States::EPlusMinus:
                    if ( std::isdigit( c ) ) {
                        state = States::EDigits;
                        exponent *= 10;
                        exponent += c - '0';
                        rawToken.push_back( c );
                    }
                    else {
                        throw exception::InvalidCharacter( c, "0123456789", position() );
                    }
                    break;
                case States::EDigits:
                    if ( std::isdigit( c ) ) {
                        exponent *= 10;
                        exponent += c - '0';
                        rawToken.push_back( c );
                    }
                    else {
                        quit = true;
                    }
                    break;
                }
            }

            if ( isE ) {
                if ( !isReal ) {
                    // try to remain number as an integer
                    if ( isExponentMinus ) {
                        auto ex = exponent;
                        while ( ex ) {
                            // reduction is possible
                            if ( integer % 10 == 0 ) {
                                integer /= 10;
                                --ex;
                            }
                            else
                                break;
                        }
                        if ( ex ) {
                            real = static_cast<long double>( integer );
                            isReal = true;
                        }
                    }
                    else {
                        while ( exponent ) {
                            integer *= 10;
                            --exponent;
                        }
                    }
                }
                // not else!
                if ( isReal ) {
                    void( *op )( long double & ) = []( long double &r ) { r *= 10; };
                    if ( isExponentMinus )
                        op = []( long double &r ) { r /= 10; };

                    while ( exponent ) {
                        op( real );
                        --exponent;
                    }
                }
            }

            if ( isReal ) {
                token = Token( Token::Type::RealNumber, rawToken, before );
                if ( isMinus )
                    real *= -1;
                token.real() = real;
            }
            else {
                token = Token( Token::Type::Integer, rawToken, before );
                if ( isMinus )
                    integer *= -1;
                token.integer() = integer;
            }
        }
    }
}

