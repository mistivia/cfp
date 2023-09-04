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
     end)
     _(test);
// ===== end scripting =====

    return 0;
}

