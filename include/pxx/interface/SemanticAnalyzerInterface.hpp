#pragma once
#include <pxx/AST/AST.hpp>
#include <pxx/AST/ASTNodePublic.hpp>


class SemanticAnalyzerInterface {
public:
    virtual ~SemanticAnalyzerInterface() = default;
    virtual void checkSemantics(AST* tree) = 0;
};
