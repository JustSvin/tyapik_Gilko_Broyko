#include <pxx/optimizer/CalculateVisitor.hpp>


void CalculateVisitor::visitLeaf(Leaf *_acceptor) {
    this->return_result = _acceptor->token.getValue();
}

void CalculateVisitor::visitBinaryNode(BinaryNode *_acceptor) {
    _acceptor->left->accept(this);
    std::string left_res = this->return_result; 
    _acceptor->right->accept(this);
    std::string right_res = this->return_result;

    int l = std::stoi(left_res);
    int r = std::stoi(right_res);
    int res = 0;
    switch(_acceptor->op->token.getType()) {
        case Type::orkw:
            res = l || r;
            break;
        case Type::andkw:
            res = l && r;
            break;
        case Type::plus:
            res = l + r;
            break;
        case Type::minus:
            res = l - r;
            break;
        case Type::star:
            res = l * r;
            break;
        case Type::div:
            res = l / r;
            break;
        case Type::greater:
            res = l > r;
            break;
        case Type::less:
            res = l < r;
            break;
        case Type::equal:
            res = l == r;
            break;
        case Type::noteq:
            res = l != r;
            break;
        case Type::grequal:
            res = l >= r;
            break;
        case Type::lequal:
            res = l <= r;
            break;
        case Type::modkw:
            res = l % r;
            break;
    }
    this->return_result = std::to_string(res);
}

void CalculateVisitor::visitUnaryNode(UnaryNode *_acceptor) {
    _acceptor->operand->accept(this);
    std::string value = this->return_result; 

    int v = std::stoi(value);
    int res = 0;
    switch(_acceptor->op->token.getType()) {
        case Type::notkw:
            res = !v;
            break;
        case Type::minus:
            res = -v;
            break;
        case Type::plus:
            res = v;
            break;
    }
    this->return_result = std::to_string(res);
}