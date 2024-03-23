#include <pxx/semanalyzer/SemanticAnalyzer.hpp>

SemanticAnalyzer::SemanticAnalyzer()
{
    visitor = std::make_unique<SemanticVisitor>();
    visitor->stdinit();
}

void SemanticAnalyzer::checkSemantics(AST* _tree)
{
    _tree->accept(visitor.get());
}
