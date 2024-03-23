#pragma once
#include <pxx/interface/LexerInterface.hpp>
#include <pxx/AST/AST.hpp>

class ParserInterface
{
public:
    virtual void setLexer(LexerInterface *) = 0;
    virtual AST *getAST() = 0;
    virtual ~ParserInterface() = default;
};
