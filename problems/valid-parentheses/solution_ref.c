// Time: O(n)  — single-pass stack
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool isValid(char* s) {
    int n = strlen(s);
    char* st = malloc(n + 1);
    int top = 0;
    for (int i = 0; s[i]; i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            st[top++] = c;
        } else {
            if (top == 0) { free(st); return false; }
            char o = st[--top];
            if ((c == ')' && o != '(') ||
                (c == ']' && o != '[') ||
                (c == '}' && o != '{')) {
                free(st);
                return false;
            }
        }
    }
    bool ok = (top == 0);
    free(st);
    return ok;
}
