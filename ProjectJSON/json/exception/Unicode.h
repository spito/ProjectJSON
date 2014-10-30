#pragma once

#include "Exception.h"
#include <string>

namespace json {
    namespace exception {

        class Unicode : public Exception {
            std::string _msg;
        protected:
            Unicode( const char *prefix, const char *msg ) :
                Exception( "" ),
                _msg( prefix )
            {
                _msg += msg;
            }
        public:
            Unicode( const char *msg ) :
                Exception( "" ),
                _msg( msg )
            {}

            const char *what() const JSON_NOEXCEPT override{
                return _msg.c_str();
            }
            Position position() const JSON_NOEXCEPT override{
                return Position::nowhere();
            }

        };

        class UnicodeEncoding : public Unicode {
            Position _position;
        public:
            UnicodeEncoding( const Unicode &ex, Position p ) :
                Unicode( "Unicode encoding error - ", ex.what() ),
                _position( std::move( p ) )
            {}

            UnicodeEncoding( const char *msg, Position p ) :
                Unicode( "Unicode encoding error - ", msg ),
                _position( std::move( p ) )
            {}

            Position position() const JSON_NOEXCEPT override{
                return _position;
            }
        };

        class UnicodeDecoding : public Unicode {
            Position _position;
        public:
            UnicodeDecoding( const Unicode &ex, Position p ) :
                Unicode( "Unicode decoding error - ", ex.what() ),
                _position( std::move( p ) )
            {}

            Position position() const JSON_NOEXCEPT override{
                return _position;
            }
        };
    }
}