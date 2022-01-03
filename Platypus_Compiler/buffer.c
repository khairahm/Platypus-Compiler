

/*	File name:		buffer.c
	Compiler:		MS Visual Studio 2019
	Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
	Course:			CST 8152 – Compilers, Lab Section: [23 & 21]
	Assignment:		2
	Date:			Sep 19, 2020
	Professor:		Paulo Sousa / Haider Miraj.
	Purpose:		To allocate memory and craete buffer and be able to dybnamically resize buffer to add characters, print buffer's content and empty buffer when needed.
	Function list:	bufferCreate(), bufferAddChar(), bufferLoad(), bufferGetChar(), bufferPrint(), bufferGetCapacity()
					bufferGetAddCPosition(), bufferGetOpMode(), bufferGetIncrement(), bufferGetFlags(), bufferRewind()
					bufferGetString(), bufferSetEnd(), bufferFree(), bufferClear(), bufferIsFull(), bufferIsEmpty(), bufferRetract(), bufferReset()
					bufferGetCPosition(), bufferSetMarkPosition()
*/

#include "buffer.h"


/*
* Function name:	bufferCreate
* Purpose:			Create and allocate memory for a new buffer.
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions: calloc(), malloc(), free()
* Parameters:		short initCapacity, char incFactor, char opMode
* Return value:		Buffer*, NULL
* Algorithm:		allocate memory for a buffer. set the operational mode and initial capacity & increment factor based on the mode.
					Finally, set the flag value to the default value (FFFC)
*/

pBuffer bufferCreate(short initCapacity, char incFactor, char opMode) {

	/* ensure a valid capacity is given in parameter --> capacity must be >= 0 and < max value of short */
	if (initCapacity < 0 || initCapacity > MAXIMUM_ALLOWED_POSITIVE_VALUE)
	{
		return NULL;
	}

	/*allocating one buffer and initializes it to zero*/
	Buffer* newBuffer = (Buffer*)calloc(1, sizeof(Buffer));


	/* checks if buffer is valid, and returns NULL if it's not valid */
	if (!newBuffer)
	{
		return NULL;
	}

	/* if initial capacity is zero allocate memory for the buffer's string with the size of the default initial capacity.
	   for fixed operational mode, increment factor is ZERO. for additive and multiplicative modes, increment factor is set to default (15).
	*/
	if (initCapacity == ZERO)
	{
		newBuffer->string = (char*)malloc(sizeof(char) * DEFAULT_INIT_CAPACITY);
		initCapacity = DEFAULT_INIT_CAPACITY;
		if (newBuffer->string == NULL)
		{
			return NULL;
		}
		if (opMode == FIXED)
		{
			incFactor = ZERO;

		}
		else if (opMode == ADDITIVE || opMode == MULTI)
		{
			incFactor = DEFAULT_INC_FACTOR;
		}
		else
		{
			free(newBuffer->string);
			free(newBuffer);
			return NULL;
		}
	}


	if (incFactor == ZERO && initCapacity != ZERO)
	{
		newBuffer->string = (char*)malloc(sizeof(char) * initCapacity);
		newBuffer->increment = ZERO;
		newBuffer->opMode = ZERO;
	}
	else
	{
		newBuffer->string = (char*)malloc(sizeof(char) * initCapacity);
		switch (opMode)
		{
		case ADDITIVE:
			/* if increment factor out of boundary, return null*/
			if ((unsigned char)incFactor < ZERO || (unsigned char)incFactor>MAXIMUM_ALLOWED_POSITIVE_VALUE) {

				return NULL;
			}

			/* set the operational mode to additive and buffer's increment to increment factor*/
			newBuffer->opMode = VAL_ADDITIVE;
			newBuffer->increment = incFactor;
			break;

		case FIXED:
			/* set the operational mode to fixed and buffer's increment to ZERO*/
			newBuffer->opMode = VAL_FIXED;
			newBuffer->increment = ZERO;
			break;

		case MULTI:
			/* if increment factor out of boundary, return null*/
			if ((unsigned char)incFactor < ZERO || (unsigned char)incFactor>100)
				return NULL;

			/* set the operational mode to multiplicative and buffer's increment to increment factor*/
			newBuffer->opMode = VAL_MULTI;
			newBuffer->increment = incFactor;
			break;

		default:
			return NULL;
		}
	}

	/* set the buffer's flag to default (FFFC) and capacity to initial capacity value from parameters then return the buffer*/
	newBuffer->capacity = initCapacity;
	newBuffer->flags = DEFAULT_FLAGS;
	return newBuffer;
}


