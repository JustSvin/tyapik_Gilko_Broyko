#pragma once
#include <pxx/interface/NodeVisitorInterface.hpp>
#include <pxx/token/Token.hpp>
#include <map>
#include <queue>
#include <stack>
#include <utility>
#include <ostream>

class CodeEmittingNodeVisitor : public NodeVisitorInterface
{
private:
    std::ostream& stream;
    void write(std::string s);
    void write(Token token);
    void write(Leaf* leaf);
public:
    CodeEmittingNodeVisitor(std::ostream& _stream);
    void visitLeaf(Leaf *_acceptor);
    void visitFormalParamsNode(FormalParamsNode *_acceptor);
    void visitActualParamsNode(ActualParamsNode *_acceptor);
    void visitCallNode(CallNode *_acceptor);
    void visitBinaryNode(BinaryNode *_acceptor);
    void visitUnaryNode(UnaryNode *_acceptor);
    void visitAssignmentNode(AssignmentNode *_acceptor);
    void visitBlockNode(BlockNode *_acceptor);
    void visitProgramNode(ProgramNode *_acceptor);
    void visitFunctionNode(FunctionNode *_acceptor);
    void visitProcedureNode(ProcedureNode *_acceptor);
    void visitElseNode(ElseNode *_acceptor);
    void visitIfNode(IfNode *_acceptor);
    void visitWhileNode(WhileNode *_acceptor);
    void visitForNode(ForNode *_acceptor);
    void visitVarDeclNode(VariableDeclarationNode *_acceptor);
    void visitListDeclNode(ListDeclarationNode *_acceptor);
};
