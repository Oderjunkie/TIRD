#include "todo.h"
#include "interpreter.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

void *zalloc(size_t bytes) {
  return memset(malloc(bytes), 0, bytes);
}
#define Xvalue(X) \
  X(double, DOUBLE) \
  X(char *, STRING)
#define AS_ENUM(T, name) , name
#define AS_DECL(T, name) T name;

struct value {
  enum { UNINIT Xvalue(AS_ENUM) } type;
  union { Xvalue(AS_DECL) } as;
};

struct btree {
  struct value data;
  struct btree *left, *right, *up;
};

void free_data(struct btree *tree) {
  if (tree->data.type == STRING)
    free(tree->data.as.STRING),
    tree->data.as.STRING = (char *) 0;
}

void clone_value(struct value *dst, const struct value *src) {
  if (src->type == STRING) {
    dst->type = src->type;
    dst->as.STRING = malloc(strlen(src->as.STRING) + 1);
    memcpy(dst->as.STRING, src->as.STRING, strlen(src->as.STRING) + 1);
  } else {
    memcpy(dst, src, sizeof(struct value));
  }
}

void free_tree(struct btree *tree) {
  if (tree->left)
    free_tree(tree->left),
    tree->left = (struct btree *) 0;
  if (tree->right)
    free_tree(tree->right),
    tree->right = (struct btree *) 0;
  free_data(tree);
  free(tree);
}

struct btree *clone_tree(const struct btree *tree) {
  if (!tree)
    return (struct btree *) 0;
  struct btree *clone = zalloc(sizeof(struct btree));
  clone_value(&clone->data, &tree->data);
  clone->left = clone_tree(tree->left);
  clone->right = clone_tree(tree->right);
  clone->up = (struct btree *) 0;
  return clone;
}

void print_value(const struct value *val) {
  switch (val->type) {
    case UNINIT: fatal("tried to print an uninitialized value");
    case DOUBLE: printf("\x1b[36m%g\x1b[0m", val->as.DOUBLE); break;
    case STRING: printf("%s", val->as.STRING); break;
  }
}

unsigned int boolify_value(const struct value *val) {
  switch (val->type) {
    case UNINIT: return 0;
    case DOUBLE: return val->as.DOUBLE != 0.0 ? 1 : 0;
    case STRING: return *val->as.STRING != '\0' ? 1 : 0;
    default: fatal("boolified a non-value?");
  }
}

