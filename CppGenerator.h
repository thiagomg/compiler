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
#include <memory>

namespace generator {

    struct BlockCmd {
        using string = std::string;
        string cmd;
        std::vector<string> params;
        int line = 0;
        BlockCmd(int line, const string &cmd, const std::vector<string> &params) {
            this->line = line;
            this->cmd = cmd;
            this->params = params;
        }

    };

    struct Block {
        std::string cmd;
        std::vector<std::string> blockParams;
        int blockLine = 0;
        bool waitingStart = false;
        bool blockStarted = false;
        std::vector<BlockCmd> blockContent;
        void addSubBlock() { _subBlockCount++; }
        bool remSubBlock() { _subBlockCount--; return _subBlockCount<0 ? true : false; }
        int _subBlockCount = 0;
    };

    class CppGenerator {

        using string = std::string;
        using ParamVector = std::vector<string>;

        string _before;
        string _after;
        std::stringstream _code;
        std::unordered_set<string> _varNames;
        std::unique_ptr<Block> _curBlock;
        std::unique_ptr<Block> _savedBlock;
        static int _var_count;

    protected:

        void _validateVar(int line, const string &varName);
        bool _addVar(const string &varName);

        void _processSingleBlock(std::unique_ptr<Block> &block);
        void _processBlock(std::unique_ptr<Block> &block, std::unique_ptr<Block> &savedBlock);
        void _addBlockCmd(int line, const string &cmd, std::vector<string> &params);
        int _getNextVar() { return ++_var_count; }

    public:
        CppGenerator();
        void addCmd(int line, const string &cmd, std::vector<string> &params);

        string finish();

    };


}

#endif	/* ASMGENERATOR_H */

