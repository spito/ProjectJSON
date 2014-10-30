#pragma once

#include "BaseObject.h"
#include "../Unicode.h"

#include <string>

namespace json {
    namespace objects {

        // This class represents unicode string using UTF-8 encoding.
        class String : public BaseObject {
            std::string _value;
        public:
            String( std::string v = std::string() ) :
                BaseObject( Type::String ),
                _value( v )
            {}

            String( const String &o ) = default;
            String( String &&o ) :
                BaseObject( Type::String ),
                _value( std::move( o._value ) )
            {}

            String &operator=( String o ) {
                swap( o );
                return *this;
            }

            std::string toString( Charset charset = Charset::ASCII ) const override {
                switch ( charset ) {
                case Charset::UTF8:
                    return '"' + Unicode::escapeSpecials( value() ) + '"';
                case Charset::ASCII:
                default:
                    return '"' + Unicode::toAscii( value() ) + '"';
                }
            }

            std::string &value() {
                return _value;
            }

            const std::string &value() const {
                return _value;
            }

            void swap( String &o ) {
                using std::swap;
                swap( _value, o._value );
            }

            bool operator==( const String &o ) const {
                return value() == o.value();
            }

            bool operator!=( const String &o ) const {
                return value() != o.value();
            }

            bool operator<( const String &o ) const {
                return value() < o.value();
            }
            bool operator<=( const String &o ) const {
                return value() <= o.value();
            }
            bool operator>( const String &o ) const {
                return value() > o.value();
            }
            bool operator>=( const String &o ) const {
                return value() >= o.value();
            }
        };
    }
}