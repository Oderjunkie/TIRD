#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include <readline/readline.h>
#include <readline/history.h>

int main(void) {
  printf("TIRD repl v0.0, otesunki 2023\n");
  using_history();
  for (;;) {
    static char *src;
    src = readline("\x1b[31m>\x1b[0m ");
    if (!src)
      return EXIT_FAILURE;
    add_history(src);
    interpret(src);
    free(src);
  }
}
