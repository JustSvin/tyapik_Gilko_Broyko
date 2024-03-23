#pragma once
#include <pxx/interface/SemanticAnalyzerInterface.hpp>
#include <pxx/semanalyzer/SemanticVisitor.hpp>
#include <memory>

class SemanticAnalyzer : public SemanticAnalyzerInterface
{
private:
    std::unique_ptr<SemanticVisitor> visitor;

public:
    SemanticAnalyzer();
    void checkSemantics(AST *_tree) override;
};
