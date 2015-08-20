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
    
    bool add(int line_num, const std::vector<std::string> &line_chunks);

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
        {"[", 0},
        {"]", 0}
    };
};

#endif	/* TOKENPROCESSOR_H */

