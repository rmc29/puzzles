#include <stdlib.h>
#include <stdio.h>

#define OPCODE_MASK (1 << 31 | 1 << 30 | 1 << 29 | 1 << 28 )
#define A_MASK (1 << 8 | 1 << 7 | 1 << 6 )
#define B_MASK (1 << 5 | 1 << 4 | 1 << 3 )
#define C_MASK (1 << 2 | 1 << 1 | 1 << 0 )
#define ORTHOGRAPHY_A_MASK (1 << 27 | 1 << 26 | 1 << 25 )


enum eOpcode
{
	CONDITIONALMOVE,
	ARRAYINDEX,
	ARRAYAMEND,
	ADD,
	MULTIPLY,
	DIVIDE,
	NAND,
	HALT,
	ALLOC,
	FREE,
	OUTPUT,
	INPUT,
	LOADPROGRAM,
	ORTHOGRAPHY
};



/*helper function to read the input file from a file handle into an unsigned long array*/
unsigned long * fileReadInToArray(FILE * fpInput, unsigned long * pnSize)
{
	char c;
	unsigned long ii;
	unsigned long * anArray = NULL;
	if (!pnSize) return NULL;
	while (EOF != (c = getc(fpInput))) (*pnSize)++; /*count how big an array we need to allocate*/
	*pnSize = *pnSize / 4; /*divide char count by 4 to get int32 count*/
	rewind(fpInput); /*so that we can read the data in again*/
	anArray = malloc(*pnSize * sizeof(*anArray)); 
	for (ii = 0; ii < *pnSize - 1; ii++)
	{
		anArray[ii] = (unsigned char)getc(fpInput) << 24 | (unsigned char)getc(fpInput) << 16 | (unsigned char)getc(fpInput) << 8 | (unsigned char)getc(fpInput);	
	}	

	return anArray;
}

int main(int argc, char ** argv)
{
	unsigned long registers[8] = {0,0,0,0,0,0,0,0};

	unsigned long nNumArrays = 1;
	unsigned long ** aanArrays = calloc(nNumArrays, sizeof(*aanArrays));
	unsigned long * anArraySizes = calloc(nNumArrays, sizeof(*anArraySizes));
	
	FILE * fpInput = fopen(argv[1], "rb");
	unsigned long * pc = NULL;
	unsigned long nOp;
	unsigned long nRegisterA, nRegisterB, nRegisterC;
	unsigned long nTemp, nInstructionNumber;
	anArraySizes[0] = 0;
	aanArrays[0] = fileReadInToArray(fpInput, anArraySizes);
	//if (aanArrays[0]) pc = aanArrays[0];
	for (nInstructionNumber = 0; nInstructionNumber < anArraySizes[0]; )
	{
		pc = &(aanArrays[0][nInstructionNumber]);
		nOp = (*pc & OPCODE_MASK) >> 28;
		nRegisterA = (*pc & A_MASK) >> 6;
		nRegisterB = (*pc & B_MASK) >> 3;
		nRegisterC = (*pc & C_MASK);
		//printf("pc is %o, instruction %d is %d    %d %d %d\n", *pc, nInstructionNumber, nOp, nRegisterA, nRegisterB, nRegisterC);
		nInstructionNumber++;
		switch (nOp)
		{
			case CONDITIONALMOVE:
				if (registers[nRegisterC] != 0) registers[nRegisterA] = registers[nRegisterB];
				break;
			case ARRAYINDEX:
				registers[nRegisterA] = aanArrays[registers[nRegisterB]][registers[nRegisterC]];
				break;
			case ARRAYAMEND:
				aanArrays[registers[nRegisterA]][registers[nRegisterB]] = registers[nRegisterC];
				break;
			case ADD:
				registers[nRegisterA] = registers[nRegisterB] + registers[nRegisterC];
				/*will %2^32 happen automatically?*/
				break;
			case MULTIPLY:
				registers[nRegisterA] = registers[nRegisterB] * registers[nRegisterC];
				/*will %2^32 happen automatically?*/
				break;
			case DIVIDE:
				if (0 == (unsigned long) registers[nRegisterC]) 
				{
					printf("Error: divide by zero");
					return -1;
				}
				registers[nRegisterA] = (unsigned long) registers[nRegisterB] / (unsigned long) registers[nRegisterC];
				/*will %2^32 happen automatically? are they unsigned ints already?*/
				break;
			case NAND:
				registers[nRegisterA] = ~ (registers[nRegisterB] &  registers[nRegisterC]);
				//~registers[nRegisterB] |  ~registers[nRegisterC];
				break;
			case HALT:
				return 0; /*break not needed*/
			case ALLOC:
				aanArrays = realloc(aanArrays, (++nNumArrays));
				anArraySizes = realloc(anArraySizes, nNumArrays);
				aanArrays[nNumArrays] = calloc(registers[nRegisterC], sizeof(unsigned long));
				/*calloc will set to 0 on this architecture, at least*/
				anArraySizes[nNumArrays] = registers[nRegisterC];
				registers[nRegisterB] = nNumArrays;
				/*pc is pointing to an offset in aanArrays[0]. Has aanArrays[0] moved? If so, pc needs to be reassigned*/
				/*pc = aanArrays[0][nInstructionNumber];*/
				break;
			case FREE:
				free(aanArrays[registers[nRegisterC]]);
				aanArrays = realloc(aanArrays, (--nNumArrays));
				/*pc is pointing to an offset in aanArrays[0]. Has aanArrays[0] moved? If so, pc needs to be reassigned*/
				/*pc = aanArrays[0][nInstructionNumber];*/
				break;
			case OUTPUT:
				if (registers[nRegisterC] >= 0 && registers[nRegisterC] <= 255) putchar(registers[nRegisterC]);
				break;
			case INPUT:
				nTemp = getchar();
				if (nTemp >= 0 && nTemp <= 255) registers[nRegisterC] = nTemp;
				else if (nTemp == EOF) registers[nRegisterC] = ~0;
				/*todo error otherwise*/
				break;
			case LOADPROGRAM:
				if (registers[nRegisterB] != 0)
				{
					free(aanArrays[0]);
					aanArrays[0] = calloc(anArraySizes[registers[nRegisterB]], sizeof(*(aanArrays[0])));
					for (nTemp=0;nTemp<anArraySizes[registers[nRegisterB]];nTemp++)
					{
						aanArrays[0][nTemp] = aanArrays[registers[nRegisterB]][nTemp];
					}
				}
				nInstructionNumber = registers[nRegisterC];
				
				continue; /*in for loop */
				/* break;			not needed*/
			case ORTHOGRAPHY:
				nRegisterA = (*pc >> 25) & 7;
				registers[nRegisterA] = *pc & 33554431;
				//~ORTHOGRAPHY_A_MASK & ~OPCODE_MASK;
//				printf ("Load register %d with %d\n", nRegisterA, (*pc & 33554431));
				break;
			default:
				/*todo error*/
				break;			
		}
		
		
	}
	

}
