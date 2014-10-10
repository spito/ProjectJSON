#pragma once

#include "Exception.h"
#include "../parser/Token.h"

#include <vector>

namespace json {
    namespace exception {

        class InvalidToken : public Exception {
            parser::Token _given;
            std::vector< parser::Token::Type > _expected;
        public:
            InvalidToken( parser::Token given, std::vector< parser::Token::Type > expected ) :
                Exception( "invalid token" ),
                _given( std::move( given ) ),
                _expected( std::move( expected ) )
            {}

            InvalidToken( parser::Token given, parser::Token::Type expected ) :
                InvalidToken( std::move( given ), std::vector< parser::Token::Type >() )
            {
                _expected.emplace_back( std::move( expected ) );
            }

            const parser::Token &given() const JSON_NOEXCEPT {
                return _given;
            }

            const Position &position() const JSON_NOEXCEPT override{
                return _given.position();
            }

            const std::vector< parser::Token::Type > &expected() const JSON_NOEXCEPT {
                return _expected;
            }
        };
    }
}
