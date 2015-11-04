//
//  infix_parser.cpp
//  compiler
//
//  Adapted from : http://stackoverflow.com/questions/13853774/infix-calculator-expression-parser
//  Not my code. Just created a C++ wrap
//

#include "infix_parser.h"

// Bare bones scanner and parser for the following LL(1) grammar:
// expr -> term { [+-] term }     ; An expression is terms separated by add ops.
// term -> factor { [*/] factor } ; A term is factors separated by mul ops.
// factor -> unsigned_factor      ; A signed factor is a factor,
//         | - unsigned_factor    ;   possibly with leading minus sign
// unsigned_factor -> ( expr )    ; An unsigned factor is a parenthesized expression
//         | NUMBER               ;   or a number
//
// The parser returns the floating point value of the expression.

#include <iostream>

namespace alien {
    
    struct infix_parser {
        
        std::string _buffer_in;
        std::string::const_iterator _cur_pos;
        
        // The token buffer. We never check for overflow! Do so in production code.
        char buf[1024];
        int n = 0;
        
        // The current character.
        int ch;
        
        // The look-ahead token.  This is the 1 in LL(1).
        enum LOOK_AHEAD { ADD_OP, MUL_OP, LEFT_PAREN, RIGHT_PAREN, NUMBER, END_INPUT };
        LOOK_AHEAD look_ahead;
        
        // Forward declarations.
        // void init(void);
        // void advance(void);
        // double expr(void);
        // void error(const char *msg);
        
        // Just die on any error.
        void error(const char *msg)
        {
            fprintf(stderr, "Error: %s. I quit.\n", msg);
            exit(1);
        }
        
        char read_char() {
            if( _cur_pos == _buffer_in.end() )
                return '\n';
            char c = *_cur_pos;
            _cur_pos++;
            return c;
        }
        
        // Buffer the current character and read a new one.
        void read()
        {
            buf[n++] = ch;
            buf[n] = '\0';  // Terminate the string.
            ch = read_char();
        }
        
        // Ignore the current character.
        void ignore()
        {
            ch = read_char();
        }
        
        // Reset the token buffer.
        void reset()
        {
            n = 0;
            buf[0] = '\0';
        }
        
        // The scanner.  A tiny deterministic finite automaton.
        int scan()
        {
            reset();
        START:
            switch (ch) {
                case ' ': case '\t': case '\r':
                    ignore();
                    goto START;
                    
                case '-': case '+':
                    read();
                    return ADD_OP;
                    
                case '*': case '/':
                    read();
                    return MUL_OP;
                    
                case '(':
                    read();
                    return LEFT_PAREN;
                    
                case ')':
                    read();
                    return RIGHT_PAREN;
                    
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    read();
                    goto IN_LEADING_DIGITS;
                    
                case '\n':
                    ch = ' ';    // delayed ignore()
                    return END_INPUT;
                    
                default:
                    error("bad character");
            }
            
        IN_LEADING_DIGITS:
            switch (ch) {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    read();
                    goto IN_LEADING_DIGITS;
                    
                case '.':
                    read();
                    goto IN_TRAILING_DIGITS;
                    
                default:
                    return NUMBER;
            }
            
        IN_TRAILING_DIGITS:
            switch (ch) {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    read();
                    goto IN_TRAILING_DIGITS;
                    
                default:
                    return NUMBER;
            }
        }
        
        // To advance is just to replace the look-ahead.
        void advance()
        {
            look_ahead = (LOOK_AHEAD)scan();
        }
        
        // Clear the token buffer and read the first look-ahead.
        void init(const std::string &s)
        {
            _buffer_in = s;
            _cur_pos = _buffer_in.begin();
            reset();
            ignore(); // junk current character
            advance();
        }
        
        double unsigned_factor()
        {
            double rtn = 0;
            switch (look_ahead) {
                case NUMBER:
                    sscanf(buf, "%lf", &rtn);
                    advance();
                    break;
                    
                case LEFT_PAREN:
                    advance();
                    rtn = expr();
                    if (look_ahead != RIGHT_PAREN) error("missing ')'");
                    advance();
                    break;
                    
                default:
                    error("unexpected token");
            }
            return rtn;
        }
        
        double factor()
        {
            double rtn = 0;
            // If there is a leading minus...
            if (look_ahead == ADD_OP && buf[0] == '-') {
                advance();
                rtn = -unsigned_factor();
            }
            else
                rtn = unsigned_factor();
            return rtn;
        }
        
        double term()
        {
            double rtn = factor();
            while (look_ahead == MUL_OP) {
                switch(buf[0]) {
                    case '*':
                        advance(); 
                        rtn *= factor(); 
                        break; 
                        
                    case '/':
                        advance();
                        rtn /= factor();
                        break;
                }
            }
            return rtn;
        }
        
        double expr()
        {
            double rtn = term();
            while (look_ahead == ADD_OP) {
                switch(buf[0]) {
                    case '+': 
                        advance();
                        rtn += term(); 
                        break; 
                        
                    case '-':
                        advance();
                        rtn -= term();
                        break;
                }
            }
            return rtn;
        }
        
        
        
    };
    
    double parse(const std::string &s) {
        
        infix_parser parser;
        
        //std::string s = "3 + 4 + ( 11 - ( 3 * 2 ) ) / 2";
        
        parser.init(s);
        double val = parser.expr();
        //printf("val: %f\n", val);
        //if (look_ahead != END_INPUT) error("junk after expression");
        parser.advance();  // past end of input mark
        
        return val;
        
    }
    
}


