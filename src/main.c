#include <insns.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regs.h>
// GENERAL TODOS
// TODO: add implicit .text default

// TODO: memory addressing
// TODO: add all operand types
typedef struct reg {
  int val;
  enum reg_enum reg;
} reg;

typedef union operand {
  int constant;
  char *reg;
} operand;

typedef struct instruction {
  char *name;
  size_t numOperands;
  operand *operands;
} instruction;

typedef struct section {
  char *name;
  size_t instructionsLen;
  instruction *instructions;
} section;

struct AST {
  size_t sectionsLen;
  size_t sectionsSize;
  section *sections;
};

void readFile(char *filename) {
  FILE *fptr;

  fptr = fopen(filename, "rb");

  struct AST ast;
  ast.sections = malloc(sizeof(section));
  ast.sectionsSize = 1;
  ast.sectionsLen = 0;

  char *buf = NULL;
  size_t bufSize;

  size_t currentSection = -1; // Index into AST

  while (getline(&buf, &bufSize, fptr) != -1) {
    // TODO: comments
    // Read section
    if (strncmp(buf, "section", strlen("section")) == 0) {
      // TODO: check if section name already exists
      if (ast.sectionsLen + 1 == ast.sectionsSize) {
        ast.sectionsSize *= 2;
        ast.sections =
            realloc(ast.sections, ast.sectionsSize * sizeof(section));
      }

      currentSection++;
      ast.sectionsLen++;

      // TODO: eat whitespace properly
      char *name = buf + strlen("section ");
      // Remove trailing newline if applicable
      size_t nameLen = strlen(name);
      if (name[nameLen - 1] == '\n') {
        name[nameLen - 1] = 0;
        nameLen--;
      }

      // Copy name from getline buffer to section struct
      ast.sections[currentSection].name = malloc(nameLen);
      strcpy(ast.sections[currentSection].name, name);

    } else { // Read instruction
      printf("non-section: %s\n", buf);
      // TODO: everything
    }
  }

  // Print the data to make sure it was read correctly
  printf("num sections %d\n", ast.sectionsLen);
  for (int i = 0; i < ast.sectionsLen; i++) {
    printf("name: %s\n", ast.sections[i].name);
  }
}

int numFound;

void get_instruction_enum(char *name) {
  size_t enumSize = 1976; // TODO: remove magic number
  for (int i = 0; i < enumSize; i++) {
    if (strcmp(nasm_insn_names[i], name) == 0) {
      numFound++;
      return;
    }
  }
}

void get_register(char* name) {
  for (size_t i = 0; i<EXPR_REG_END; i++) {
    if (strcmp(nasm_reg_names[i], name) == 0) {
      numFound++;
      return;
    }
  }
}

int main(int argc, char **argv) {
  for (int i = 0; i < 25000; i++) {
    get_instruction_enum("wrssd");
    get_instruction_enum("encls");
    get_instruction_enum("wrssq");
    get_register("rax");
    
  }
  printf("numfound: %d\n", numFound);
}
