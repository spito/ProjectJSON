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


    objects::Handle Factory::parseJson( const std::string &json ) {
        parser::Parser p( json );

        return invokeParsing( p );
    }

    objects::Handle Factory::parseFile( const std::string &name ) {
        std::ifstream file( name.c_str() );

        return parseFile( file );
    }
    
    objects::Handle Factory::parseFile( std::ifstream &file ) {
        parser::Parser p( file );

        return invokeParsing( p );
    }
    
    objects::Handle Factory::invokeParsing( parser::Parser &p ) {

        objects::Handle handle;

        try {
            handle = p.getTree();
        }
        catch ( std::bad_alloc & ) {
            errorMessage( exception::NoMemory( p.position() ) );
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

