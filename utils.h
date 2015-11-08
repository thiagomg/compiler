#pragma once

#ifndef UTILS
#define UTILS

#include <string>

namespace Utils {

    bool is_number(const std::string& s);
    bool is_value(const std::string &varName);

    template<typename T, typename Pred>
    bool is_equal(const T &l, const T &r, Pred f) {
        if( l.size() != r.size() )
            return false;
        auto bl = std::begin(l);
        auto br = std::begin(r);
        auto el = std::end(l);
        auto er = std::end(r);
        while( (bl != el) && (br != er) ) {
            if( ! f(*bl, *br) ) return false;
            bl++;
            br++;
        }

        return true;
    }
    
    template<typename T, typename Pred>
    bool is_equal(const T &l, const T &r) {
        if( l.size() != r.size() )
            return false;
        auto bl = std::begin(l);
        auto br = std::begin(r);
        auto el = std::end(l);
        auto er = std::end(r);
        while( (bl != el) && (br != er) ) {
            if( *bl != *br ) return false;
            bl++;
            br++;
        }
        
        return true;
    }
    
    bool is_equal(const std::string &l, const std::string &r, bool case_sensitive=false);
    
    std::string get_value(const std::string &s);
}

#endif // UTILS

