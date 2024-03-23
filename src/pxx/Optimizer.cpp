#include <pxx/optimizer/Optimizer.hpp>

void Optimizer::optimize(AST *ast)
{
    ast->root->accept(new ShortenExpressionVisitor());
}