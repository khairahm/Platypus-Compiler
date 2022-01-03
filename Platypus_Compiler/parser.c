/*	File name:		parser.c
	Compiler:		MS Visual Studio 2019
	Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
	Course:			CST 8152 – Compilers, Lab Section: [23 & 21]
	Assignment:		3
	Date:			DEC 13, 2020
	Professor:		Paulo Sousa / Kaci Taouch
	Purpose:		Parser implementation
	Function list:	processParser(), matchToken(), syncErrorHandler(), printError(), program(), additiveArithmeticExpressionPrime(),
					additiveArithmeticExpression(), arithmeticExpression(), assignmentExpression(), assignmentStatement(), conditionalExpression(),
					inputStatement(), iterationStatement(), logicalAndExpression(), logicalOrExpression(), logicalNotExpression(),
					logicalAndExpressionPrime(), logicalOrExpressionPrime(), multiplicativeRrithmeticExpression(), multiplicativeArithmeticExpressionPrime(),
					outputList(), optionalStatements(), outputStatement(), primarySrelationalExpressionPrime(), primaryArelationalExpressionPrime(),
					primaryArithmeticExpression(), primarySrelationalExpression(), primaryArelationalExpression(), primaryStringExpression(), preCondition(),
					relationalExpression(), statement(), statements(), stringExpressionPrime(), statementPrime(), selectionStatement(), stringExpression(),
					variableListPrime(), variableList(), variableIdentifier(), unaryArithmeticExpression()


*/

#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"



/*
* Function name:	processParser
* Purpose:			determine the next token state. start program, and match end of file
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-12-13
* Called functions: processToken(), program(), matchToken(), printf()
* Parameters:		void
* Return value:		void
* Algorithm:		- store the value of processToken into lookahead, then call program method. then match the end of file & print the file parsed statment.
*/
void processParser(void) {
	//if (debugMode)
	//	fprintf(stderr, "[00] Processing parser...\n");
	lookahead = processToken();
	program();
	matchToken(SEOF_T, NO_ATTR);
	printf("%s\n", "PLATY: Source file parsed");
}

/*
* Function name:	matchToken
* Purpose:			compare and match the value of token code to tokenCode parameter and do prober error reporting when needed
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-12-13
* Called functions: syncErrorHandler(), printError(), processToken()
* Parameters:		int tokenCode, int tokenAttribute
* Return value:		void
* Algorithm:		- Check if token attribute is a keyword, if so, then check if it exist in the keyword array and set matchflag
					  to false if not (call error handler).
					- if token is logical operator, arithmitic operator, or relational operator & attribute integer is differenr from tokenAttribute parameter value,
					  call syncErrorHandler.
					- if token coed equals end of file, return.
					- if token code matchs tokenCode parameter value and token code is error, call print error & increment error counter.
*/
void matchToken(int tokenCode, int tokenAttribute) {
	int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
	case REL_OP_T:
	case ART_OP_T:
	case LOG_OP_T:
		// TODO_05
		/* if token is keyword */
		if (tokenAttribute == KW_T)
		{
			/* if token attribute is different from keyword index, set matchflag to false */
			if (tokenAttribute != lookahead.attribute.kwt_idx)
			{
				matchFlag = 0;
			}

		}
		/* if token is logical operator, arithmitic operator, or relational operator */
		else if (tokenCode == LOG_OP_T || tokenCode == ART_OP_T || tokenCode == REL_OP_T)
		{
			/* if attribute integer is differenr from tokenAttribute parameter value, call syncErrorHandler */
			if (lookahead.attribute.get_int != tokenAttribute)
			{
				syncErrorHandler(tokenCode);
				return;
			}
		}


	default:
		// TODO_06
		/* if token code is differnt from tokenCode parameter value set match falg to false*/
		if (lookahead.code != tokenCode) {
			matchFlag = 0;
		}


	}
	if (matchFlag && lookahead.code == SEOF_T)
		// TODO_07
		return;
	if (matchFlag) {/* if token code matchs tokenCode parameter value*/
		lookahead = processToken();
		if (lookahead.code == ERR_T)/* if token code is error, call print error & increment error counter */
		{
			printError();
			lookahead = processToken();
			syntaxErrorNumber++;
			return;
		}
		// TODO_08
	}
	else {/* if token code is differnt from tokenCode parameter value, call syncErrorHandler */
		// TODO_09
		syncErrorHandler(tokenCode);
	}

	return;
}

