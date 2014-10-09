#pragma once

#include "../Position.h"

#include <stdexcept>

namespace json {
    namespace exception {

        class Exception : public std::runtime_error {
        protected:
            Exception( const char *msg ) :
                std::runtime_error( msg )
            {}
        public:
            virtual ~Exception(){}
            virtual const Position &position() const = 0;
        };
    }
}
