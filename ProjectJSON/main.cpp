#include <iostream>
#include <string>

#include "tests.h"
#include "json/json.h"

int main( int argc, char **argv ) {


    if ( argc < 2 ) {
        std::cout << "Nothing to parse." << std::endl;
        return 0;
    }

    if ( argv[ 1 ] == std::string( "test" ) )
        test::Test().tokenizer();

    else {
        json::Factory factory( std::cerr );

        auto handle = factory.parseFile( argv[ 1 ] );

        if ( !handle )
            return -1;

        std::cout << "Dump of the JSON structure: " << std::endl;
        std::cout << handle->toString() << std::endl;
    }
    return 0;
}