#pragma once

#include "InvalidCharacter.h"

namespace json {
    namespace exception {
    
        class EndOfFile : public InvalidCharacter {
        public:
            EndOfFile( std::string expected, Position p ) :
                InvalidCharacter( "EOF", std::move( expected ), std::move( p ) )
            {}
        };
    }
}
