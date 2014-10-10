#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "objects.h"
#include "parser/Parser.h"

namespace json {

    class Factory {
        std::ostream &_out;
    public:
        Factory( std::ostream &out ) :
            _out( out )
        {}

        objects::Handle parseJson( const std::string & );
        objects::Handle parseFile( const std::string & );
        objects::Handle parseFile( std::ifstream & );
    private:
        objects::Handle invokeParsing( parser::Parser & );

        void errorMessage( const exception::Exception & );

        template< typename Given, typename Expected >
        void errorMessage( const exception::Exception &, Given, Expected );
    };
}

#ifdef JSON_NOEXCEPT
#undef JSON_NOEXCEPT
#endif
