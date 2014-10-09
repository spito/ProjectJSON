#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <cctype>

#include "../Position.h"

namespace json {
    namespace parser {

        class InputBuffer {
            size_t _size;
            std::unique_ptr< char[] > _buffer;
            Position _position;
        public:
            InputBuffer( const std::string &json ) :
                _size( json.size() ),
                _buffer( new char[ _size ] )
            {
                std::copy( json.begin(), json.end(), _buffer.get() );
            }

            InputBuffer( std::ifstream &file )
            {
                readFile( file );
            }

            InputBuffer() :
                _size( 0 ),
                _buffer( nullptr )
            {}

            void readJson( const std::string &json ) {
                _size = json.size();
                _buffer.reset( new char[ _size ] );
                _position = Position();

                std::copy( json.begin(), json.end(), _buffer.get() );
            }

            void openFile( const char *name ) {
                std::ifstream file( name, std::ios::in );
                _position.reset();
                readFile( file );
            }


            bool eof() const {
                return _position.position() >= _size;
            }

            // get only non-white characters
            char readChar() {
                char c;
                do {
                    c = read();
                } while ( std::isspace( c ) );
                return c;
            }

            // get all type of characters
            char read() {
                if ( eof() )
                    return char( -1 );

                char c = _buffer[ _position.position() ];

                if ( c == '\n' )
                    _position.nextLine();
                else
                    _position.nextChar();
                return c;
            }

            void skipWhite() {
                while ( !eof() ) {
                    char c = _buffer[ _position.position() ];
                    if ( !std::isspace( c ) )
                        break;

                    if ( c == '\n' )
                        _position.nextLine();
                    else
                        _position.nextChar();
                }
            }

            const Position &position() const {
                return _position;
            }

            void position( Position p ) {
                _position = std::move( p );
            }

        private:

            void readFile( std::ifstream &file ) {
                file.seekg( 0, std::ios::end );
                _size = size_t(file.tellg());
                file.seekg( 0, std::ios::beg );
                _buffer.reset( new char[ _size ] );

                std::copy( std::istream_iterator<char>( file ),
                           std::istream_iterator<char>(),
                           _buffer.get() );

            }
        };
    }
}
