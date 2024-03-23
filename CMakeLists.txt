cmake_minimum_required(VERSION 3.8)
project(PascalToCXX
    VERSION 0.1.0
    DESCRIPTION "Pascal to C++ translator"
    LANGUAGES C CXX)

include_directories(include)

set(exename pxx)

file(COPY code\ examples DESTINATION .)

set(tokenlib src/pxx/Token.cpp)
set(astlib src/pxx/ASTNode.cpp src/pxx/AST.cpp)
set(lexlib src/pxx/Lexer.cpp src/pxx/LexerStates.cpp src/pxx/FileData.cpp)
set(parslib src/pxx/Parser.cpp)
set(semanalyzerlib src/pxx/SemanticAnalyzer.cpp src/pxx/SemanticVisitor.cpp src/pxx/Symbol.cpp)
set(codegenlib src/pxx/CodeGenerator.cpp src/pxx/CodeEmittingNodeVisitor.cpp)
set(optimizerlib src/pxx/Optimizer.cpp src/pxx/CalculateVisitor.cpp src/pxx/ShortenExpressionVisitor.cpp)

add_library(token STATIC ${tokenlib})
add_library(ast STATIC ${astlib})
add_library(lexer STATIC ${lexlib})
add_library(parser STATIC ${parslib})
add_library(semanalyzer STATIC ${semanalyzerlib})
add_library(codegen STATIC ${codegenlib})
add_library(optimizer STATIC ${optimizerlib})

target_link_libraries(lexer token)
target_link_libraries(parser lexer ast token)
target_link_libraries(optimizer ast token)

set(libs lexer parser codegen semanalyzer optimizer)

add_executable(${exename} main.cpp)
set_property(TARGET ${exename} PROPERTY CXX_STANDARD 17)
set_property(TARGET codegen PROPERTY CXX_STANDARD 17)

if(MSVC)
  target_compile_options(${exename} PRIVATE /W4 /WX)
else()
  target_compile_options(${exename} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(${exename} ${libs})
