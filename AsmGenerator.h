/* 
 * File:   AsmGenerator.h
 * Author: thiago
 *
 * Created on 6 de Maio de 2015, 23:38
 */

#ifndef ASMGENERATOR_H
#define	ASMGENERATOR_H

#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>

namespace generator {

    class AsmGenerator {
        std::string _before;
        std::string _after;
        std::stringstream _code;
        std::unordered_set<std::string> _varNames;
    protected:

        void _validateVar(const std::string &varName);
        bool _addVar(const std::string &varName);

    public:
        AsmGenerator();
        void addCmd(const std::string &cmd, std::vector< std::string > &params);

        std::string finish();

    };


}

#endif	/* ASMGENERATOR_H */

