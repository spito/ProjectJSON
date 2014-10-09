#pragma once

#include "../objects.h"
#include "Tokenizer.h"


#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include <stack>

namespace json {
    namespace parser {

        // simple recursive-descend parser
        class Parser {

            Tokenizer _tokenizer;
            static const int limitRecursionDepth = 1000;
            const int _maximalDepth;

        public:
            Parser( const std::string &json ) :
                Parser( json, 100 )
            {}

            Parser( const std::string &json, int maximalDepth ) :
                _tokenizer( json ),
                _maximalDepth( std::min( maximalDepth, limitRecursionDepth ) )
            {}

            Parser( std::ifstream &file ) :
                Parser( file, 100 )
            {}

            Parser( std::ifstream &file, int maximalDepth ) :
                _tokenizer( file ),
                _maximalDepth( std::min( maximalDepth, limitRecursionDepth ) )
            {}

            objects::Handle getTree();

            const Position &position() const {
                return _tokenizer.position();
            }
        private:
            objects::Handle extractObject( int );
            objects::Handle extractArray( int );
            objects::Handle extractValue( int );

            void checkDepth( int );

        };
    }

}
