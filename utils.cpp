//
// Created by Thiago on 8/19/15.
//
#include "utils.h"
#include <algorithm>
#include <string>
#include <iostream>

namespace Utils {

    bool is_number(const std::string &s) {
        if (s.empty())
            return false;

        return std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    }

    bool is_value(const std::string &varName) {
        if (varName[0] == '\"' || varName[0] == '\'' || is_number(varName)) {
            return true;
        }
        return false;
    }

    bool is_equal(const std::string &l, const std::string &r) {
        return is_equal<std::string>(l, r);
    }
    
    std::string get_value(const std::string &s) {
        
        //TODO: Gerar um range para nao fazer copia de string
        if( s.size() < 2 ) return s;
        return s.substr(1, s.size() - 2);
        
    }

}