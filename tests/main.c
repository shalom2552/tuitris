#include <stdio.h>
#include <unistd.h>

void test_board(void);
void test_state(void);

#define TEST(fn) test(fn, #fn)

static void test(void (*test_suite)(void), const char* name)
{
    printf("Running %s...", name);
    fflush(stdout);
    test_suite();
    printf("[OK]\n");
}

int main(void)
{
    printf("Tuitris unit tests\n");
    printf("=================================================\n");

    TEST(test_board);
    TEST(test_state);

    printf("\nAll tests passed!\n");
    return 0;
}

