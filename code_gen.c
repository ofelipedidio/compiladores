#include "code_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

uint64_t last_id = 0;

uint64_t iloc_next_id() {
    return last_id++;
}

iloc_program_t *iloc_program_new() {
    iloc_program_t *iloc_program = (iloc_program_t*) malloc(sizeof(iloc_program_t));
    if (iloc_program == NULL) {
        fprintf(stderr, "Failed to allocate memory for iloc_program_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        return NULL;
    }
    iloc_instruction_t* iloc_instructions = (iloc_instruction_t*) malloc(0*sizeof(iloc_instruction_t));
    if (iloc_instructions == NULL) {
        free(iloc_program);
        fprintf(stderr, "Failed to allocate memory for iloc_instruction_t* (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        return NULL;
    }
    iloc_program->instructions = iloc_instructions;
    iloc_program->length = 0;
    return iloc_program;
}

iloc_instruction_t iloc_instruction_new(iloc_instruction_type_t type, uint64_t r1, uint64_t r2, uint64_t r3) {
    iloc_instruction_t instruction;
    instruction.instruction = type;
    instruction.r1 = r1;
    instruction.r2 = r2;
    instruction.r3 = r3;
    return instruction;
}

void iloc_program_push(iloc_program_t *program, iloc_instruction_t instruction) {
    iloc_instruction_t *new_instructions = realloc(program->instructions, (program->length+1)*sizeof(iloc_instruction_t));
    program->instructions = new_instructions;
    program->instructions[program->length] = instruction;
    program->length++;
}

void iloc_program_append(iloc_program_t *dest, iloc_program_t *src) {
    iloc_instruction_t *new_instructions = realloc(dest->instructions, (dest->length+src->length)*sizeof(iloc_instruction_t));
    dest->instructions = new_instructions;
    memcpy(&dest->instructions[dest->length], src->instructions, src->length * sizeof(iloc_instruction_t));
    dest->length += src->length;
}

void iloc_program_free(iloc_program_t *program) {
    free(program->instructions);
    free(program);
}

void iloc_instruction_to_string(iloc_instruction_t *instruction) {
    switch (instruction->instruction) {
        case nop:
            fprintf(stdout, "nop\n"); // does nothing
            break;
        case add:
            fprintf(stdout, "add r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 + r2
            break;
        case sub:
            fprintf(stdout, "sub r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 - r2
            break;
        case mult:
            fprintf(stdout, "mult r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 * r2
            break;
        case _div:
            fprintf(stdout, "div r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 / r2
            break;
        case add_i:
            fprintf(stdout, "addI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 + c2
            break;
        case sub_i:
            fprintf(stdout, "subI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 - c2
            break;
        case rsub_i:
            fprintf(stdout, "rsubI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = c2 - r1
            break;
        case mult_i:
            fprintf(stdout, "multI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 * c2
            break;
        case div_i:
            fprintf(stdout, "divI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 / c2
            break;
        case rdiv_i:
            fprintf(stdout, "rdivI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = c2 / r1
            break;
        case lshift:
            fprintf(stdout, "lshift r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 << r2
            break;
        case lshift_i:
            fprintf(stdout, "lshiftI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 << c2
            break;
        case rshift:
            fprintf(stdout, "rshift r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 >> r2
            break;
        case rshift_i:
            fprintf(stdout, "rshiftI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 >> c2
            break;
        case load:
            fprintf(stdout, "load r%ld => r%ld\n", instruction->r1, instruction->r2); // r2 = Memoria(r1)
            break;
        case load_ai:
            fprintf(stdout, "loadAI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = Memoria(r1 + c2)
            break;
        case load_a0:
            fprintf(stdout, "loadA0 r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = Memoria(r1 + r2)
            break;
        case cload:
            fprintf(stdout, "cload r%ld => r%ld\n", instruction->r1, instruction->r2); // caractere load
            break;
        case cload_ai:
            fprintf(stdout, "cloadAI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // caractere loadAI
            break;
        case cload_a0:
            fprintf(stdout, "cloadA0 r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // caractere loadA0
            break;
        case load_i:
            fprintf(stdout, "loadI %ld => r%ld\n", instruction->r1, instruction->r2); // r2 = c1
            break;
        case store:
            fprintf(stdout, "store r%ld => r%ld\n", instruction->r1, instruction->r2); // Memoria(r2) = r1
            break;
        case store_ai:
            fprintf(stdout, "storeAI r%ld => r%ld, %ld\n", instruction->r1, instruction->r2, instruction->r3); // Memoria(r2 + c3) = r1
            break;
        case store_ao:
            fprintf(stdout, "storeAO r%ld => r%ld, r%ld\n", instruction->r1, instruction->r2, instruction->r3); // Memoria(r2 + r3) = r1
            break;
        case cstore:
            fprintf(stdout, "cstore r%ld => r%ld\n", instruction->r1, instruction->r2); // caractere store
            break;
        case cstore_ai:
            fprintf(stdout, "cstoreAI r%ld => r%ld, %ld\n", instruction->r1, instruction->r2, instruction->r3); // caractere storeAI
            break;
        case cstore_ao:
            fprintf(stdout, "cstoreAO r%ld => r%ld, r%ld\n", instruction->r1, instruction->r2, instruction->r3); // caractere storeAO
            break;
        case i2i:
            fprintf(stdout, "i2i r%ld => r%ld\n", instruction->r1, instruction->r2); // r2 = r1 para inteiros
            break;
        case c2c:
            fprintf(stdout, "c2c r%ld => r%ld\n", instruction->r1, instruction->r2); // r2 = r1 para caracteres
            break;
        case c2i:
            fprintf(stdout, "c2i r%ld => r%ld\n", instruction->r1, instruction->r2); // converte um caractere para um inteiro
            break;
        case i2c:
            fprintf(stdout, "i2c r%ld => r%ld\n", instruction->r1, instruction->r2); // converte um inteiro para caractere
            break;
        case cmp_lt:
            fprintf(stdout, "cmp_LT r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 < r2, senão r3 = false
            break;
        case cmp_le:
            fprintf(stdout, "cmp_LE r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 <= r2, senão r3 = false
            break;
        case cmp_eq:
            fprintf(stdout, "cmp_EQ r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 = r2, senão r3 = false
            break;
        case cmp_ge:
            fprintf(stdout, "cmp_GE r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 >= r2, senão r3 = false
            break;
        case cmp_gt:
            fprintf(stdout, "cmp_GT r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 > r2, senão r3 = false
            break;
        case cmp_ne:
            fprintf(stdout, "cmp_NE r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 != r2, senão r3 = false
            break;
        case cbr:
            fprintf(stdout, "cbr r%ld -> L%ld, L%ld\n", instruction->r1, instruction->r2, instruction->r3); // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
            break;
        case jump_i:
            fprintf(stdout, "jumpI -> L%ld\n", instruction->r1); // PC = endereço(l1)
            break;
        case jump:
            fprintf(stdout, "jump -> r%ld\n", instruction->r1); // PC = r1
            break;
        case label:
            fprintf(stdout, "L%ld:\n", instruction->r1); // PC = r1
            break;
        default:
            fprintf(stderr, "Could not print a instruction\n");
            break;
    }
}

void iloc_program_to_string(iloc_program_t *program) {
    for (uint64_t i = 0; i < program->length; i++) {
        iloc_instruction_to_string(&program->instructions[i]);
    }
}

