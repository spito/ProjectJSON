#pragma once

#include "Exception.h"

#include <string>

namespace json {
    namespace exception {

        class UnsupportedFeature : public Exception {
            Position _position;
            std::string _msg;
        public:
            UnsupportedFeature( const char *msg, Position p ) :
                Exception( "" ),
                _position( std::move( p ) ),
                _msg( "unsupported feature - " )
            {
                _msg += msg;
            }

            const Position &position() const override {
                return _position;
            }

            const char *what() const override {
                return _msg.c_str();
            }
        };
    }
}
