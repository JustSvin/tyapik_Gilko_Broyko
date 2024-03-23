#include <pxx/parser/Parser.hpp>
#include <pxx/token/Token.hpp>
#include <pxx/AST/ASTNode.hpp>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>

static const std::map<std::string, std::set<Type>> FIRSTS = {
    {"variable_declaration", {Type::varkw}},
    {"root_stmts", {Type::varkw, Type::functionkw, Type::procedurekw}},
    {"sum", {Type::string, Type::id, Type::plus, Type::number, Type::minus, Type::lpr}},
    {"disjunction", {Type::plus, Type::id, Type::string, Type::number, Type::minus, Type::notkw, Type::lpr}},
    {"simple_stmt", {Type::plus, Type::id, Type::string, Type::number, Type::minus, Type::varkw, Type::notkw, Type::lpr}},
    {"type_part", {Type::colon}},
    {"func_call", {Type::lpr}},
    {"block", {Type::beginkw}},
    {"while_stmt", {Type::whilekw}},
    {"inversion", {Type::plus, Type::id, Type::string, Type::number, Type::minus, Type::notkw, Type::lpr}},
    {"conjunction", {Type::plus, Type::id, Type::string, Type::number, Type::minus, Type::notkw, Type::lpr}},
    {"atom", {Type::number, Type::string, Type::id}},
    {"procedure_declaration", {Type::procedurekw}},
    {"program_block", {Type::varkw, Type::beginkw, Type::functionkw, Type::procedurekw}},
    {"compound_stmt", {Type::whilekw, Type::ifkw, Type::forkw}},
    {"term", {Type::string, Type::id, Type::plus, Type::number, Type::minus, Type::lpr}},
    {"list_declaration", {Type::varkw}},
    {"expression", {Type::plus, Type::id, Type::string, Type::number, Type::minus, Type::notkw, Type::lpr}},
    {"if_stmt", {Type::ifkw}},
    {"root_stmt", {Type::varkw, Type::functionkw, Type::procedurekw}},
    {"assignment", {Type::id}},
    {"statements", {Type::plus, Type::id, Type::string, Type::forkw, Type::number, Type::minus, Type::varkw, Type::notkw, Type::whilekw, Type::ifkw, Type::lpr}},
    {"statement", {Type::plus, Type::id, Type::string, Type::forkw, Type::number, Type::minus, Type::varkw, Type::notkw, Type::whilekw, Type::ifkw, Type::lpr}},
    {"formal_params", {Type::id}},
    {"for_stmt", {Type::forkw}},
    {"function_declaration", {Type::functionkw}},
    {"else_block", {Type::elsekw}},
    {"comparison", {Type::string, Type::id, Type::plus, Type::number, Type::minus, Type::lpr}},
    {"program", {Type::beginkw, Type::procedurekw, Type::programkw, Type::functionkw, Type::varkw}},
    {"factor", {Type::string, Type::id, Type::plus, Type::number, Type::minus, Type::lpr}},
    {"program_heading", {Type::programkw}},
    {"actual_params", {Type::plus, Type::id, Type::string, Type::number, Type::minus, Type::notkw, Type::lpr}},
    {"primary", {Type::string, Type::number, Type::lpr, Type::id}}};

Parser::Parser() : token(Token("хуй", Type::autokw)){};

void Parser::setLexer(LexerInterface *lexer)
{
    this->lexer = lexer;
    this->next_token();
}

AST *Parser::getAST()
{
    return new AST(this->program());
}

bool Parser::is_token_in_firsts(std::string grammar_node)
{
    // Возвращает True, если текущий токен кода можно получить, спускаясь по узлу грамматика grammar_node
    return FIRSTS.at(grammar_node).find(this->token.getType()) != FIRSTS.at(grammar_node).end();
}

bool Parser::token_matches_any(std::vector<Type> types)
{
    // Возвращает True, если текущий токен кода имеет тип, имеющийся в types
    for (Type type : types)
    {
        if (token_matches(type))
        {
            return true;
        }
    }
    return false;
}

bool Parser::token_matches(Type type)
{
    // Возвращает True, если текущий токен кода имеет тип type
    return this->get_token().getType() == type;
}

