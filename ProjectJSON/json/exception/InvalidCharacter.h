#pragma once

#include "Exception.h"
#include "../Unicode.h"

#include <string>

namespace json {
    namespace exception {

        class InvalidCharacter : public Exception {
            std::string _given;
            std::string _expected;
            Position _position;
        protected:
            InvalidCharacter( std::string given, std::string expected, Position p ) :
                Exception( "invalid character" ),
                _given( std::move( given ) ),
                _expected( std::move( expected ) ),
                _position( std::move( p ) )
            {}
        public:
            InvalidCharacter( char given, std::string expected, Position p ) :
                Exception( "invalid character" ),
                _expected( std::move( expected ) ),
                _position( std::move( p ) )
            {
                if ( std::isprint( given ) )
                    _given = given;
                else {
                    char c[ 3 ] = {0,};
                    Unicode::fromCharToHex( given, c[ 0 ], c[ 1 ] );
                    _given = "0x";
                    _given += c;
                }
            }

            InvalidCharacter( char given, char expected, Position p ) :
                InvalidCharacter( given, std::string( 1, expected ), std::move( p ) )
            {}

            InvalidCharacter( char given, Position p ) :
                InvalidCharacter( given, "", std::move( p ) )
            {}

            virtual const std::string &given() const JSON_NOEXCEPT {
                return _given;
            }
            virtual const std::string &expected() const JSON_NOEXCEPT {
                return _expected;
            }
            Position position() const JSON_NOEXCEPT override {
                return _position;
            }
        };
    }
}
