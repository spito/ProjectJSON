#pragma once

#include "../Position.h"

#include <stdexcept>

#ifdef _MSC_VER
#define JSON_NOEXCEPT
#else
#define JSON_NOEXCEPT noexcept
#endif

namespace json {
    namespace exception {

        class Exception : public std::runtime_error {
        protected:
            Exception( const char *msg ) :
                std::runtime_error( msg )
            {}
        public:
            virtual ~Exception(){}
            virtual Position position() const JSON_NOEXCEPT = 0;
        };
    }
}
