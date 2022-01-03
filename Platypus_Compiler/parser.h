/*	File name:		parser.h
	Compiler:		MS Visual Studio 2019
	Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
	Course:			CST 8152 – Compilers, Lab Section: [23 & 21]
	Assignment:		3
	Date:			DEC 13, 2020
	Professor:		Paulo Sousa / Kaci Taouch
	Purpose:		define the keywords constatnts and the functions needed for parser implementation
*/

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "token.h"
#include "buffer.h"

/* Global vars */
// TODO_01: lookahead
static Token lookahead;
// TODO_02: syntaxErrorNumber
int syntaxErrorNumber;
extern Buffer* stringLiteralTable;
extern int line;
extern Token processToken();
extern char* keywordTable[];

/* Constants */
#define	NO_ATTR (-1)
// TODO_03: Create ALL constants for all keywords
#define PROGRAM 0
#define IF 1
#define THEN 2
#define ELSE 3
#define WHILE 4
#define DO 5
#define INPUT 6
#define OUTPUT 7
#define TRUE 8
#define FALSE 9


/* Function definitions */
void processParser(void);
void matchToken(int, int);
void syncErrorHandler(int);
void printError();

// TODO_04 All FunctionsForNonTerminals
void program(void);
void additiveArithmeticExpressionPrime(void);
void additiveArithmeticExpression(void);
void arithmeticExpression(void);
void assignmentExpression(void);
void assignmentStatement(void);
void conditionalExpression(void);
void inputStatement(void);
void iterationStatement(void);
void logicalAndExpression(void);
void logicalOrExpression(void);
void logicalNotExpression(void);
void logicalAndExpressionPrime(void);
void logicalOrExpressionPrime(void);
void multiplicativeRrithmeticExpression(void);
void multiplicativeArithmeticExpressionPrime(void);
void outputList(void);
void optionalStatements(void);
void outputStatement(void);
void primarySrelationalExpressionPrime(void);
void primaryArelationalExpressionPrime(void);
void primaryArithmeticExpression(void);
void primarySrelationalExpression(void);
void primaryArelationalExpression(void);
void primaryStringExpression(void);
void preCondition(void);
void relationalExpression(void);
void statement(void);
void statements(void);
void stringExpressionPrime(void);
void statementPrime(void);
void selectionStatement(void);
void stringExpression(void);
void variableListPrime(void);
void variableList(void);
void variableIdentifier(void);
void unaryArithmeticExpression(void);
