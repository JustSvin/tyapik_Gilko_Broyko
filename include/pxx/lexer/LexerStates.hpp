#pragma once
#include <pxx/interface/LexerStateInterface.hpp>
#include <pxx/interface/LexerInterface.hpp>
#include <pxx/lexer/FileData.hpp>
#include <memory>
#include <stack>

#define st(name)                                         \
    class name : public BaseLexerState                   \
    {                                                    \
    public:                                              \
        name(LexerInterface *_lex, FileData *_filedata); \
        bool recognize(char _c);                         \
    };

typedef std::stack<unsigned int> instack;

class BaseLexerState : public LexerStateInterface
{
protected:
    LexerInterface *lexer;
    FileData *filedata;
    unsigned int initpos;
    Type type;
    BaseLexerState(LexerInterface *_lex, FileData *_filedata);
};

st(Start)
st(Skip)
st(Id)
st(FirstNumPart)
st(String)
st(Colon)
st(Dot)
st(Plus)
st(Minus)
st(Star)
st(Div)
st(Equal)
st(Reference)
st(Dereference)
st(Greater)
st(Less)
st(Lpr)
st(Rpr)
st(Lsbr)
st(Rsbr)
st(Ternary)
st(Comment)
st(Comma)
st(Semicolon)
st(Newline)

class SecondNumPart : public BaseLexerState
{
private:
    bool created = true;
public:
    SecondNumPart(LexerInterface *_lex, FileData *_filedata, unsigned int _initpos);
    bool recognize(char _c);
};

#undef st
