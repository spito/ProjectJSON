#pragma once

#include "Exception.h"

#include <string>

namespace json {
    namespace exception {

        class InternalError : public Exception {
            std::string _msg;
            Position _position;
        public:
            InternalError( const std::string &msg, Position p ) :
                Exception( "" ),
                _msg( "internal error - " + msg ),
                _position( std::move( p ) )
            {}

            const char *what() const override {
                return _msg.c_str();
            }

            const Position &position() const override {
                return _position;
            }
        };
    }
}
