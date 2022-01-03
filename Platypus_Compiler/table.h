/*	File name:		table.h
	Compiler:		MS Visual Studio 2019
	Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
	Course:			CST 8152 – Compilers, Lab Section: [23 & 21]
	Assignment:		2
	Date:			Nov 3, 2020
	Professor:		Paulo Sousa / Haider Miraj.
	Purpose:		define the transition table values and the differnt tables to facilitae the implemntation of the scanner

	Function list:		Token aStateFuncAVID(char lexeme[]);
						Token aStateFuncSVID(char lexeme[]);
						Token aStateFuncIL(char lexeme[]);
						Token aStateFuncFPL(char lexeme[]);
						Token aStateFuncSL(char lexeme[]);
						Token aStateFuncErr(char lexeme[]);
*/


/* Macro section */
#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef TOKEN_H_
#include "token.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

 /*  Source end-of-file (SEOF) sentinel symbol
  *    '\0' or one of 255,0xFF,EOF
  */
#define CHARSEOF0 '\0'
#define CHARSEOF255 255

  /*  Special case tokens processed separately one by one
   *  in the token-driven part of the scanner
   *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
   *  white space
   *  !!comment , ',' , ';' , '-' , '+' , '*' , '/', ## ,
   *  _AND_, _OR_, _NOT_ , SEOF,
   */
#define ES  11		/* Error state  with no retract */
#define ER  12		/* Error state  with retract */
#define IS  -1		/* Illegal state */

#define MAX_INT_P 32767

   /* State transition table definition */
#define TABLE_COLUMNS 8

/* Transition table - type of states defined in separate table */
int  transitionTable[][TABLE_COLUMNS] = {
	/*				[A-z](0),	0(1),	[1-9](2),	.(3),	$(4),	"(5),	SEOF(6),	other(7)	*/
	/*				L,			Z,		N,			P,		S,		Q,		E,			O			*/

	/*state 0: Start state*/
	/* State 00 */  {1,			6,		4,			ES,		ES,		9,		ER,			ES},

	/*state 0: Letter state*/
	/* State 01 */  {1,			1,		1,			2,		3,		2,		2,			2},

	/*state 2: accepting AVID*/
	/* State 02 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS},

	/*state 3: accepting SVID*/
	/* State 03 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS},

	/*state 4: Non zero state (N)*/
	/* State 04 */  {ES,		4,		4,			7,		5,		ES,		ER,			5},

	/*state 5: accepting Integer literal (IL)*/
	/* State 05 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS},

	/*state 6: Zero state (0)*/
	/* State 06 */  {ES,		6,		ES,			7,		5,		ES,		ER,			5},

	/*state 7: Point state (P)*/
	/* State 07 */  {8,			7,		7,			8,		8,		8,		ER,			8},

	/*state 8: accepting Floating point literal (FPL)*/
	/* State 08 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS},

	/*state 9: quotation state (Q)*/
	/* State 09 */  {9,			9,		9,			9,		9,		10,		ER,			9},

	/*stste 10: accepting String literal(SL)*/
	/* State 10 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS},

	/*stste 11: ES*/
	/* State 11 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS},

	/*stste 12: ER*/
	/* State 12 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS}
};

/* Accepting state table definition */
#define ASWR	1	/* accepting state with retract */
#define ASNR	2	/* accepting state with no retract */
#define NOAS	3	/* not accepting state */

/* List of acceptable states */
int stateType[] = {
	NOAS,
	NOAS,
	ASWR,
	ASNR,
	NOAS,
	ASWR,
	NOAS,
	NOAS,
	ASWR,
	NOAS,
	ASNR,
	ASNR,
	ASWR
};

/* Accepting action function declarations */
Token aStateFuncAVID(char lexeme[]);
Token aStateFuncSVID(char lexeme[]);
Token aStateFuncIL(char lexeme[]);
Token aStateFuncFPL(char lexeme[]);
Token aStateFuncSL(char lexeme[]);
Token aStateFuncErr(char lexeme[]);
/* Defining a new type: pointer to function (of one char * argument)
   returning Token
*/
typedef Token(*PTR_AAF)(char* lexeme);

/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*finalStateTable[])(char lexeme[]) = {
 */
PTR_AAF finalStateTable[] = {
	NULL,
	NULL,
	/* state 2 (AVID)*/
	 aStateFuncAVID,
	 /* state 3 (SVID)*/
	aStateFuncSVID,
	NULL,
	/* state 5 (IL)*/
	aStateFuncIL,
	NULL,
	NULL,
	/* state 8 (FPL)*/
	aStateFuncFPL,
	NULL,
	/* state 10 (SL)*/
	aStateFuncSL,
	/* state 11 & 12 (ES & ER)*/
	aStateFuncErr,
	aStateFuncErr
};

/* Keyword lookup table (_AND_, _OR_ and _NOT_ are not keywords) */
#define KWT_SIZE 10

/* Keyword list */
char* keywordTable[] = {
	"PROGRAM",
	"IF",
	"THEN",
	"ELSE",
	"WHILE",
	"DO",
	"INPUT",
	"OUTPUT",
	"TRUE",
	"FALSE"
};

#endif
