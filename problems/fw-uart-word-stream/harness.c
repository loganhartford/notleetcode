#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "nlc.h"
#include "solution.c"

/* --- self-contained stdout capture (nlc.h has no capture helpers) ---------- */
static int _saved_fd = -1;

static void cap_begin(void) {
    fflush(stdout);
    _saved_fd = dup(STDOUT_FILENO);
    int fd = open("nlc_cap.tmp", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

static char *cap_end(void) {
    fflush(stdout);
    dup2(_saved_fd, STDOUT_FILENO);
    close(_saved_fd);
    FILE *f = fopen("nlc_cap.tmp", "rb");
    if (!f) { char *e = calloc(1, 1); return e; }
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc((size_t)n + 1);
    size_t got = fread(buf, 1, (size_t)n, f);
    buf[got] = '\0';
    fclose(f);
    return buf;
}

/* Expected console output: every space becomes a newline. */
static char *expected_of(const char *s) {
    size_t len = strlen(s);
    char *e = malloc(len + 1);
    for (size_t i = 0; i < len; i++) e[i] = (s[i] == ' ') ? '\n' : s[i];
    e[len] = '\0';
    return e;
}

/* Feed s one byte at a time. `pattern` cycles through gap sizes = how many ISR
 * calls happen before each main_loop (each in [1,3], honoring the guarantee). */
static char *drive(const char *s, const int *pattern, int plen) {
    uart_init();
    cap_begin();
    int since = 0, pi = 0, gap = pattern[0];
    for (size_t i = 0; s[i]; i++) {
        UART_RX_REGISTER = s[i];
        uart_receive_isr();
        if (++since >= gap) {
            main_loop();
            since = 0;
            pi = (pi + 1) % plen;
            gap = pattern[pi];
        }
    }
    main_loop();               /* the forever-loop eventually drains the rest */
    return cap_end();
}

static void run(const char *label, const char *s, const int *pat, int plen) {
    char *got = drive(s, pat, plen);
    char *exp = expected_of(s);
    int ok = strcmp(got, exp) == 0;
    char input[160];
    snprintf(input, sizeof input, "feed \"%s\"", s);
    nlc_case(label, ok, input, exp, got, "");
    free(got);
    free(exp);
}

int main(void) {
    nlc_begin();

    static const int gap1[]  = {1};          /* main_loop after every byte      */
    static const int gap3[]  = {3};          /* main_loop after every 3 bytes   */
    static const int mixA[]  = {1, 2, 3};    /* varying 1..3                    */
    static const int mixB[]  = {3, 1, 2};    /* varying, different phase        */
    static const int mixC[]  = {2, 3, 1, 3}; /* varying                        */

    const char *example = "Here we are ";

    /* The example, under every interleaving — output must be identical. */
    run("example, main_loop every byte",   example, gap1, 1);
    run("example, main_loop every 3 bytes", example, gap3, 1);
    run("example, varying 1..3 (A)",       example, mixA, 3);
    run("example, varying 1..3 (B)",       example, mixB, 3);
    run("example, varying 1..3 (C)",       example, mixC, 4);

    /* No trailing space (last word has no newline after it). */
    run("no trailing space, every byte",   "Here we are", gap1, 1);
    run("no trailing space, every 3",      "Here we are", gap3, 1);

    /* Single short word, and a lone byte. */
    run("single word",                     "hi ",  gap3, 1);
    run("one byte",                        "x",    gap1, 1);

    /* Word longer than MAX_WORD_SIZE and longer than the ring: must still stream. */
    run("long token streams (every 3)",    "abcdefghij ", gap3, 1);
    run("long word in a sentence",         "Streaming words work ", mixA, 3);

    /* Consecutive spaces -> blank line between them. */
    run("double space -> blank line",      "a  b ", mixB, 3);

    /* Long sentence: forces many wraps around the ring buffer. */
    run("long sentence, every 3",          "the quick brown fox jumps ", gap3, 1);
    run("long sentence, varying",          "the quick brown fox jumps ", mixC, 4);

    nlc_end();
    return 0;
}