/*
* Function name:	syncErrorHandler
* Purpose:			panic mode error handling
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-12-13
* Called functions: printError(), processToken()
* Parameters:		int syncTokenCode
* Return value:		void
* Algorithm:		- call print error, then increment the error counter.
					- loop until token code is similar to synctoken code opr end of file sign is found, if end of file found,
					  exit and pass the syntax error number.
*/
void syncErrorHandler(int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	// TODO_10
	while (lookahead.code != syncTokenCode) {/* loop until token code is similar to synctoken code  */
		// TODO_11
		lookahead = processToken();
		if (lookahead.code == SEOF_T)/* if end of file sign is found exit and provide the syntax error number */
		{
			exit(syntaxErrorNumber);
		}
	}
	if (lookahead.code != SEOF_T) {
		// TODO_12
		lookahead = processToken();
	}

}


/*
* Function name:	printError
* Purpose:			print error statments for the tokens
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-12-13
* Called functions: printf()
* Parameters:		void
* Return value:		void
* Algorithm:		- print the proper error message based on the mathing token code & attribute
*/
void printError() {
	Token t = lookahead;
	//if (debugMode)
	//	fprintf(stderr, "[39] PrintError\n");
	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
		// TODO_13
	case  ERR_T: /* Error token */
		printf("%s\n", t.attribute.err_lex);
		break;
	case  SEOF_T: /* Source end-of-file token */
		printf("SEOF_T\t\t%d\t\n", t.attribute.seof);
		break;
	case  AVID_T: /* Arithmetic Variable identifier token */
	case  SVID_T:/* String Variable identifier token */
		printf("%s\n", t.attribute.vid_lex);
		break;
	case  FPL_T: /* Floating point literal token */
		printf("%5.1f\n", t.attribute.flt_value);
		break;
	case INL_T: /* Integer literal token */
		printf("%d\n", t.attribute.get_int);
		break;
	case STR_T:/* String literal token */
		printf("%s\n", bufferGetString(stringLiteralTable, t.attribute.str_offset));
		break;
	case SCC_OP_T: /* String concatenation operator token */
		printf("NA\n");
		break;
	case  ASS_OP_T:/* Assignment operator token */
		printf("NA\n");
		break;
	case  ART_OP_T:/* Arithmetic operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  REL_OP_T: /* Relational operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  LOG_OP_T:/* Logical operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  LPR_T: /* Left parenthesis token */
		printf("NA\n");
		break;
	case  RPR_T: /* Right parenthesis token */
		printf("NA\n");
		break;
	case LBR_T: /* Left brace token */
		printf("NA\n");
		break;
	case RBR_T: /* Right brace token */
		printf("NA\n");
		break;
	case KW_T: /* Keyword token */
		printf("%s\n", keywordTable[t.attribute.get_int]);
		break;
	case COM_T: /* Comma token */
		printf("NA\n");
		break;
	case EOS_T: /* End of statement *(semi - colon) */
		printf("NA\n");
		break;
	default:
		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
	}/*end switch*/
}


/*************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (PLATYPUS)}.
 ************************************************************/
void program(void) {
	//if (debugMode)
	//	fprintf(stderr, "[01] Program\n");
	matchToken(KW_T, (0));
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	printf("%s\n", "PLATY: Program parsed");
}

// TODO_14: All functions...// TODO_14: All functions...

/*
* opt_statements
* BNF: <opt_statements> -> <statements> | ϵ
* FIRST(<opt_statements>) = { FIRST(<statements>) } = { AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(INPUT), KW_T(OUTPUT), ϵ}
*/
void optionalStatements(void)
{
	/* check token */
	if (lookahead.code == KW_T)
	{
		if (lookahead.attribute.get_int == IF || lookahead.attribute.get_int == WHILE ||
			lookahead.attribute.get_int == INPUT || lookahead.attribute.get_int == OUTPUT)
		{
			statements();
		}
		else
		{
			/* empty string */
			printf("%s\n", "PLATY: Opt_statements parsed");
		}
	}
	else if (lookahead.code == AVID_T || lookahead.code == SVID_T)
	{
		statements();
	}
	else
	{
		/* empty string */
		printf("%s\n", "PLATY: Opt_statements parsed");
	}

}

