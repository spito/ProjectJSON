#pragma once

#include "Exception.h"

namespace json {
    namespace exception {

        class InvalidFile : public Exception {
        public:
            InvalidFile() :
                Exception( "invalid input file" )
            {}

            virtual Position position() const JSON_NOEXCEPT override {
                return Position::nowhere();
            }

        };
    }
}
