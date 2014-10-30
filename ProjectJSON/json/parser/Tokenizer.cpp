
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

        bool Tokenizer::processWord( const char* expected ) {
            for ( ; *expected; ++expected ) {
                Position p = position();
                char given = _input.read();
                if ( given != *expected ) {
                    _input.position( p );
                    return false;
                }
            }
            return true;
        }

        void Tokenizer::processWord( Token::Type type, Token &token ) {

            Position before = position();

            if ( processWord( Token::value( type ) ) )
                token = Token( type, before );
            else
                token = Token( Token::Type::Invalid, before );
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
                if ( _input.eof() ) {
                    throw exception::EndOfFile( "", position() );
                }
                Position p = position();
                char c = _input.read();

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
                        break;
                    default:
                        throw exception::InvalidCharacter( c, "\"\\/bfnrtu", p );
                    }
                    special = false;
                }
                else if ( c == '\\' )
                    special = true;
                else if ( c == '"' )
                    break;
                else if ( isBadChar( c ) )
                    throw exception::InvalidCharacter( c, p );
                else
                    rawToken.push_back( c );
            }
            token = Token( Token::Type::String, rawToken, before );
        }

        std::string Tokenizer::processUnicodeSequence() {
            std::string unicode;
            for ( int i = 0; i < 4; ++i ) {
                Position p = position();
                unicode += _input.read();
                if ( !std::isxdigit( unicode.back() ) )
                    throw exception::InvalidCharacter( unicode.back(), "0123456789abcdefABCDEF", p );
            }
            return unicode;
        }

        std::string Tokenizer::processUnicode() {
            Position before = position();
            try {

                std::string first = processUnicodeSequence();
                std::string second;

                if ( Unicode::isFirstPart( first ) ) {
                    Position before = position();
                    if ( !processWord( "\\u" ) )
                        throw exception::UnicodeEncoding( "Expected unicode in escape sequenced format", before );
                    second = processUnicodeSequence();

                    // roll back if needed
                    if ( !Unicode::isSecondPart( second ) ) {
                        _input.position( before );
                        second.clear();
                    }
                }

                return Unicode::getUTF8( first + second );
            }
            catch ( exception::Unicode &ex ) {
                throw exception::UnicodeEncoding( ex, before );
            }
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

