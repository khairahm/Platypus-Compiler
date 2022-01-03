/*	File name:		scanner.c
	Compiler:		MS Visual Studio 2019
	Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
	Course:			CST 8152 – Compilers, Lab Section: [23 & 21]
	Assignment:		2
	Date:			Nov 3, 2020
	Professor:		Paulo Sousa / Haider Miraj.
	Purpose:		Functions implementing a Lexical Analyzer(Scanner) as required for CST8152, Assignment #2
					scanner_init() must be called before using the scanner.

	Function list:	initScanner(), processToken(), getNextState(), nextTokenClass(), aStateFuncAVID(), aStateFuncSVID(), aStateFuncIL(), aStateFuncFPL(), aStateFuncSL(),
					aStateFuncErr(), isKeyword(), bufferIsEmpty(), bufferRewind(), bufferClear(), bufferGetChar(), bufferSetMarkPosition(), bufferGetCPosition(),
					isalnum(), bufferRetract(), bufferGetCPosition(), bufferCreate(), bufferAddChar(), bufferReset(), bufferSetEnd(), bufferGetString(), bufferFree(),
					fprintf(), isalpha(), isdigit(), strlen(), strcpy(), atol(), strtof(), strcmp()
*/


/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define does not have any effect in Borland compiler projects.
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

 /*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern pBuffer stringLiteralTable;		/* String literal table */
int line;								/* current line number of the source code */
extern int errorNumber;					/* defined in platy_st.c - run-time error number */

static char debugMode = 0;				/* optional for debugging */

/* Local(file) global objects - variables */
static pBuffer lexemeBuffer;			/* pointer to temporary lexeme buffer */
static pBuffer sourceBuffer;			/* pointer to input source buffer */
/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int nextTokenClass(char c);		/* character class function */
static int getNextState(int, char);		/* state machine function */
static int isKeyword(char* kw_lexeme);	/* keywords lookup functuion */

/* TODO_02: Follow the standard and adjust all function headers */

/*************************************************************
 * Intitializes scanner
 ************************************************************/