/*

 * Function name:		bufferAddChar
 * Purpose:				The function adds a symbol to the operational character buffer depending on it's mod
 * Author:				Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
 * History/Versions:	1.0 2020-09-20
 * Called functions:	realloc()
 * Parameters:			pBuffer const pBE, char symbol
 * Return value:		pBE, NULL
 * Algorithm:			using bitwise operation, the function resets the flag R to ZERO a
						then check the buffer's capacity to add the new characternd. if capacity have enough space, new character is added to buffer
						if no enough space, capacity is resized based on the operational mode
 */

pBuffer bufferAddChar(pBuffer const pBE, char symbol) {

	short newCapacity, availbleSpace, newIncrement;
	char* relocate;

	if (pBE == NULL)
	{
		return NULL;
	}

	/* reset R flag back to default (0) */
	else
	{
		pBE->flags &= RESET_R_FLAG;

	}

	/* if buffers capacity have space for the new character*/
	if (pBE->addCPosition < pBE->capacity && pBE->capacity <= MAXIMUM_ALLOWED_POSITIVE_VALUE)
	{
		*(pBE->string + pBE->addCPosition++) = symbol;

		return pBE;
	}

	/* if buffer's capacity have no enought space to add new character*/
	else {

		switch (pBE->opMode)
		{
			/* if mode is fixed, return null*/
		case VAL_FIXED:

			return NULL;

			break;

			/* if mode is additive, check for capacity boundary then create a new capacity that is bigger than the buffer's capacity by an increment unit*/
		case VAL_ADDITIVE:
			newCapacity = pBE->capacity + (unsigned char)pBE->increment;

			if (newCapacity < ZERO)
			{
				return NULL;
			}

			if (newCapacity > ZERO && newCapacity > MAXIMUM_ALLOWED_POSITIVE_VALUE)
			{
				newCapacity = MAXIMUM_ALLOWED_POSITIVE_VALUE;
			}
			break;

			/* multiplicative mode*/
		case VAL_MULTI:

			/*If the current capacity can not be incremented anymore because
			it has already reached the maximum capacity of the buffer, the
			function returns NULL*/
			if (pBE->capacity >= MAXIMUM_ALLOWED_POSITIVE_VALUE || pBE->addCPosition >= MAXIMUM_ALLOWED_POSITIVE_VALUE)
			{
				return NULL;
			}

			/* increment current capacity using the given formula in assignment 1 specs document*/
			availbleSpace = MAXIMUM_ALLOWED_POSITIVE_VALUE - pBE->capacity;
			newIncrement = (short)(availbleSpace * ((double)(pBE->increment) / 100));
			newCapacity = pBE->capacity + newIncrement;

			/*if new capacity can't be incremented and still smaller than the maximum allowed capacity*/
			if (newCapacity == pBE->addCPosition && newCapacity < MAXIMUM_ALLOWED_POSITIVE_VALUE)
			{
				newCapacity = MAXIMUM_ALLOWED_POSITIVE_VALUE;
			}

			/*if the current capacity is bigger than the MAXIMUM ALLOWED POSITIVE VALUE*/
			if (newCapacity > MAXIMUM_ALLOWED_POSITIVE_VALUE)
			{
				newCapacity = MAXIMUM_ALLOWED_POSITIVE_VALUE;
			}

			if (newCapacity < ZERO)
			{
				return NULL;
			}

			break;

		default:
			return NULL;
			break;
		}

		/* reallocate memory for the character with the new capacity*/
		pBE->capacity = newCapacity;
		relocate = realloc(pBE->string, newCapacity);


		if (relocate == NULL) {
			return NULL;
		}

		//set r flag
		pBE->flags &= RESET_R_FLAG;

		//print buffer relocation lab 3 task 4

		//printf("Buffer Reallocated\n");

		pBE->string = relocate;

		/* check again the buffer's capacity before adding the new chracter*/
		if (pBE->addCPosition < pBE->capacity)
		{
			*(pBE->string + pBE->addCPosition++) = symbol;
			return pBE;
		}
		else {
			return NULL;
		}
	}
}


