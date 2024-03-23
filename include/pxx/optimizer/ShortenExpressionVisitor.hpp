#pragma once
#include <pxx/interface/NodeVisitorInterface.hpp>
#include <pxx/token/Token.hpp>
#include <pxx/optimizer/CalculateVisitor.hpp>
#include <vector>
#include <map>

class ShortenExpressionVisitor : public NodeVisitorInterface {
    ExpressionNode* shortenedExpr;
    std::map<std::string, ExpressionNode*> id_node_table;
    std::map<std::string, bool> is_constant_table;
    bool is_leaf;
    bool is_assign;
    CalculateVisitor* calculator = new CalculateVisitor();

public:
    std::vector<AssignmentNode> return_result;

    void visitLeaf(Leaf *_acceptor);
    void visitBlockNode(BlockNode *_acceptor);
    void visitCallNode(CallNode *_acceptor);
    void visitBinaryNode(BinaryNode *_acceptor);
    void visitUnaryNode(UnaryNode *_acceptor);
    void visitAssignmentNode(AssignmentNode *_acceptor);
    
    void visitFormalParamsNode(FormalParamsNode *_acceptor) {};
    void visitActualParamsNode(ActualParamsNode *_acceptor);
    void visitProgramNode(ProgramNode *_acceptor);
    void visitFunctionNode(FunctionNode *_acceptor);
    void visitProcedureNode(ProcedureNode *_acceptor);
    void visitElseNode(ElseNode *_acceptor);
    void visitIfNode(IfNode *_acceptor);
    void visitWhileNode(WhileNode *_acceptor);
    void visitForNode(ForNode *_acceptor);
    void visitVarDeclNode(VariableDeclarationNode *_acceptor);
    void visitListDeclNode(ListDeclarationNode *_acceptor) {};
};