# C for Functional Programming

Use C macros to create a new line in C, which becomes a lisp-like dynamic functional programming language.

I used 2 GNU C extensions here to create lambda:

- [Nested function]()
- [Statement expresiion]()

Which means this project can only be compiled in gcc. Statement expression is supported in clang, but nested function is not. However, clang support another extension which serves similar purpose: [block](). But I didn't add support for it in this project.

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


