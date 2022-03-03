#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "node.h"
#include "stack.h"

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->top = 0;
    s->capacity = capacity;
    s->items = (Node **) malloc(sizeof(Node *) * capacity);
    return s;
}

void stack_delete(Stack **s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
}

bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    } else {
        return false;
    }
}

bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    } else {
        return false;
    }
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[stack_size(s)] = n; //insert Node n at the top of the stack
    s->top += 1;
    return true;
}

bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    *n = s->items[stack_size(s)
                  - 1]; //Node n = top-1 index in stack (top points to 1 above top array index)
    s->top -= 1;
    return true;
}

void stack_print(Stack *s) {
    for (uint32_t i = 0; i < stack_size(s); i += 1) {
        printf("-%c", s->items[i]->symbol);
        printf(" %" PRIu64 " ", s->items[i]->frequency);
    }
    printf("\n");
}
