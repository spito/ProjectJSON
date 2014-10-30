#pragma once

#include "BaseObject.h"

#include <sstream>

namespace json {
    namespace objects {

        class Integer : public BaseObject {
            long long _value;
        public:
            Integer( long long v = 0 ) :
                BaseObject( Type::Integer ),
                _value( v )
            {}

            std::string toString( Charset = Charset::ASCII ) const override {
                std::ostringstream s;
                s << value();
                return s.str();
            }

            long long &value() {
                return _value;
            }
            long long value() const {
                return _value;
            }
        };
    }
}