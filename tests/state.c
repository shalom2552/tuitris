#define _XOPEN_SOURCE 700
#include "state.h"

#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

// === HELPERS ================================================================

static char old_cwd[4096];
static char tmpdir[] = "/tmp/tuitris_testXXXXXX";

static void sandbox_enter(void) {
    assert(getcwd(old_cwd, sizeof old_cwd));
    assert(mkdtemp(tmpdir));
    assert(chdir(tmpdir) == 0);
}

static void sandbox_exit(void) {
    assert(chdir(old_cwd) == 0);
}

// === TESTS ================================================================
static void test_state_init(void)
{
    state_init();
    assert(state_level() == 1);
    assert(state_score() == 0);
    assert(state_lines() == 0);
}

static void test_state_score(void)
{
    state_init();
    assert(state_score() == 0);
    state_add_score(100);
    assert(state_score() == 100);
    state_add_score(200);
    assert(state_score() == 300);
}

static void test_state_lines(void)
{
    state_init();
    assert(state_lines() == 0);
    state_add_lines(1);
    assert(state_lines() == 1);
    state_add_lines(2);
    assert(state_lines() == 3);
}

static void test_state_high_score(void)
{
    sandbox_enter();
    state_init();
    assert(state_high_score() == 0);
    state_add_score(100);
    assert(state_score() == 100);
    assert(state_high_score() == 0);
    state_save_high_score();
    assert(state_high_score() == 100);
    state_init();
    assert(state_high_score() == 100);
    assert(state_score() == 0);
    sandbox_exit();
}

// === Main ===================================================================
void test_state(void)
{
    test_state_init();
    test_state_score();
    test_state_lines();
    test_state_high_score();
}

