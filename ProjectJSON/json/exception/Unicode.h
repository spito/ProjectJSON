#pragma once

#include "Exception.h"
#include <string>

namespace json {
    namespace exception {

        class UnicodeEncoding : public Exception {
            std::string _msg;
        public:
            UnicodeEncoding( const char *msg ) :
                Exception( "" ),
                _msg( "Unicode encoding error - " )
            {
                _msg += msg;
            }

            const char *what() const JSON_NOEXCEPT override{
                return _msg.c_str();
            }

            Position position() const JSON_NOEXCEPT override{
                return Position::nowhere();
            }
        };

        class UnicodeDecoding : public Exception {
            std::string _msg;
        public:
            UnicodeDecoding( const char *msg ) :
                Exception( "" ),
                _msg( "Unicode decoding error - " )
            {
                _msg += msg;
            }

            const char *what() const JSON_NOEXCEPT override{
                return _msg.c_str();
            }

            Position position() const JSON_NOEXCEPT override{
                return Position::nowhere();
            }
        };
    }
}