/*
* Function name:	bufferLoad
* Purpose:			Loads a input file into Fi into a buffer and also finds out the number of chars in file
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	bufferAddChar(), feof()
* Parameters:		FILE* fi, Buffer* pBE
* Return value:		int numChar
* Algorithm:		Checks if fi or pBE is null if it is return Fail -1
*					Loop through file until end of file and and adds char to the string
*					each loop will take one letter and add it to the string with function bufferAddChar()
*					increment the number of char
*					if the letter return null there will be a LOAD FAIL
*					return the number of char
*/
int bufferLoad(FILE* const fi, Buffer* const pBE) {
	/*Holds char temporarly*/
	char letter;
	/*Holds int value of number of char*/
	int numChar = 0;
	/*checks if file or buffer is null and will return fail -1*/
	if (fi == NULL || pBE == NULL)
	{
		return RT_FAIL_1;
	}
	letter = (char)fgetc(fi);

	/*Loop until end of file*/
	while (!feof(fi))
	{
		if (!bufferAddChar(pBE, letter))
		{
			ungetc(letter, fi);
			return RT_FAIL_1;
		}
		letter = (char)fgetc(fi);
		numChar++;
	}

	if (ferror(fi))
	{
		return RT_FAIL_1;
	}
	/*return numchar*/
	return numChar;
}



/*
* Function name:	bufferGetChar
* Purpose:			Gets one letter from the string and returns it
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	none
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_2, int ZERO, Buffer*(pBE->string + pBE->getCPosition++)
* Algorithm:		Check if buffer is null if so return fail -2
*					check if addcposition is less than getcposition
*					if getCpostition is the same as add cposition then set eob flags and return zero
*					this means that the buffer is full with a return 0
*					else reset flags and return char lettter
*/
char bufferGetChar(Buffer* const pBE) {
	/*Check if pBE is null if so return fail -2*/
	if (pBE == NULL)
	{
		return RT_FAIL_2;
	}
	/*if no room return fail -2*/
	if (pBE->addCPosition < pBE->getCPosition)
	{
		return RT_FAIL_2;
	}
	/*If buffer is full then set eob flag and return zero*/
	if (pBE->getCPosition == pBE->addCPosition)
	{
		pBE->flags |= SET_EOB;
		return ZERO;
	}
	else {//else rest eob flag
		pBE->flags &= RESET_EOB;
	}
	/*return a char from the string*/
	return *(pBE->string + pBE->getCPosition++);
}



/*
* Function name:	bufferPrint
* Purpose:			Prints the string in the buffer
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	bufferGetChar() , printf()
* Parameters:		Buffer* pBE , char newLine
* Return value:		int numChar
* Algorithm:		Check if pBE is null > return fail -1
*					check if string is null > return fail -1
*					Loop where only if the end of buffer is reached will it exit //prints out entire buffer string
*					charHolder get a letter from buffer useing function bufferGetChar()
*					charHolder is printed out using printf()
*					increment the number of chars integer
*					If reached end of string break out of loop
*					if new line =0 print a new line
*					return numChar
*
*/
int bufferPrint(Buffer* const pBE, char newLine) {
	char charHolder;
	int charNum = 0;

	if (pBE == NULL || (!pBE->string))
	{
		return RT_FAIL_1;
	}

	if (pBE->string == NULL)
	{
		return RT_FAIL_1;
	}

	charHolder = bufferGetChar(pBE);

	while (!(pBE->flags & CHECK_EOB))
	{
		printf("%c", charHolder);
		charNum++;
		charHolder = bufferGetChar(pBE);
	}

	/*print new line*/
	if (newLine)
	{
		printf("\n");
	}
	/*return num of chars*/
	return charNum;
}


/*
* Function name:	bufferGetCapacity
* Purpose:			return the buffer's current capacity
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1, short pBE->capacity
* Algorithm:		Check if pBE is null > return fail -1
*					else, return the buffer's capacity
*/
short bufferGetCapacity(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	/*return the current capacity of the buffer*/
	else {
		return pBE->capacity;
	}
}



/*
* Function name:	bufferGetAddCPosition
* Purpose:			return the buffer's addCPosition
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1, short pBE->addCPosition
* Algorithm:		Check if pBE is null > return fail -1
*					else, return the buffer's addCPosition
*/
short bufferGetAddCPosition(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	/*return the current addCPosition of the buffer*/
	else {
		return pBE->addCPosition;
	}
}



