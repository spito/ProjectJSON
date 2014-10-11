#pragma once

#include "../Position.h"

#include <string>
#include <iostream>

namespace json {
    namespace parser {

        class Token {
        public:

            enum class Type {
                Comma,//  char: ,
                Colon,// char: :
                LeftBrace,// char: {
                RightBrace,// char: }
                LeftBracket,// char: [
                RightBracket,// char: ]
                True,// true
                False,// false
                Null,// null,
                String,// "blabla"
                Integer,// 123
                RealNumber,// 3.14
                EndOfFile,
                Invalid//
            };


            static const char *value( Type );
            static const char *typeName( Type );

        private:
            std::string _token;
            Type _type;
            Position _position;

            union _N {
                long long integer;
                long double real;

                _N( long long i ) : integer( i ) {}
                _N( long double r ) : real( r ) {}
                _N() = default;
            } _number;

            static const char *_values[];
            static const char *_typeNames[];
        public:

            Token() :
                _type( Type::Invalid ),
                _position()
            {}

            Token( Type t, Position p = Position() ) :
                Token( t, value( t ), p )
            {}

            Token( Type t, const char *token, Position p = Position() ) :
                _token( token ),
                _type( t ),
                _position( std::move( p ) )
            {}

            Token( Type t, std::string token, Position p = Position() ) :
                _token( std::move( token ) ),
                _type( t ),
                _position( std::move( p ) )
            {}

            Token( const Token &o ) :
                _token( o._token ),
                _type( o._type ),
                _position( o._position )
            {
                switch ( _type ) {
                case Type::Integer:
                    _number.integer = o._number.integer;
                    break;
                case Type::RealNumber:
                    _number.real = o._number.real;
                    break;
                default:
                    break;
                }
            }

            Token( Token &&o ) :
                _token( std::move( o._token ) ),
                _type( o._type ),
                _position( std::move( o._position ) )
            {
                switch ( _type ) {
                case Type::Integer:
                    _number.integer = o._number.integer;
                    break;
                case Type::RealNumber:
                    _number.real = o._number.real;
                    break;
                default:
                    break;
                }
            }

            void swap( Token &o ) {
                using std::swap;
                swap( _token, o._token );
                swap( _type, o._type );
                swap( _position, o._position );
                swap( _number, o._number );
            }


            Token &operator=( Token t ) {
                swap( t );
                return *this;
            }

            size_t size() const {
                return _token.size();
            }

            Type type() const {
                return _type;
            }

            Position &position() {
                return _position;
            }
            const Position &position() const {
                return _position;
            }

            std::string &token() {
                return _token;
            }

            const std::string &token() const {
                return _token;
            }

            long long &integer() {
                return _number.integer;
            }
            long long integer() const {
                return _number.integer;
            }

            long double &real() {
                return _number.real;
            }
            long double real() const {
                return _number.real;
            }

            char &operator[]( size_t index ) {
                return _token[ index ];
            }
            char operator[]( size_t index ) const {
                return _token[ index ];
            }

            operator Type() const {
                return _type;
            }

            auto begin()
                -> decltype( _token.begin() )
            {
                return _token.begin();
            }
            auto end() 
                -> decltype( _token.end() )
            {
                return _token.end();
            }
            auto begin() const
                -> decltype( _token.begin() )
            {
                return _token.begin();
            }
            auto end() const
                -> decltype( _token.end() )
            {
                return _token.end();
            }


            bool operator==( const Token &o ) const {
                return
                    _type == o._type &&
                    _position == o._position &&
                    _token == o._token;
            }

            bool operator!=( const Token &o ) const {
                return !( *this == o );
            }
        };
    }
}

namespace std {
    template<>
    inline void swap( ::json::parser::Token &lhs, ::json::parser::Token &rhs ) {
        lhs.swap( rhs );
    }
}

std::ostream &operator<<( std::ostream &, json::parser::Token::Type );
