#include <pxx/optimizer/ShortenExpressionVisitor.hpp>

void ShortenExpressionVisitor::visitLeaf(Leaf *_acceptor)
{   
    std::string id = _acceptor->token.getValue();
    if (is_constant_table.find(id) != is_constant_table.end() && 
        is_constant_table.at(id)) {
        // Если переменная - константа
        shortenedExpr = id_node_table.at(id);
        is_leaf = true;
    } else {
        shortenedExpr = _acceptor;
        is_leaf = _acceptor->token.getType() == Type::number;
    }    
    is_assign = false;
}

void ShortenExpressionVisitor::visitBlockNode(BlockNode *_acceptor)
{
    this->id_node_table = {};
    this->is_constant_table = {};
    this->return_result = {};

    for (int i = 0; i < _acceptor->children.size(); i++) {
        auto child = _acceptor->children[i];
        child->accept(this);
        if (this->is_leaf) {
            _acceptor->children.erase(_acceptor->children.begin() + i);
            i--;
        } 
    }
}

void ShortenExpressionVisitor::visitCallNode(CallNode *_acceptor)
{
    _acceptor->params->accept(this);
    is_leaf = false;
    is_assign = false;
    shortenedExpr = _acceptor;
}

void ShortenExpressionVisitor::visitBinaryNode(BinaryNode *_acceptor)
{
    _acceptor->left->accept(this);
    _acceptor->left = this->shortenedExpr;
    bool left_is_leaf = this->is_leaf;
    _acceptor->right->accept(this);
    _acceptor->right = this->shortenedExpr;
    bool right_is_leaf = this->is_leaf;
    if (left_is_leaf && right_is_leaf) {
        _acceptor->accept(this->calculator);
        std::string calc_value = this->calculator->return_result;
        this->shortenedExpr = new Leaf(Token(calc_value, Type::number));
        this->is_leaf = true;
    } else {
        this->shortenedExpr = _acceptor;
        this->is_leaf = false;
    }
    is_assign = false;
}

void ShortenExpressionVisitor::visitUnaryNode(UnaryNode *_acceptor)
{
    _acceptor->operand->accept(this);
    _acceptor->operand = this->shortenedExpr;
    bool value_is_leaf = this->is_leaf;
    if (value_is_leaf) {
        _acceptor->accept(this->calculator);
        std::string calc_value = this->calculator->return_result;
        this->shortenedExpr = new Leaf(Token(calc_value, Type::number));
        this->is_leaf = true;
    } else {
        this->shortenedExpr = _acceptor;
        this->is_leaf = false;
    }
    is_assign = false;
}

void ShortenExpressionVisitor::visitAssignmentNode(AssignmentNode *_acceptor)
{
    _acceptor->right->accept(this);
    _acceptor->right = shortenedExpr;
    std::string id = _acceptor->left->token.getValue();
    this->is_constant_table.insert({id, this->is_leaf});
    this->id_node_table.insert({id, this->shortenedExpr});
    is_assign = true;
    is_leaf = false;
}
void ShortenExpressionVisitor::visitProgramNode(ProgramNode *_acceptor)
{
    for (auto child: _acceptor->children) {
        child->accept(this);
    }
}

void ShortenExpressionVisitor::visitFunctionNode(FunctionNode *_acceptor)
{
    _acceptor->body->accept(this);
}

void ShortenExpressionVisitor::visitProcedureNode(ProcedureNode *_acceptor)
{
    _acceptor->body->accept(this);
}

void ShortenExpressionVisitor::visitElseNode(ElseNode *_acceptor)
{
    _acceptor->body->accept(this);
}

void ShortenExpressionVisitor::visitIfNode(IfNode *_acceptor)
{
    _acceptor->body->accept(this);
}

void ShortenExpressionVisitor::visitWhileNode(WhileNode *_acceptor)
{
    _acceptor->body->accept(this);
}

void ShortenExpressionVisitor::visitForNode(ForNode *_acceptor)
{
    _acceptor->body->accept(this);
}

void ShortenExpressionVisitor::visitVarDeclNode(VariableDeclarationNode *_acceptor)
{
    if (_acceptor->right_expr != nullptr) {
        _acceptor->right_expr->accept(this);
        _acceptor->right_expr = shortenedExpr;
        std::string id = _acceptor->var_name.getValue();
        this->is_constant_table.insert({id, this->is_leaf});
        this->id_node_table.insert({id, this->shortenedExpr});
        is_assign = true;
        is_leaf = false;
    }
}

void ShortenExpressionVisitor::visitActualParamsNode(ActualParamsNode *_acceptor)
{
    for (int i = 0; i < _acceptor->params.size(); i++) {
        auto param = _acceptor->params[i];
        param->accept(this);
        _acceptor->params[i] = shortenedExpr;
    }
}
