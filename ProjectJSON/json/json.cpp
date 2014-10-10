#include "json.h"

#include <iostream>

namespace json {

    void Factory::errorMessage( const exception::Exception &ex ) {
        _out << "Line: " << ex.position().line() << ", column: " <<
            ex.position().column() << "\tParse error: " << ex.what() <<
            '.' << std::endl;
    }

    template< typename Given, typename Expected >
    void Factory::errorMessage( const exception::Exception &ex, Given given, Expected expected ) {
        errorMessage( ex );

        _out << "\tExpected: ";

        int i = 0;
        for ( auto item : expected ) {
            if ( i++ )
                _out << ", ";
            _out << '`' << item << '\'';
        }
        if ( !i )
            _out << "almost everything";
        _out << ", but got `" << given << "'." << std::endl;
    }


    Handle Factory::parseJson( const std::string &json ) {
        return invokeParsing( json );
    }

    Handle Factory::parseFile( const std::string &name ) {
        std::ifstream file( name.c_str() );

        return invokeParsing( file );
    }
    
    Handle Factory::parseFile( std::ifstream &file ) {
        return invokeParsing( file );
    }
    
    template< typename T >
    Handle Factory::invokeParsing( T &t ) {

        Handle handle;
        std::unique_ptr< parser::Parser > p;
        try {
            p.reset( new parser::Parser( t ) );
            handle = p->getTree();
        }
        catch ( std::bad_alloc & ) {
            errorMessage( exception::NoMemory( p ? p->position() : Position() ) );
        }
        catch ( exception::InvalidCharacter &ex ) {
            errorMessage(
                ex,
                ex.given(),
                ex.expected() );
        }
        catch ( exception::InvalidToken &ex ) {
            errorMessage(
                ex,
                ex.given(),
                ex.expected()
                );
        }
        catch ( exception::Exception &ex ) {
            errorMessage( ex );
        }
        catch ( ... ) {
            std::cerr << "Internal parser error." << std::endl;
        }
        return handle;
    }
}