void Parser::error(std::string message)
{
    std::string pos = std::to_string(this->get_token().getPos());
    std::string row = std::to_string(this->get_token().getRow());
    std::string value = this->get_token().getValue();
    std::string type = type_to_str(this->get_token().getType());
    message += " pos=" + pos + " row=" + row + " type=" + type + " value=" + value;
    std::cout << message << std::endl;
    throw std::runtime_error(message);
}

void Parser::next_token()
{
    if (this->future_tokens.empty())
    {
        this->token = this->lexer->getToken();
    }
    else
    {
        // Забираем токен из списка будущих токенов
        this->token = this->future_tokens[0];
        this->future_tokens.erase(this->future_tokens.begin());
    }
}

Token Parser::forward(int k)
{
    while (this->future_tokens.size() < k)
    {
        this->future_tokens.push_back(this->lexer->getToken());
    }
    return this->future_tokens[k - 1];
}

Token Parser::get_token()
{
    return this->token;
}

Token Parser::check_get_next(Type type)
{
    // Проверяет, что текущий токен имеет тип type, возвращает его и сразу получает следующий токен
    if (this->token.getType() != type)
    {
        this->error("Unexpected token");
    }
    Token tmp = this->token;
    this->next_token();
    return tmp;
}

ProgramNode *Parser::program()
{
    /*
    program:
        | program_heading program_block EOF
        | program_block EOF
     */
    ProgramNode *program = new ProgramNode();
    if (this->is_token_in_firsts("program_heading"))
    {
        this->program_heading(program);
    }
    this->program_block(program);
    program->add_child(new Leaf(this->check_get_next(Type::eof)));
    return program;
};

void Parser::program_heading(ProgramNode *program_block)
{
    /*
    program_heading:
        | PROGRAM ID SEMICOLON
    */
    this->check_get_next(Type::programkw);
    program_block->program_name = this->check_get_next(Type::id).getValue();
    this->check_get_next(Type::semicolon);
}

void Parser::program_block(ProgramNode *program_block)
{
    /*
    program_block:
        | root_stmts block DOT
        | block DOT
    */
    if (this->is_token_in_firsts("root_stmts"))
    {
        this->root_stmts(program_block);
    }
    program_block->add_child(this->block());
    this->check_get_next(Type::dot);
}

void Parser::root_stmts(ProgramNode *program_block)
{
    /*
    root_stmts:
        | root_stmt SEMICOLON
        | root_stmt SEMICOLON root_stmts
    */
    while (this->is_token_in_firsts("root_stmt"))
    {
        this->root_stmt(program_block);
        this->check_get_next(Type::semicolon);
    }
}

void Parser::root_stmt(BlockNode *parent_block)
{
    /*
    root_stmt:
        | variable_declaration
        | function_declaration
        | procedure_declaration
        | list_declaration
     */
    if (this->is_token_in_firsts("list_declaration") && (this->forward(3).getType() == Type::arraykw))
    {
        parent_block->add_child(this->list_declaration());
    }
    else if (this->is_token_in_firsts("variable_declaration"))
    {
        parent_block->add_child(this->variable_declaration());
    }
    else if (this->is_token_in_firsts("function_declaration"))
    {
        parent_block->add_child(this->function_declaration());
    }
    else if (this->is_token_in_firsts("procedure_declaration"))
    {
        parent_block->add_child(this->procedure_declaration());
    }
    else
    {
        this->error("statement");
    }
}

Token Parser::type_part()
{
    this->check_get_next(Type::colon);
    return this->check_get_next(Type::id);
}

VariableDeclarationNode *Parser::variable_declaration()
{
    /*
    variable_declaration:
        | VAR ID ASSIGN expression
        | VAR ID type_part
    */
    this->check_get_next(Type::varkw);
    Token var_name = this->check_get_next(Type::id);
    ExpressionNode *right_expr = nullptr;
    Leaf *type = nullptr;
    if (this->token_matches(Type::assign))
    {
        this->next_token();
        right_expr = this->expression();
    }
    else
    {
        type = new Leaf(this->type_part());
    }
    return new VariableDeclarationNode(var_name, right_expr, type);
}