int initScanner(pBuffer psc_buf) {
	if (bufferIsEmpty(psc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	bufferRewind(psc_buf);
	bufferClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS;/*0*/
/*   scerrnum = 0;  *//*no need - global ANSI C */
}


/*
* Function name:		processToken
* Purpose:				This function reads tokens from the buffer and decides the token type before returning the token.
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:		1.0 2020-11-03
* Called functions:		bufferGetChar(), isalnum(), bufferSetMarkPosition(), bufferGetCPosition(), bufferRetract(), bufferGetCPosition(),
						bufferCreate(), bufferAddChar(), bufferReset(), bufferSetEnd(), bufferGetString(), bufferFree()
* Parameters:			N/A
* Return value:			Token
* Algorithm:			if character is alphanumeric or quataion, transition table driven scanner part is followed,
						we mark the current position in buffer and assign that position to lexStart then get the next token's state.
						loop until the next state is accepting state. if it's accepting with retract, retract buffer.
						set lexEnd to the current buffer position & allocate memory for a new buffer.
						reset the buffer to add character to the markCPosition(lexend) position.
						set the endf of lexemeBuffer with the terminator character then token is assigned from finalStateTable
						then free the lexemeBuffer and return token.
						if character in buffer is not alphanumeric or quataion then we read from the buffer and set the proper
						token's type based on the character read.
*/
Token processToken(void) {
	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	unsigned char c;	/* input symbol */
	int state = 0;		/* initial state of the FSM */
	short lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	short lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	/*int lexLength;		/* token length */
	/*int i;				/* counter */
	/*int isError;		/* error identification */
	unsigned char newc;	/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bufferGetChar(sourceBuffer);

		switch (c) {
		case '\n':/*end of line*/
			line++;
			continue;

		case ' ':/*space*/
		case '\t':
			continue;

		case CHARSEOF0:/*end of file*/
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			return currentToken;

		case '$':/*possible string concatination*/
			c = bufferGetChar(sourceBuffer);

			/*if a second '$' is found then it's a string concatination*/
			if (c == '$')
			{
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			else
			{/*error*/
				bufferRetract(sourceBuffer);
				currentToken = aStateFuncErr("$");
				return currentToken;
			}

		case '(':/*left parentheses*/
			currentToken.code = LPR_T;
			return currentToken;

		case ')':/*right parentheses*/
			currentToken.code = RPR_T;
			return currentToken;

		case '*':/*multiplication*/
			currentToken.code = ART_OP_T; /*multiplication operator*/
			currentToken.attribute.arr_op = MUL;
			return currentToken;

		case '/':/*division*/
			currentToken.code = ART_OP_T; /*division operator*/
			currentToken.attribute.arr_op = DIV;
			return currentToken;

		case '_':/*possible logical operators*/
			bufferSetMarkPosition(sourceBuffer, bufferGetCPosition(sourceBuffer));
			c = bufferGetChar(sourceBuffer);
			if (c == 'A')
			{/*check if it's '_AND_'*/
				if (bufferGetChar(sourceBuffer) == 'N' && bufferGetChar(sourceBuffer) == 'D' && bufferGetChar(sourceBuffer) == '_')
				{
					currentToken.code = LOG_OP_T;
					currentToken.attribute.log_op = AND;
					return currentToken;
				}
			}
			else if (c == 'N')
			{/*check if it's '_NOT_'*/
				if (bufferGetChar(sourceBuffer) == 'O' && bufferGetChar(sourceBuffer) == 'T' && bufferGetChar(sourceBuffer) == '_')
				{
					currentToken.code = LOG_OP_T;
					currentToken.attribute.log_op = NOT;
					return currentToken;
				}
			}
			else if (c == 'O')
			{/*check if it's '_OR_'*/
				if (bufferGetChar(sourceBuffer) == 'R' && bufferGetChar(sourceBuffer) == '_')
				{
					currentToken.code = LOG_OP_T;
					currentToken.attribute.log_op = OR;
					return currentToken;
				}
			}

			/*if none of the cases above is true then it's an error*/
			bufferReset(sourceBuffer);
			currentToken = aStateFuncErr("_");
			return currentToken;

		case ';': currentToken.code = EOS_T;/*end of statement*/
			return currentToken;

		case '{':
			currentToken.code = LBR_T;/*left brace*/
			return currentToken;

		case '}':
			currentToken.code = RBR_T;/*right brace*/
			return currentToken;

		case '+':
			currentToken.code = ART_OP_T;/*addition operator*/
			currentToken.attribute.arr_op = ADD;
			return currentToken;

		case '-':
			currentToken.code = ART_OP_T;/*subtraction operator*/
			currentToken.attribute.arr_op = SUB;
			return currentToken;

		case '!':/*possible comment*/
			newc = bufferGetChar(sourceBuffer);
			c = newc;

			/* check if there's one '!' followed by end of file of new line symbols (error case)*/
			if (c == CHARSEOF0 || c == CHARSEOF255 || c == '\n') {
				bufferRetract(sourceBuffer);
				currentToken.code = ERR_T;
				currentToken.attribute.err_lex[0] = '!';
				currentToken.attribute.err_lex[1] = c;
				currentToken.attribute.err_lex[2] = CHARSEOF0;
				return currentToken;
			}

			/*go through until the end of line to check for possible enf of file symbols*/
			while (c != '\n')
			{
				c = bufferGetChar(sourceBuffer);

				/*error case of end of file symbol before the end of the comment line*/
				if (c == CHARSEOF0 || c == CHARSEOF255)
				{
					bufferRetract(sourceBuffer);
					currentToken.code = ERR_T;
					currentToken.attribute.err_lex[0] = '!';
					currentToken.attribute.err_lex[1] = '!';
					currentToken.attribute.err_lex[2] = CHARSEOF0;
					return currentToken;
				}
			}
			/*increment line counter*/
			line++;

			/*if a second '!' is found then it's a comment*/
			if (newc == '!')
			{
				continue;
			}
			else
			{/*error*/
				bufferRetract(sourceBuffer);
				currentToken.code = ERR_T;
				currentToken.attribute.err_lex[0] = '!';
				currentToken.attribute.err_lex[1] = newc;
				currentToken.attribute.err_lex[2] = CHARSEOF0;
				return currentToken;
			}

		case '>':
			currentToken.code = REL_OP_T;/*greater than operator*/
			currentToken.attribute.rel_op = GT;
			return currentToken;

		case ',':
			currentToken.code = COM_T;/*Comma*/
			return currentToken;

		case '<':/*possible smaller than or not equal operator*/
			c = bufferGetChar(sourceBuffer);

			if (c != '>')
			{/*if next char is NOT >, then this is a smaller than operator*/
				bufferRetract(sourceBuffer);
				currentToken.code = REL_OP_T;
				currentToken.attribute.rel_op = LT;
			}
			else
			{/*next character is >, then we have a <>. that is a not equal operator*/
				currentToken.code = REL_OP_T;
				currentToken.attribute.rel_op = NE;
			}

			return currentToken;

		case '=':/*possible assignment or equality check operator*/
			c = bufferGetChar(sourceBuffer);

			/*assignment operator*/
			if (c != '=')
			{
				bufferRetract(sourceBuffer);
				currentToken.code = ASS_OP_T;
			}
			else
			{/*equality check operator (==)*/
				currentToken.code = REL_OP_T;
				currentToken.attribute.rel_op = EQ;
			}

			return currentToken;

		default:

			/*current character alphanumeric or quotatian*/
			if (isalnum(c) != 0 || c == '"')
			{
				/*mark the current position in buffer and assign that position to lexStart*/
				bufferSetMarkPosition(sourceBuffer, bufferGetCPosition(sourceBuffer) - 1);
				lexStart = sourceBuffer->markCPosition;

				/*get the token state*/
				state = getNextState(state, c);

				/*if the state is not accepting, keep looping thrugh the buffer until the state change*/
				while (stateType[state] == NOAS)
				{
					c = bufferGetChar(sourceBuffer);
					state = getNextState(state, c);
				}

				/*if state is accepting with return, retract the buffer*/
				if (stateType[state] == ASWR)
				{
					bufferRetract(sourceBuffer);
				}

				/*set lexEnd to the current buffer position & allocate memory for a new buffer*/
				lexEnd = bufferGetCPosition(sourceBuffer);
				lexemeBuffer = bufferCreate(lexEnd - lexStart, 0, 'f');/*buffer's capacity is lexend-lesstart*/

				/*run time error check for buffercreate failure*/
				if (!lexemeBuffer)
				{
					currentToken.code = ERR_T;
					errorNumber = 1;
					strcpy(currentToken.attribute.err_lex, "RUN TIME ERROR: ");
					return currentToken;
				}
				/*reset buffer's char position to the markCPosition*/
				bufferReset(sourceBuffer);

				/*copy chars from buffer and add them to lexemeBuffer until lexEnd position is reached*/
				while (bufferGetCPosition(sourceBuffer) < lexEnd)
				{
					c = bufferGetChar(sourceBuffer);
					bufferAddChar(lexemeBuffer, c);
				}

				bufferSetEnd(lexemeBuffer, '\0');

				currentToken = finalStateTable[state](bufferGetString(lexemeBuffer, 0));
				bufferFree(lexemeBuffer);
				return currentToken;
			}
			else {/*error*/
				currentToken.code = ERR_T;
				currentToken.attribute.err_lex[0] = c;
				currentToken.attribute.err_lex[1] = CHARSEOF0;
				return currentToken;
			}


		} // switch

	} //while

} //processToken




/* DO NOT MODIFY THE CODE OF THIS FUNCTION
YOU CAN REMOVE THE COMMENTS ONLY */
/*************************************************************
 * Get Next State
 ************************************************************/

int getNextState(int state, char c) {
	int col;
	int next;
	if (debugMode)
		fprintf(stderr, "[03] getNextState\n");
	col = nextTokenClass(c);
	next = transitionTable[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif
	/*
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:

	Assertion failed: test, file filename, line linenum

	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	*/
	assert(next != IS);

	/*
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUF is used - see the top of the file.
	*/
#ifdef DEBUG
	if (next == IS) {
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	return next;
}

/*
* Function name:		nextTokenClass
* Purpose:				This function returns the column number in the transition table for the input character.
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:		1.0 2020-11-03
* Called functions:		fprintf(), isalpha(), isdigit(), strcpy()
* Parameters:			char c
* Return value:			Token
* Algorithm:			check the type of input(c) and return the column number of the corresponding input type from the Transition table
*/
int nextTokenClass(char c) {
	int val = -1;
	if (debugMode)
		fprintf(stderr, "[04] NextTokenClass\n");


	/*				[A-z](0),	0(1),	[1-9](2),	.(3),	$(4),	"(5),	SEOF(6),	other(7) */
	switch (c) {
	case '0':		/*Zero*/
		val = 1;
		break;

	case '.':		/*Point*/
		val = 3;
		break;

	case '$':		/*SVID*/
		val = 4;
		break;

	case '"':		/*SL*/
		val = 5;
		break;

	case CHARSEOF0: /*end of file*/
	case CHARSEOF255:
		val = 6;
		break;

	default:
		if (isalpha(c)) {/*(a-z)*/
			val = 0;
		}
		else if (isdigit(c)) {/*(1-9)*/
			val = 2;
		}

		else {/*other*/
			val = 7;
		}
	} //switch
	return val;
}


/*
* Function name:		aStateFuncAVID
* Purpose:				set token type to AVID, copy the lexeme's characters and store it in the token attribute
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:		1.0 2020-11-03
* Called functions:		isKeyword(), strlen(), strcpy()
* Parameters:			char lexeme[]
* Return value:			Token
* Algorithm:			Set token type to AVID, check if token length is within range(0-8), if so, copy it to the token's attribute
						if it's out of range, copy the first 8 characters only. the return token
*/
Token aStateFuncAVID(char lexeme[])
{
	Token currentToken = { 0 };
	int i = 0;	/*counter*/

	/*check first if lexeme is a keyword, and set the token type and attributes if so*/
	if (isKeyword(lexeme) != -1)
	{
		currentToken.code = KW_T;
		currentToken.attribute.kwt_idx = isKeyword(lexeme);
		return currentToken;
	}
	else
	{/*lexeme is not a keyword*/
		if (strlen(lexeme) > VID_LEN)
		{/*if lexeme longer than 8 characters, copy the first 8 characters to the the token attribute*/
			while (i < VID_LEN)
			{
				currentToken.attribute.vid_lex[i] = lexeme[i];
				i++;
			}

			/*add '\0' to the end to make it a c type string*/
			currentToken.attribute.vid_lex[VID_LEN] = CHARSEOF0;
		}
		else
		{/*token is within range of variable identifier length*/

			strcpy(currentToken.attribute.vid_lex, lexeme);
		}

		currentToken.code = AVID_T; /*assigning token code to Arithmetic Variable identifier token*/
	}

	return currentToken;

}

/*
* Function name:		aStateFuncSVID
* Purpose:				set token type to SVID, copy the lexeme's characters and store it in the token attribute
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:		1.0 2020-11-03
* Called functions:		strlen(), strcpy()
* Parameters:			char lexeme[]
* Return value:			Token
* Algorithm:			Set token type to SVID, check if token length is within range(0-8), if so, copy it to the token's attribute
						if it's out of range, copy the first 7 characters only then add '$' to the end. the return token
*/
Token aStateFuncSVID(char lexeme[]) {
	Token currentToken = { 0 };
	int i = 0;	/*counter*/
	currentToken.code = SVID_T; /*set the token type to SVID*/

	/*if lexeme longer than 8 characters*/
	if (strlen(lexeme) > VID_LEN)
	{
		for (i = 0; i < VID_LEN - 1; i++)
		{
			currentToken.attribute.vid_lex[i] = lexeme[i]; /*add the first 7 characters only*/
		}
		currentToken.attribute.vid_lex[VID_LEN - 1] = '$';/*add the $ symbol to the end of the string*/
	}

	else
	{/*if lexeme length less than or equal 8 characters*/
		strcpy(currentToken.attribute.vid_lex, lexeme);
	}

	/*add '\0' to the end to make it a c type string*/
	currentToken.attribute.vid_lex[VID_LEN] = CHARSEOF0;
	return currentToken;
}


/*
* Function name:		aStateFuncIL
* Purpose:				Convert the lexeme to a long int value, store it in the token attribute, set token type and return the token
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:		1.0 2020-11-03
* Called functions:		atol()
* Parameters:			char lexeme[]
* Return value:			Token
* Algorithm:			Convert the lexeme to a long value then check if that value is out of the acceptable range,
						if so, set the token type to error,
						else, set the token type to int and set the token attribute then, return token
*/
Token aStateFuncIL(char lexeme[])
{
	Token currentToken = { 0 };

	/*check for out of range error cases for the Integer value of the lexeme, if out of boundaries, set the token to error state*/
	if (atol(lexeme) > SHRT_MAX || atol(lexeme) < SHRT_MIN)
	{
		currentToken = finalStateTable[11](lexeme);
	}
	else
	{/*lexeme is a valid Integer, set the token type to int, and the token's attribute to the lexeme's value*/
		currentToken.attribute.int_value = atol(lexeme);
		currentToken.code = INL_T;
	}

	return currentToken;
}


/*
* Function name:		aStateFuncFPL
* Purpose:				Convert the lexeme to a floating point value, store it in the token attribute, set token type and return the token
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:		1.0 2020-11-03
* Called functions:		strtof()
* Parameters:			char lexeme[]
* Return value:			Token
* Algorithm:			Convert the lexeme to a floating point representation value then check if that value is out of the acceptable range,
						if so, set the token type to error,
						else, set the token type to FPL and set the token attribute then, return token
*/
Token aStateFuncFPL(char lexeme[])
{

	Token currentToken = { 0 };

	/*check for out of range error cases for the floating point value of the lexeme, and set the token to error state if any error is found */
	if (strtof(lexeme, NULL) < 0 || ((strtof(lexeme, NULL) < FLT_MIN || strtof(lexeme, NULL) > FLT_MAX) && (strtof(lexeme, NULL) >= 0 && strlen(lexeme) > 7)))
	{
		/*token is not a valid floating point literal token*/
		currentToken = finalStateTable[11](lexeme);
	}
	else
	{/*lexeme is a valid FPL, set the token type to FPL, and the token's attribute to the lexeme's value*/
		currentToken.code = FPL_T;
		currentToken.attribute.flt_value = strtof(lexeme, NULL);
	}

	return currentToken;
}



/*
* Function name:		aStateFuncSL
* Purpose:				Set the token type to string literal and return that token
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:		1.0 2020-11-03
* Called functions:		bufferGetAddCPosition(), strlen(), bufferAddChar(), bufferAddChar()
* Parameters:			char lexeme[]
* Return value:			Token
* Algorithm:			set the token type to string literal, set the token's attribute offset value,
						then copy the lexeme's characters to the string literal table (minus the ""), and return the token
*/
Token aStateFuncSL(char lexeme[])
{
	Token currentToken = { 0 };

	unsigned int i = 0;		/*counter*/
	currentToken.code = STR_T;/*set the token type to string literal*/

	/*the attribute of the token's postion's is the position of the beginning of the string in the string literal table*/
	currentToken.attribute.str_offset = bufferGetAddCPosition(stringLiteralTable);

	/*ignore index 0 and last index so we don't add the opening/closing (") */
	for (i = 1; i < strlen(lexeme) - 1; i++)
	{
		/*increment the line counter if a new line symbol is found within the quotation marks*/
		if (lexeme[i] == '\n')
		{
			line++;
		}
		/*add the lexeme characters to stringLiteralTable buffer*/
		bufferAddChar(stringLiteralTable, lexeme[i]);
	}

	/*add '\0' to the end to make it a c type string*/
	bufferAddChar(stringLiteralTable, CHARSEOF0);
	return currentToken;
}

/*
* Function name:		aStateFuncErr
* Purpose:				Set the token type to error and copy the eroor lexeme into the error token attribute and return that token
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:		1.0 2020-11-03
* Called functions:		strlen(), strcpy()
* Parameters:			char lexeme[]
* Return value:			Token
* Algorithm:			set the token type to error, check if the error lexeme is within rane (20 chars), if so, copy it to the error token and return that
						if not, then copy the first 17 characters of the lexeme then add 3 dots (...) at the end then return the token.
*/
Token aStateFuncErr(char lexeme[]) {
	Token currentToken = { 0 };
	unsigned int i = 0;					/*counter*/
	currentToken.code = ERR_T;	/*set the token type to error*/

	if (strlen(lexeme) > ERR_LEN) /*if error lexeme is bigger than 20 characters*/
	{
		for (i = 0; i < ERR_LEN; i++)
		{
			if (i < 17) /*add only the first 17 characters*/
			{
				currentToken.attribute.err_lex[i] = lexeme[i];
			}

			else /*add 3 dots at the end*/
			{
				currentToken.attribute.err_lex[i] = '.';
			}
		}
		/*add '\0' to the end to make it a c type string*/
		currentToken.attribute.err_lex[ERR_LEN] = CHARSEOF0;
	}

	/*error lexeme is less than 20 characters*/
	else
	{
		strcpy(currentToken.attribute.err_lex, lexeme);
	}

	/*increment line counter if line terminators found*/
	for (i = 0; i < strlen(lexeme); i++)
	{
		if (lexeme[i] == '\n')
		{
			line++;
		}
	}
	return currentToken;
}


/*
* Function name:		isKeyword
* Purpose:				Check if lexeme is a kweyword and return it's index in the keywordTable or return -1 if it's not a keyword.
* Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-11-03
* Called functions:	strcmp()
* Parameters:			char*  kw_lexeme
* Return value:		int: index number in keywordTable[] or -1
* Algorithm:			Check if the paramerter passed is null,
					   if not, then loop through the arry of keywords to search for the keyword lexeme,
					   if keyword is found, return it's index number,
					   if not, return error (-1)
*/
int isKeyword(char* kw_lexeme)
{
	int i = 0; /*counter*/

	/* check for errors first */
	if (!kw_lexeme)
	{
		return ERR_T;
	}

	/* search keywordTable array for the keyword and return its index number if found */
	for (i = 0; i < KWT_SIZE; i++)
	{
		if (strcmp(keywordTable[i], kw_lexeme) == 0) {/*compare the parameter input with all the values in the keyword table and return the index of the matching keyword if any*/
			return i;
		}
	}

	/*lexeme is not a keyword, return error*/
	return ERR_T;
}
