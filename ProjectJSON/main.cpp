#include <iostream>
#include <string>

#include "tests.h"
#include "json/json.h"

int main( int argc, char **argv ) {


    test::Test().tokenizer();


    json::Factory factory( std::cerr );

    auto handle = factory.parseFile( "test1.json" );

    if ( !handle )
        return -1;

    std::cout << handle->toString() << std::endl;

    return 0;
}