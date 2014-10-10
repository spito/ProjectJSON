#pragma once

#include "Exception.h"

namespace json {
    namespace exception {

        class InvalidFile : public Exception {
            Position _position;
        public:
            InvalidFile() :
                Exception( "invalid input file" )
            {}

            virtual const Position &position() const JSON_NOEXCEPT override {
                return _position;
            }

        };
    }
}
