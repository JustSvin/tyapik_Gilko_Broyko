#include <iostream>
#include <filesystem>
#include <fstream>
#include <pxx/lexer/Lexer.hpp>
#include <pxx/parser/Parser.hpp>
#include <pxx/semanalyzer/SemanticAnalyzer.hpp>
#include <pxx/codegen/CodeGenerator.hpp>
#include <pxx/optimizer/Optimizer.hpp>


void lexer() {
    std::ifstream input("code examples/input.pas");
    std::ofstream lexer_output("code examples/lexer_output.txt");

    auto lexer = std::make_unique<Lexer>();

    lexer->open(input);

    lexer->print_all_tokens(lexer_output);
}

void parser() {
    std::ifstream input("code examples/input.pas");
    std::ofstream parser_output("code examples/parser_output.txt");

    auto lexer = std::make_unique<Lexer>();
    auto parser = std::make_unique<Parser>();

    lexer->open(input);
    parser->setLexer(lexer.get());
    auto ast = parser->getAST();

    ast->root->print(0, parser_output);
}

void semalyzer() {
    std::ifstream input("code examples/input.pas");
    auto lexer = std::make_unique<Lexer>();
    auto parser = std::make_unique<Parser>();
    auto seman = std::make_unique<SemanticAnalyzer>();

    lexer->open(input);
    parser->setLexer(lexer.get());
    auto ast = parser->getAST();

    seman->checkSemantics(ast);
}

void optimizer() {
    std::ifstream input("code examples/input.pas");
    std::ofstream optimized_output("code examples/optimized_output.cpp");

    auto lexer = std::make_unique<Lexer>();
    auto parser = std::make_unique<Parser>();
    auto seman = std::make_unique<SemanticAnalyzer>();
    auto codegen = std::make_unique<CodeGenerator>(optimized_output);
    auto optimizer = std::make_unique<Optimizer>();

    lexer->open(input);
    parser->setLexer(lexer.get());
    auto ast = parser->getAST();

    optimizer->optimize(ast);
    codegen->generate(ast);
}

void codegen() {
    std::ifstream input("code examples/input.pas");
    std::ofstream output("code examples/output.cpp");

    auto lexer = std::make_unique<Lexer>();
    auto parser = std::make_unique<Parser>();
    auto seman = std::make_unique<SemanticAnalyzer>();
    auto codegen = std::make_unique<CodeGenerator>(output);
    auto optimizer = std::make_unique<Optimizer>();

    lexer->open(input);
    parser->setLexer(lexer.get());
    auto ast = parser->getAST();

    codegen->generate(ast);
}


int main()
{
    try {
        lexer();
        parser();
        semalyzer();
        optimizer();
        codegen();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
