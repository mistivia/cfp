// Copyright (C) 2023 Arcane Lactiflora <arcanelactiflora@outlook.com>
// Licensed under The 2-Clause BSD License

#include "cfp.h"

#include <stdio.h>

value_t make_int(int x) {
    return integer(x);
}

int main() {
    cfp_env_t *env = cfp_make_env();
    
// ===== begin scripting =====
     define(test, lambda
        define(add, lambda args2(x, y)
            return make_int($integer(x) + $integer(y));
        end)
        define(minus, lambda args2(x, y)
            return make_int($integer(x) - $integer(y));
        end)
        define(fibo, lambda args1(x)
            if ($integer(x) < 2) return $(x);
            return _(add, _(fibo, _(minus, $(x), make_int(1))),
                          _(fibo, _(minus, $(x), make_int(2))));
        end)
        define(input, make_int(29));
        setq(input, _(add, $(input), make_int(1)))
        define(result, _(fibo, $(input)));
        printf("%lld", $integer(result));
     end)
     _(test);
// ===== end scripting =====

    return 0;
}

