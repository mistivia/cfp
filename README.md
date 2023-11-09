# C for Functional Programming

Use macros to create a new lisp-like dynamic functional programming language in plain C.

I used 2 GNU C extensions here to create lambda:

- [Nested function](https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html)
- [Statement expression](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)

Which means this project can only be compiled in gcc. Statement expressions are supported in clang, but nested functions are not. However, clang support another extension which serves similar purpose: [blocks](https://en.wikipedia.org/wiki/Blocks_(C_language_extension)). But I didn't add a support for it in this project.

Here is a example of counter implemented using closure:

    define(get_counter, lambda args1(init_value)
        define(value, $(init_value))

        define(counter, lambda
           setq(value, make_int($(value).integer + 1));
           return $(value);
        end)

        return $(counter);
    end)

    define(cnt, _(get_counter, make_int(0)))


    printf("%d\n", _(cnt).integer);
    printf("%d\n", _(cnt).integer);
    printf("%d\n", _(cnt).integer);
    printf("%d\n", _(cnt).integer);
    printf("%d\n", _(cnt).integer);

    // Equivalent lisp code looks like:
    // (define get-counter (lambda (init_value)
    //     (define value init_value)
    //     (define counter (lambda ()
    //         (set! value (+ value 1))))
    //     counter))
    //
    // (define cnt (get-counter 0))
    //
    // (display (cnt))
    // (display (cnt))
    // (display (cnt))
    // (display (cnt))

