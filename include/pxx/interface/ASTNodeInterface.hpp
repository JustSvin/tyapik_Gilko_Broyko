#pragma once
#include <iostream>

class ASTNodeInterface
{
public:
    virtual ~ASTNodeInterface() = default;
    virtual void print(int indent, std::ostream& stream) = 0;
};
