#include <iostream>
#include <string>
#include "json/json.h"

int main( int argc, char **argv ) {
    try {
        if ( argc < 2 ) {
            std::cout << "Nothing to parse." << std::endl;
            return 0;
        }

        json::Factory factory( std::cerr );

        auto handle = factory.parseFile( argv[ 1 ] );

        if ( !handle || handle->isNull() )
            return -1;

        std::cout << "Dump of the JSON structure: " << std::endl;
        std::cout << handle << std::endl;
    }
    catch ( std::bad_alloc & ) {
        std::cerr << "Allocation has failed!" << std::endl;
    }
    return 0;
}
