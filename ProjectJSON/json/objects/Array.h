#pragma once

#include "BaseObject.h"

#include <vector>

namespace json {
    namespace objects {

        class Array : public BaseObject {
            std::vector< Handle > _objects;
        public:
            Array() :
                BaseObject( Type::Array )
            {}

            std::string toString( Charset charset = Charset::ASCII ) const override {
                std::string s( "[ " );
                int i = 0;
                for ( const auto &o : _objects ) {
                    if ( i++ )
                        s += ", ";
                    s += o->toString( charset );
                }
                return s + " ]";
            }

            void pushBack( Handle &&o ) {
                _objects.emplace_back( std::move( o ) );
            }

            size_t size() const {
                return _objects.size();
            }

            Ptr operator[]( size_t index ) {
                return _objects[ index ].get();
            }
            const Ptr operator[]( size_t index ) const {
                return _objects[ index ].get();
            }

            auto begin()
                -> decltype( _objects.begin() )
            {
                return _objects.begin();
            }
            auto end()
                -> decltype( _objects.end() )
            {
                return _objects.end();
            }

            auto begin() const
                -> decltype( _objects.begin() )
            {
                return _objects.begin();
            }
            auto end() const
                -> decltype( _objects.end() )
            {
                return _objects.end();
            }
        };
    }
}