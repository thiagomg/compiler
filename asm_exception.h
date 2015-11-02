//
// Created by thiago on 08/06/15.
//

#ifndef COMPILER_ASM_EXCEPTION_H
#define COMPILER_ASM_EXCEPTION_H

#include <exception>
#include <string>
#include <vector>

struct asm_exception : std::exception
{
    //char text[1000];
    std::string _text;
    int _line;

    asm_exception(const std::string &s)
    {
        _text = s;
    }

    asm_exception(int line, const std::string &s)
    {
        _text.reserve(24 + s.size());
        _text = "Linha: " + std::to_string(line) + ": " + s;
    }

    asm_exception(int line, const std::string &s, const std::vector<std::string> &params)
    {
        _text.reserve(24 + s.size());
        _text = "Linha: " + std::to_string(line) + ": " + s + " (";
        for(const auto &p : params) {
            _text += p;
            _text += " ";
        }
        _text += ")";
    }
/*
    asm_exception(char const* fmt, ...) __attribute__((format(printf,2,3))) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(text, sizeof text, fmt, ap);
        va_end(ap);
    }
*/
    char const* what() const throw() { return _text.c_str(); }
    int line() { return _line; }
};

#endif //COMPILER_ASM_EXCEPTION_H
