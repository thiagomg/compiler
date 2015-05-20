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

namespace generator {
    class AsmGenerator;
}

class TokenProcessor {
public:
    TokenProcessor(generator::AsmGenerator *generator);
    TokenProcessor(const TokenProcessor& orig) = delete;
    virtual ~TokenProcessor();
    
    bool add(const std::string &token);

protected:
    void processCmd(const std::string &cmd, std::vector< std::string > &params);

private:

    generator::AsmGenerator *_generator;

    std::string _cmd;
    std::vector< std::string > _params;
    int _missing = 0;
    
    std::map<std::string, int> _cmdSize {
        {"escreva", 1},
        {"pergunta", 3}
    };
};

#endif	/* TOKENPROCESSOR_H */

