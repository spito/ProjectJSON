#include "Token.h"

namespace json {
    namespace parser {

        const char *Token::_values[] = {
            ",",
            ":",
            "{",
            "}",
            "[",
            "]",
            "true",
            "false",
            "null",
        };

        const char *Token::_typeNames[] = {
            "tComma",
            "tColon",
            "tLeftBrace",
            "tRightBrace",
            "tLeftBracket",
            "tRightBracket",
            "tTrue",
            "tFalse",
            "tNull",
            "tString",
            "tInteger",
            "tRealNumber",
            "tEndOfFile"
        };

        const char *Token::value( Type type ) {
            switch ( type ) {
            case Type::Comma:
                return _values[ 0 ];
            case Type::Colon:
                return _values[ 1 ];
            case Type::LeftBrace:
                return _values[ 2 ];
            case Type::RightBrace:
                return _values[ 3 ];
            case Type::LeftBracket:
                return _values[ 4 ];
            case Type::RightBracket:
                return _values[ 5 ];
            case Type::True:
                return _values[ 6 ];
            case Type::False:
                return _values[ 7 ];
            case Type::Null:
                return _values[ 8 ];
            default:
                return "invalid";
            }
        }

        const char *Token::typeName( Type type ) {
            switch ( type ) {
            case Type::Comma:
                return _typeNames[ 0 ];
            case Type::Colon:
                return _typeNames[ 1 ];
            case Type::LeftBrace:
                return _typeNames[ 2 ];
            case Type::RightBrace:
                return _typeNames[ 3 ];
            case Type::LeftBracket:
                return _typeNames[ 4 ];
            case Type::RightBracket:
                return _typeNames[ 5 ];
            case Type::True:
                return _typeNames[ 6 ];
            case Type::False:
                return _typeNames[ 7 ];
            case Type::Null:
                return _typeNames[ 8 ];
            case Type::String:
                return _typeNames[ 9 ];
            case Type::Integer:
                return _typeNames[ 10 ];
            case Type::RealNumber:
                return _typeNames[ 11 ];
            case Type::EndOfFile:
                return _typeNames[ 12 ];
            default:
                return "tInvalid";
            }
        }
    }
}
