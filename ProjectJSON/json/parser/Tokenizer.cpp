
#include "Tokenizer.h"
#include "../exceptions.h"
#include "NumberParser.h"

#include <string>
#include <sstream>
#include <cstring>

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
            size_t size = std::strlen( string );

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
        void Tokenizer::processNumber( Token &token ) {

            Position before = position();
            NumberParser p( _input );

            p.run();

            if ( p.isReal() ) {
                token = Token( Token::Type::RealNumber, p.token(), before );
                token.real() = p.real();
            }
            else {
                token = Token( Token::Type::Integer, p.token(), before );
                token.integer() = p.integer();
            }
        }
    }
}

