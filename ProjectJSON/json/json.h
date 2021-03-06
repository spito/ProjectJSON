#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "objects.h"
#include "parser/Parser.h"
#include "Output.h"

namespace json {

    using Handle = objects::Handle;

    class Factory {
        std::ostream &_out;
    public:
        Factory( std::ostream &out ) :
            _out( out )
        {}

        Factory( const Factory & ) = delete;
        Factory operator=( const Factory & ) = delete;

        Handle parseJson( const std::string & );
        Handle parseFile( const std::string & );
        Handle parseFile( std::ifstream & );

    private:
        template< typename T >
        Handle invokeParsing( T & );

        void errorMessage( const exception::Exception & );
        void errorMessage( const std::exception & );

        template< typename Given, typename Expected >
        void errorMessage( const exception::Exception &, Given, Expected );

    };
}

#ifdef JSON_NOEXCEPT
#undef JSON_NOEXCEPT
#endif
