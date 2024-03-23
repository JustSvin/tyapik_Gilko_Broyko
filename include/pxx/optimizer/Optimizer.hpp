#pragma once
#include <pxx/interface/ParserInterface.hpp>
#include "pxx/AST/ASTNode.hpp"
#include "pxx/AST/AST.hpp"
#include <pxx/optimizer/ShortenExpressionVisitor.hpp>
#include <vector>


class Optimizer {
public:
    void optimize(AST* ast);
};