/*
* pre-condition
* BNF:<pre-condition> -> TRUE | FALSE
* FIRST(<pre-condition>) = { TRUE, FALSE }
*/
void preCondition(void)
{
	if (lookahead.code == KW_T)
	{
		if (lookahead.attribute.kwt_idx == TRUE)
		{
			/* keyword true */
			matchToken(KW_T, TRUE);
		}
		else
		{
			/* keyword false */
			matchToken(KW_T, FALSE);
		}
	}
}

/*
* input statement
* BNF: <input statement> -> INPUT (<variable list>);
* FIRST(<input statement>) = { KW_T(INPUT) } */
void inputStatement(void)
{
	matchToken(KW_T, INPUT);
	matchToken(LPR_T, NO_ATTR);
	variableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Input statement parsed");
}

/*
* output list
* BNF: This function implements <output list> -> <opt variable list> | STR_T;
* FIRST (<output list>) = { FIRST (<opt variable list >) , STR_T} = {AVID_T, SVID_T, STR_T, ϵ } */
void outputList(void)
{
	/* checks if token  is AVID_T or SVID_T, if so, call variableList() */
	if (lookahead.code == AVID_T || lookahead.code == SVID_T)
	{
		variableList();
	}
	else if (lookahead.code == STR_T)
	{
		/* check if its string literal */
		matchToken(STR_T, NO_ATTR);
		printf("%s\n", "PLATY: Output list (string literal) parsed");
	}
	else
	{
		/* empty output list */
		printf("%s\n", "PLATY: Output list (empty) parsed");
	}
}

/*
* output statement
 * BNF: <output statement> -> OUTPUT (<output list>);
* FIRST(<output statement>) = { KW_T(OUTPUT) } */
void outputStatement(void)
{
	matchToken(KW_T, OUTPUT);
	matchToken(LPR_T, NO_ATTR);
	outputList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Output statement parsed");
}

/*
* <variable identifier>
* BNF: <variable identifier> -> AVID_T | SVID_T
* FIRST(<variable identifier>) = { AVID_T, SVID_T } */
void variableIdentifier(void)
{
	if (lookahead.code == AVID_T || lookahead.code == SVID_T)/* check if token is  AVID_T or SVID_T*/
	{

		matchToken(lookahead.code, NO_ATTR);
	}
	else
	{
		/* error */
		printError();
	}
}

/*
* variable listPrime
* BNF: This function implements , <variable identifier> <variable listPrime> | ϵ
* FIRST(<variable listPrime>) = { COM_T, ϵ } */
void variableListPrime(void)
{
	if (lookahead.code == COM_T)
	{
		/* token start with a ","*/
		matchToken(COM_T, NO_ATTR);
		variableIdentifier();
		variableListPrime();
	}
}

/*
* variable list
* BNF: <variable list> -> <variable identifier> <variable listPrime>
* FIRST(<variable list>) = { FIRST(<variable identifier>) } = { AVID_T, SVID_T }
*/
void variableList(void)
{
	variableIdentifier();
	variableListPrime();
	printf("%s\n", "PLATY: Variable list parsed");
}


/*
* conditional expression
* BNF:<conditional expression> -> <logical OR expression>
* FIRST(<conditional expression>) = { FIRST(<logical OR expression>) } = { AVID_T, FPL_T, INL_T, SVID_T, STR_T }
*/
void conditionalExpression(void)
{
	logicalOrExpression();
	printf("%s\n", "PLATY: Conditional expression parsed");
}


/*
* logical OR expression
* BNF: <logical OR expression> -> <logical AND expression> <logical OR expressionPrime>
* FIRST(<logical OR expression>) = { FIRST(<logical AND expression>) } = { AVID_T, FPL_T, INL_T, SVID_T, STR_T }
*/
void logicalOrExpression(void)
{
	logicalAndExpression();
	logicalOrExpressionPrime();
}

/*
* logical OR expressionPrime
* BNF:<logical OR expressionPrime> -> _OR_ <logical AND expression> <logical OR expressionPrime> | ϵ
* FIRST(<logical OR expressionPrime>) = { _OR_ , ϵ }*/
void logicalOrExpressionPrime(void)
{
	if (lookahead.code == LOG_OP_T)
	{
		if (lookahead.attribute.log_op == OR)/* check if logical operator is OR */
		{
			matchToken(LOG_OP_T, OR);
			logicalAndExpression();
			logicalOrExpressionPrime();
			printf("%s\n", "PLATY: Logical OR expression parsed");
		}
	}
}

