//
// Created by lukew on 23.12.2022.
//

#include <stdio.h>
#include <string.h>
#include "utest.h"

void assert_true_f(bool condition, const char* msg, test_context_t* ctx, const char* filename, int line) {
    ctx->assertions++;

    if (!condition) {
        ctx->success = false;

        if (ctx->msg == NULL) return;

        sprintf(ctx->msg, "%s   ....Error on line %d in file %s. %s\n", ctx->msg, line, filename, msg);
    }
}

void assert_false_f(bool condition, const char* msg, test_context_t* ctx, const char* filename, int line) {
    assert_true_f(!condition, msg, ctx, filename, line);
}

void assert_equals_str(const char* expected, const char* actual, const char* msg, test_context_t* ctx, const char* filename, int line) {
    ctx->assertions++;

    if (expected == actual) {
        return;
    }

    if (expected == NULL || actual == NULL) {
        ctx->success = false;
        if (ctx->msg == NULL) return;
        sprintf(ctx->msg,
                "%s   ....Error on line %d in file %s. The given expected or actual parameter are NULL pointer! Expected: \"%s\", Actual: \"%s\", %s\n",
                ctx->msg, line, filename, expected != NULL ? expected : "NULL", actual != NULL ? actual : "NULL", msg);
    }

    if (strcmp(expected, actual) != 0) {
        ctx->success = false;
        if (ctx->msg == NULL) return;
        sprintf(ctx->msg, "%s   ....Error on line %d in file %s. Expected \"%s\", actual \"%s\". %s\n",
                ctx->msg, line, filename, expected, actual, msg);
    }
}

void assert_equals_decimal(long expected, long actual, const char* msg, test_context_t* ctx, const char* filename, int line) {
    ctx->assertions++;

    if (expected != actual) {
        char new_msg[128];
        sprintf(new_msg, "Expected %ld, actual %ld. %s", expected, actual, msg);
        assert_true_f(false, new_msg, ctx, filename, line);
    }
}

void assert_equals_float(double expected, double actual, double tolerance, const char* msg, test_context_t* ctx, const char* filename, int line) {
    ctx->assertions++;
    double minVal = expected - tolerance;
    double maxVal = expected + tolerance;

    if (minVal >= actual && maxVal <= actual) {
        char new_msg[128];
        sprintf(new_msg, "Expected %f, actual %f. %s", expected, actual, msg);
        assert_true_f(false, new_msg, ctx, filename, line);
    }
}

void run_tests(TestSuite suite) {
    unsigned int assertions = 0;
    unsigned int passed = 0;
    unsigned int fail = 0;

    printf("Suite: %s\n=============================================\n", suite.name);

    for (int i = 0; i < suite.count; i++) {
        test_context_t ctx = { .success = true, .msg = (char[500]) {} };
        suite.tests[i].function(&ctx);

        if (ctx.success) {
            printf(" > %s [ \033[32m%-5s\033[m ]\n", suite.tests[i].name, "Ok");
            passed++;
        } else {
            printf(" > %s [ \033[31m%-5s\033[m ]\n%s\n", suite.tests[i].name, "Fail", ctx.msg);
            fail++;
        }

        assertions += ctx.assertions;
    }

    int percentage = passed / (float)(suite.count) * 100;
    printf("%d of %ld (%d%%) successful, assertions: %u\n\n", passed, suite.count, percentage, assertions);
}