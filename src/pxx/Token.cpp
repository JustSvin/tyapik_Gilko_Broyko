#include <pxx/token/Token.hpp>
#include <unordered_set>

const std::string& Token::getValue() const
{
    return value;
}

Type Token::getType() const
{
    return type;
}

unsigned int Token::getPos() const
{
    return pos;
}

unsigned int Token::getRow() const
{
    return row;
}

void Token::setValue(std::string _value)
{
    this->value = _value;
}

void Token::setType(Type _type)
{
    this->type = _type;
}

Token::Token(std::string _value, Type _type, unsigned int _row, unsigned int _pos) : value(_value), type(_type), row(_row), pos(_pos) {}

bool Token::operator==(const Token &_other) const
{
    return this->type == _other.getType() &&
           this->value == _other.getValue() &&
           this->row == _other.getRow() &&
           this->pos == _other.getPos();
}

bool Token::operator!=(const Token &_other) const
{
    return this->type != _other.getType() ||
           this->value != _other.getValue() ||
           this->row != _other.getRow() ||
           this->pos != _other.getPos();
}

std::string type_to_str(Type type)
{
    switch (type)
    {
    case (Type::id):
        return "ID";
    case (Type::number):
        return "NUMBER";
    case (Type::string):
        return "STRING";
    case (Type::lpr):
        return "LPR";
    case (Type::rpr):
        return "RPR";
    case (Type::lsbr):
        return "LSBR";
    case (Type::rsbr):
        return "RSBR";
    case (Type::semicolon):
        return "SEMICOLON";
    case (Type::colon):
        return "COLON";
    case (Type::dot):
        return "DOT";
    case (Type::comma):
        return "COMMA";
    case (Type::plus):
        return "PLUS";
    case (Type::minus):
        return "MINUS";
    case (Type::star):
        return "STAR";
    case (Type::div):
        return "DIV";
    case (Type::greater):
        return "GREATER";
    case (Type::less):
        return "LESS";
    case (Type::equal):
        return "EQUAL";
    case (Type::noteq):
        return "NOTEQ";
    case (Type::grequal):
        return "GREQUAL";
    case (Type::lequal):
        return "LEQUAL";
    case (Type::assign):
        return "ASSIGN";
    case (Type::plusass):
        return "PLUSASS";
    case (Type::minass):
        return "MINASS";
    case (Type::mulass):
        return "MULASS";
    case (Type::divass):
        return "DIVASS";
    case (Type::deref):
        return "DEREF";
    case (Type::ref):
        return "REF";
    case (Type::ternary):
        return "TERNARY";
    case (Type::andkw):
        return "ANDKW";
    case (Type::arraykw):
        return "ARRAYKW";
    case (Type::askw):
        return "ASKW";
    case (Type::autokw):
        return "AUTOKW";
    case (Type::beginkw):
        return "BEGINKW";
    case (Type::casekw):
        return "CASEKW";
    case (Type::classkw):
        return "CLASSKW";
    case (Type::constkw):
        return "CONSTKW";
    case (Type::constructorkw):
        return "CONSTRUCTORKW";
    case (Type::destructorkw):
        return "DESTRUCTORKW";
    case (Type::divkw):
        return "DIVKW";
    case (Type::dokw):
        return "DOKW";
    case (Type::downtokw):
        return "DOWNTOKW";
    case (Type::elsekw):
        return "ELSEKW";
    case (Type::endkw):
        return "ENDKW";
    case (Type::eventkw):
        return "EVENTKW";
    case (Type::exceptkw):
        return "EXCEPTKW";
    case (Type::extensionmethodkw):
        return "EXTENSIONMETHODKW";
    case (Type::filekw):
        return "FILEKW";
    case (Type::finalizationkw):
        return "FINALIZATIONKW";
    case (Type::finallykw):
        return "FINALLYKW";
    case (Type::forkw):
        return "FORKW";
    case (Type::foreachkw):
        return "FOREACHKW";
    case (Type::functionkw):
        return "FUNCTIONKW";
    case (Type::gotokw):
        return "GOTOKW";
    case (Type::ifkw):
        return "IFKW";
    case (Type::implementationkw):
        return "IMPLEMENTATIONKW";
    case (Type::inkw):
        return "INKW";
    case (Type::inheritedkw):
        return "INHERITEDKW";
    case (Type::initializationkw):
        return "INITIALIZATIONKW";
    case (Type::interfacekw):
        return "INTERFACEKW";
    case (Type::iskw):
        return "ISKW";
    case (Type::labelkw):
        return "LABELKW";
    case (Type::lockkw):
        return "LOCKKW";
    case (Type::loopkw):
        return "LOOPKW";
    case (Type::modkw):
        return "MODKW";
    case (Type::nilkw):
        return "NILKW";
    case (Type::notkw):
        return "NOTKW";
    case (Type::ofkw):
        return "OFKW";
    case (Type::operatorkw):
        return "OPERATORKW";
    case (Type::orkw):
        return "ORKW";
    case (Type::procedurekw):
        return "PROCEDUREKW";
    case (Type::programkw):
        return "PROGRAMKW";
    case (Type::propertykw):
        return "PROPERTYKW";
    case (Type::raisekw):
        return "RAISEKW";
    case (Type::recordkw):
        return "RECORDKW";
    case (Type::repeatkw):
        return "REPEATKW";
    case (Type::sealedkw):
        return "SEALEDKW";
    case (Type::setkw):
        return "SETKW";
    case (Type::sequencekw):
        return "SEQUENCEKW";
    case (Type::shlkw):
        return "SHLKW";
    case (Type::shrkw):
        return "SHRKW";
    case (Type::sizeofkw):
        return "SIZEOFKW";
    case (Type::templatekw):
        return "TEMPLATEKW";
    case (Type::thenkw):
        return "THENKW";
    case (Type::tokw):
        return "TOKW";
    case (Type::trykw):
        return "TRYKW";
    case (Type::typekw):
        return "TYPEKW";
    case (Type::typeofkw):
        return "TYPEOFKW";
    case (Type::untilkw):
        return "UNTILKW";
    case (Type::useskw):
        return "USESKW";
    case (Type::usingkw):
        return "USINGKW";
    case (Type::varkw):
        return "VARKW";
    case (Type::wherekw):
        return "WHEREKW";
    case (Type::whilekw):
        return "WHILEKW";
    case (Type::withkw):
        return "WITHKW";
    case (Type::xorkw):
        return "XORKW";
    case (Type::abstractkw):
        return "ABSTRACTKW";
    case (Type::defaultkw):
        return "DEFAULTKW";
    case (Type::externalkw):
        return "EXTERNALKW";
    case (Type::forwardkw):
        return "FORWARDKW";
    case (Type::internalkw):
        return "INTERNALKW";
    case (Type::onkw):
        return "ONKW";
    case (Type::overloadkw):
        return "OVERLOADKW";
    case (Type::overridekw):
        return "OVERRIDEKW";
    case (Type::paramskw):
        return "PARAMSKW";
    case (Type::privatekw):
        return "PRIVATEKW";
    case (Type::protectedkw):
        return "PROTECTEDKW";
    case (Type::publickw):
        return "PUBLICKW";
    case (Type::readkw):
        return "READKW";
    case (Type::reintroducekw):
        return "REINTRODUCEKW";
    case (Type::unitkw):
        return "UNITKW";
    case (Type::virtualkw):
        return "VIRTUALKW";
    case (Type::writekw):
        return "WRITEKW";
    case (Type::unexpected):
        return "UNEXPECTED";
    case (Type::eof):
        return "EOF";
    }
    return "";
}