#pragma once

#include "BaseObject.h"
#include "String.h"

#include <map>

namespace json {
    namespace objects {

        class Object : public BaseObject {
            std::map< String, Handle > _objects;
        public:
            Object() :
                BaseObject( Type::Object )
            {}

            std::string toString( Charset charset = Charset::ASCII ) const override {
                std::string result( "{ " );
                int i = 0;
                for ( const auto &p : _objects ) {
                    if ( i++ )
                        result += ", ";
                    result += p.first.toString( charset );
                    result += " : ";
                    result += p.second->toString( charset );
                }
                return result + " }";
            }

            Ptr operator[]( const String &s ) {
                return _objects[ s ].get();
            }

            const Ptr operator[]( const String &s ) const {
                auto item = _objects.find( s );
                if ( item == _objects.end() )
                    return nullptr;
                return item->second.get();
            }

            void insert( std::pair< String, Handle > &&pair ) {
                _objects.insert( std::move( pair ) );
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