/*
* logical AND expression
* BNF: <logical AND expression> -> <logical NOT expression> <logical AND expressionPrime>
* FIRST(<logical AND expression>) = { FIRST(<logical NOT expression>) } = { AVID_T, FPL_T, INL_T, SVID_T, STR_T }
 */
void logicalAndExpression(void)
{
	//logicalNotExpression();
	relationalExpression();
	logicalAndExpressionPrime();
}


/*
* logical AND expressionPrime
* BNF: <logical AND expressionPrime> -> _AND_ <logical NOT expression> <logical AND expressionPrime>
* FIRST(<logical AND expressionPrime>) = { _AND_ , ϵ }
*/
void logicalAndExpressionPrime(void)
{
	if (lookahead.code == LOG_OP_T)
	{
		if (lookahead.attribute.log_op == AND)/* check if logical operator is AND */
		{
			matchToken(LOG_OP_T, AND);
			relationalExpression();
			logicalAndExpressionPrime();
			printf("%s\n", "PLATY: Logical AND expression parsed");
		}
	}
}


/*
* logical NOT expression
* BNF:<logical NOT expression> -> _NOT_ <relational expression>| <relational expression>
* FIRST(<logical NOT expression>) = { _NOT_ , ϵ }
*/
void logicalNotExpression()
{
	if (lookahead.code == LOG_OP_T)
	{
		if (lookahead.attribute.log_op == NOT)/* check if logical operator is NOT */
		{
			matchToken(LOG_OP_T, NOT);
			relationalExpression();
			printf("%s\n", "PLATY: Logical NOT expression parsed");
		}
	}

}


/*
* relational expression
* BNF <relational expression> -> <primary a_relational expression> <primary a_relational expressionPrime>
									| <primary s_relational expression> <primary s_relational expressionPrime>
* FIRST(<relational expression>) = { FIRST(<primary a_relational expression>) }
*/
void relationalExpression(void)
{
	if (lookahead.code == SVID_T || lookahead.code == STR_T)
	{
		primarySrelationalExpression();
		primarySrelationalExpressionPrime();
	}
	else if (lookahead.code == AVID_T || lookahead.code == FPL_T || lookahead.code == INL_T)
	{
		primaryArelationalExpression();
		primaryArelationalExpressionPrime();
	}
	else
	{
		printError();
	}

	printf("%s\n", "PLATY: Relational expression parsed");
}


/*
* primary a_relational expression
* BNF: <primary s_relational expression> -> == <primary s_relational expressionPrime> | <> <primary s_relational expression>
												| > <primary s_relational expression> | < <primary s_relational expression>
* FIRST(<primary a_relational expression>) = { AVID_T, FPL_T, INL_T }
*/
void primaryArelationalExpression(void)
{
	if (lookahead.code == SVID_T || lookahead.code == STR_T)
	{
		printError();
		printf("%s\n", "PLATY: Primary a_relational expression parsed");
	}
	else if (lookahead.code == AVID_T || lookahead.code == FPL_T || lookahead.code == INL_T)
	{
		matchToken(lookahead.code, NO_ATTR);
		printf("%s\n", "PLATY: Primary a_relational expression parsed");
	}
	else
	{
		printError();
	}
}

/*
* primary s_relational expression
* BNF :<primary s_relational expression> -> == <primary a_relational expressionPrime> | <> <primary a_relational expression>
												| <primary a_relational expression> | < <primary a_relational expression>
* FIRST(<primary a_relational expressionPrime>) = { ==, <>, >, < }
*/
void primaryArelationalExpressionPrime(void)
{
	if (lookahead.code == REL_OP_T)
	{
		if (lookahead.attribute.rel_op == EQ || lookahead.attribute.rel_op == NE || lookahead.attribute.rel_op == GT
			|| lookahead.attribute.rel_op == LT)
		{
			matchToken(REL_OP_T, lookahead.attribute.rel_op);
			primaryArelationalExpression();
		}
		else
		{
			printError();
		}
	}
}


