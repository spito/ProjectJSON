#pragma once

#include <string>
#include <vector>

#include "json/parser/Token.h"
#include "json/exceptions.h"

namespace test {

    using ::json::parser::Token;
    using ::json::exception::Exception;

    bool reportProblem( const std::string &, const Token &, const Token & );
    bool reportProblem( const Exception & );

    Token tokenize( std::string, const Token & );
    bool testCase( std::string, const Token & );
    bool testCase( std::string, const std::vector< Token > & );
    bool testCase( std::string, Exception & );

}