#ifndef QUOTEDWORD_H
#define QUOTEDWORD_H

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

class QuotedWord
{
    public:
        operator std::string const& () const { return data; }
        const std::string &operator()() { return data; }

    private:
		std::string     data;
      friend std::ostream& operator<<(std::ostream& str, QuotedWord const& value)
      {
        return str << value.data;
      }
      friend std::istream& operator>>(std::istream& str, QuotedWord& value)
      {
        char x;
        str >> x;
        if ((str) && (x == '"' || x == '\''))
        {
            std::string extra;
            std::getline(str, extra, x);
            value.data = std::string("\"").append(extra).append("\"");
        }
        else
        {
            str.putback(x);
            str >> value.data;
        }
        return str;
      }
};

#endif // QUOTEDWORD_H
