#pragma once

#include "Exception.h"

namespace json {
    namespace exception {

        class NoMemory : public Exception {
            Position _position;
        public:
            NoMemory( Position p ) :
                Exception( "run out of memory" ),
                _position( std::move( p ) )
            {}

            const Position &position() const override {
                return _position;
            }
        };
    }
}
