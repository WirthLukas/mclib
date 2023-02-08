//
// Created by lukew on 12.01.2023.
//

#ifndef MODERNC_MACROS_H
#define MODERNC_MACROS_H

/*      References      */

#define valof *
#define refof &
#define ref(type) type*
#define dref(type) type**
#define tref(type) type***

/*      Utils       */

// https://stackoverflow.com/questions/1597007/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr
#define concat_helper(x,y) x##y
#define concat(x,y) concat_helper(x,y)
#define macro_var(name) concat(name, __LINE__)

#define string_cat(str1, str2) (str1 "" str2)

// https://www.youtube.com/watch?v=QpAhX-gsHMs
#define defer(start, end) for (         \
    int macro_var(_i_) = ((start), 0);  \
    !macro_var(_i_);                    \
    (macro_var(_i_) += 1), (end))       \

#define scope(end) defer(0, end)

#define foreach(item, array)                                            \
    for (int keep = 1, count = 0, size = sizeof(array)/sizeof *(array); \
    keep && count != size;                                              \
    keep = !keep, count++)                                              \
        for (item = (array)+count; keep; keep = !keep)                  \


// This requires nested functions to be allowed.
// Only GCC supports it.
// ... Unconfirmed if Clang does. It might.
#if defined(__clang__) || defined(__GNUC__)
// A cleaner, but slightly more cumbersome lambda:
#define lambda(ret_type, _body) ({ ret_type concat(_f_, __LINE__) _body concat(_f_, __LINE__); })
// e.g. int (*max)(int, int) = lambda (int, (int x, int y) { return x > y ? x : y; });
// Pros:
// * Woot, easier to pass, as the user has to know the signature anyway.
// * Name not part of lambda definition. More lambda-y.
// * Body of function inside macro, feels more like a lambda.
// * Uses expression disgnator (GCC-only), which creates a properly constructed function pointer.
// * It *may* work under Clang too!
// Cons:
// * The signature isn't constructed for the user, they have to both know and understand it.
#endif

#endif //MODERNC_MACROS_H
