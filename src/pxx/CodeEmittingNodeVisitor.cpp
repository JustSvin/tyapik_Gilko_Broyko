#include <pxx/codegen/CodeEmittingNodeVisitor.hpp>
#include <pxx/AST/ASTNode.hpp>
#include <map>

CodeEmittingNodeVisitor::CodeEmittingNodeVisitor(std::ostream& _stream): stream(_stream){}

void CodeEmittingNodeVisitor::write(std::string s) {
    std::map<std::string, std::string> reserved = {
        {"integer", "int"},
        {"string", "std::string"}  
    };
    if (reserved.find(s) != reserved.end()) {
        this->stream << reserved.at(s);
    } else {
        this->stream << s;
    }
}

void CodeEmittingNodeVisitor::write(Token token) {
    write(token.getValue());
}

void CodeEmittingNodeVisitor::write(Leaf* leaf) {
    write(leaf->token);
}

void CodeEmittingNodeVisitor::visitLeaf(Leaf *_acceptor)
{
    write(_acceptor);
}

void CodeEmittingNodeVisitor::visitFormalParamsNode(FormalParamsNode *_acceptor)
{
    int count = _acceptor->names.size();
    for (int i = 0; i < count; i++) {
        write(_acceptor->types[i]);
        write(" ");
        write(_acceptor->names[i]);
        if (i != (count - 1))
            write(", ");
    }
}
void CodeEmittingNodeVisitor::visitActualParamsNode(ActualParamsNode *_acceptor)
{
    int count = _acceptor->params.size();
    for (int i = 0; i < count; i++) {
        _acceptor->params[i]->accept(this);
        if (i != (count - 1))
            write(", ");
    }
}
void CodeEmittingNodeVisitor::visitCallNode(CallNode *_acceptor)
{
    write(_acceptor->callable);
    write("(");
    _acceptor->params->accept(this);
    write(")");
}
void CodeEmittingNodeVisitor::visitBinaryNode(BinaryNode *_acceptor)
{
    std::map<Type, std::string> bin_op_strs = {
        {Type::orkw, "||"},
        {Type::andkw, "&&"},
        {Type::plus, "+"},
        {Type::minus, "-"},
        {Type::star, "*"},
        {Type::div, "/"},
        {Type::greater, ">"},
        {Type::less, "<"},
        {Type::equal, "=="},
        {Type::noteq, "!="},
        {Type::grequal, ">="},
        {Type::lequal, "<="},
        {Type::modkw, "%"},
    };
    Type op = _acceptor->op->token.getType();
    write("(");
    _acceptor->left->accept(this);
    write(") ");
    write(bin_op_strs.at(op));
    write(" (");
    _acceptor->right->accept(this);
    write(")");
}
void CodeEmittingNodeVisitor::visitUnaryNode(UnaryNode *_acceptor)
{
    std::map<Type, std::string> bin_op_strs = {
        {Type::notkw, "!"},
        {Type::plus, "+"},
        {Type::minus, "-"}
    };
    Type op = _acceptor->op->token.getType();
    write(bin_op_strs.at(op));
    write("(");
    _acceptor->operand->accept(this);
    write(")");
}
void CodeEmittingNodeVisitor::visitAssignmentNode(AssignmentNode *_acceptor)
{
    write(_acceptor->left);
    write(" = ");
    _acceptor->right->accept(this);
}
void CodeEmittingNodeVisitor::visitBlockNode(BlockNode *_acceptor)
{
    write("{\n");
    for (auto child: _acceptor->children) {
        child->accept(this);
        write(";\n");
    }
    write("}");
}
void CodeEmittingNodeVisitor::visitProgramNode(ProgramNode *_acceptor)
{
    for (auto child: _acceptor->children) {
        child->accept(this);
        write(";\n");
    }
}
void CodeEmittingNodeVisitor::visitFunctionNode(FunctionNode *_acceptor)
{
    write(_acceptor->return_type);
    write(" ");
    write(_acceptor->id);
    write("(");
    _acceptor->formal_params->accept(this);
    write(")\n");
    _acceptor->body->accept(this);
}
void CodeEmittingNodeVisitor::visitProcedureNode(ProcedureNode *_acceptor)
{
    write("void");
    write(" ");
    write(_acceptor->id);
    write("(");
    _acceptor->formal_params->accept(this);
    write(")\n");
    _acceptor->body->accept(this);
}
void CodeEmittingNodeVisitor::visitElseNode(ElseNode *_acceptor)
{
    write("else");
    _acceptor->body->accept(this);
}
void CodeEmittingNodeVisitor::visitIfNode(IfNode *_acceptor)
{
    write("if");
    write(" ");
    write("(");
    _acceptor->condition->accept(this);
    write(")\n");
    _acceptor->body->accept(this);
    _acceptor->next_else->accept(this);
}
void CodeEmittingNodeVisitor::visitWhileNode(WhileNode *_acceptor)
{
    write("while");
    write(" ");
    write("(");
    _acceptor->condition->accept(this);
    write(")\n");
    _acceptor->body->accept(this);  
}
void CodeEmittingNodeVisitor::visitForNode(ForNode *_acceptor)
{
    write("for");
    write(" ");
    write("(");
    write("int ");
    write(_acceptor->iterator);
    write(" = ");
    write(_acceptor->from);
    write("; ");
    write(_acceptor->iterator);
    write(" < ");
    write(_acceptor->to);
    write("; ");
    write(_acceptor->iterator);
    write("++");
    write(")\n");
    _acceptor->body->accept(this);  
}
void CodeEmittingNodeVisitor::visitVarDeclNode(VariableDeclarationNode *_acceptor)
{
    if (_acceptor->type == nullptr) {
        write("auto");
    } else {
        write(_acceptor->type);
    }
    write(" ");
    write(_acceptor->var_name);
    if (_acceptor->right_expr != nullptr) {
        write(" = ");
        _acceptor->right_expr->accept(this);
    }
}
void CodeEmittingNodeVisitor::visitListDeclNode(ListDeclarationNode *_acceptor) {
    write(_acceptor->type);
    write(" ");
    write(_acceptor->var_name);
    write("[");
    write(std::to_string(_acceptor->size));
    write("]");
}