/*
* primary s_relational expression
* BNF: <primary s_relational expression> -> <primary string expression>
* FIRST(<primary s_relational_expression>) = { FIRST(<primary string expression> } = { SVID_T, STR_T } */
void primarySrelationalExpression(void)
{
	if (lookahead.code == AVID_T || lookahead.code == FPL_T || lookahead.code == INL_T)
	{
		/* token is  AVID_T, FPL_T, or INL_T */
		printError();
		printf("%s\n", "PLATY: Primary s_relational expression parsed");
	}
	else if (lookahead.code == SVID_T || lookahead.code == STR_T)
	{
		/* token is SVID_T or STR_T */
		primaryStringExpression();
		printf("%s\n", "PLATY: Primary s_relational expression parsed");
	}
	else
	{
		printError();
	}
}

/*
* primary s_relational expressionPrime
* BNF: <primary s_relational expressionPrime> -> == <primary s_relational expression> | <> <primary s_relational expression>
												| > <primary s_relational expression> | < <primary s_relational expression>
* FIRST(<primary s_relational expressionPrime>) = { ==, <>, >, < }
*/
void primarySrelationalExpressionPrime(void)
{
	if (lookahead.code == REL_OP_T)
	{
		/* token is relational operator */
		if (lookahead.attribute.rel_op == EQ || lookahead.attribute.rel_op == NE || lookahead.attribute.rel_op == GT
			|| lookahead.attribute.rel_op == LT)
		{
			/* relational operator is EQ, NE, GT,or LT */
			matchToken(REL_OP_T, lookahead.attribute.rel_op);
			primarySrelationalExpression();
		}
		else
		{
			printError();
		}
	}
}


/*
* assignment statement
* BNF: <assignment statement> -> <assignment expression>
* FIRST(<assignment statement>) = { FIRST(<assignment expression>) } = { AVID_T, SVID_T }
*/
void assignmentStatement(void)
{
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Assignment statement parsed");
}

/*
* string expression
* BNF: This function implements <string expression> -> <primary string expression> | <string expressionPrime>
* FIRST(<string expression>) = {FIRST(<primary string expression>)}  = {SVID_T,STR_T}
*/
void stringExpression(void)
{
	primaryStringExpression();
	stringExpressionPrime();
	printf("%s\n", "PLATY: String expression parsed");
}

/*
* string expressionPrime
* BNF: <string expressionPrime> -> $$  <primary string expression> <string expressionPrime> | ϵ
* FIRST(<string expressionPrime>) = { $$, ϵ }
*/
void stringExpressionPrime(void)
{
	if (lookahead.code == SCC_OP_T)
	{
		matchToken(SCC_OP_T, NO_ATTR);
		primaryStringExpression();
		stringExpressionPrime();
	}
}


/*
* primary string expression
* BNF: This function implements <primary string expression> -> SVID_T | STR_T
* FIRST(<primary string expression>) = { SVID_T, STR_T }
*/
void primaryStringExpression(void)
{
	if (lookahead.code == SVID_T)/* check for SVID_T */
	{
		matchToken(SVID_T, NO_ATTR);
	}

	if (lookahead.code == STR_T)/* check for STR_T */
	{
		matchToken(STR_T, NO_ATTR);
	}

	printf("%s\n", "PLATY: Primary string expression parsed");
}

/*
* assignment expression
* BNF: <assignment expression> -> AVID = <arithmetic expression> | SVID = <string expression>
* FIRST(<assignment expression>) = { AVID_T, SVID_T }
*/
void assignmentExpression(void)
{
	if (lookahead.code == SVID_T)
	{
		/* token is SVID_T */
		matchToken(SVID_T, NO_ATTR);
		matchToken(ASS_OP_T, EQ);
		stringExpression();
		printf("%s\n", "PLATY: Assignment expression (string) parsed");
	}

	if (lookahead.code == AVID_T)
	{
		/* token is AVID_T */
		matchToken(AVID_T, NO_ATTR);
		matchToken(ASS_OP_T, EQ);
		arithmeticExpression();
		printf("%s\n", "PLATY: Assignment expression (arithmetic) parsed");
	}
}


