#pragma once

#include "InputBuffer.h"
#include "Token.h"

namespace json {
    namespace parser {

        class Tokenizer {
            InputBuffer _input;
        public:
            Tokenizer( const std::string &json ) :
                _input( json )
            {}

            Tokenizer( std::ifstream &file ) :
                _input( file )
            {}

            // 
            // if return value is true => destroyable reading
            // if return value is false => reading do not change the buffer
            bool isTokenType( Token::Type, Token & );

            bool Tokenizer::isTokenType( Token::Type type ) {
                Token token;
                return isTokenType( type, token );
            }

            // destroying
            void readToken( Token & );

            // non destroying
            void lookAtToken( Token & );

            Position position() const {
                return _input.position();
            }

        private:

            void position( Position p ) {
                return _input.position( std::move( p ) );
            }

            bool isBadChar( char );

            void getToken( Token & );

            void processString( Token & );
            void processNumber( Token & );
            void processWord( Token::Type, Token & );
            std::string processUnicode();// TODO: implement
        };
    }
}
