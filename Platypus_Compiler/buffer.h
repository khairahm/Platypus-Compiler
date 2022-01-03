/*	File name:		buffer.c
	Compiler:		MS Visual Studio 2019
	Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
	Course:			CST 8152 – Compilers, Lab Section: [23 & 21]
	Assignment:		2
	Date:			Sep 19, 2020
	Professor:		Paulo Sousa / Haider Miraj.
	Purpose:		Purpose: Preprocessor directives, type declarations and prototypes necessary for buffer implementation.
	Function list:	bufferCreate(), bufferAddChar(), bufferLoad(), bufferGetChar(), bufferPrint(), bufferGetCapacity()
					bufferGetAddCPosition(), bufferGetOpMode(), bufferGetIncrement(), bufferGetFlags(), bufferRewind()
					bufferGetString(), bufferSetEnd(), bufferFree(), bufferClear(), bufferIsFull(), bufferIsEmpty(), bufferRetract(), bufferReset()
					bufferGetCPosition(), bufferSetMarkPosition()
*/

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* constant definitions */
#define RT_FAIL_1 (-1)				/* operation failure return value -1 */
#define RT_FAIL_2 (-2)				/* operation failure return value -2 */
#define LOAD_FAIL (-2)				/* load fail return value -2*/
#define DEFAULT_INIT_CAPACITY 200	/* default initial buffer capacity */
#define DEFAULT_INC_FACTOR 15       /* default increment factor */

/* You should add your own constant definitions here */

#define ZERO 0 /*Holds value of zero*/
#define ONE 1 /*holds value of one */
#define TWO_FIFTY_FIVE 255 /*Holds value of 255*/
#define HUNDRED 100 /*Holds value of 100*/
#define FIXED 'f' /*Holds f char*/
#define ADDITIVE 'a'/* Holds a Char*/
#define MULTI 'm' /* holds m char*/
#define VAL_FIXED 0 /* Holds opmode */
#define VAL_ADDITIVE  1 /* opmode 1*/
#define VAL_MULTI -1 /* opmode -1*/
#define MAXIMUM_ALLOWED_POSITIVE_VALUE (SHRT_MAX-1) /*Max value in capacity*/
#define BUFFER_INCREMENT_RUNTIME_ERR 0x100	/*run time error return value for buffer increment function*/

/* Add your bit-masks constant definitions here */
#define DEFAULT_FLAGS  0xFFFC 	/* 1111.1111 1111.1100*/
#define SET_EOB		0x0001 /* Sets EOB to 1*/
#define RESET_EOB 0xFFFE/* Reset to 0*/
#define CHECK_EOB 0x0001 /* Check eob mask*/
#define SET_R_FLAG 0x0002 /* set r flag mask (second last bit)*/
#define RESET_R_FLAG 0xFFFD /*Reset r flag mask to 0*/
#define CHECK_R_FLAG 0x0002 /* Check r flag mask*/

/* user data type declarations */
typedef struct BufferEntity {
	char* string;   /* pointer to the beginning of character array (character buffer) */
	short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
	short addCPosition;  /* the offset (in chars) to the add-character location */
	short getCPosition;  /* the offset (in chars) to the get-character location */
	short markCPosition; /* the offset (in chars) to the mark location */
	char  increment; /* character array increment factor */
	char  opMode;       /* operational mode indicator*/
	unsigned short flags;     /* contains character array reallocation flag and end-of-buffer flag */
} Buffer, * pBuffer;


void err_printf(char* fmt, ...);
void display(Buffer* ptr_Buffer);
long get_filesize(char* fname);
pBuffer bufferCreate(short initCapacity, char incFactor, char opMode);
pBuffer bufferAddChar(pBuffer const pBE, char symbol);
int bufferLoad(FILE* const fi, Buffer* const pBE);
int bufferPrint(Buffer* const pBE, char newLine);
char bufferGetChar(Buffer* const pBE);
short bufferGetCapacity(Buffer* const pBE);
short bufferGetAddCPosition(Buffer* const pBE);
int bufferGetOpMode(Buffer* const pBE);
size_t bufferGetIncrement(Buffer* const pBE);
unsigned short bufferGetFlags(pBuffer const pBE);
int bufferRewind(Buffer* const pBE);
char* bufferGetString(Buffer* const pBE, short charPosition);
Buffer* bufferSetEnd(Buffer* const pBE, char symbol);
void bufferFree(Buffer* const pBE);
int bufferClear(Buffer* const pBE);
int bufferIsFull(Buffer* const pBE);
int bufferIsEmpty(Buffer* const pBE);
short bufferRetract(Buffer* const pBE);
short bufferReset(Buffer* const pBE);
short bufferGetCPosition(Buffer* const pBE);
Buffer* bufferSetMarkPosition(pBuffer const pBE, short mark);

#endif