#define JSON_TEST

#include <iostream>
#include <vector>
#include <string>

#include "catch.h"

#include "tests.h"
#include "json/parser/Tokenizer.h"

namespace test {

    template< typename T >
    T abs( T value ) {
        return value < 0 ? value * -1 : value;
    }

    Token tokenize( std::string json, bool exceptionExpected = false ) {
        auto tokenizer = ::json::parser::Tokenizer( json );
        Token token;
        try {
            tokenizer.lookAtToken( token );
        }
        catch ( Exception &ex ) {
            if ( exceptionExpected ) throw;
            WARN( std::string( "" ) + "exception raised while tokenizing this input:\n" + json );
            return Token();
        }
        return token;
    }

    bool tokenEquality( const Token &given, const Token &expected ) {
        if ( given != expected ) {
            std::cout << "Token mismatch! Got " << given << ", expected " << expected << "." << std::endl;
            return false;
        }
        return true;
    }
}  // namespace test

using ::json::Position;
using ::json::parser::Token;
using ::json::exception::Exception;
using ::test::tokenize;
using ::test::tokenEquality;

TEST_CASE( "Basic tokenizer tests", "[tokenizer]" ) {
    CHECK( tokenEquality( tokenize( "[" ), Token( Token::Type::LeftBracket, "[" ) ) );
    CHECK( tokenEquality( tokenize( "]" ), Token( Token::Type::RightBracket, "]" ) ) );
    CHECK( tokenEquality( tokenize( "{" ), Token( Token::Type::LeftBrace, "{" ) ) );
    CHECK( tokenEquality( tokenize( "}" ), Token( Token::Type::RightBrace, "}" ) ) );
    CHECK( tokenEquality( tokenize( ":" ), Token( Token::Type::Colon, ":" ) ) );
    CHECK( tokenEquality( tokenize( "  ," ), Token( Token::Type::Comma, ",", Position( 2, 1, 3 ) ) ) );
    CHECK( tokenEquality( tokenize( "true" ), Token( Token::Type::True, "true" ) ) );
    CHECK( tokenEquality( tokenize( "   true" ), Token( Token::Type::True, "true", Position( 3, 1, 4 ) ) ) );
    CHECK( tokenEquality( tokenize( "false" ), Token( Token::Type::False, "false" ) ) );
    CHECK( tokenEquality( tokenize( "\nfalse" ), Token( Token::Type::False, "false", Position( 1, 2, 1 ) ) ) );
    CHECK( tokenEquality( tokenize( "null" ), Token( Token::Type::Null, "null" ) ) );
    CHECK( tokenEquality( tokenize( "  null:" ), Token( Token::Type::Null, "null", Position( 2, 1, 3 ) ) ) );
}

TEST_CASE( "String tokenizer tests", "[tokenizer]" ) {
    Token::Type t = Token::Type::String;

    CHECK( tokenEquality( tokenize( "\"\"" ), Token( t, "" ) ) );
    CHECK( tokenEquality( tokenize( "\"tralala\"" ), Token( t, "tralala" ) ) );
    CHECK( tokenEquality( tokenize( "\"\\n\"" ), Token( t, "\n" ) ) );
    CHECK( tokenEquality( tokenize( "\"\\t\"" ), Token( t, "\t" ) ) );
    CHECK( tokenEquality( tokenize( "\" \"" ), Token( t, " " ) ) );
    // CHECK(tokenEquality(tokenize("\"\\u00FD\"", false), Token(t, "\u00fd")));
    /* TODO add support for multiple tokens
    CHECK(tokenEquality(tokenize("\"true\"true", false), std::vector< Token >{
    Token(t, "true"),
    Token(Token::Type::True, "true", Position(6, 1, 7))
    }));
    */
}

TEST_CASE( "Integer tokenizer tests", "[tokenizer]" ) {
    Token::Type t = Token::Type::Integer;

    auto token = tokenize( "0" );
    CHECK( tokenEquality( token, Token( t, "0" ) ) );
    CHECK( token.integer() == 0 );

    token = tokenize( "42" );
    CHECK( tokenEquality( token, Token( t, "42" ) ) );
    CHECK( token.integer() == 42 );

    token = tokenize( "1e2" );
    CHECK( tokenEquality( token, Token( t, "1e2" ) ) );
    CHECK( token.integer() == 100 );

    token = tokenize( "100e-1" );
    CHECK( tokenEquality( token, Token( t, "100e-1" ) ) );
    CHECK( token.integer() == 10 );

    token = tokenize( "100e-2" );
    CHECK( tokenEquality( token, Token( t, "100e-2" ) ) );
    CHECK( token.integer() == 1 );

    token = tokenize( "  -4" );
    CHECK( tokenEquality( token, Token( t, "-4", Position( 2, 1, 3 ) ) ) );
    CHECK( token.integer() == -4 );
}

TEST_CASE( "Reals tokenizer tests", "[tokenizer]" ) {
    Token::Type t = Token::Type::RealNumber;

    auto token = tokenize( "0.0" );
    CHECK( tokenEquality( token, Token( t, "0.0" ) ) );
    CHECK( token.real() == 0.0 );

    token = tokenize( "1e-1" );
    CHECK( tokenEquality( token, Token( t, "1e-1" ) ) );
    CHECK( abs( token.real() - 0.1 ) <= 1e-6 );
}
