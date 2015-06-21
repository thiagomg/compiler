#ifndef UTILS
#define UTILS

namespace Utils {

    bool is_number(const std::string& s)
    {
        if( s.empty() )
            return false;

        return std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    }

}

#endif // UTILS

