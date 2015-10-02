/* 
 * File:   TokenProcessor.h
 * Author: thiago
 *
 * Created on 6 de Maio de 2015, 23:05
 */

#ifndef TOKENPROCESSOR_H
#define	TOKENPROCESSOR_H

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace stt {
    template<typename T>
    struct Range {

        Range(typename T::iterator b, typename T::iterator e) : _begin(b), _end(e) { }
        Range(T &container) : _begin(container.begin()), _end(container.end()) { }

        typename T::iterator begin() { return _begin; }
        const typename T::iterator begin() const { return _begin; }

        typename T::iterator end() { return _end; }
        const typename T::iterator end() const { return _end; }

    private:
        typename T::iterator _begin, _end;
    };
}

struct CmdToken {
    CmdToken(int line, const std::string &cmd, std::vector< std::string > &params) {
        this->line = line;
        this->cmd = cmd;
        this->params = params;
    }
    CmdToken(const CmdToken &src) {
        this->line = src.line;
        this->cmd = src.cmd;
        this->params = src.params;
    }
    int line;
    std::string cmd;
    std::vector<std::string> params;
};

class TokenProcessor {
public:
    TokenProcessor();
    TokenProcessor(const TokenProcessor& orig) = delete;
    virtual ~TokenProcessor();

    using Iterator = std::vector<CmdToken>::iterator;
    using TokenVector = std::vector<CmdToken>;
    using Range = stt::Range<TokenVector>;

    bool add(int line_num, const std::vector<std::string> &line_chunks);


    Range getRange();
    TokenVector &tokenList();


protected:
    void _processCmd(int line_num, const std::string &cmd, std::vector< std::string > &params);
	int _getSize(const std::string &cmd);

private:

    std::vector<CmdToken> _tokens;

    std::string _cmd;
    std::vector< std::string > _params;
    int _missing = 0;
    
    std::map<std::string, int> _cmdSize {
        {"escreva", 1},
        {"pergunta", 3},
        {"se", 3},
        {"senao", 0},
        {"repita", 2},
        {"defina", 3},
        {"[", 0},
        {"]", 0}
    };
};

#endif	/* TOKENPROCESSOR_H */

