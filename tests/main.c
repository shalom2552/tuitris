#define TDRAW_IMPL
#include "tdraw.h"
#include <stdio.h>
#include <unistd.h>

void test_board(void);
void test_state(void);

#define TEST(fn) test(fn, #fn)

static void test(void (*test_suite)(void), const char* name)
{
    fprintf(stderr, "Running %s...", name);
    fflush(stdout);
    test_suite();
    fprintf(stderr, "[OK]\n");
}

int main(void)
{
    fprintf(stderr, "Tuitris unit tests\n");
    fprintf(stderr, "=================================================\n");

    TEST(test_board);
    TEST(test_state);

    fprintf(stderr, "\nAll tests passed!\n");
    return 0;
}

