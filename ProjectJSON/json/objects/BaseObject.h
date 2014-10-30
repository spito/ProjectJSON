#pragma once

#include "Type.h"
#include "Charset.h"
#include "../exceptions.h"

#include <memory>
#include <string>

namespace json {
    namespace objects {

        class BaseObject {
            Type _type;
        protected:
            BaseObject( Type t ) :
                _type( t )
            {}
        public:
            virtual ~BaseObject() {}

            bool is( Type t ) const {
                return _type == t;
            }

            bool is( const BaseObject &o ) const {
                return _type == o._type;
            }

            Type type() const {
                return _type;
            }

            bool isNull() const {
                return is( Type::Null );
            }

            template< typename Target >
            Target *cast() {
                return dynamic_cast<Target *>( this );
            }

            virtual std::string toString( Charset = Charset::ASCII ) const = 0;
        };

        using Handle = std::unique_ptr < BaseObject >;
        using Ptr = BaseObject *;
    }
}
