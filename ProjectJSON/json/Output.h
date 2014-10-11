#pragma once

#include <iostream>
#include <string>
#include "objects.h"

namespace json {
    
    class Output {
        std::ostream &_out;
        const objects::Ptr _ptr;
        const std::string _padding;
    public:
        Output( std::ostream &out, const objects::Ptr ptr ) :
            _out( out ),
            _ptr( ptr ),
            _padding( "    " )
        {}

        void flush() {
            flushLayer( _ptr, 0 );
        }
    private:
        void flushLayer( const objects::Ptr ptr, int layer ) {
            if ( auto p = ptr->cast< objects::Array >() )
                flushArray( p, layer );
        }

        void flushArray( const objects::Array *a, int layer ) {
            padding( layer );
            _out << "[";
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
