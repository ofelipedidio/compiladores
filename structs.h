#pragma once

#include <inttypes.h>
#include "list.h"

/*********\
* Generic *
\*********/
typedef enum {
    type_undefined,
    type_int,
    type_float,
    type_bool
} type_t;

/******************\
* Lexical Analysis *
\******************/
typedef enum {
    lex_ident,
    lex_int,
    lex_float,
    lex_bool,
} lexeme_type_t;

typedef union {
    struct {
        lexeme_type_t type;
        char *value;
        uint64_t line;
        uint64_t column;
    } lex_ident_t;
    struct {
        lexeme_type_t type;
        uint64_t value;
        uint64_t line;
        uint64_t column;
    } lex_int_t;
    struct {
        lexeme_type_t type;
        double value;
        uint64_t line;
        uint64_t column;
    } lex_float_t;
    struct {
        lexeme_type_t type;
        int value;
        uint64_t line;
        uint64_t column;
    } lex_bool_t;
} lexeme_t;

/*******************\
* Semantic Analysis *
\*******************/

/******************************\
* Intermediate Code Generation *
\******************************/
typedef enum {
    nop,         // nop                      // does nothing
    
    // Arithmetic
    add,         // add r1, r2 => r3         // r3 = r1 + r2
    sub,         // sub r1, r2 => r3         // r3 = r1 - r2
    mult,        // mult r1, r2 => r3        // r3 = r1 * r2
    _div,         // div r1, r2 => r3         // r3 = r1 / r2
    add_i,       // addI r1, c2 => r3        // r3 = r1 + c2
    sub_i,       // subI r1, c2 => r3        // r3 = r1 - c2
    rsub_i,      // rsubI r1, c2 => r3       // r3 = c2 - r1
    mult_i,      // multI r1, c2 => r3       // r3 = r1 * c2
    div_i,       // divI r1, c2 => r3        // r3 = r1 / c2
    rdiv_i,      // rdivI r1, c2 => r3       // r3 = c2 / r1
    
    // Shift
    lshift,      // lshift r1, r2 => r3      // r3 = r1 << r2
    lshift_i,    // lshiftI r1, c2 => r3     // r3 = r1 << c2
    rshift,      // rshift r1, r2 => r3      // r3 = r1 >> r2
    rshift_i,    // rshiftI r1, c2 => r3     // r3 = r1 >> c2
    
    // Load
    load,        // load r1 => r2            // r2 = Memoria(r1)
    load_ai,     // loadAI r1, c2 => r3      // r3 = Memoria(r1 + c2)
    load_a0,     // loadA0 r1, r2 => r3      // r3 = Memoria(r1 + r2)
    cload,       // cload r1 => r2           // caractere load
    cload_ai,    // cloadAI r1, c2 => r3     // caractere loadAI
    cload_a0,    // cloadA0 r1, r2 => r3     // caractere loadA0
    load_i,      // loadI c1 => r2           // r2 = c1
    
    // Store
    store,       // store r1 => r2           // Memoria(r2) = r1
    store_ai,    // storeAI r1 => r2, c3     // Memoria(r2 + c3) = r1
    store_ao,    // storeAO r1 => r2, r3     // Memoria(r2 + r3) = r1
    cstore,      // cstore r1 => r2          // caractere store
    cstore_ai,   // cstoreAI r1 => r2, c3    // caractere storeAI
    cstore_ao,   // cstoreAO r1 => r2, r3    // caractere storeAO
    
    // Copy
    i2i,         // i2i r1 => r2             // r2 = r1 para inteiros
    c2c,         // c2c r1 => r2             // r2 = r1 para caracteres
    c2i,         // c2i r1 => r2             // converte um caractere para um inteiro
    i2c,         // i2c r1 => r2             // converte um inteiro para caractere
    
    // Comparisons
    cmp_lt,      // cmp_LT r1, r2 -> r3      // r3 = true se r1 < r2, senão r3 = false
    cmp_le,      // cmp_LE r1, r2 -> r3      // r3 = true se r1 <= r2, senão r3 = false
    cmp_eq,      // cmp_EQ r1, r2 -> r3      // r3 = true se r1 = r2, senão r3 = false
    cmp_ge,      // cmp_GE r1, r2 -> r3      // r3 = true se r1 >= r2, senão r3 = false
    cmp_gt,      // cmp_GT r1, r2 -> r3      // r3 = true se r1 > r2, senão r3 = false
    cmp_ne,      // cmp_NE r1, r2 -> r3      // r3 = true se r1 != r2, senão r3 = false
    cbr,         // cbr r1 -> l2, l3         // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
    
    // Jump
    jump_i,      // jumpI -> l1              // PC = endereço(l1)
    jump,        // jump -> r1               // PC = r1
    
    label,       // L<r1>:
} iloc_instruction_type_t;

typedef struct {
    iloc_instruction_type_t instruction;
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
} iloc_instruction_t;

typedef struct {
    iloc_instruction_t *instructions;
    uint64_t length;
} iloc_program_t;

/********************\
* Syntactic Analysis *
\********************/
typedef enum {
    ast_global_list,
    ast_var_decl,
    ast_func_decl,
    ast_func_header,
    ast_expr
} ast_label_t;

typedef struct ast {
    ast_label_t label;
    struct ast **children;
    lexeme_t *lexeme;
    type_t type;
    iloc_program_t *program;
} ast_t;