/*
* arithmetic expression
* BNF: <arithmetic expression> - > <unary arithmetic expression> | <additive arithmetic expression>
* FIRST(<arithmetic expression>) = { FIRST(<unary arithmetic expression> }
*/
void arithmeticExpression()
{
	if (lookahead.code == ART_OP_T)
	{
		/* token  is arithmitic operator add or subtract */
		if (lookahead.attribute.arr_op == ADD || lookahead.attribute.arr_op == SUB)
		{
			unaryArithmeticExpression();
		}
		else
		{
			printError();/* error */
		}

		printf("%s\n", "PLATY: Arithmetic expression parsed");
	}
	else if (lookahead.code == AVID_T || lookahead.code == FPL_T || lookahead.code == INL_T || lookahead.code == LPR_T)
	{
		additiveArithmeticExpression();
		printf("%s\n", "PLATY: Arithmetic expression parsed");
	}
	else
	{
		printError();
	}

}


/*
* multiplicative arithmetic expression
* BNF: <multiplicative arithmetic expression> -> <primary arithmetic expression> <multiplicative arithmetic expressionPrime>
* FIRST(<multiplicative arithmetic expression >) = { FIRST(<primary arithmetic expression>)} = { AVID_T, FPL_T, INL_T, ( }
*/
void multiplicativeRrithmeticExpression(void)
{
	primaryArithmeticExpression();
	multiplicativeArithmeticExpressionPrime();
}

/*
* multiplicative arithmetic expressionPrime
* BNF: <multiplicative arithmetic expressionPrime> -> * < primary arithmetic expression> <multiplicative arithmetic expressionPrime>
													| / < primary arithmetic expression> < multiplicative arithmetic expressionPrime>
* FIRST(<multiplicative arithmetic expressionPrime>) = { *, /, ϵ }
*/
void multiplicativeArithmeticExpressionPrime(void)
{
	if (lookahead.code == ART_OP_T)
	{
		if (lookahead.attribute.arr_op == DIV)
		{
			/* arithmetic operator is division */
			matchToken(ART_OP_T, DIV);
			primaryArithmeticExpression();
			multiplicativeArithmeticExpressionPrime();
			printf("%s\n", "PLATY: Multiplicative arithmetic expression parsed");
		}
		else if (lookahead.attribute.arr_op == MUL)
		{
			/* arithmetic operator is multiplication */
			matchToken(ART_OP_T, MUL);
			primaryArithmeticExpression();
			multiplicativeArithmeticExpressionPrime();
			printf("%s\n", "PLATY: Multiplicative arithmetic expression parsed");
		}
	}
}

/*
* additive arithmetic expression
* BNF: <additive arithmetic expression> -> <multiplicative arithmetic expression> <additive arithmetic expressionPrime>
* FIRST(<additive arithmetic expression>) = {FIRST(<multiplicative arithmetic expression>)} = { AVID_T, FPL_T, INL_T, ( }
*/
void additiveArithmeticExpression(void)
{
	multiplicativeRrithmeticExpression();
	additiveArithmeticExpressionPrime();
}

/*
* additive arithmetic expressionPrime
* BNF: <additive arithmetic expressionPrime> -> + <multiplicative arithmetic expression> <additive arithmetic expressionPrime>
											| - <multiplicative arithmetic expression> <additive arithmetic expressionPrime> | ϵ
* FIRST(<additive arithmetic expressionPrime>) = { +, -, ϵ }
*/
void additiveArithmeticExpressionPrime(void)
{
	if (lookahead.code == ART_OP_T)
	{
		if (lookahead.attribute.arr_op == SUB)
		{
			/* arithmetic operator is subtract */
			matchToken(ART_OP_T, SUB);
			multiplicativeRrithmeticExpression();
			additiveArithmeticExpressionPrime();
			printf("%s\n", "PLATY: Additive arithmetic expression parsed");
		}
		else if (lookahead.attribute.arr_op == ADD)
		{
			/* arithmetic operator is add */
			matchToken(ART_OP_T, ADD);
			multiplicativeRrithmeticExpression();
			additiveArithmeticExpressionPrime();
			printf("%s\n", "PLATY: Additive arithmetic expression parsed");
		}
		else
		{
			/*empty string*/
			printError();
		}

	}
}

