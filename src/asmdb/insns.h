#include <nasm.h>

extern const uint8_t nasm_bytecodes[];

/* if changed, ITEMPLATE_END should be also changed accordingly */
struct itemplate {
    enum opcode     opcode;             /* the token, passed from "parser.c" */
    int             operands;           /* number of operands */
    opflags_t       opd[MAX_OPERANDS];  /* bit flags for operand types */
    decoflags_t     deco[MAX_OPERANDS]; /* bit flags for operand decorators */
    const uint8_t   *code;              /* the code it assembles to */
    uint32_t        iflag_idx;          /* some flags referenced by index */
};

extern const struct itemplate instrux[];
extern const struct itemplate * const nasm_instructions[];

/* Disassembler table structure */

/*
 *  * If n == -1, then p points to another table of 256
 *   * struct disasm_index, otherwise p points to a list of n
 *    * struct itemplates to consider.
 *     */
struct disasm_index {
	    const void *p;
	        int n;
};

#define ITEMPLATE_END {I_none,0,{0,},{0,},NULL,0}
