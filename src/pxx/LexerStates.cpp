#include <pxx/lexer/LexerStates.hpp>
#include <pxx/lexer/FileData.hpp>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <string>

#define impl(name)                                                                         \
    name::name(LexerInterface *lex, FileData *filedata) : BaseLexerState(lex, filedata) {} \
    bool name::recognize(char _c)
#define newstate(name) lexer->setState(new name(lexer, filedata))
#define fac(state) \
    static inline LexerStateInterface *state##Factory(LexerInterface *a, FileData *b) { return new state(a, b); }
#define tab(symbol, state)     \
    {                          \
        symbol, state##Factory \
    }

typedef std::function<LexerStateInterface *(LexerInterface *, FileData *)> stateFactory_t;

fac(String)
fac(Colon)
fac(Dot)
fac(Plus)
fac(Minus)
fac(Star)
fac(Div)
fac(Equal)
fac(Reference)
fac(Greater)
fac(Less)
fac(Dereference)
fac(Lpr)
fac(Rpr)
fac(Lsbr)
fac(Rsbr)
fac(Ternary)
fac(Comma)
fac(Skip)
fac(Semicolon)
fac(Newline)

static std::unordered_map<char, stateFactory_t> table = {
    tab('+', Plus),
    tab('-', Minus),
    tab('*', Star),
    tab('/', Div),
    tab('=', Equal),
    tab('@', Dereference),
    tab('^', Reference),
    tab('?', Ternary),
    tab('<', Less),
    tab('>', Greater),
    tab('.', Dot),
    tab(',', Comma),
    tab('(', Lpr),
    tab(')', Rpr),
    tab('[', Lsbr),
    tab(']', Rsbr),
    tab(':', Colon),
    tab('"', String),
    tab(';', Semicolon),
    tab('\n', Newline),
};

static inline bool isSuitableForIdBeginning(char _c)
{
    return std::isalpha(_c) || _c == '_';
}

static inline bool isSuitableForId(char _c)
{
    return std::isalnum(_c) || _c == '_';
}

BaseLexerState::BaseLexerState(LexerInterface *_lex, FileData *_filedata) : lexer(_lex), filedata(_filedata), initpos(_filedata->pos) {}

static stateFactory_t tablestate(char _c)
{
    if (table.find(_c) != table.end())
        return table[_c];
    else if (_c == '\0')
    {
        return nullptr;
    }
    else
    {
        return SkipFactory;
    }
}

impl(Start)
{
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else if (_c != '\n')
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Skip)
{
    filedata->pos++;
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Id)
{
    filedata->pos++;
    if (isSuitableForId(_c))
    {
        filedata->accum.push_back(_c);
    }
    else
    {
        filedata->put(Type::id, filedata->row, initpos);
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(String)
{
    filedata->pos++;
    if (_c == '"')
    {
        filedata->put(Type::string, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->accum.push_back(_c);
    }
    return false;
}

impl(Colon)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::assign, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::colon, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Dot)
{
    filedata->pos++;
    filedata->put(Type::dot, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(FirstNumPart)
{
    filedata->pos++;
    if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
    }
    else if (_c == '.')
    {
        lexer->setState(new SecondNumPart(lexer, filedata, initpos));
    }
    else
    {
        filedata->put(Type::number, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

SecondNumPart::SecondNumPart(LexerInterface* _lex, FileData* _filedata, unsigned int _initpos) : BaseLexerState(_lex, _filedata)
{
    initpos = _initpos;
}
bool SecondNumPart::recognize(char _c)
{
    filedata->pos++;
    if (std::isdigit(_c))
    {
        if (created)
        {
            filedata->accum.push_back('.');
            created = false;
        }
        filedata->accum.push_back(_c);
    }
    else
    {
        filedata->put(Type::number, filedata->row, initpos);
        filedata->put(Type::dot, filedata->row, filedata->pos);
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Plus)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::plusass, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::plus, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Minus)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::minass, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::minus, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Star)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::mulass, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::star, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Div)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::divass, filedata->row, initpos);
        newstate(Skip);
    }
    else if (_c == '/')
    {
        newstate(Comment);
    }
    else
    {
        filedata->put(Type::div, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Reference)
{
    filedata->pos++;
        filedata->put(Type::ref, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    
    return false;
}

impl(Greater)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::grequal, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::greater, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Less)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::lequal, filedata->row, initpos);
        newstate(Skip);
    }
    else if (_c == '>')
    {
        filedata->put(Type::noteq, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::less, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Equal)
{
    filedata->pos++;
        filedata->put(Type::equal, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    
    return false;
}

impl(Dereference)
{
    filedata->pos++;
        filedata->put(Type::deref, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    

    return false;
}

impl(Lpr)
{
    filedata->pos++;
    filedata->put(Type::lpr, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Rpr)
{
    filedata->pos++;
    filedata->put(Type::rpr, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Lsbr)
{
    filedata->pos++;
    filedata->put(Type::lsbr, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Rsbr)
{
    filedata->pos++;
    filedata->put(Type::rsbr, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Ternary)
{
    filedata->pos++;
        filedata->put(Type::ternary, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    

    return false;
}

impl(Comment)
{
    if (_c == '\n')
    {
        filedata->row++;
        filedata->pos = 0;
        newstate(Skip);
    }
    return false;
}

impl(Comma)
{
    filedata->pos++;
    filedata->put(Type::comma, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Semicolon)
{
    filedata->pos++;
    filedata->put(Type::semicolon, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Newline)
{
    filedata->row++;
    filedata->pos = 0;
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}