/*
* unary arithmetic expression
* BNF: <unary arithmetic expression> -> - <primary arithmetic expression> | + <primary arithmetic expression>
* FIRST(<unary arithmetic expression>) = { -, + }
*/
void unaryArithmeticExpression(void)
{
	if (lookahead.code == ART_OP_T)
	{
		if (lookahead.attribute.arr_op == SUB)
		{
			/* token arithmetic operator is subtract*/
			matchToken(ART_OP_T, SUB);
			primaryArithmeticExpression();
		}
		else if (lookahead.attribute.arr_op == ADD)
		{
			/*token arithmetic operator is add*/
			matchToken(ART_OP_T, ADD);
			primaryArithmeticExpression();
		}
	}

	printf("%s\n", "PLATY: Unary arithmetic expression parsed");
}

/*
* statements
* BNF: <statements> -> <statement><statementsPrime>
* FIRST(<statements>) = { FIRST(<statement>) } = { AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(INPUT), KW_T(OUTPUT) }
*/
void statements(void)
{
	statement();
	statementPrime();
}

/*
* statementsPrime
* BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
* FIRST(<statementsPrime>) = { AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(INPUT), KW_T(OUTPUT) , ϵ }
*/
void statementPrime(void)
{
	if (lookahead.code == AVID_T || lookahead.code == SVID_T)
	{
		/*token  AVID_T or SVID_T*/
		statement();
		statementPrime();
	}
	else if (lookahead.code == KW_T)
	{
		if (lookahead.attribute.get_int == IF || lookahead.attribute.get_int == WHILE
			|| lookahead.attribute.get_int == INPUT || lookahead.attribute.get_int == OUTPUT)
		{
			statements();
		}
	}
}

/*
* primary arithmetic expression
* BNF: <primary arithmetic expression> -> AVID_T | FPL_T | INL_T | (<arithmetic expression>)
* FIRST(<primary arithmetic expression>) = { AVID_T, FPL_T, INL_T, ( } */
void primaryArithmeticExpression(void)
{
	if (lookahead.code == AVID_T || lookahead.code == FPL_T || lookahead.code == INL_T)
	{
		matchToken(lookahead.code, NO_ATTR);
	}
	else if (lookahead.code == LPR_T)
	{
		matchToken(LPR_T, NO_ATTR);
		arithmeticExpression();
		matchToken(RPR_T, NO_ATTR);
	}

	printf("%s\n", "PLATY: Primary arithmetic expression parsed");
}

/*
* BNF: <statement> -> <assignment statement> | <selection statement> | <iteration statement> | <input statement> | <output statement>
* FIRST(<statement>) = { FIRST(<assignment statement>), FIRST(<selection statements>), FIRST(<iteration statement>),
	FIRST(<input statement>), FIRST(<output statement>) } = { AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(INPUT),
	KW_T(OUTPUT) }
 */
void statement(void)
{
	if (lookahead.code == AVID_T || lookahead.code == SVID_T)
	{
		assignmentStatement();
	}
	else if (lookahead.code == KW_T)
	{
		/* token is KW_T */
		switch (lookahead.attribute.get_int)
		{
		case IF:
			selectionStatement();
			break;
		case WHILE:
			iterationStatement();
			break;
		case INPUT:
			inputStatement();
			break;
		case OUTPUT:
			outputStatement();
			break;
		default:
			break;
		}
	}
	else
	{
		/* error */
		printError();
	}
}


/*
* iteration statement
* BNF: <iteration statement> -> WHILE <pre-condition> (<conditional expression>) DO { <statements>};
* FIRST(<iteration statement>) = { KW_T(WHILE) }*/
void iterationStatement(void)
{
	matchToken(KW_T, WHILE);
	preCondition();
	matchToken(LPR_T, NO_ATTR);
	conditionalExpression();
	matchToken(RPR_T, NO_ATTR);
	matchToken(KW_T, DO);
	matchToken(LBR_T, NO_ATTR);
	statements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Iteration statement parsed");
}



/*
* selection statement
* <selection statement> -> IF <pre-condition>  (<conditional expression>) THEN { <optional statements> } ELSE { <optional statements> } ;
* FIRST(<selection statement>) = { KW_T(IF) }
*/
void selectionStatement(void)
{
	matchToken(KW_T, IF);
	preCondition();
	matchToken(LPR_T, NO_ATTR);
	conditionalExpression();
	matchToken(RPR_T, NO_ATTR);
	matchToken(KW_T, THEN);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, ELSE);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Selection statement parsed");
}