/*
* Function name:	bufferGetOpMode
* Purpose:			return the buffer's operational mode
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_2, int the operational mode value(-1,0,1)
* Algorithm:		Check if pBE is null > return fail -2
*					else, return the buffer's operational mode

*/
int bufferGetOpMode(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_2;
	}
	else {/*return the value of the operational mode*/

		return pBE->opMode;
	}
}


/*
* Function name:	bufferGetIncrement
* Purpose:			return the buffer's increment value
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int BUFFER_INCREMENT_RUNTIME_ERR (256), the non-negative value of the buffer's increment
* Algorithm:		Check if pBE is null > return BUFFER_INCREMENT_RUNTIME_ERR (256)
*					else, return the casted non-negative value of the buffer's increment
*/
size_t bufferGetIncrement(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return BUFFER_INCREMENT_RUNTIME_ERR;
	}
	else {
		return (size_t)(unsigned char)pBE->increment;
	}
}


/*
* Function name:	bufferGetFlags
* Purpose:			return the buffer's flags
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1, short pBE->flags
* Algorithm:		Check if pBE is null > return fail -1
*					else, return the buffer's flags
*/
unsigned short bufferGetFlags(pBuffer const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return (unsigned short)RT_FAIL_1;
	}
	else {
		return pBE->flags;
	}
}


/*
* Function name:	bufferRewind
* Purpose:			reset the getCPosition & markCPosition to 0 so the buffer can start reading again from the beginning
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1, int ZERO
* Algorithm:		Check if pBE is null > return fail -1
*					else, reset the values of getCPosition & markCPosition to zero and return zero on sucess
*/
int bufferRewind(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	/*reset the values of markCPosition & getCPosition to zero and return zero*/
	else {
		pBE->getCPosition = ZERO;
		pBE->markCPosition = ZERO;
		return ZERO;
	}
}



/*
* Function name:	bufferGetString
* Purpose:			The function returns a pointer to the location of the character buffer
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE, short charPosition
* Return value:		NULL, char* pBE->string+charPosition
* Algorithm:		Check if pBE is null > return fail -1
					else, return a pointer to the location of the chracter buffer
*/
char* bufferGetString(Buffer* const pBE, short charPosition) {
	/*check for runtime errors*/
	if (pBE == NULL)
	{
		return NULL;
	}
	else {
		return pBE->string + charPosition;
	}
}


/*
* Function name:	bufferSetEnd
* Purpose:			The function shrinks (or in some cases may expand) the buffer to a new capacity.
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE, char symbol
* Return value:		NULL, Buffer* pBE
* Algorithm:		Check if pBE is null > return fail -1
					else, make the new capacity equals  getCPosition + 1
					and allocate memory for the new capacity,
					update the buffer's addCPosition+1 to the parameter value (symbol),
					reset the buffer's flags, then return a pointer to the buffer
*/
Buffer* bufferSetEnd(Buffer* const pBE, char symbol)
{
	short newCap = 0;
	char* newChar = NULL;

	/*check for runtime errors*/
	if (!pBE || !pBE->string)
	{
		return NULL;
	}

	newCap = pBE->addCPosition + 1;

	if (newCap <= ZERO)
	{
		return NULL;
	}

	newChar = (char*)realloc(pBE->string, (sizeof(char) * newCap));

	if (!newChar)
	{
		return NULL;
	}

	pBE->flags &= RESET_R_FLAG;
	pBE->flags |= SET_EOB;
	pBE->string = newChar;
	pBE->string[pBE->addCPosition++] = symbol;
	pBE->capacity = newCap;

	return pBE;
}

/*
* Function name:	bufferFree
* Purpose:			The function frees the memory occupied by the character buffer and the Buffer structure
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE, short charPosition
* Return value:		NULL, char* pBE->string+charPosition
* Algorithm:		Check if pBE is null > if not, then free the memory allocated to buffer and character array
*/
void bufferFree(Buffer* const pBE) {
	/*check for rutime errors*/
	if (!pBE) {
		return;
	}
	else {
		free(pBE->string);
		free(pBE);
	}
}


