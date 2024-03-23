#pragma once
#include <string>

/// @brief Тип токена
enum class Type
{
    id,             // Идентификатор
    number,         // Любое число, на последующих этапах трансляции будет распознано как целое или вещественное
    string,         // Строка
    lpr,            // Символ (
    rpr,            // Символ )
    lsbr,           // Символ [
    rsbr,           // Символ ]
    semicolon,      // Символ ;
    colon,          // Символ :
    dot,            // Символ .
    comma,          // Символ ,
    plus,           // Оператор +
    minus,          // Оператор -
    star,           // Оператор *
    div,            // Оператор /
    greater,        // Оператор >
    less,           // Оператор <
    equal,          // Оператор =
    noteq,          // Оператор <>
    grequal,        // Оператор >=
    lequal,         // Оператор <=
    assign,         // Оператор :=
    plusass,        // Оператор +=
    minass,         // Оператор -=
    mulass,         // Оператор *=
    divass,         // Оператор /=
    deref,          // Оператор ^
    ref,            // Оператор @
    ternary,        // Оператор ?
    andkw,          // Ключевое слово and
    arraykw,        // Ключевое слово array
    askw,           // Ключевое слово as
    autokw,         // Ключевое слово auto
    beginkw,        // Ключевое слово begin
    casekw,         // Ключевое слово case
    classkw,        // Ключевое слово class
    constkw,        // Ключевое слово const
    constructorkw,  // Ключевое слово constructor
    destructorkw,   // Ключевое слово destructor
    divkw,          // Ключевое слово div
    dokw,           // Ключевое слово do
    downtokw,       // Ключевое слово downto
    elsekw,         // Ключевое слово else
    endkw,          // Ключевое слово end
    eventkw,        // Ключевое слово event
    exceptkw,       // Ключевое слово except
    extensionmethodkw,  // Ключевое слово extensionmethod
    filekw,         // Ключевое слово file
    finalizationkw, // Ключевое слово finalization
    finallykw,      // Ключевое слово finally
    forkw,          // Ключевое слово for
    foreachkw,      // Ключевое слово foreach
    functionkw,     // Ключевое слово function
    gotokw,         // Ключевое слово goto
    ifkw,           // Ключевое слово if
    implementationkw,   // Ключевое слово implementation
    inkw,           // Ключевое слово in
    inheritedkw,    // Ключевое слово inherited
    initializationkw,   // Ключевое слово initialization
    interfacekw,    // Ключевое слово interface
    iskw,           // Ключевое слово is
    labelkw,        // Ключевое слово label
    lockkw,         // Ключевое слово lock
    loopkw,         // Ключевое слово loop
    modkw,          // Ключевое слово mod
    nilkw,          // Ключевое слово nil
    notkw,          // Ключевое слово not
    ofkw,           // Ключевое слово of
    operatorkw,     // Ключевое слово operator
    orkw,           // Ключевое слово or
    procedurekw,    // Ключевое слово procedure
    programkw,      // Ключевое слово program
    propertykw,     // Ключевое слово property
    raisekw,        // Ключевое слово raise
    recordkw,       // Ключевое слово record
    repeatkw,       // Ключевое слово repeat
    sealedkw,       // Ключевое слово sealed
    setkw,          // Ключевое слово set
    sequencekw,     // Ключевое слово sequence
    shlkw,          // Ключевое слово shl
    shrkw,          // Ключевое слово shr
    sizeofkw,       // Ключевое слово sizeof
    templatekw,     // Ключевое слово template
    thenkw,         // Ключевое слово then
    tokw,           // Ключевое слово to
    trykw,          // Ключевое слово try
    typekw,         // Ключевое слово type
    typeofkw,       // Ключевое слово typeof
    untilkw,        // Ключевое слово until
    useskw,         // Ключевое слово uses
    usingkw,        // Ключевое слово using
    varkw,          // Ключевое слово var
    wherekw,        // Ключевое слово where
    whilekw,        // Ключевое слово while
    withkw,         // Ключевое слово with
    xorkw,          // Ключевое слово xor
    abstractkw,     // Ключевое слово abstract
    defaultkw,      // Ключевое слово default
    externalkw,     // Ключевое слово external
    forwardkw,      // Ключевое слово forward
    internalkw,     // Ключевое слово internal
    onkw,           // Ключевое слово on
    overloadkw,     // Ключевое слово overload
    overridekw,     // Ключевое слово override
    paramskw,       // Ключевое слово params
    privatekw,      // Ключевое слово private
    protectedkw,    // Ключевое слово protected
    publickw,       // Ключевое слово public
    readkw,         // Ключевое слово read
    reintroducekw,  // Ключевое слово reintroduce
    unitkw,         // Ключевое слово unit
    virtualkw,      // Ключевое слово virtual
    writekw,        // Ключевое слово write
    unexpected,     // Неизвестный токен
    eof             // Конец файла
};

std::string type_to_str(Type type);

class Token
{
private:
    std::string value;
    Type type;
    unsigned int row;
    unsigned int pos;

public:
    const std::string& getValue() const;
    Type getType() const;
    unsigned int getPos() const;
    unsigned int getRow() const;
    void setValue(std::string _value);
    void setType(Type _type);
    Token(std::string _value, Type _type, unsigned int _row = 0, unsigned int _pos = 0);
    bool operator==(const Token &_other) const;
    bool operator!=(const Token &_other) const;
};
