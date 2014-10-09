#pragma once

#include "BaseObject.h"

namespace json {
    namespace objects {

        class Null : public BaseObject {
        public:
            Null() :
                BaseObject( Type::Null )
            {}

            std::string toString() const override {
                return "null";
            }
        };
    }
}