ListDeclarationNode *Parser::list_declaration()
{
    /*
    list_declaration:
        | VAR ID COLON ARRAY LSBR NUMBER DOT DOT NUMBER RSBR OF ID
    */
    this->check_get_next(Type::varkw);
    Token var_name = this->check_get_next(Type::id);
    this->check_get_next(Type::colon);
    this->check_get_next(Type::arraykw);
    this->check_get_next(Type::lsbr);
    this->check_get_next(Type::number);
    this->check_get_next(Type::dot);
    this->check_get_next(Type::dot);
    int size = std::stoi(this->check_get_next(Type::number).getValue());
    this->check_get_next(Type::rsbr);
    this->check_get_next(Type::ofkw);
    Token type = this->check_get_next(Type::id);
    return new ListDeclarationNode(var_name, type, size);
}

FunctionNode *Parser::function_declaration()
{
    /*
    function_declaration:
        | FUNCTION ID LPR formal_params RPR COLON ID block
        | FUNCTION ID LPR RPR COLON ID block
    */
    this->check_get_next(Type::functionkw);
    Leaf *id = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::lpr);
    FormalParamsNode *formal_params;
    if (this->is_token_in_firsts("formal_params"))
    {
        formal_params = this->formal_params();
    }
    else
    {
        formal_params = new FormalParamsNode({}, {});
    }
    this->check_get_next(Type::rpr);
    this->check_get_next(Type::colon);
    Leaf *return_type = new Leaf(this->check_get_next(Type::id));
    BlockNode *body = this->block();
    return new FunctionNode(id, formal_params, return_type, body);
};

ProcedureNode *Parser::procedure_declaration()
{
    /*
    procedure_declaration:
        | PROCEDURE ID LPR formal_params RPR block
        | PROCEDURE ID LPR RPR block
     */
    this->check_get_next(Type::procedurekw);
    Leaf *id = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::lpr);
    FormalParamsNode *formal_params;
    if (this->is_token_in_firsts("formal_params"))
    {
        formal_params = this->formal_params();
    }
    else
    {
        formal_params = new FormalParamsNode({}, {});
    }
    this->check_get_next(Type::rpr);
    BlockNode *body = this->block();
    return new ProcedureNode(id, formal_params, body);
}

FormalParamsNode *Parser::formal_params()
{
    /*
    formal_params:
        | ID type_part SEMICOLON formal_params
        | ID type_part
     */
    FormalParamsNode *params = new FormalParamsNode({}, {});
    while (this->is_token_in_firsts("formal_params"))
    {
        Leaf *name = new Leaf(this->check_get_next(Type::id));
        Leaf *type = new Leaf(this->type_part());
        params->add_param(name, type);
        if (this->token_matches(Type::semicolon))
        {
            this->next_token();
        }
    }
    return params;
};

BlockNode *Parser::block()
{
    /*
    block:
        | BEGIN statements END
     */
    BlockNode *block = new BlockNode();
    this->check_get_next(Type::beginkw);
    this->statements(block);
    this->check_get_next(Type::endkw);
    return block;
}

void Parser::statements(BlockNode *parent_block)
{
    /*
    statements:
        | statement SEMICOLON statements
        | statement SEMICOLON
     */
    while (this->is_token_in_firsts("statements"))
    {
        this->statement(parent_block);
        this->check_get_next(Type::semicolon);
    }
};

void Parser::statement(BlockNode *parent_block)
{
    /*
    statement:
        | compound_stmt
        | simple_stmt
     */
    if (this->is_token_in_firsts("compound_stmt"))
    {
        this->compound_stmt(parent_block);
    }
    else if (this->is_token_in_firsts("simple_stmt"))
    {
        this->simple_stmt(parent_block);
    }
}

void Parser::compound_stmt(BlockNode *parent_block)
{
    /*
    compound_stmt:
        | if_stmt
        | for_stmt
        | while_stmt
     */
    if (this->is_token_in_firsts("if_stmt"))
    {
        parent_block->add_child(this->if_stmt());
    }
    else if (this->is_token_in_firsts("for_stmt"))
    {
        parent_block->add_child(this->for_stmt());
    }
    else if (this->is_token_in_firsts("while_stmt"))
    {
        parent_block->add_child(this->while_stmt());
    }
    else
    {
        this->error("compound_stmt");
    }
};

