#include <pxx/AST/ASTNode.hpp>
#include <iostream>

Leaf::Leaf(Token token) : token(token) {}

ActualParamsNode::ActualParamsNode(std::vector<ExpressionNode *> params) : params(params) {}

void ActualParamsNode::add_child(ExpressionNode *param)
{
    this->params.push_back(param);
}

CallNode::CallNode(Token callable, ActualParamsNode *params) : callable(callable), params(params) {}

BinaryNode::BinaryNode(ExpressionNode *left, Leaf *op, ExpressionNode *right) : left(left), op(op), right(right) {}

UnaryNode::UnaryNode(Leaf *op, ExpressionNode *operand) : op(op), operand(operand) {}

AssignmentNode::AssignmentNode(Leaf *left, ExpressionNode *right) : left(left), right(right) {}

BlockNode::BlockNode(std::vector<BaseASTNode *> children) : children(children) {}

BlockNode::BlockNode() : children({}) {}

FunctionNode::FunctionNode(Leaf *id, FormalParamsNode *formal_params, Leaf *return_type, BlockNode *body)
    : id(id), formal_params(formal_params), return_type(return_type), body(body) {}

ProcedureNode::ProcedureNode(Leaf *id, FormalParamsNode *formal_params, BlockNode *body)
    : id(id), formal_params(formal_params), body(body) {}

VariableDeclarationNode::VariableDeclarationNode(Token var_name, ExpressionNode* right_expr, Leaf* type)
    : var_name(var_name), right_expr(right_expr), type(type) {}

ListDeclarationNode::ListDeclarationNode(Token var_name, Token type, int size)
    : var_name(var_name), type(type), size(size) {}

IfNode::IfNode(ExpressionNode *condition, BlockNode *body)
    : condition(condition), body(body)
{
    this->next_else = nullptr;
}

ElseNode::ElseNode(BlockNode *body) : body(body) {}

WhileNode::WhileNode(ExpressionNode *condition, BlockNode *body) : condition(condition), body(body) {}

ForNode::ForNode(Leaf *iterator, Leaf *from, Leaf *to, BlockNode *body)
    : iterator(iterator), from(from), to(to), body(body) {}

void BlockNode::add_child(BaseASTNode *child)
{
    this->children.push_back(child);
}

FormalParamsNode::FormalParamsNode(std::vector<Leaf *> params, std::vector<Leaf *> types) : names(params), types(types) {}

void FormalParamsNode::add_param(Leaf *name, Leaf *type)
{
    this->names.push_back(name);
    this->types.push_back(type);
}

void print_indented_line(std::string text, int indent, std::ostream& stream)
{
    std::string indent_str = "   |";
    for (int i = 0; i < indent; i++)
    {
        stream << indent_str;
    }
    stream << text;
    stream << "\n";
}

void Leaf::print(int indent, std::ostream& stream)
{
    std::string text = "<" + type_to_str(this->token.getType()) + ", " + this->token.getValue() + ">";
    print_indented_line(text, indent, stream);
}

void FormalParamsNode::print(int indent, std::ostream& stream)
{
    std::string text = "FormalParams";
    print_indented_line(text, indent, stream);
    print_indented_line("names:", indent + 1, stream);
    for (auto name : this->names)
    {
        name->print(indent + 2, stream);
    }
    print_indented_line("types:", indent + 1, stream);
    for (auto type : this->types)
    {
        type->print(indent + 2, stream);
    }
}

void ActualParamsNode::print(int indent, std::ostream& stream)
{
    std::string text = "ActualParams";
    print_indented_line(text, indent, stream);
    for (auto param : this->params)
    {
        param->print(indent + 1, stream);
    }
}

void CallNode::print(int indent, std::ostream& stream)
{
    std::string text = "Call";
    print_indented_line(text, indent, stream);
    print_indented_line("callable:", indent + 1, stream);
    print_indented_line(this->callable.getValue(), indent + 2, stream);
    print_indented_line("params:", indent + 1, stream);
    this->params->print(indent + 2, stream);
}

void BinaryNode::print(int indent, std::ostream& stream)
{
    std::string text = "BinaryOp (" + type_to_str(this->op->token.getType()) + ")";
    print_indented_line(text, indent, stream);
    this->left->print(indent + 1, stream);
    this->right->print(indent + 1, stream);
}

void UnaryNode::print(int indent, std::ostream& stream)
{
    std::string text = "UnaryOp (" + type_to_str(this->op->token.getType()) + ")";
    print_indented_line(text, indent, stream);
    this->operand->print(indent + 1, stream);
}

