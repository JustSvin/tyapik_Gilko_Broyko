#pragma once
#include <pxx/AST/ASTNodePublic.hpp>

class NodeVisitorInterface
{
public:
    virtual ~NodeVisitorInterface() = default;

    virtual void visitLeaf(Leaf *_acceptor) = 0;
    virtual void visitFormalParamsNode(FormalParamsNode *_acceptor) = 0;
    virtual void visitActualParamsNode(ActualParamsNode *_acceptor) = 0;
    virtual void visitCallNode(CallNode *_acceptor) = 0;
    virtual void visitBinaryNode(BinaryNode *_acceptor) = 0;
    virtual void visitUnaryNode(UnaryNode *_acceptor) = 0;
    virtual void visitAssignmentNode(AssignmentNode *_acceptor) = 0;
    virtual void visitBlockNode(BlockNode *_acceptor) = 0;
    virtual void visitProgramNode(ProgramNode *_acceptor) = 0;
    virtual void visitFunctionNode(FunctionNode *_acceptor) = 0;
    virtual void visitProcedureNode(ProcedureNode *_acceptor) = 0;
    virtual void visitElseNode(ElseNode *_acceptor) = 0;
    virtual void visitIfNode(IfNode *_acceptor) = 0;
    virtual void visitWhileNode(WhileNode *_acceptor) = 0;
    virtual void visitForNode(ForNode *_acceptor) = 0;
    virtual void visitVarDeclNode(VariableDeclarationNode *_acceptor) = 0;
    virtual void visitListDeclNode(ListDeclarationNode *_acceptor) = 0;
};
