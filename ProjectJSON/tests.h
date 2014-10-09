#pragma once

#include <string>
#include <vector>

#include "json/parser/Token.h"
#include "json/exceptions.h"

namespace test {

    using ::json::parser::Token;
    using ::json::exception::Exception;

    struct Test {


        bool reportProblem( const std::string &, Token &, Token & );
        bool reportProblem( Exception & );

        void tokenizer();

        Token testCase( std::string, Token & );
        bool testCase( std::string, std::vector< Token > & );
        bool testCase( std::string, Exception & );
    };


}
