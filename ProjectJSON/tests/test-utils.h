#define JSON_TEST

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "catch.h"

#include "json/parser/Tokenizer.h"

namespace test {
    namespace {

        using ::json::parser::Token;
        using ::json::exception::Exception;

        inline Approx< long double > approx( long double value ) {
            return Approx< long double >( value );
        }

        Token tokenize( std::string json ) {
            auto tokenizer = ::json::parser::Tokenizer( json );
            Token token;
            try {
                tokenizer.lookAtToken( token );
            }
            catch ( Exception & ) {
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

        template< typename ExpectedException >
        bool exceptionRaised( std::string json, ExpectedException exex ) {
            auto tokenizer = ::json::parser::Tokenizer( json );
            Token token;
            try {
                tokenizer.lookAtToken( token );
            }
            catch ( ExpectedException & ) {
                return true;
            }
            catch ( std::exception &ex ) {
                WARN( std::string() + "Expected exception " + exex.what() + " but " + ex.what() + " got." );
            }
            catch ( ... ) {
                WARN( std::string() + "Unknown exception raised." );
            }
            return false;
        }

        namespace _internal {

            template< typename Ex >
            struct ExceptionEquality;

            template<>
            struct ExceptionEquality < ::json::exception::Exception > {
                using Ex = ::json::exception::Exception;
                const Ex *_expected;
                const Ex *_given;
                std::vector< std::string > _messages;

                void init( const Ex &expected, const Ex &given ) {
                    _expected = &expected;
                    _given = &given;
                }

                bool equal() {
                    return
                        checkWhat() &&
                        checkPosition();
                }

                bool checkWhat() {
                    if ( std::string( _expected->what() ) == _given->what() )
                        return true;
                    std::ostringstream s;
                    s << "what() - expected '" <<
                        _expected->what() << "' but '" <<
                        _given->what() << "' got.";
                    addMessage( s.str() );
                    return false;
                }

                bool checkPosition() {
                    if ( _expected->position() == _given->position() )
                        return true;
                    std::ostringstream s;
                    s << "position() - expected " <<
                        _expected->position() << " but " <<
                        _given->position() << " got.";
                    addMessage( s.str() );
                    return false;
                }
                const std::vector< std::string > messages() const {
                    return _messages;
                }
                void addMessage( std::string msg ) {
                    _messages.emplace_back( std::move( msg ) );
                }
                void warn() const {
                    WARN( std::string() + "Exceptions are not equal." );
                    for ( const auto &m : _messages ) {
                        WARN( m );
                    }
                }
            };

            using BaseEx = ExceptionEquality < ::json::exception::Exception > ;

            template<>
            struct ExceptionEquality < ::json::exception::InvalidCharacter > : BaseEx{
                using Ex = ::json::exception::InvalidCharacter;
                const Ex *_expected;
                const Ex *_given;

                void init( const Ex &expected, const Ex &given ) {
                    BaseEx::init( expected, given );
                    _expected = &expected;
                    _given = &given;
                }

                bool equal() {
                    return
                        BaseEx::equal() &&
                        checkExpected() &&
                        checkGiven();
                }

                bool checkExpected() {
                    if ( _expected->expected() == _given->expected() )
                        return true;
                    std::ostringstream s;
                    s << "expected() - expected '" <<
                        _expected->expected() << "' but '" <<
                        _given->expected() << "' got.";
                    addMessage( s.str() );
                    return false;
                }

                bool checkGiven() {
                    if ( _expected->given() == _given->given() )
                        return true;
                    std::ostringstream s;
                    s << "given() - expected '" <<
                        _expected->given() << "' but '" <<
                        _given->given() << "' got.";
                    addMessage( s.str() );
                    return false;
                }
            };

            template<>
            struct ExceptionEquality < ::json::exception::EndOfFile > : ExceptionEquality < ::json::exception::InvalidCharacter > {};

            template<>
            struct ExceptionEquality < ::json::exception::InternalError > : BaseEx{};

            template<>
            struct ExceptionEquality< ::json::exception::InvalidFile > : BaseEx{};

            template<>
            struct ExceptionEquality< ::json::exception::UnsupportedFeature > : BaseEx{};

            template<>
            struct ExceptionEquality< ::json::exception::InvalidToken > : BaseEx{
                using Ex = ::json::exception::InvalidToken;
                const Ex *_expected;
                const Ex *_given;

                void init( const Ex &expected, const Ex &given ) {
                    BaseEx::init( expected, given );
                    _expected = &expected;
                    _given = &given;
                }

                bool equal() {
                    return
                        BaseEx::equal() &&
                        checkExpected() &&
                        checkGiven();
                }

                bool checkExpected() {
                    if ( _expected->expected() == _given->expected() )
                        return true;
                    std::ostringstream s;
                    s << "expected() - expected '";
                    int i = 0;
                    for ( auto t : _expected->expected() ) {
                        if ( i++ )
                            s << ", ";
                        s << t;
                    }
                    s << "' but '";
                    i = 0;
                    for ( auto t : _given->expected() ) {
                        if ( i++ )
                            s << ", ";
                        s << t;
                    }
                    s << "' got.";
                    addMessage( s.str() );
                    return false;
                }

                bool checkGiven() {
                    if ( _expected->given() == _given->given() )
                        return true;
                    std::ostringstream s;
                    s << "given() - expected '" <<
                        _expected->given() << "' but '" <<
                        _given->given() << "' got.";
                    addMessage( s.str() );
                    return false;
                }
            };
        }
        template< typename ExpectedException >
        bool exceptionEquality( std::string json, ExpectedException exex ) {
            auto tokenizer = ::json::parser::Tokenizer( json );
            Token token;
            try {
                tokenizer.lookAtToken( token );
                WARN( std::string() + "No exception raised" );
            }
            catch ( ExpectedException &ex ) {
                _internal::ExceptionEquality< ExpectedException > eq;
                eq.init( exex, ex );
                if ( eq.equal() )
                    return true;
                eq.warn();
            }
            catch ( std::exception &ex ) {
                WARN( std::string() + "Expected exception " + exex.what() + " but " + ex.what() + " got." );
            }
            catch ( ... ) {
                WARN( std::string() + "unknown exception raised" );
            }
            return false;
        }
    } // anonymous namespace
}  // namespace test
