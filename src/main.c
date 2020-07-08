#include <ctype.h>
#include <insns.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regs.h>
// GENERAL TODOS
// TODO: add implicit .text default

// TODO: memory addressing
// TODO: add all operand types
// TODO: change everything to strok_r, because this will eventually be a library

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

// Trims whitespace on both ends of string
char *trimWhitespace(char *str) {
  while (str[0] != '\0' && str[0] == ' ')
    str++;
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == ' ') {
      str[i] = '\0';
      return str;
    }
  }
  return str;
}

enum reg_enum get_reg(char *name) {
  for (size_t i = 0; i < EXPR_REG_END; i++) {
    if (strcmp(nasm_reg_names[i], name) == 0) {
      return i;
    }
  }
  return R_none;
}

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
  size_t nread;

  while ((nread = getline(&buf, &bufSize, fptr)) != EOF) {
    // Remove trailing newline (if applicable)
    if (buf[nread - 1] == '\n') {
      buf[nread - 1] = 0;
    }
    printf("\nread line: '%s'\n", buf);

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

      // Copy name from getline buffer to section struct
      ast.sections[currentSection].name = malloc(strlen(name));
      strcpy(ast.sections[currentSection].name, name);

    } else { // Read instruction
      // Remove any characters after ';' (comment start character)
      if (buf[0] == ';')
        continue;
      // We use a new variable called line, because we cannot modify buf because
      // it is dynamically allocated
      char *line = strtok(buf, ";");
      printf("comment removal: '%s'\n", buf);

      // TODO: eat whitespace before reading instruction name
      // Read instruction name
      char *inst = strtok(line, " ");
      if (inst == NULL)
        continue; // No instruction was found, continue
      printf("instruction: '%s'\n", inst);

      char *arg;
      while ((arg = strtok(NULL, ",")) != NULL) {
        arg = trimWhitespace(arg);

        // detect if number
        char *num = arg;
        if (num[0] == '-')
          num++;
        while (isdigit(num[0]) && num[0] != '\0')
          num++;
        if (num[0] == '\0') {
          printf("number: '%s'\n", arg);
          continue;
        }

        // detect if register
        enum reg_enum reg = get_reg(arg);
        if (reg != R_none) {
          printf("register: '%s'\n", arg);
          continue;
        }

        // default case
        printf("unknown argument: '%s'\n", arg);
      }
    }
  }

  // Print the data to make sure it was read correctly
  printf("\nnum sections %d\n", ast.sectionsLen);
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

int main(int argc, char **argv) { readFile(argv[1]); }
