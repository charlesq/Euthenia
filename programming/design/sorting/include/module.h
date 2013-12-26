#ifndef __MODULE_H__
#define __MODULE_H__
#include <stddef.h>
#include <stdio.h>
#include <compare.h>

#ifdef __cplusplus
extern "C" {
#endif

#define O_RECURSIVE 0x0 /* indicates recurisve version of the algorithm */
#define O_VERBOSE  0x1 /*  if set, print out verbose logging information */
#define O_PRINT_INTERMEDIATE  0x2/* if set, print out intermediate result in execution */
#define O_MEASURE_RUN_TIME 0x4
typedef struct _module  module;
struct _module {
    module *next;
    char *name;
    /* for non-comparison-based algorithms, cmp shall be skipped , thus the default NULL value is assumed */
    void (*sort)(void *base, size_t nelem, size_t size, compare cmp);
    void (*test_module)(size_t count);
    unsigned int(*get_runtime)(void);
    const char * (*get_description)(void);
    void (*set_verbose)(int b, print_element p);
};


void register_module (module *mod);
module *get_module(const char *);
void destroy_module(const module *);
#define SORTING_MODULE(MOD) \
static void __init_##MOD (void) __attribute__((constructor));\
static void __init_##MOD (void) { \
    register_module(&MOD);\
}

#ifdef __cplusplus
}
#endif

#endif