IfNode *Parser::if_stmt()
{
    /*
    if_stmt:
        | IFKW expression THENKW block else_block
        | IFKW expression THENKW block
     */
    this->check_get_next(Type::ifkw);
    ExpressionNode *condition = this->expression();
    this->check_get_next(Type::thenkw);
    BlockNode *body = this->block();
    IfNode *if_node = new IfNode(condition, body);
    if (this->is_token_in_firsts("else_block"))
    {
        if_node->next_else = this->else_block();
    }
    return if_node;
};

ElseNode *Parser::else_block()
{
    /*
    else_block:
        | ELSEKW block
     */
    this->check_get_next(Type::elsekw);
    BlockNode *body = this->block();
    return new ElseNode(body);
};

WhileNode *Parser::while_stmt()
{
    /*
    while_stmt:
        | WHILEKW expression DO block
     */
    this->check_get_next(Type::whilekw);
    ExpressionNode *condition = this->expression();
    this->check_get_next(Type::dokw);
    BlockNode *body = this->block();
    return new WhileNode(condition, body);
};

ForNode *Parser::for_stmt()
{
    /*
    for_stmt:
        | FOR ID ASSIGN (ID | NUMBER) TO (ID | NUMBER) DO block
    */
    this->check_get_next(Type::forkw);
    Leaf *iterator = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::assign);
    Leaf *from;
    Leaf *to;
    if (this->token_matches_any({Type::number, Type::id}))
    {
        from = new Leaf(this->get_token());
        this->next_token();
    }
    else
    {
        this->error("for_stmt");
    }
    this->check_get_next(Type::tokw);
    if (this->token_matches_any({Type::number, Type::id}))
    {
        to = new Leaf(this->get_token());
        this->next_token();
    }
    else
    {
        this->error("for_stmt");
    }
    this->check_get_next(Type::dokw);
    BlockNode *body = this->block();
    return new ForNode(iterator, from, to, body);
};

void Parser::simple_stmt(BlockNode *parent_block)
{
    /*
    simple_stmt:
        | assignment
        | expression
        | variable_declaration
        | list_declaration
     */
    if (this->is_token_in_firsts("assignment") && this->forward(1).getType() == Type::assign)
    {
        parent_block->add_child(this->assignment());
    }
    else if (this->is_token_in_firsts("expression"))
    {
        parent_block->add_child(this->expression());
    }
    else if (this->is_token_in_firsts("list_declaration") && (this->forward(3).getType() == Type::arraykw))
    {
        parent_block->add_child(this->list_declaration());
    }
    else if (this->is_token_in_firsts("variable_declaration"))
    {
        parent_block->add_child(this->variable_declaration());
    }
    else
    {
        this->error("simple_stmt");
    }
};

AssignmentNode *Parser::assignment()
{
    /*
    assignment:
        | ID ASSIGN expression
     */
    Leaf *left = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::assign);
    ExpressionNode *right = this->expression();
    return new AssignmentNode(left, right);
};

ExpressionNode *Parser::expression()
{
    /*
    expression:
        | disjunction
     */
    return this->disjunction();
};

