#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "todo.h"
#include <readline/readline.h>
#include <readline/history.h>


int main(int argc, char *argv[]) {
  static char *buf;
  if (argc > 1) {
    static int bufsz;
    for (int argi = 1; argi < argc; argi++) {
      static FILE *f;
      static char *bufp;
      if (!(f = fopen(argv[argi], "r")))
        fatal("file not found.");
      bufsz = 64;
      buf = malloc(bufsz);
      bufp = buf;
      while (fread(bufp, 1, 64, f) == 64)
        buf = realloc(buf, bufsz += 64), bufp += 64;
      if (ferror(f))
        fatal("file reading error");
      buf = realloc(buf, strlen(buf));
      fclose(f);
      interpret(buf);
      free(buf);
    }
  } else {
    printf("TIRD repl v1.0, otesunki 2023\n");
    using_history();
    for (;;) {
      buf = readline("\x1b[31m>\x1b[0m ");
      if (!buf)
        return EXIT_FAILURE;
      add_history(buf);
      interpret(buf);
      free(buf);
    }
  }
}
