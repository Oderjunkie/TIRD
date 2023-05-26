#ifndef TODO_H
#define TODO_H
  #include <stdio.h>
  #include <stdlib.h>
  #define STRINGIFY(n) STRINGIZE(n)
  #define STRINGIZE(n) #n
  #define todo(str) ( \
    fprintf(stderr, __FILE__ ":" STRINGIFY(__LINE__) ": not implemented: " str "\n"), \
    exit(EXIT_FAILURE), \
    (void) 0 \
  )
  #define fatal(str) ( \
    fprintf(stderr, __FILE__ ":" STRINGIFY(__LINE__) ": fatal error: " str "\n"), \
    exit(EXIT_FAILURE), \
    (void) 0 \
  )
#endif
