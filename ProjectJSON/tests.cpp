#define JSON_TEST

#include "tests.h"
#include "json/parser/Tokenizer.h"

#include <iostream>

namespace test {

    template< typename T >
    T abs( T value ) {
        return value < 0 ? value * -1 : value;
    }

    Token Test::testCase( std::string json, const Token &expected ) {
        
        auto tokenizer = ::json::parser::Tokenizer( json );

        Token token;
        try {
            tokenizer.lookAtToken( token );
            if ( token != expected )
                reportProblem( json, token, expected );
        }
        catch ( Exception & ) {
            return Token();
        }
        return token;
    }

    bool Test::testCase( std::string json, const std::vector< Token > &tokens ) {

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
            return false;
        }
        return true;
    }

    bool Test::testCase( std::string json, Exception &ex ) {
        return false;
    }

    bool Test::reportProblem( const std::string &json, const Token &given, const Token &expected ) {

        std::cerr << "json: " << std::endl <<
            '\t' << json << std::endl << std::endl <<
            "given token:" << std::endl <<
            '\t' << std::string( given.begin(), given.end() ) << std::endl <<
            "expected token:" << std::endl <<
            '\t' << std::string( expected.begin(), expected.end() ) << std::endl;

        return false;
    }

    void Test::tokenizer() {

        using ::json::Position;

        // basics
        testCase( "[", Token( Token::Type::LeftBracket, "[" ) );
        testCase( "]", Token( Token::Type::RightBracket, "]" ) );
        testCase( "{", Token( Token::Type::LeftBrace, "{" ) );
        testCase( "}", Token( Token::Type::RightBrace, "}" ) );
        testCase( ":", Token( Token::Type::Colon, ":" ) );
        testCase( "  ,", Token( Token::Type::Comma, ",", Position( 2, 1, 3 ) ) );
        testCase( "true", Token( Token::Type::True, "true" ) );
        testCase( "   true", Token( Token::Type::True, "true", Position( 3, 1, 4 ) ) );
        testCase( "false", Token( Token::Type::False, "false" ) );
        testCase( "\nfalse", Token( Token::Type::False, "false", Position( 1, 2, 1 ) ) );
        testCase( "null", Token( Token::Type::Null, "null" ) );
        testCase( "  null:", Token( Token::Type::Null, "null", Position( 2, 1, 3 ) ) );

        // strings
        {
            Token::Type t = Token::Type::String;

            testCase( "\"\"", Token( t, "" ) );
            testCase( "\"tralala\"", Token( t, "tralala" ) );
            testCase( "\"\\n\"", Token( t, "\n" ) );
            testCase( "\"\\t\"", Token( t, "\t" ) );
            testCase( "\" \"", Token( t, " " ) );
            //testCase( "\"\\u00FD\"", Token( t, "\u00fd" ) );
            testCase( "\"true\"true", std::vector< Token >{
                Token( t, "true" ),
                Token( Token::Type::True, "true", Position( 6, 1, 7 ) )
            } );
        }
        // integers
        {
            Token::Type t = Token::Type::Integer;

            auto token = testCase( "0", Token( t, "0" ) );
            if ( token.integer() != 0 )
                std::cerr << "invalid number, got [" << token.integer() <<
                "]; expected 0" << std::endl;

            token = testCase( "42", Token( t, "42" ) );
            if ( token.integer() != 42 )
                std::cerr << "invalid number, got [" << token.integer() <<
                "]; expected 42" << std::endl;

            token = testCase( "1e2", Token( t, "1e2" ) );
            if ( token.integer() != 100 )
                std::cerr << "invalid number, got [" << token.integer() <<
                "]; expected 100" << std::endl;

            token = testCase( "100e-1", Token( t, "100e-1" ) );
            if ( token.integer() != 10 )
                std::cerr << "invalid number, got [" << token.integer() <<
                "]; expected 10" << std::endl;

            token = testCase( "100e-2", Token( t, "100e-2" ) );
            if ( token.integer() != 1 )
                std::cerr << "invalid number, got [" << token.integer() <<
                "]; expected 1" << std::endl;

            token = testCase( "  -4", Token( t, "-4", Position( 2, 1, 3 ) ) );
            if ( token.integer() != -4 )
                std::cerr << "invalid number, got [" << token.integer() <<
                "]; expected -4" << std::endl;
        }

        // reals
        {
            Token::Type t = Token::Type::RealNumber;

            auto token = testCase( "0.0", Token( t, "0.0" ) );
            if ( token.real() != 0.0 )
                std::cerr << "invalid number, got [" << token.real() <<
                "]; expected 0.0" << std::endl;

            token = testCase( "1e-1", Token( t, "1e-1" ) );
            if ( abs( token.real() - 0.1 ) > 1e-6 )
                std::cerr << "invalid number, got [" << token.real() <<
                "]; expected 0.1" << std::endl;


        }
    }

}
