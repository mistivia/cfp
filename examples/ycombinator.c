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
        define(Y, lambda args1(h)
            return apply(apply(lambda args1(x)
                return apply($(h), lambda args1(a)
                    return apply(apply($(x), $(x)), $(a)); end); end),
                            lambda args1(x) 
                                return apply(
                                    $(h),
                                    lambda args1(a)
                                        return apply(apply($(x), $(x)), $(a));
                                    end);
                            end);
        end)
     end)
     _(test);
// ===== end scripting =====

    return 0;
}

