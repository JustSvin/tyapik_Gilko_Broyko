#include <pxx/lexer/Lexer.hpp>
#include <pxx/lexer/LexerStates.hpp>
#include <unordered_map>
#include <iostream>
#define CHARCOUNT 1000

Type Lexer::recognize(const std::string &_id) const
{
    static const std::unordered_map<std::string, Type> map = {
        {"and", Type::andkw},
        {"array", Type::arraykw},
        {"as", Type::askw},
        {"auto", Type::autokw},
        {"begin", Type::beginkw},
        {"case", Type::casekw},
        {"class", Type::classkw},
        {"const", Type::constkw},
        {"constructor", Type::constructorkw},
        {"destructor", Type::destructorkw},
        {"div", Type::divkw},
        {"do", Type::dokw},
        {"downto", Type::downtokw},
        {"else", Type::elsekw},
        {"end", Type::endkw},
        {"event", Type::eventkw},
        {"except", Type::exceptkw},
        {"extensionmethod", Type::extensionmethodkw},
        {"file", Type::filekw},
        {"finalization", Type::finalizationkw},
        {"finally", Type::finallykw},
        {"for", Type::forkw},
        {"foreach", Type::foreachkw},
        {"function", Type::functionkw},
        {"goto", Type::gotokw},
        {"if", Type::ifkw},
        {"implementation", Type::implementationkw},
        {"in", Type::inkw},
        {"inherited", Type::inheritedkw},
        {"initialization", Type::initializationkw},
        {"interface", Type::interfacekw},
        {"is", Type::iskw},
        {"label", Type::labelkw},
        {"lock", Type::lockkw},
        {"loop", Type::loopkw},
        {"mod", Type::modkw},
        {"nil", Type::nilkw},
        {"not", Type::notkw},
        {"of", Type::ofkw},
        {"operator", Type::operatorkw},
        {"or", Type::orkw},
        {"procedure", Type::procedurekw},
        {"program", Type::programkw},
        {"property", Type::propertykw},
        {"raise", Type::raisekw},
        {"record", Type::recordkw},
        {"repeat", Type::repeatkw},
        {"sealed", Type::sealedkw},
        {"set", Type::setkw},
        {"sequence", Type::sequencekw},
        {"shl", Type::shlkw},
        {"shr", Type::shrkw},
        {"sizeof", Type::sizeofkw},
        {"template", Type::templatekw},
        {"then", Type::thenkw},
        {"to", Type::tokw},
        {"try", Type::trykw},
        {"type", Type::typekw},
        {"typeof", Type::typeofkw},
        {"until", Type::untilkw},
        {"uses", Type::useskw},
        {"using", Type::usingkw},
        {"var", Type::varkw},
        {"where", Type::wherekw},
        {"while", Type::whilekw},
        {"with", Type::withkw},
        {"xor", Type::xorkw},
        {"abstract", Type::abstractkw},
        {"default", Type::defaultkw},
        {"external", Type::externalkw},
        {"forward", Type::forwardkw},
        {"internal", Type::internalkw},
        {"on", Type::onkw},
        {"overload", Type::overloadkw},
        {"override", Type::overridekw},
        {"params", Type::paramskw},
        {"private", Type::privatekw},
        {"protected", Type::protectedkw},
        {"public", Type::publickw},
        {"read", Type::readkw},
        {"reintroduce", Type::reintroducekw},
        {"unit", Type::unitkw},
        {"virtual", Type::virtualkw},
        {"write", Type::writekw},
    };

    if (map.find(_id) != map.end())
        return map.at(_id);
    else
        return Type::id;
}

void Lexer::open(std::istream &_stream)
{
    filedata.reset(new FileData());
    currBuff.reset(new std::string(CHARCOUNT + 1, '\0'));
    otherBuff.reset(new std::string());

    this->stream = &_stream;

    this->stream->read(&(*currBuff)[0], CHARCOUNT);
    iter = currBuff->cbegin();
    setState(new Start(this, filedata.get()));
}

void Lexer::setState(LexerStateInterface *_state)
{
    this->state.reset(_state);
}

Token Lexer::getToken()
{
    while (filedata->queue.empty() && iter != currBuff->cend())
    {
        this->state->recognize(*iter++);
    }

    Token tok = filedata->get();
    bool failed = false;

    if (tok.getType() == Type::eof)
    {
        if (iter == currBuff->cend())
        {
            otherBuff->assign(CHARCOUNT + 1, '\0');
            stream->read(&(*otherBuff)[0], CHARCOUNT);
            iter = otherBuff->cbegin();
            currBuff.swap(otherBuff);
            while (filedata->queue.empty() && iter != currBuff->cend())
            {
                this->state->recognize(*iter++);
            }
            tok = filedata->get();
            if (tok.getType() == Type::eof)
            {
                failed = true;
            }
        }
        else
        {
            failed = true;
        }
    }
    if (failed)
    {
        return tok;
    }

    if (tok.getType() == Type::id)
    {
        tok.setType(recognize(tok.getValue()));
    }
    return tok;
}

void Lexer::print_all_tokens(std::ostream& output) {
    Token token("", Type::id);
    while ((token = this->getToken()).getType() != Type::eof) {
        output << type_to_str(token.getType()) << " " << token.getValue() << "\n";
    }
    output << type_to_str(token.getType()) << " " << token.getValue() << "\n";
}