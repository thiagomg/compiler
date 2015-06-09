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

    struct BlockCmd {
        using string = std::string;
        string cmd;
        std::vector<string> params;
        int line;
        BlockCmd(int line, const string &cmd, const std::vector<string> &params) {
            this->line = line;
            this->cmd = cmd;
            this->params = params;
        }
    };

    class CppGenerator {

        using string = std::string;
        using ParamVector = std::vector<string>;

        string _before;
        string _after;
        std::stringstream _code;
        std::unordered_set<string> _varNames;

        string _block;
        std::vector<string> _blockParams;
        int _blockLine = 0;
        bool _waitingStart = false;
        bool _blockStarted = false;
        std::vector<BlockCmd> *_blockContent;

    protected:

        void _validateVar(int line, const string &varName);
        bool _addVar(const string &varName);

        void _processBlock(int line, const std::string cmd, std::vector<BlockCmd> *blockContent);

    public:
        CppGenerator();
        void addCmd(int line, const string &cmd, std::vector<string> &params);

        string finish();

    };


}

#endif	/* ASMGENERATOR_H */

