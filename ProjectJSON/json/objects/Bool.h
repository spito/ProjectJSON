#pragma once

#include "BaseObject.h"

namespace json {
    namespace objects {
    
        class Bool : public BaseObject {
            bool _value;
        public:
            Bool( bool value ) :
                BaseObject( Type::Bool ),
                _value( value )
            {}

            std::string toString( Charset = Charset::ASCII ) const override {
                return value() ? "true" : "false";
            }

            bool value() const {
                return _value;
            }
        };
    }
}
