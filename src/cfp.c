// Copyright (C) 2023 Arcane Lactiflora <arcanelactiflora@outlook.com>
// Licensed under The 2-Clause BSD License

#include "cfp.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <gc.h>

#include "mmhash.h"

static void print_stack_and_quit(char *errmsg) {
    fprintf(stderr, "error: %s\n", errmsg);
    abort();
}

value_t nil = {.type = T_NIL, .boolean = 0};
value_t cfp_eol = {.type = T_EOL};

value_t integer(int64_t value) {
    return (value_t){.type = T_INTEGER, .integer= value};
}

value_t real(double number) {
    return (value_t){.type=T_REAL, .real = number};
}

value_t boolean(int boolean) {
    return (value_t){.type = T_BOOLEAN, .boolean = boolean};
}

value_t make_function(cfp_env_t *env, cfp_func_t func) {
    return (value_t){
        .type = T_FUNCTION,
        .func = {
            .env = env, 
            .fn = func,
        }
    };
}

value_t *new_value(value_t value) {
    if (value.type == T_NIL) return &nil;
    value_t *ret = GC_malloc(sizeof(value_t));
    *ret = value;
    return ret;
}

value_t cons(value_t a, value_t b) {
    value_t ret;
    ret.type = T_PAIR;
    ret.car = new_value(a);
    ret.cdr = new_value(b);
    return ret;
}

// TODO: vector
// TODO: object

typedef struct {
    const char *name;
    value_t value;
} value_table_entry_t;

static uint64_t value_table_entry_hasher(void *pentry_) {
    value_table_entry_t *pentry = pentry_;
    return mmhash(pentry->name, strlen(pentry->name), 0);
}

static bool value_table_entry_eq(void *pa_, void *pb_) {
    value_table_entry_t *pa = pa_;
    value_table_entry_t *pb = pb_;
    return strcmp(pa->name, pb->name) == 0;
}

cfp_env_t* cfp_make_env() {
    cfp_env_t *ret = GC_malloc(sizeof(cfp_env_t));
    ret->ht = GC_malloc(sizeof(hash_table_t));
    ret->up = NULL;
    init_hash_table(ret->ht, sizeof(value_table_entry_t), -1,
                    value_table_entry_hasher, value_table_entry_eq);
    return ret;
}

value_t *cfp_getvar(cfp_env_t *env, const char *name) {
    value_table_entry_t f = {.name = name};
    value_table_entry_t *iter;
    while (env != NULL) {
        iter = hash_table_find(env->ht, &f);
        if (iter != NULL) break;
        env = env->up;
    }
    if (iter == NULL) {
        print_stack_and_quit("symbol not found");
    }
    return &(iter->value);
}

value_t *cfp_setvar(cfp_env_t *env, const char *name, value_t value) {
    value_table_entry_t f = {.name = name};
    value_table_entry_t *iter;
    while (env != NULL) {
        iter = hash_table_find(env->ht, &f);
        if (iter != NULL) break;
        env = env->up;
    }
    if (iter == NULL) {
        print_stack_and_quit("symbol not found");
    }
    iter->value = value;
    return &(iter->value);
}

value_t *cfp_defvar(cfp_env_t *env, const char *name, value_t value) {
    value_table_entry_t new_entry = {.name = name, value = value};
    value_table_entry_t *iter = hash_table_find(env->ht, &new_entry);
    if (iter != NULL) {
        return &(iter->value);
    }
    hash_table_insert(env->ht, &new_entry);
    iter = hash_table_find(env->ht, &new_entry);
    return &(iter->value);
}

cfp_env_t* cfp_push_env(cfp_env_t *penv) {
    cfp_env_t* new_env = cfp_make_env();
    new_env->up = penv;
    return new_env;
}

cfp_env_t* cfp_pop_env(cfp_env_t*penv) {
    return penv->up;
}

value_t cfp_apply(value_t fn, value_t args) {
    cfp_env_t *env = cfp_push_env(fn.func.env);
    return (*(fn.func.fn))(env, args);
}

static value_t reverse(value_t lst) {
    value_t new_lst = nil;
    while (lst.type == T_PAIR) {
        new_lst = cons(*(lst.car), new_lst);
        lst = *(lst.cdr);
    }
    return new_lst;
}

value_t cfp_list(int _placeholder, ...) {
    value_t lst = nil;
    value_t* last = NULL;
    va_list arglist;
    va_start(arglist, _placeholder);
    value_t arg = va_arg(arglist, value_t);
    while(arg.type != T_EOL) {
        lst = cons(arg, lst);
        arg = va_arg(arglist, value_t);
    }
    return reverse(lst);
}

