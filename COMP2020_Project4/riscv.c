#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "riscv.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int U_TYPE = 3;
const int UNKNOWN_TYPE = 4;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
static int get_op_type(char *op)
{
    const char *r_type_op[] = {"add", "sub", "and", "or", "xor","nor", "slt", "sll", "sra"};
    const char *i_type_op[] = {"addi", "andi", "ori", "xori", "slti"};
    const char *mem_type_op[] = {"lw", "lb", "sw", "sb"};
    const char *u_type_op[] = {"lui"};
    for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(r_type_op[i], op) == 0)
        {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(i_type_op[i], op) == 0)
        {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(mem_type_op[i], op) == 0)
        {
            return MEM_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(u_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(u_type_op[i], op) == 0)
        {
            return U_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
// TODO: create any additional variables to store the state of the interpreter
hashtable_t *mem;

void init(registers_t *starting_registers)
{
    registers = starting_registers;
    // TODO: initialize any additional variables needed for state
	mem = ht_init(1024);
}

// TODO: create any necessary helper functions
char * trim(char * s) {
    int l = strlen(s);

    while(isspace(s[l - 1])) --l;
    while(* s && isspace(* s)) ++s, --l;

    return strndup(s, l);
}

int get_num(char* str, int num_bit){
	int value = 0;
	if (strchr(str, 'x')!= NULL){
		value = strtol(str,NULL,0);
	}
	else{
		value = strtol(str,NULL,10);

	}
	if (value > ((1<<(num_bit - 1)) -1)){
			value = value - (1<<num_bit);
	}
	return value;
}

int sign(int value, int num_bit) {
	if (value > ((1<<(num_bit - 1)) -1)){
			value = value - (1<<num_bit);
	}
	return value;
} 

void step(char *instruction)
{
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE)
    {
        return;
    }

    // TODO: write logic for evaluating instruction on current interpreter state
	char* temp;
	

	if (op_type == R_TYPE){
		char* rd = trim(strsep(&instruction, ","));
		char* rs1 = trim(strsep(&instruction, ","));
		char* rs2 = trim(instruction);
		int rd_value = strtol(&rd[1],&temp,10);
		int rs1_value = strtol(&rs1[1],&temp,10);
		int rs2_value = strtol(&rs2[1],&temp,10);
		//printf("R-TYPE: op: %s, rd: %d, rs1: %d, rs2: %d\n", op, rd_value, rs1_value, rs2_value);
		if (strcmp(op,"add") == 0){
			registers->r[rd_value] = registers->r[rs1_value] + registers->r[rs2_value];
		}
		else if (strcmp(op,"sub") == 0){
			registers->r[rd_value] = registers->r[rs1_value] - registers->r[rs2_value];
		}
		else if (strcmp(op,"and") == 0){
			registers->r[rd_value] = registers->r[rs1_value] & registers->r[rs2_value];

		}
		else if (strcmp(op,"or") == 0){
			registers->r[rd_value] = registers->r[rs1_value] | registers->r[rs2_value];

		}
		else if (strcmp(op,"xor") == 0){
			registers->r[rd_value] = registers->r[rs1_value] ^ registers->r[rs2_value];

		}
		else if (strcmp(op,"nor") == 0){
			registers->r[rd_value] = !(registers->r[rs1_value] | registers->r[rs2_value]);

		}

		else if (strcmp(op,"slt") == 0){
			if (registers->r[rs1_value] < registers->r[rs2_value]){
				registers->r[rd_value] = 1;
			}
			else {
				registers->r[rd_value] = 0;
			}

		}
		else if (strcmp(op,"sll") == 0){
			registers->r[rd_value] = registers->r[rs1_value] << (registers->r[rs2_value] & 31);

		}
		else if (strcmp(op,"sra") == 0){
			registers->r[rd_value] = registers->r[rs1_value] >> (registers->r[rs2_value] & 31);
		}
	}
	if (op_type == I_TYPE){
		char* rd = trim(strsep(&instruction, ","));
		char* rs1 = trim(strsep(&instruction, ","));
		char* imm = trim(instruction);
		int rd_value = strtol(&rd[1],&temp,10);
		int rs1_value = strtol(&rs1[1],&temp,10);
		int imm_value = get_num(imm,12);
		//printf("I-TYPE: op: %s, rd: %d, rs1: %d, imm: %d\n", op, rd_value, rs1_value, imm_value);
		if (strcmp(op,"addi") == 0){
			registers->r[rd_value] = registers->r[rs1_value] + imm_value;
		}
		if (strcmp(op,"andi") == 0){
			registers->r[rd_value] = registers->r[rs1_value] & imm_value;
		}
		if (strcmp(op,"ori") == 0){
			registers->r[rd_value] = registers->r[rs1_value] | imm_value;
		}
		if (strcmp(op,"xori") == 0){
			registers->r[rd_value] = registers->r[rs1_value] ^ imm_value;
		}
		if (strcmp(op,"slti") == 0){
			if (registers->r[rs1_value] < imm_value){
					registers->r[rd_value] = 1;
			}
			else {
				registers->r[rd_value] = 0;
			}
		}

	}
	if (op_type == U_TYPE){
		char* rd = trim(strsep(&instruction, ","));
		char* imm = trim(instruction);
		int rd_value = strtol(&rd[1],&temp,10);
		int imm_value = get_num(imm,20);
		//printf("U-TYPE: op: %s, rd: %d, imm: %d\n", op, rd_value, imm_value);
		if (strcmp(op, "lui") == 0){
			registers->r[rd_value] = imm_value << 12;
		}
	}
	if (op_type == MEM_TYPE){
		char *rd = trim(strsep(&instruction, ","));
		char *imm = trim(strsep(&instruction,"("));
		char *rs1 = trim(strsep(&instruction,")"));
		int rd_value = strtol(&rd[1],&temp,10);
		int imm_value = get_num(imm,12);
		int rs1_value = strtol(&rs1[1],&temp,10);
		//printf("MEM-TYPE: op: %s, rd: %d, imm: %d, rs1: %d\n", op, rd_value, imm_value, rs1_value);
		if (strcmp(op,"lw") == 0){
			int sum = ht_get(mem, registers->r[rs1_value] + imm_value) & 255;
			sum += (ht_get(mem, registers->r[rs1_value] + imm_value +1) & 255) << 8;
			sum += (ht_get(mem, registers->r[rs1_value] + imm_value +2) & 255) << 16;
			sum += (ht_get(mem, registers->r[rs1_value] + imm_value +3) & 255) << 24;
			registers->r[rd_value] = sum;
		}
		if (strcmp(op,"lb") == 0){
			registers->r[rd_value] = ht_get(mem, registers->r[rs1_value] + imm_value);
			
		}
		if (strcmp(op,"sw") == 0){
			ht_add(mem, registers->r[rs1_value] + imm_value, sign(registers->r[rd_value] & 255, 8));
			ht_add(mem, registers->r[rs1_value] + imm_value + 1, sign((registers->r[rd_value] >> 8) & 255,8));
			ht_add(mem, registers->r[rs1_value] + imm_value + 2, sign((registers->r[rd_value] >> 16) & 255,8));
			ht_add(mem, registers->r[rs1_value] + imm_value + 3, sign((registers->r[rd_value] >> 24) & 255,8));
			
		}
		if (strcmp(op,"sb") == 0){
			ht_add(mem, registers->r[rs1_value] + imm_value, sign(registers->r[rd_value] & 255,8));
		}
	}

}
