#pragma once

#include <iostream>
#include <string>
#include "objects.h"
#include "Unicode.h"

namespace json {
    
    class Output {
        std::ostream &_out;
        objects::ConstPtr _ptr;
        const std::string _padding;
        Charset _charset;
    public:
        Output( std::ostream &out, objects::ConstPtr ptr, Charset charset = Charset::ASCII ) :
            _out( out ),
            _ptr( ptr ),
            _padding( "    " ),
            _charset( charset )
        {}

        Output( const Output & ) = delete;
        Output &operator=( const Output & ) = delete;

        void flush() {
            flushLayer( _ptr, 0 );
        }
    private:
        void flushLayer( objects::ConstPtr ptr, int layer, bool aplyLayerNow = true ) {
            if ( aplyLayerNow )
                padding( layer );

            switch ( ptr->type() ) {
            case objects::Type::Array:
                flushArray( ptr->cast< objects::Array >(), layer );
                break;
            case objects::Type::Bool:
                flushBool( ptr->cast< objects::Bool >() );
                break;
            case objects::Type::Integer:
                flushInteger( ptr->cast< objects::Integer >() );
                break;
            case objects::Type::Null:
                flushNull();
                break;
            case objects::Type::Object:
                flushObject( ptr->cast< objects::Object >(), layer );
                break;
            case objects::Type::RealNumber:
                flushRealNumber( ptr->cast< objects::RealNumber >() );
                break;
            case objects::Type::String:
                flushString( ptr->cast< objects::String >() );
                break;
            }
        }

        void flushArray( const objects::Array *a, int layer ) {
            _out << "[" << std::endl;

            int i = 0;
            for ( const auto &handle : *a ) {
                if ( i++ )
                    _out << "," << std::endl;
                flushLayer( handle.get(), layer + 1 );
            }
            _out << std::endl;
            padding( layer );
            _out << "]";
        }

        void flushBool( const objects::Bool *b ) {
            _out << (b->value() ? "true" : "false");
        }

        void flushNull() {
            _out << "null";
        }

        void flushInteger( const objects::Integer *i ) {
            _out << i->value();
        }

        void flushObject( const objects::Object *o, int layer ) {
            _out << "{" << std::endl;

            int i = 0;
            for ( const auto &handle : *o ) {
                if ( i++ )
                    _out << "," << std::endl;
                padding( layer + 1 );
                flushString( &handle.first );
                _out << " : ";
                flushLayer( handle.second.get(), layer + 1, false );
            }
            _out << std::endl;
            padding( layer );
            _out << "}";
        }

        void flushRealNumber( const objects::RealNumber *r ) {
            _out << r->value();
        }

        void flushString( const objects::String *s ) {
            _out << s->toString( _charset );
        }

        void padding( int layer ) {
            for ( int i = 0; i < layer; ++i )
                _out << _padding;
        }
    };
}
inline std::ostream &operator<<( std::ostream &out, const json::objects::Handle &handle ) {
    if ( handle ) {
        json::Output( out, handle.get() ).flush();
    }
    return out;
}

