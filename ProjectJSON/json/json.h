#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "objects.h"
#include "parser/Parser.h"

namespace json {

    using Handle = objects::Handle;

    class Factory {
        std::ostream &_out;
    public:
        Factory( std::ostream &out ) :
            _out( out )
        {}

        Handle parseJson( const std::string & );
        Handle parseFile( const std::string & );
        Handle parseFile( std::ifstream & );
    private:
        template< typename T >
        Handle invokeParsing( T & );

        void errorMessage( const exception::Exception & );

        template< typename Given, typename Expected >
        void errorMessage( const exception::Exception &, Given, Expected );
    };
}

#ifdef JSON_NOEXCEPT
#undef JSON_NOEXCEPT
#endif
