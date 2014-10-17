#define JSON_TEST

#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "tests.h"
#include "json/parser/Tokenizer.h"

#include <iostream>

namespace test {

    template< typename T >
    T abs( T value ) {
        return value < 0 ? value * -1 : value;
    }

    Token tokenize( std::string json, const Token &expected ) {      
        auto tokenizer = ::json::parser::Tokenizer( json );
        Token token;
        try {
            tokenizer.lookAtToken( token );
            if ( token != expected )
                reportProblem( json, token, expected );
        }
        catch ( Exception & ) {
            WARN(std::string("") + "exception raised while tokenizing this input:\n" + json);
            return Token();
        }
        return token;
    }

    bool testCase( std::string json, const Token &expected ) {
        auto tokenizer = ::json::parser::Tokenizer( json );
        Token token;
        try {
            tokenizer.lookAtToken( token );
            if ( token != expected )
                return reportProblem( json, token, expected );
        }
        catch ( Exception & ex ) {
            WARN(std::string("") + "exception raised while tokenizing this input:\n" + json + "\n");
            return false;
        }
        return true;
    }

    bool testCase( std::string json, const std::vector< Token > &tokens ) {

        auto tokenizer = ::json::parser::Tokenizer( json );
        Token token;
        try {
            for ( auto &t : tokens ) {
                tokenizer.readToken( token );
                if ( token != t )
                    return reportProblem( json, token, t );
            }
        }
        catch ( Exception & ) {
            WARN(std::string("") + "exception raised while tokenizing this input:\n" + json + "\n");
            return false;
        }
        return true;
    }

    bool testCase( std::string json, Exception &ex ) {
        return false;
    }

    bool reportProblem( const std::string &json, const Token &given, const Token &expected ) {
        std::cerr << "json: " << std::endl <<
            '\t' << json << std::endl << std::endl <<
            "given token:" << std::endl <<
            '\t' << std::string( given.begin(), given.end() ) << std::endl <<
            "expected token:" << std::endl <<
            '\t' << std::string( expected.begin(), expected.end() ) << std::endl;
        return false;
    }
}

using ::json::Position;
using ::json::parser::Token;
using ::json::exception::Exception;
using ::test::testCase;
using ::test::tokenize;

TEST_CASE("Basic tokenizer tests", "[tokenizer]") {
    CHECK(testCase( "[", Token( Token::Type::LeftBracket, "[" ) ));
    CHECK(testCase( "]", Token( Token::Type::RightBracket, "]" ) ));
    CHECK(testCase( "{", Token( Token::Type::LeftBrace, "{" ) ));
    CHECK(testCase( "}", Token( Token::Type::RightBrace, "}" ) ));
    CHECK(testCase( ":", Token( Token::Type::Colon, ":" ) ));
    CHECK(testCase( "  ,", Token( Token::Type::Comma, ",", Position( 2, 1, 3 ) ) ));
    CHECK(testCase( "true", Token( Token::Type::True, "true" ) ));
    CHECK(testCase( "   true", Token( Token::Type::True, "true", Position( 3, 1, 4 ) ) ));
    CHECK(testCase( "false", Token( Token::Type::False, "false" ) ));
    CHECK(testCase( "\nfalse", Token( Token::Type::False, "false", Position( 1, 2, 1 ) ) ));
    CHECK(testCase( "null", Token( Token::Type::Null, "null" ) ));
    CHECK(testCase( "  null:", Token( Token::Type::Null, "null", Position( 2, 1, 3 ) ) ));
}

TEST_CASE("String tokenizer tests", "[tokenizer]") {
    Token::Type t = Token::Type::String;

    CHECK(testCase( "\"\"", Token( t, "" ) ));
    CHECK(testCase( "\"tralala\"", Token( t, "tralala" ) ));
    CHECK(testCase( "\"\\n\"", Token( t, "\n" ) ));
    CHECK(testCase( "\"\\t\"", Token( t, "\t" ) ));
    CHECK(testCase( "\" \"", Token( t, " " ) ));
    //CHECK(testCase( "\"\\u00FD\"", Token( t, "\u00fd" ) ));
    CHECK(testCase( "\"true\"true", std::vector< Token >{
        Token( t, "true" ),
        Token( Token::Type::True, "true", Position( 6, 1, 7 ) )
    } ));
}

TEST_CASE("Integer tokenizer tests", "[tokenizer]") {
    Token::Type t = Token::Type::Integer;

    auto token = tokenize( "0", Token( t, "0" ) );
    CHECK( token.integer() == 0 );

    token = tokenize( "42", Token( t, "42" ) );
    CHECK( token.integer() == 42 );

    token = tokenize( "1e2", Token( t, "1e2" ) );
    CHECK( token.integer() == 100 );

    token = tokenize( "100e-1", Token( t, "100e-1" ) );
    CHECK( token.integer() == 10 );

    token = tokenize( "100e-2", Token( t, "100e-2" ) );
    CHECK( token.integer() == 1 );

    token = tokenize( "  -4", Token( t, "-4", Position( 2, 1, 3 ) ) );
    CHECK( token.integer() == -4 );
}

TEST_CASE("Reals tokenizer tests", "[tokenizer]") {
    Token::Type t = Token::Type::RealNumber;

    auto token = tokenize( "0.0", Token( t, "0.0" ) );
    CHECK( token.real() == 0.0 );

    token = tokenize( "1e-1", Token( t, "1e-1" ) );
    CHECK( abs( token.real() - 0.1 ) <= 1e-6 );
}
