#pragma once

#include "InputBuffer.h"

namespace json {
    namespace parser {

        // Deterministic finite automaton for processing numbers
        class NumberParser {
            InputBuffer &_input;

            enum class States {
                Init,
                Minus,
                Zero,
                Digits,
                Point,
                DecimalDigits,
                E,
                EPlusMinus,
                EDigits,
                Quit
            };

            bool _isReal;
            bool _isMinus;
            bool _isE;
            bool _isExponentMinus;
            bool _quit;
            States _state;
            double _denominator;

            std::string _rawToken;
            long long _exponent;
            long long _integer;
            long double _real;
        public:
            NumberParser( InputBuffer &buffer ) :
                _input( buffer )
            {}

            void run();

            long long integer() const {
                return _integer;
            }
            long double real() const {
                return _real;
            };

            bool isInteger() const {
                return !_isReal;
            }
            bool isReal() const {
                return _isReal;
            }

            const std::string &token() {
                return _rawToken;
            }
        private:
            States stateInit( char );
            States stateMinus( char );
            States stateZero( char );
            States stateDigits( char );
            States statePoint( char );
            States stateDecimalDigits( char );
            States stateE( char );
            States stateEplusMinus( char );
            States stateEDigits( char );

            void postProcessing();
            void prepare();
        };
    }
}
