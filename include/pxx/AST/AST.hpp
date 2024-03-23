#pragma once
#include <pxx/AST/ASTNode.hpp>
#include <pxx/interface/NodeVisitorInterface.hpp>

class AST
{
    

public:
    BaseASTNode *root;
    AST(BaseASTNode *_root);
    void accept(NodeVisitorInterface *_visitor);
};
