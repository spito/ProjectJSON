#pragma once

#include "BaseObject.h"

#include <sstream>

namespace json {
    namespace objects {

        class RealNumber : public BaseObject {
            long double _value;
        public:
            RealNumber( long double d = 0.0 ) :
                BaseObject( Type::RealNumber ),
                _value( d )
            {}

            std::string toString( Charset = Charset::ASCII ) const override {
                std::ostringstream s;
                s << value();
                return s.str();
            }

            long double &value() {
                return _value;
            }
            long double value() const {
                return _value;
            }
        };
    }
}