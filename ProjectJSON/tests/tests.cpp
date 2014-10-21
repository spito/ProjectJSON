#include "test-utils.h"

using ::json::Position;
using ::json::parser::Token;
using ::json::exception::Exception;
using ::test::tokenize;
using ::test::tokenEquality;
using ::test::approx;
using ::test::exceptionEquality;
using ::test::exceptionRaised;

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
    CHECK( token.real() == approx( 0.0 ) );

    token = tokenize( "1e-1" );
    CHECK( tokenEquality( token, Token( t, "1e-1" ) ) );
    CHECK( token.real() == approx( 0.1 ) );

    token = tokenize( "0.42e2" );
    CHECK( tokenEquality( token, Token( t, "0.42e2" ) ) );
    CHECK( token.real() == approx( 42 ) );

    token = tokenize( "-0.42e2" );
    CHECK( tokenEquality( token, Token( t, "-0.42e2" ) ) );
    CHECK( token.real() == approx( -42 ) );
}

TEST_CASE( "Exceptions tests", "[exceptions]" ) {
    // check for strict exception equality
    CHECK( exceptionEquality( "whoou", ::json::exception::InvalidCharacter( 'w', ",:{}[]tfn\"-0123456789", Position() ) ) );
    // check only for type of the exception
    CHECK( exceptionRaised( "whoou", ::json::exception::InvalidCharacter( '-', Position() ) ) );


    CHECK( exceptionEquality( "\"", ::json::exception::EndOfFile( "", Position( 1, 1, 2 ) ) ) );
}