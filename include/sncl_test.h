#ifndef _TESTFRAMEWORK_H__
#define _TESTFRAMEWORK_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef int (*test_entry_t)(void);
typedef void (*test_void_t)(void);

typedef struct test_case {
    test_entry_t entry;
    const char *name;
    struct test_case *next;
} test_case_t;

extern test_case_t *all_tests;
extern test_void_t before_eachfn;
extern test_void_t before_allfn;

extern size_t num_tests;
extern char *test_err_msg;

static inline void add_test(test_case_t *t) {
    num_tests++;
    if (all_tests == NULL)
        all_tests = t;
    else {
        t->next = all_tests;
        all_tests = t;
    }
}

#define BEFORE_ALL()                                                                                                   \
    static void before_all(void);                                                                                      \
    static void test__register_before_all(void) __attribute__((constructor));                                          \
    static void test__register_before_all(void) { before_allfn = before_all; }                                         \
    static void before_all(void)

#define BEFORE_EACH()                                                                                                  \
    static void before_each(void);                                                                                     \
    static void test__register_before_each(void) __attribute__((constructor));                                         \
    static void test__register_before_each(void) { before_eachfn = before_each; }                                      \
    static void before_each(void)

#define TEST_CASE(name)                                                                                                \
    static int name(void);                                                                                             \
    static test_case_t test__test_##name = { name, #name, NULL };                                                      \
    static void test__register_##name(void) __attribute__((constructor));                                              \
    static void test__register_##name(void) { add_test(&test__test_##name); }                                          \
    static int name(void)

#define STR(x) #x

#define ASSERT_FAIL(__CODE, ...)                                                                                       \
    do {                                                                                                               \
        test_err_msg = NULL;                                                                                           \
        asprintf(&test_err_msg, __VA_ARGS__);                                                                          \
        asprintf(&test_err_msg, "%s:%d: %s", __FILE__, __LINE__, test_err_msg);                                        \
        return __CODE;                                                                                                 \
    } while (0)

#define ASSERT_EQUAL(a, b)                                                                                             \
    do {                                                                                                               \
        if ((a) != (b))                                                                                                \
            ASSERT_FAIL(-1, "`%s' did not equal `%s'", STR(a), STR(b));                                                \
    } while (0)

#define ASSERT_STREQUAL(a, b)                                                                                          \
    do {                                                                                                               \
        if (strcmp((a), (b)) != 0)                                                                                     \
            ASSERT_FAIL(-1, "`%s' (%s) did not equal `%s' (%s)", STR(a), a, STR(b), b);                                \
    } while (0)

#define ASSERT_NOTEQUAL(a, b)                                                                                          \
    do {                                                                                                               \
        if ((a) == (b))                                                                                                \
            ASSERT_FAIL(-1, "`%s' did not equal `%s'", STR(a), STR(b));                                                \
    } while (0)

#define ASSERT_EQUALFMT(a, b, fmt)                                                                                     \
    if ((a) != (b))                                                                                                    \
        ASSERT_FAIL(-1, fmt, a, b);

#define ASSERT_TRUE(a)                                                                                                 \
    do {                                                                                                               \
        if (!(a))                                                                                                      \
            ASSERT_FAIL(-2, "`%s' was not true", STR(a));                                                              \
    } while (0)

#define ASSERT_FALSE(a)                                                                                                \
    do {                                                                                                               \
        if (a)                                                                                                         \
            ASSERT_FAIL(-3, "`%s' was not false", STR(a));                                                             \
    } while (0)

#define STASH_STREAM(stream)                                                                                           \
    FILE *old_##stream = stream;                                                                                       \
    fflush(stream);                                                                                                    \
    stream = freopen("/dev/null", "w", stream)

#define RESTORE_STREAM(stream)                                                                                         \
    fflush(stream);                                                                                                    \
    stream = old_##stream

#endif // _TESTFRAMEWORK_H__