ExpressionNode *Parser::disjunction()
{
    /*
    disjunction:
        | conjunction OR disjunction
        | conjunction
     */
    ExpressionNode *left = this->conjunction();
    if (token_matches(Type::orkw))
    {
        Token op = this->check_get_next(Type::orkw);
        ExpressionNode *right = this->conjunction();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (token_matches(Type::orkw))
        {
            op = this->check_get_next(Type::orkw);
            right = this->conjunction();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
};

ExpressionNode *Parser::conjunction()
{
    /*
    conjunction:
        | inversion AND conjunction
        | inversion
     */
    ExpressionNode *left = this->inversion();
    if (token_matches(Type::andkw))
    {
        Token op = this->check_get_next(Type::andkw);
        ExpressionNode *right = this->inversion();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (token_matches(Type::andkw))
        {
            op = this->check_get_next(Type::andkw);
            right = this->inversion();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
};

ExpressionNode *Parser::inversion()
{
    /*
    inversion:
        | NOT inversion
        | comparison
     */
    if (token_matches(Type::notkw))
    {
        Token op = this->check_get_next(Type::notkw);
        ExpressionNode *operand = this->inversion();
        return new UnaryNode(new Leaf(op), operand);
    }
    else
    {
        return this->comparison();
    }
};

ExpressionNode *Parser::comparison()
{
    /*
    comparison:
        | sum GREATER comparison
        | sum LESS comparison
        | sum EQUAL comparison
        | sum NOTEQ comparison
        | sum GREQUAL comparison
        | sum LEQUAL comparison
        | sum
     */

    ExpressionNode *left = this->sum();
    std::vector<Type> ok_ops = {Type::greater, Type::less, Type::equal, Type::noteq, Type::grequal, Type::lequal};
    if (this->token_matches_any(ok_ops))
    {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->sum();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (this->token_matches_any(ok_ops))
        {
            op = this->get_token();
            this->next_token();
            right = this->sum();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
}

ExpressionNode *Parser::sum()
{
    /*
    sum:
        | term PLUS sum
        | term MINUS sum
        | term
     */
    ExpressionNode *left = this->term();
    std::vector<Type> ok_ops = {Type::plus, Type::minus};
    if (this->token_matches_any(ok_ops))
    {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->term();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (this->token_matches_any(ok_ops))
        {
            op = this->get_token();
            this->next_token();
            right = this->term();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
};

ExpressionNode *Parser::term()
{
    /*
    term:
        | factor STAR term
        | factor DIV term
        | factor MOD term
        | factor
     */
    ExpressionNode *left = this->factor();
    std::vector<Type> ok_ops = {Type::star, Type::div, Type::modkw};
    if (this->token_matches_any(ok_ops))
    {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->factor();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (this->token_matches_any(ok_ops))
        {
            op = this->get_token();
            this->next_token();
            right = this->factor();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
};

ExpressionNode *Parser::factor()
{
    /*
    factor:
        | PLUS factor
        | MINUS factor
        | primary
     */
    if (this->token_matches_any({Type::plus, Type::minus}))
    {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *operand = this->factor();
        return new UnaryNode(new Leaf(op), operand);
    }
    else
    {
        return this->primary();
    }
};

ExpressionNode *Parser::primary()
{
    /*
    primary:
        | LPR expression RPR
        | atom
        | atom func_call
     */
    if (this->token_matches(Type::lpr))
    {
        this->check_get_next(Type::lpr);
        auto expr = this->expression();
        this->check_get_next(Type::rpr);
        return expr;
    }
    else if (this->is_token_in_firsts("atom"))
    {
        Leaf *atom = this->atom();
        if (this->is_token_in_firsts("func_call"))
        {
            ActualParamsNode *params = this->func_call();
            return new CallNode(atom->token, params);
        }
        return atom;
    }
    this->error("primary");
    return nullptr;
};

ActualParamsNode *Parser::func_call()
{
    /*
    func_call:
        | LPR actual_params RPR
        | LPR RPR
     */
    this->check_get_next(Type::lpr);
    if (this->is_token_in_firsts("actual_params"))
    {
        ActualParamsNode *params = this->actual_params();
        this->check_get_next(Type::rpr);
        return params;
    }
    else
    {
        this->check_get_next(Type::rpr);
        return new ActualParamsNode({});
    }
};

Leaf *Parser::atom()
{
    /*
    atom:
        | ID
        | STRING
        | NUMBER
     */
    if (this->token_matches_any({Type::id, Type::string, Type::number}))
    {
        Leaf *leaf = new Leaf(this->get_token());
        this->next_token();
        return leaf;
    }
    else
    {
        this->error("atom");
        return nullptr;
    }
};

ActualParamsNode *Parser::actual_params()
{
    /*
    arguments:
        | expression COMMA actual_params
        | expression
     */
    ActualParamsNode *params = new ActualParamsNode({});
    while (this->is_token_in_firsts("actual_params"))
    {
        params->add_child(this->expression());
        if (token_matches(Type::comma))
        {
            this->next_token();
        }
    }
    return params;
};