void AssignmentNode::print(int indent, std::ostream& stream)
{
    std::string text = "Assignment";
    print_indented_line(text, indent, stream);
    this->left->print(indent + 1, stream);
    this->right->print(indent + 1, stream);
}

void BlockNode::print(int indent, std::ostream& stream)
{
    std::string text = "Block";
    print_indented_line(text, indent, stream);
    for (auto child : this->children)
    {
        child->print(indent + 1, stream);
    }
}

void ProgramNode::print(int indent, std::ostream& stream)
{
    std::string text = "Program " + this->program_name;
    print_indented_line(text, indent, stream);
    for (auto child : this->children)
    {
        child->print(indent + 1, stream);
    }
}

void FunctionNode::print(int indent, std::ostream& stream)
{
    std::string text = "Function declaration";
    print_indented_line(text, indent, stream);
    print_indented_line("name:", indent + 1, stream);
    this->id->print(indent + 2, stream);
    print_indented_line("return type:", indent + 1, stream);
    this->return_type->print(indent + 2, stream);
    this->formal_params->print(indent + 1, stream);
    this->body->print(indent + 1, stream);
}

void ProcedureNode::print(int indent, std::ostream& stream)
{
    std::string text = "Procedure declaration";
    print_indented_line(text, indent, stream);
    print_indented_line("name:", indent + 1, stream);
    this->id->print(indent + 2, stream);
    this->formal_params->print(indent + 1, stream);
    this->body->print(indent + 1, stream);
}

void ElseNode::print(int indent, std::ostream& stream)
{
    std::string text = "Else";
    print_indented_line(text, indent, stream);
    this->body->print(indent + 1, stream);
};

void IfNode::print(int indent, std::ostream& stream)
{
    std::string text = "If";
    print_indented_line(text, indent, stream);
    print_indented_line("condition:", indent + 1, stream);
    this->condition->print(indent + 2, stream);
    this->body->print(indent + 1, stream);
    if (this->next_else != nullptr)
    {
        this->next_else->print(indent + 1, stream);
    }
}

void WhileNode::print(int indent, std::ostream& stream)
{
    std::string text = "While";
    print_indented_line(text, indent, stream);
    print_indented_line("condition:", indent + 1, stream);
    this->condition->print(indent + 2, stream);
    this->body->print(indent + 1, stream);
}

void ForNode::print(int indent, std::ostream& stream)
{
    std::string text = "For";
    print_indented_line(text, indent, stream);
    print_indented_line("iterator:", indent + 1, stream);
    this->iterator->print(indent + 2, stream);
    print_indented_line("from:", indent + 1, stream);
    this->from->print(indent + 2, stream);
    print_indented_line("to:", indent + 1, stream);
    this->to->print(indent + 2, stream);
    this->body->print(indent + 1, stream);
}

void VariableDeclarationNode::print(int indent, std::ostream& stream) {
    std::string text = "Variable Declaration";
    print_indented_line(text, indent, stream);
    print_indented_line("name:", indent + 1, stream);
    print_indented_line(this->var_name.getValue(), indent + 2, stream);
    if (this->right_expr != nullptr) {
        print_indented_line("equals:", indent + 1, stream);
        this->right_expr->print(indent + 2, stream);
    }
    if (this->type != nullptr) {
        print_indented_line("type:", indent + 1, stream);
        print_indented_line(this->type->token.getValue(), indent + 2, stream);
    }
}

void ListDeclarationNode::print(int indent, std::ostream& stream) {
    std::string text = "List Declaration";
    print_indented_line(text, indent, stream);
    print_indented_line("name:", indent + 1, stream);
    print_indented_line(this->var_name.getValue(), indent + 2, stream);
    print_indented_line("type:", indent + 1, stream);
    print_indented_line(this->type.getValue(), indent + 2, stream);
    print_indented_line("size:", indent + 1, stream);
    print_indented_line(std::to_string(this->size), indent + 2, stream);
}

void Leaf::accept(NodeVisitorInterface *_visitor) { _visitor->visitLeaf(this); }
void FormalParamsNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitFormalParamsNode(this); }
void ActualParamsNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitActualParamsNode(this); }
void CallNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitCallNode(this); }
void BinaryNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitBinaryNode(this); }
void UnaryNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitUnaryNode(this); }
void AssignmentNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitAssignmentNode(this); }
void BlockNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitBlockNode(this); }
void ProgramNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitProgramNode(this); }
void FunctionNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitFunctionNode(this); }
void ProcedureNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitProcedureNode(this); }
void VariableDeclarationNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitVarDeclNode(this); }
void ListDeclarationNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitListDeclNode(this); }
void ElseNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitElseNode(this); }
void IfNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitIfNode(this); }
void WhileNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitWhileNode(this); }
void ForNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitForNode(this); }