/*
* Function name:	bufferClear
* Purpose:			The function retains the memory space currently allocated to the buffer
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		NULL, int RT_FAIL_1 (-1), int ONE (1)
* Algorithm:		Check if pBE is null > return fail -1,
*					else, reset value of the buffer members
*/
int bufferClear(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	/*initialize the values to 0 to allow new characters added to buffer to be added to the beginning */
	pBE->addCPosition = ZERO;
	pBE->getCPosition = ZERO;
	pBE->markCPosition = ZERO;
	pBE->flags = DEFAULT_FLAGS;

	return ONE;
}


/*
* Function name:	bufferIsFull
* Purpose:			The function checks if buffer is full and return 1 if full or zero if not
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1 (-1), int ONE (1), int ZERO (0)
* Algorithm:		Check if pBE is null > return fail -1,
*					else, check if addCPosition is equal or bigger than the buffer's capacity,
*					if true that means buffer is full, return one. else, return zero
*/
int bufferIsFull(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	/*if buffer's character position is bigger than or equal the capacity, buffer is full. return 1 for true*/
	if (pBE->addCPosition >= pBE->capacity) {
		return ONE;
	}
	/*return zero if buffer is not full*/
	else {
		return ZERO;
	}
}


/*
* Function name:	bufferIsEmpty
* Purpose:			The function checks if buffer is empty and return 1 if empty or zero if not
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1 (-1), int ONE (1), int ZERO (0)
* Algorithm:		Check if pBE is null > return fail -1,
*					else, check if addCPosition is equal zero,
*					if true that means buffer is empty, return one. else, return zero
*/
int bufferIsEmpty(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	/*if buffer's character position is equal zero, then the bufferis empty. return 1 for true*/
	if (pBE->addCPosition == ZERO) {
		return ONE;
	}
	/*return zero if not empty*/
	else {
		return ZERO;
	}
}


/*
* Function name:	bufferRetract
* Purpose:			The function decrements getCPosition by 1
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1 (-1), Buffer* pBE->getCPosition
* Algorithm:		Check if pBE is null > return fail -1,
*					else, decrement getCPosition by 1 and return getCPosition
*/
short bufferRetract(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	/*decrement getCPosition by 1*/
	pBE->getCPosition -= 1;

	return pBE->getCPosition;
}


/*
* Function name:	bufferReset
* Purpose:			The function sets getCPosition to the value of the current markCPosition
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1 (-1), Buffer* pBE->getCPosition
* Algorithm:		Check if pBE is null > return fail -1,
*					else, set getCPosition to the value of the current markCPosition
*					and return getCPosition
*/
short bufferReset(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	/*set getCPosition to the value of the current markCPosition*/
	pBE->getCPosition = pBE->markCPosition;

	return pBE->getCPosition;
}


/*
* Function name:	bufferGetCPosition
* Purpose:			The function returns getCPosition.
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE
* Return value:		int RT_FAIL_1 (-1), Buffer* pBE->getCPosition
* Algorithm:		Check if pBE is null > return fail -1,
*					else, return getCPosition
*/
short bufferGetCPosition(Buffer* const pBE) {
	/*check for runtime errors*/
	if (pBE == NULL) {
		return RT_FAIL_1;
	}
	return pBE->getCPosition;
}


/*
* Function name:	bufferSetMarkPosition
* Purpose:			The function sets markCPosition to the value of the paarmeter mark.
* Author:			Khair Ahmed (04094681) lab_section (23) - Shadi Abdalla (040933412) lab_section (21)
* History/Versions:	1.0 2020-09-20
* Called functions:	N/A
* Parameters:		Buffer* pBE, short mark
* Return value:		NULL, Buffer* pBE
* Algorithm:		Check if pBE is null > return fail -1,
*					else, if the value of "mark is within 0 and markCPosition inclusive,
					set the value of markCPosition to "mark" then return a pointer to the buffer
*/
Buffer* bufferSetMarkPosition(pBuffer const pBE, short mark)
{
	/*check for runtime errors*/
	if (!pBE)
	{
		return NULL;
	}
	/*if the value of "mark is within 0 and markCPosition inclusive, set the value of markCPosition to mark*/
	if (mark < (pBE->addCPosition) && mark >= 0)
	{
		pBE->markCPosition = mark;
	}
	else
	{
		return NULL;
	}

	return pBE;
}
