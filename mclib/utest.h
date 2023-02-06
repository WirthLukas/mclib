//
// Created by lukew on 23.12.2022.
//

#ifndef MODERNC_UTEST_H
#define MODERNC_UTEST_H

// https://nemequ.github.io/munit
// https://github.com/silentbicycle/greatest/blob/master/greatest.h
// https://github.com/if-03-22-prpr/if.03.22-10_trim_string/blob/master/shortcut.h

#include <stddef.h>
#include <stdbool.h>

typedef struct test_context {
    unsigned int assertions;
    bool success;
    char* msg;
} test_context_t;

#define ASSERT_TRUE(condition, msg) assert_true_f(condition, msg, ctx, __FILE__, __LINE__)
#define ASSERT_FALSE(condition, msg) assert_false_f(condition, msg, ctx, __FILE__, __LINE__)
#define ASSERT_EQUALS_FLOAT(expected, actual, tolerance, msg) assert_equals_float(expected, actual, tolerance, msg, ctx, __FILE__, __LINE__)
#define ASSERT_EQUALS_STR(expected, actual, msg) assert_equals_str(expected, actual, msg, ctx, __FILE__, __LINE__)
#define ASSERT_EQUALS_DECIMAL(expected, actual, msg) assert_equals_decimal(expected, actual, msg, ctx, __FILE__, __LINE__)

//#define EPSILON 0.001

//#define ASSERT_EQUALS(expected, actual, msg) \
//    _Generic((expected),                     \
//        int:  assert_equals_decimal(expected, actual, msg, ctx, __FILE__, __LINE__),  \
//        long: assert_equals_decimal(expected, actual, msg, ctx, __FILE__, __LINE__), \
//        double: assert_equals_float(expected, actual, EPSILON, msg, ctx, __FILE__, __LINE__), \
//        default: assert_equals_str(expected, actual, msg, ctx, __FILE__, __LINE__))

#define TEST(function_name) void function_name(test_context_t* ctx)

void assert_true_f(bool condition, const char* msg, test_context_t* ctx, const char* filename, int line);
void assert_false_f(bool condition, const char* msg, test_context_t* ctx, const char* filename, int line);
void assert_equals_str(const char* expected, const char* actual, const char* msg, test_context_t* ctx, const char* filename, int line);
void assert_equals_decimal(long expected, long actual, const char* msg, test_context_t* ctx, const char* filename, int line);
void assert_equals_float(double expected, double actual, double tolerance, const char* msg, test_context_t* ctx, const char* filename, int line);

typedef void (* TestFunction)(test_context_t* ctx);

typedef struct {
    TestFunction function;
    const char* name;
} Test;

#define ADD_TEST(test_function) (Test) { .function = test_function, .name = #test_function }

typedef struct TestSuite {
    Test* tests;
    size_t count;
    const char* name;
} TestSuite;

void run_tests(TestSuite suite);

#endif //MODERNC_UTEST_H
