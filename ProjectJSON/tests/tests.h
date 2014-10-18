#pragma once

#include <string>
#include <vector>

#include "json/parser/Token.h"
#include "json/exceptions.h"

namespace test {
    using ::json::parser::Token;
    using ::json::exception::Exception;

    Token tokenize(std::string json, bool exceptionExpected);
    bool tokenEquality(const Token &given, const Token &expected);
} // end namespace test
