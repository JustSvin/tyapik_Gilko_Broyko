#pragma once
#include <pxx/token/Token.hpp>
#include <pxx/interface/LexerStateInterface.hpp>

class LexerInterface
{
public:
    virtual void open(std::istream &_stream) = 0;
    virtual Token getToken() = 0;
    virtual void setState(LexerStateInterface *_state) = 0;
    virtual ~LexerInterface() = default;
};
