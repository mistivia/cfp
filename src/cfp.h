// Copyright (C) 2023 Mistivia <i@mistivia.com>
// Licensed under The 2-Clause BSD License

#ifndef CFP_H_
#define CFP_H_

#include <stdint.h>
#include <string.h>
#include "hash_table.h"
#include "gc.h"

typedef enum {
    T_INTEGER,
    T_REAL,
    T_BOOLEAN,
    T_CHAR,
    T_UNICODE,
    T_STRING,
    T_PAIR,
    T_VECTOR,
    T_OBJECT,
    T_FUNCTION,
    T_ENV,
    T_NIL,
    T_EOL,
} value_type_t;

typedef struct cfp_env {
    hash_table_t *ht;
    struct cfp_env *up;
} cfp_env_t;

typedef struct value {
    value_type_t type;
    union {
	    int64_t integer;
	    double real;
        int boolean;
	    struct {
	        int size;
	        char *buf;
	    } str;
	    struct {
	        struct value *car;
	        struct value *cdr;
	    };
	    struct {
	        int size;
	        struct value *buf;
	    } vec;
	    struct {
	        hash_table_t *ht;
	    } obj;
	    struct {
	        cfp_env_t *env;
	        struct value (*fn)(cfp_env_t* env, struct value args);
	    } func;
    };
} value_t;


extern value_t nil;
extern value_t cfp_eol;

value_t* new_value(value_t value);

typedef value_t (*cfp_func_t)(cfp_env_t *env, value_t args);

value_t integer(int64_t value);
value_t real(double number);
value_t boolean(int value);
value_t cons(value_t a, value_t b);
value_t vector();
value_t object();
value_t make_function(cfp_env_t *env, cfp_func_t fn);

cfp_env_t* cfp_make_env();
cfp_env_t* cfp_push_env(cfp_env_t *penv);
cfp_env_t* cfp_pop_env(cfp_env_t *penv);

value_t cfp_getattr(value_t obj, char *name);
value_t cfp_setattr(value_t obj, char *name, value_t value);

#define getattr(obj, name)
#define setattr(obj, name, value)

value_t* cfp_getvar(cfp_env_t *env, const char *name);
value_t* cfp_setvar(cfp_env_t *env, const char *name, value_t value);
value_t* cfp_defvar(cfp_env_t *env, const char *name, value_t value);


// apply
value_t cfp_apply(value_t func, value_t args);

value_t cfp_list(int _placeholder, ...);

#define $(name) \
    (*(cfp_getvar(env, #name)))

#define $integer(name) \
    (cfp_getvar(env, #name)->integer)

#define apply(fn, ...) \
    (cfp_apply((fn), cfp_list(0, __VA_ARGS__ __VA_OPT__(,) cfp_eol)))

#define _(fn, ...) \
    (cfp_apply($(fn), cfp_list(0, __VA_ARGS__ __VA_OPT__(,) cfp_eol)))

#define list(...) (cfp_list(0, __VA_ARGS__ __VA_OPT__(,) cfp_eol))

#define lambda \
   (make_function(env, ({ \
        value_t __cfp_lambda__(cfp_env_t *env, value_t args) {

#define end \
        } \
        __cfp_lambda__; \
    })))


#define define(name, value) \
    cfp_defvar(env, #name, (value)); \

#define args1(a) \
    define(a, *(args.car)) \
    define(remargs, *(args.cdr))

#define args2(a, b) \
    define(a, *(args.car)) \
    define(b, *(args.cdr->car)) \
    define(remargs, *(args.cdr->cdr))

#define args3(a, b, c) \
    define(a, *(args.car)) \
    define(b, *(args.cdr->car)) \
    define(c, *(args.cdr->cdr->car)) \
    define(remargs, *(args.cdr->cdr->cdr))


#define setq(name, val) \
    cfp_setvar(env, #name, (val));


#endif