void iwt(const char *src, struct btree *tp) {
  const char *ip = src;
  while (*ip != '\0')
    switch (*ip) {
      case ')':
        ip++;
        if (!tp->right)
          tp->right = zalloc(sizeof(*tp->right)), tp->right->up = tp;
        if (*ip == ':')
          ip++, clone_value(&tp->data, &tp->right->data);
        else
          tp = tp->right;
        break;
      case '(':
        ip++;
        if (!tp->left)
          tp->left = zalloc(sizeof(*tp->left)), tp->left->up = tp;
        if (*ip == ':')
          ip++, clone_value(&tp->data, &tp->left->data);
        else
          tp = tp->left;
        break;
      case '^':
        ip++;
        if (tp->up) {
          if (*ip == ':')
            ip++, clone_value(&tp->data, &tp->up->data);
          else
            tp = tp->up;
        }
        break;
      case '.':
        ip++;
        print_value(&tp->data), printf("\n");
        break;
      case ',': {
        tp->data.type = STRING;
        tp->data.as.STRING = readline("");
        ip++;
        break;
      }
      case '+': {
        ip++;
        if (!tp->left && !tp->right)
          fatal("missing arguments to +");
        if (!tp->left)
          tp->left = malloc(sizeof(*tp->left)),
          tp->left->data = tp->data,
          tp->left->left = (struct btree *) 0,
          tp->left->right = (struct btree *) 0;
        if (!tp->right)
          tp->right = malloc(sizeof(*tp->right)),
          tp->right->data = tp->data,
          tp->right->left = (struct btree *) 0,
          tp->right->right = (struct btree *) 0;
        if (tp->left->data.type != DOUBLE || tp->right->data.type != DOUBLE)
          fatal("bad arguments to +");
        free_data(tp);
        tp->data.as.DOUBLE = tp->left->data.as.DOUBLE + tp->right->data.as.DOUBLE;
        tp->data.type = DOUBLE;
        free_tree(tp->left), tp->left = (struct btree *) 0;
        free_tree(tp->right), tp->right = (struct btree *) 0;
        break;
      }
      case '-': {
        ip++;
        if (!tp->left && !tp->right)
          fatal("missing arguments to -");
        if (!tp->left)
          tp->left = malloc(sizeof(*tp->left)),
          tp->left->data = tp->data,
          tp->left->left = (struct btree *) 0,
          tp->left->right = (struct btree *) 0;
        if (!tp->right)
          tp->right = malloc(sizeof(*tp->right)),
          tp->right->data = tp->data,
          tp->right->left = (struct btree *) 0,
          tp->right->right = (struct btree *) 0;
        if (tp->left->data.type != DOUBLE || tp->right->data.type != DOUBLE)
          fatal("bad arguments to -");
        free_data(tp);
        tp->data.as.DOUBLE = tp->left->data.as.DOUBLE - tp->right->data.as.DOUBLE;
        tp->data.type = DOUBLE;
        free_tree(tp->left), tp->left = (struct btree *) 0;
        free_tree(tp->right), tp->right = (struct btree *) 0;
        break;
      }
      case '%': {
        ip++;
        if (!tp->left && !tp->right)
          fatal("missing arguments to %%");
        if (!tp->left)
          tp->left = malloc(sizeof(*tp->left)),
          tp->left->data = tp->data,
          tp->left->left = (struct btree *) 0,
          tp->left->right = (struct btree *) 0;
        if (!tp->right)
          tp->right = malloc(sizeof(*tp->right)),
          tp->right->data = tp->data,
          tp->right->left = (struct btree *) 0,
          tp->right->right = (struct btree *) 0;
        if (tp->left->data.type != DOUBLE || tp->right->data.type != DOUBLE)
          fatal("bad arguments to %%");
        free_data(tp);
        tp->data.as.DOUBLE = tp->left->data.as.DOUBLE;
        while (tp->data.as.DOUBLE >= tp->right->data.as.DOUBLE)
          tp->data.as.DOUBLE -= tp->right->data.as.DOUBLE;
        while (tp->data.as.DOUBLE < 0)
          tp->data.as.DOUBLE += tp->right->data.as.DOUBLE;
        tp->data.type = DOUBLE;
        free_tree(tp->left), tp->left = (struct btree *) 0;
        free_tree(tp->right), tp->right = (struct btree *) 0;
        break;
      }
      case '=': {
        ip++;
        if (!tp->left && !tp->right)
          fatal("missing arguments to =");
        if (!tp->left)
          tp->left = malloc(sizeof(*tp->left)),
          tp->left->data = tp->data,
          tp->left->left = (struct btree *) 0,
          tp->left->right = (struct btree *) 0;
        if (!tp->right)
          tp->right = malloc(sizeof(*tp->right)),
          tp->right->data = tp->data,
          tp->right->left = (struct btree *) 0,
          tp->right->right = (struct btree *) 0;
        if (tp->left->data.type != tp->right->data.type)
          fatal("bad arguments to =");
        free_data(tp);
        switch (tp->left->data.type) {
          case UNINIT: tp->data.type = UNINIT; break;
          case DOUBLE:
            tp->data.as.DOUBLE = tp->left->data.as.DOUBLE == tp->right->data.as.DOUBLE ? 1.0 : 0.0;
            tp->data.type = DOUBLE;
            break;
          case STRING:
            tp->data.as.DOUBLE = strcmp(tp->left->data.as.STRING, tp->right->data.as.STRING) == 0 ? 1.0 : 0.0;
            tp->data.type = DOUBLE;
            break;
        }
        free_tree(tp->left), tp->left = (struct btree *) 0;
        free_tree(tp->right), tp->right = (struct btree *) 0;
        break;
      }
      case '<': {
        ip++;
        if (!tp->left && !tp->right)
          fatal("missing arguments to <");
        if (!tp->left)
          tp->left = malloc(sizeof(*tp->left)),
          tp->left->data = tp->data,
          tp->left->left = (struct btree *) 0,
          tp->left->right = (struct btree *) 0;
        if (!tp->right)
          tp->right = malloc(sizeof(*tp->right)),
          tp->right->data = tp->data,
          tp->right->left = (struct btree *) 0,
          tp->right->right = (struct btree *) 0;
        if (tp->left->data.type != tp->right->data.type)
          fatal("bad arguments to <");
        free_data(tp);
        switch (tp->left->data.type) {
          case UNINIT: tp->data.type = UNINIT; break;
          case DOUBLE:
            tp->data.as.DOUBLE = tp->left->data.as.DOUBLE < tp->right->data.as.DOUBLE ? 1.0 : 0.0;
            tp->data.type = DOUBLE;
            break;
          case STRING:
            tp->data.as.DOUBLE = strcmp(tp->left->data.as.STRING, tp->right->data.as.STRING) < 0 ? 1.0 : 0.0;
            tp->data.type = DOUBLE;
            break;
        }
        free_tree(tp->left), tp->left = (struct btree *) 0;
        free_tree(tp->right), tp->right = (struct btree *) 0;
        break;
      }
      case '>': {
        ip++;
        if (!tp->left && !tp->right)
          fatal("missing arguments to >");
        if (!tp->left)
          tp->left = malloc(sizeof(*tp->left)),
          tp->left->data = tp->data,
          tp->left->left = (struct btree *) 0,
          tp->left->right = (struct btree *) 0;
        if (!tp->right)
          tp->right = malloc(sizeof(*tp->right)),
          tp->right->data = tp->data,
          tp->right->left = (struct btree *) 0,
          tp->right->right = (struct btree *) 0;
        if (tp->left->data.type != tp->right->data.type)
          fatal("bad arguments to >");
        free_data(tp);
        switch (tp->left->data.type) {
          case UNINIT: tp->data.type = UNINIT; break;
          case DOUBLE:
            tp->data.as.DOUBLE = tp->left->data.as.DOUBLE > tp->right->data.as.DOUBLE ? 1.0 : 0.0;
            tp->data.type = DOUBLE;
            break;
          case STRING:
            tp->data.as.DOUBLE = strcmp(tp->left->data.as.STRING, tp->right->data.as.STRING) > 0 ? 1.0 : 0.0;
            tp->data.type = DOUBLE;
            break;
        }
        free_tree(tp->left), tp->left = (struct btree *) 0;
        free_tree(tp->right), tp->right = (struct btree *) 0;
        break;
      }
      case '#': {
        ip++;
        if (tp->data.type == STRING) {
          double f;
          f = atof(tp->data.as.STRING);
          free(tp->data.as.STRING);
          tp->data.as.STRING = (char *) 0;
          tp->data.as.DOUBLE = f;
          tp->data.type = DOUBLE;
        }
        break;
      }
      case '!': {
        ip++;
        tp->data.as.DOUBLE = !boolify_value(&tp->data);
        tp->data.type = DOUBLE;
        break;
      }
      case '"': {
        static const char *start;
        static char *str;
        
        start = ++ip;
        
        while (*ip != '\0' && *ip != '\"')
          ++ip;
        
        if (*ip == '\0')
          fatal("unterminated string literal");
        str = zalloc(ip - start);
        memcpy(str, start, ip - start);
        tp->data.type = STRING;
        tp->data.as.STRING = str;
        
        ++ip;
        break;
      }
      case '[': {
        struct btree *ntp;
        const char *cond, *body;
        unsigned int nest;
        ++ip;
        cond = ip;
        nest = 1;
        while (!(*ip == '|' && nest == 1) && *ip != '\0') {
          if (*ip == '[' || *ip == '{')
            nest++;
          if (*ip == ']' || *ip == '}')
            nest--;
          ++ip;
        }
        if (*ip == '\0')
          fatal("unterminated [..., expected | somewhere");
        body = ++ip;
        nest = 1;
        while (nest >= 1 && *ip != '\0') {
          if (*ip == '[' || *ip == '{')
            nest++;
          if (*ip == ']' || *ip == '}')
            nest--;
          ++ip;
        }
        if (nest != 0)
          fatal("unterminated [...|..., expected ] somewhere");
        ntp = clone_tree(tp);
        iwt(cond, ntp);
        if (boolify_value(&ntp->data))
          iwt(body, tp);
        free_tree(ntp);
        break;
      }
      case '{': {
        struct btree *ntp;
        const char *cond, *body;
        unsigned int nest;
        ++ip;
        cond = ip;
        nest = 1;
        while (!(*ip == '|' && nest == 1) && *ip != '\0') {
          if (*ip == '[' || *ip == '{')
            nest++;
          if (*ip == ']' || *ip == '}')
            nest--;
          ++ip;
        }
        if (*ip == '\0')
          fatal("unterminated {..., expected | somewhere");
        body = ++ip;
        nest = 1;
        while (nest >= 1 && *ip != '\0') {
          if (*ip == '[' || *ip == '{')
            nest++;
          if (*ip == ']' || *ip == '}')
            nest--;
          ++ip;
        }
        if (nest != 0)
          fatal("unterminated {...|..., expected } somewhere");
        for (;;) {
          ntp = clone_tree(tp);
          iwt(cond, ntp);
          if (!boolify_value(&ntp->data))
            break;
          iwt(body, tp);
          free_tree(ntp);
        }
        break;
      }
      case '|': case ']': case '}': return;
      case ' ': case '\r': case '\n': case '\t':
        ++ip;
        break;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9': {
        double f = atof(ip);
        while ((isdigit(*ip) || *ip == '.') && *ip != '\0')
          ip++;
        tp->data.type = DOUBLE;
        tp->data.as.DOUBLE = f;
        break;
      }
      default:
        fatal("invalid command");
    }
}


void interpret(const char *src) {
  struct btree *tp = zalloc(sizeof(*tp));
  iwt(src, tp);
  free_tree(tp);
}
