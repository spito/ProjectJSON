#include "Parser.h"

namespace json {
    namespace parser {

        void Parser::checkDepth( int depth ) {
            if ( depth >= _maximalDepth )
                throw exception::InternalError( "stack limit", position() );
        }


        objects::Handle Parser::getTree() {

            objects::Handle handle;

            handle = extractValue( 1 );
            Token token;
            if ( !_tokenizer.isTokenType( Token::Type::EndOfFile, token ) )
                throw exception::InvalidToken( token, Token::Type::EndOfFile );

            return handle;
        }

        objects::Handle Parser::extractValue( int depth ) {
            checkDepth( depth );

            objects::Handle handle;
            Token token;

            _tokenizer.readToken( token );

            switch ( token.type() ) {
            case Token::Type::LeftBrace:
                handle = extractObject( depth + 1 );
                break;
            case Token::Type::LeftBracket:
                handle = extractArray( depth + 1 );
                break;
            case Token::Type::String:
                handle.reset( new objects::String( token.token() ) );
                break;
            case Token::Type::Integer:
                handle.reset( new objects::Integer( token.integer() ) );
                break;
            case Token::Type::RealNumber:
                handle.reset( new objects::RealNumber( token.real() ) );
                break;
            case Token::Type::True:
                handle.reset( new objects::Bool( true ) );
                break;
            case Token::Type::False:
                handle.reset( new objects::Bool( false ) );
                break;
            case Token::Type::Null:
                handle.reset( new objects::Null() );
                break;
            default:
                throw exception::InvalidToken( token, {
                    Token::Type::LeftBrace,
                    Token::Type::LeftBracket,
                    Token::Type::String,
                    Token::Type::Integer,
                    Token::Type::RealNumber,
                    Token::Type::True,
                    Token::Type::False,
                    Token::Type::Null
                } );
            }
            return handle;
        }

        objects::Handle Parser::extractArray( int depth ) {
            checkDepth( depth );

            objects::Array *ptr = new objects::Array();
            objects::Handle handle( ptr );
            Token token;

            bool quit = false;

            _tokenizer.readToken( token );

            // emtpy array
            if ( token.type() == Token::Type::RightBracket )
                quit = true;

            while ( !quit ) {

                switch ( token.type() ) {
                case Token::Type::LeftBrace:
                    ptr->pushBack( extractObject( depth + 1 ) );
                    break;
                case Token::Type::LeftBracket:
                    ptr->pushBack( extractArray( depth + 1 ) );
                    break;
                case Token::Type::String:
                    ptr->pushBack( objects::Handle( new objects::String( token.token() ) ) );
                    break;
                case Token::Type::Integer:
                    ptr->pushBack( objects::Handle( new objects::Integer( token.integer() ) ) );
                    break;
                case Token::Type::RealNumber:
                    ptr->pushBack( objects::Handle( new objects::RealNumber( token.real() ) ) );
                    break;
                case Token::Type::True:
                    ptr->pushBack( objects::Handle( new objects::Bool( true ) ) );
                    break;
                case Token::Type::False:
                    ptr->pushBack( objects::Handle( new objects::Bool( false ) ) );
                    break;
                case Token::Type::Null:
                    ptr->pushBack( objects::Handle( new objects::Null() ) );
                    break;
                default:
                    throw exception::InvalidToken( token, {
                        Token::Type::LeftBrace,
                        Token::Type::LeftBracket,
                        Token::Type::String,
                        Token::Type::Integer,
                        Token::Type::RealNumber,
                        Token::Type::True,
                        Token::Type::False,
                        Token::Type::Null
                    } );
                }

                _tokenizer.readToken( token );
                switch ( token.type() ) {
                case Token::Type::Comma:
                    break;
                case Token::Type::RightBracket:
                    quit = true;
                    continue;// quit while cycle
                default:
                    throw exception::InvalidToken( token, {
                        Token::Type::Comma,
                        Token::Type::RightBracket
                    } );
                }
                _tokenizer.readToken( token );
            }
            return handle;
        }

        objects::Handle Parser::extractObject( int depth ) {
            checkDepth( depth );

            objects::Object *ptr = new objects::Object();
            objects::Handle handle( ptr );
            Token token;

            bool quit = false;

            _tokenizer.readToken( token );

            // empty object
            if ( token.type() == Token::Type::RightBrace )
                quit = true;

            while ( !quit ) {

                if ( token.type() != Token::Type::String )
                    throw exception::InvalidToken( token, Token::Type::String );

                objects::String key( token.token() );

                _tokenizer.readToken( token );
                if ( token.type() != Token::Type::Colon )
                    throw exception::InvalidToken( token, Token::Type::Colon );

                _tokenizer.readToken( token );

                switch ( token.type() ) {
                case Token::Type::LeftBrace:
                    ptr->insert( { key, extractObject( depth + 1 ) } );
                    break;
                case Token::Type::LeftBracket:
                    ptr->insert( { key, extractArray( depth + 1 ) } );
                    break;
                case Token::Type::String:
                    ptr->insert( { key, objects::Handle( new objects::String( token.token() ) ) } );
                    break;
                case Token::Type::Integer:
                    ptr->insert( { key, objects::Handle( new objects::Integer( token.integer() ) ) } );
                    break;
                case Token::Type::RealNumber:
                    ptr->insert( { key, objects::Handle( new objects::RealNumber( token.real() ) ) } );
                    break;
                case Token::Type::True:
                    ptr->insert( { key, objects::Handle( new objects::Bool( true ) ) } );
                    break;
                case Token::Type::False:
                    ptr->insert( { key, objects::Handle( new objects::Bool( false ) ) } );
                    break;
                case Token::Type::Null:
                    ptr->insert( { key, objects::Handle( new objects::Null() ) } );
                    break;
                default:
                    throw exception::InvalidToken( token, {
                        Token::Type::LeftBrace,
                        Token::Type::LeftBracket,
                        Token::Type::String,
                        Token::Type::Integer,
                        Token::Type::RealNumber,
                        Token::Type::True,
                        Token::Type::False,
                        Token::Type::Null
                    } );
                }

                _tokenizer.readToken( token );
                switch ( token.type() ) {
                case Token::Type::Comma:
                    break;
                case Token::Type::RightBrace:
                    quit = true;
                    continue;// quit while cycle
                default:
                    throw exception::InvalidToken( token, {
                        Token::Type::Comma,
                        Token::Type::RightBrace
                    } );
                }
                _tokenizer.readToken( token );
            }
            return handle;
        }

    }
}
