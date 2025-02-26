#include <pxx/semanalyzer/SemanticVisitor.hpp>
#include <pxx/token/Token.hpp>
#include <pxx/AST/ASTNode.hpp>
#include <stdexcept>
#include <iostream>
#include <map>

void SemanticVisitor::visitFormalParamsNode(FormalParamsNode *_acceptor) {}
void SemanticVisitor::visitActualParamsNode(ActualParamsNode *_acceptor) {}

void SemanticVisitor::visitLeaf(Leaf *_acceptor)
{
    auto &token = _acceptor->token;
    if (token.getType() == Type::id)
    {
        auto symbol = symtable.top()->find(token.getValue());
        if (symbol == symtable.top()->end())
        {
            throw std::runtime_error(
                "Name " + token.getValue() + " is undefined\n" +
                "Occured at row: " +
                std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
        }

        lastpos = token.getPos();
        lastrow = token.getRow();
        evaluated_type = symbol->second.type;
    }
    else
    {
        switch (token.getType())
        {
        case Type::string:
            evaluated_type = set.insert("string").first;
            break;
        case Type::number:
            if (token.getValue().find('.') == token.getValue().npos)
                evaluated_type = set.insert("integer").first;
            else
                evaluated_type = set.insert("float").first;
            break;

        default:
            evaluated_type = set.insert("void").first;
            break;
        }
    }
}

void SemanticVisitor::visitFunctionNode(FunctionNode *_acceptor)
{
    auto token = _acceptor->id->token;
    auto symbol = symtable.top()->find(token.getValue());
    auto symtype = set.insert(token.getValue());
    if (symbol == symtable.top()->end() && symtype.second)
    {
        symtable.top()->insert({token.getValue(), {token, symtype.first}});
        symtable.push(std::make_unique<localtable_t>(*symtable.top()));
        auto curr = funcs.insert({token.getValue(), {}}).first;
        curr->second.first = set.insert(_acceptor->return_type->token.getValue()).first;

        auto n = _acceptor->formal_params->names.begin();
        auto t = _acceptor->formal_params->types.begin();
        while (n != _acceptor->formal_params->names.end() && t != _acceptor->formal_params->types.end())
        {
            auto token = (*n)->token;
            auto type = (*t)->token;

            symtype = set.insert(type.getValue());
            curr->second.second.push_back(symtype.first);

            auto symbol = symtable.top()->find(token.getValue());
            if (symbol == symtable.top()->end())
            {
                symtable.top()->insert({token.getValue(), {token, symtype.first}});
            }
            else
            {
                throw std::runtime_error(
                    "Name " + token.getValue() + " is already defined\n" +
                    "Defined second time at row : " +
                    std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
            }

            ++n;
            ++t;
        }

        _acceptor->body->accept(this);
        symtable.pop();
    }
    else
    {
        throw std::runtime_error(
            "Name " + token.getValue() + " is already defined\n" +
            "Defined second time at row : " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
}

void SemanticVisitor::visitProcedureNode(ProcedureNode *_acceptor)
{
    auto token = _acceptor->id->token;
    auto symbol = symtable.top()->find(token.getValue());
    auto symtype = set.insert(token.getValue());
    if (symbol == symtable.top()->end() && symtype.second)
    {
        symtable.top()->insert({token.getValue(), {token, symtype.first}});
        symtable.push(std::make_unique<localtable_t>(*symtable.top()));
        auto curr = funcs.insert({token.getValue(), {}}).first;
        curr->second.first = set.insert("void").first;

        auto n = _acceptor->formal_params->names.begin();
        auto t = _acceptor->formal_params->types.begin();
        while (n != _acceptor->formal_params->names.end() && t != _acceptor->formal_params->types.end())
        {
            auto token = (*n)->token;
            auto type = (*t)->token;

            symtype = set.insert(type.getValue());
            curr->second.second.push_back(symtype.first);

            auto symbol = symtable.top()->find(token.getValue());
            if (symbol == symtable.top()->end())
            {
                symtable.top()->insert({token.getValue(), {token, symtype.first}});
            }
            else
            {
                throw std::runtime_error(
                    "Name " + token.getValue() + " is already defined\n" +
                    "Defined second time at row : " +
                    std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
            }

            ++n;
            ++t;
        }

        _acceptor->body->accept(this);
        symtable.pop();
    }
    else
    {
        throw std::runtime_error(
            "Name " + token.getValue() + " is already defined\n" +
            "Defined second time at row : " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
}

void SemanticVisitor::visitCallNode(CallNode *_acceptor)
{
    auto token = _acceptor->callable;
    auto symbol = symtable.top()->find(token.getValue());
    if (symbol == symtable.top()->end())
    {
        throw std::runtime_error(
            "Name " + token.getValue() + " is undefined\n" +
            "Occured at row: " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
    else
    {
        auto symtype = set.find(token.getValue());
        if (symbol->second.type != symtype)
        {
            throw std::runtime_error(
                "Not a callable at row: " +
                std::to_string(token.getRow()) +
                " position: " +
                std::to_string(token.getPos()) + "\n");
        }
    }

    auto func = funcs.find(token.getValue());
    if (func->second.second.size() != _acceptor->params->params.size())
    {
        throw std::runtime_error(
            "Parameter quantity mismatch occured at row: " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }

    auto arg_iter = func->second.second.begin();

    for (auto &par : _acceptor->params->params)
    {
        par->accept(this);
        if (evaluated_type != *arg_iter)
        {
            throw std::runtime_error(
                "Parameter type mismatch occured at row: " +
                std::to_string(lastrow) + " position: " + std::to_string(lastpos) + "\n");
        }
    }
    evaluated_type = func->second.first;
}

void SemanticVisitor::visitBinaryNode(BinaryNode *_acceptor)
{
    _acceptor->left->accept(this);
    type_t a = evaluated_type;
    _acceptor->right->accept(this);
    if (a != evaluated_type)
    {
        throw std::runtime_error(
            "Type mismatch occured at row: " +
            std::to_string(lastrow) + " position: " + std::to_string(lastpos) + "\n");
    }
    switch (_acceptor->op->token.getType())
    {
    case Type::less:
    case Type::greater:
    case Type::noteq:
    case Type::notkw:
    case Type::equal:
        evaluated_type = set.insert("bool").first;
        break;
    }
}

void SemanticVisitor::visitUnaryNode(UnaryNode *_acceptor)
{
    _acceptor->operand->accept(this);
}

void SemanticVisitor::visitAssignmentNode(AssignmentNode *_acceptor)
{
    auto token = _acceptor->left->token;
    auto symbol = symtable.top()->find(token.getValue());
    if (symbol == symtable.top()->end())
    {
        throw std::runtime_error(
            "Name " + token.getValue() + " is undefined\n" +
            "Occured at row: " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
    _acceptor->right->accept(this);
    if (symbol->second.type == set.find("void"))
    {
        symbol->second.type = evaluated_type;
    }
    else if (symbol->second.type != evaluated_type)
    {
        throw std::runtime_error(
            "Type mismatch occured at row: " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
    evaluated_type = set.find("void");
}

void SemanticVisitor::visitBlockNode(BlockNode *_acceptor)
{
    for (auto &i : _acceptor->children)
    {
        i->accept(this);
    }
}

void SemanticVisitor::visitProgramNode(ProgramNode *_acceptor)
{
    for (auto child : _acceptor->children)
    {
        child->accept(this);
    }
}

void SemanticVisitor::visitElseNode(ElseNode *_acceptor)
{
    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    _acceptor->body->accept(this);
    symtable.pop();
}

void SemanticVisitor::visitIfNode(IfNode *_acceptor)
{
    _acceptor->condition->accept(this);
    if (evaluated_type != set.find("bool"))
    {
        throw std::runtime_error(
            "Condition type is not bool.\nOccured at row: " +
            std::to_string(lastrow) + " position: " + std::to_string(lastpos) + "\n");
    }

    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    _acceptor->body->accept(this);
    symtable.pop();

    if (_acceptor->next_else)
    {
        _acceptor->next_else->accept(this);
    }
}

void SemanticVisitor::visitWhileNode(WhileNode *_acceptor)
{
    _acceptor->condition->accept(this);

    if (evaluated_type != set.find("bool"))
    {
        throw std::runtime_error(
            "Condition type is not bool.\nOccured at row: " +
            std::to_string(lastrow) + " position: " + std::to_string(lastpos) + "\n");
    }

    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    _acceptor->body->accept(this);
    symtable.pop();
}

void SemanticVisitor::visitForNode(ForNode *_acceptor)
{
    type_t a, b, c;

    auto &iter = _acceptor->iterator->token;

    _acceptor->iterator->accept(this);
    a = evaluated_type;
    _acceptor->from->accept(this);
    b = evaluated_type;
    _acceptor->to->accept(this);
    c = evaluated_type;

    if (!(a == b && b == c))
    {
        throw std::runtime_error(
            "Type mismatch occured at row: " +
            std::to_string(iter.getRow()) + " position: " + std::to_string(iter.getPos()) + "\n");
    }

    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    symtable.top()->insert({iter.getValue(), {iter, a}});
    _acceptor->body->accept(this);
    symtable.pop();
}

void SemanticVisitor::visitVarDeclNode(VariableDeclarationNode *_acceptor)
{
    auto &token = _acceptor->var_name;
    type_t symtype;
    if (_acceptor->type)
    {
        auto &type = _acceptor->type->token;
        symtype = set.insert(type.getValue()).first;
    }
    else
    {
        symtype = set.insert("void").first;
    }
    auto symbol = symtable.top()->find(token.getValue());
    if (symbol == symtable.top()->end())
    {
        symtable.top()->insert({token.getValue(), {token, symtype}});
    }
    else
    {
        throw std::runtime_error(
            "Name " + token.getValue() + " is already defined\n" +
            "Defined second time at row : " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
}

void SemanticVisitor::visitListDeclNode(ListDeclarationNode *_acceptor)
{
    auto &token = _acceptor->var_name;
    auto &type = _acceptor->type;
    std::string typestr = type.getValue() + "[" + std::to_string(_acceptor->size) + "]";
    auto symbol = symtable.top()->find(token.getValue());
    auto symtype = set.insert(typestr);
    if (symbol == symtable.top()->end())
    {
        symtable.top()->insert({token.getValue(), {token, symtype.first}});
    }
    else
    {
        throw std::runtime_error(
            "Name " + token.getValue() + " is already defined\n" +
            "Defined second time at row : " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
}

void SemanticVisitor::stdinit()
{
    symtable.push(std::make_unique<localtable_t>());

    Symbol tr = {{"true", Type::id}, set.insert("bool").first};
    Symbol fl = {{"false", Type::id}, set.insert("bool").first};
    Symbol wr = {{"writeln", Type::id}, set.insert("writeln").first};

    funcs.insert({"writeln", {set.insert("void").first, {set.insert("string").first}}});

    symtable.top()->insert({tr.token.getValue(), tr});
    symtable.top()->insert({fl.token.getValue(), fl});
    symtable.top()->insert({wr.token.getValue(), wr});
}
