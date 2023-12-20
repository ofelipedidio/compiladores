#include "ast.h"
#include "code_gen.h"
#include "lexeme.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "semantics.h"

/* ################
 * # Constructors #
 * ################ */
program_t *ast_program_new() {
    program_t *program = (program_t *)malloc(sizeof(program_t));
    if (program == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for program_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    global_t **globals = (global_t **)malloc(0 * sizeof(global_t *));
    if (globals == NULL) {
        free(program);
        fprintf(stderr,
                "Failed to allocate memory for global_t* (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(globals);
        free(program);
        return NULL;
    }
    program->globals = globals;
    program->len = 0;
    program->program = iloc_program;
    return program;
}

global_t *ast_global_new_function(function_t *function) {
    global_t *global = (global_t *)malloc(sizeof(global_t));
    if (global == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for global_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(global);
        return NULL;
    }
    global->function = function;
    global->variable = NULL;
    global->program = iloc_program;
    return global;
}

global_t *ast_global_new_variable(variable_t *variable) {
    global_t *global = (global_t *)malloc(sizeof(global_t));
    if (global == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for global_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(global);
        return NULL;
    }
    global->function = NULL;
    global->variable = variable;
    global->program = iloc_program;
    return global;
}

function_t *ast_function_new(identifier_t *name, ast_type_t type,
        parameters_t *parameters, block_t *body) {
    function_t *function = (function_t *)malloc(sizeof(function_t));
    if (function == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for function_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(function);
        return NULL;
    }
    function->name = name;
    function->type = type;
    function->parameters = parameters;
    function->body = body;
    function->program = iloc_program;
    return function;
}

parameters_t *ast_parameters_new() {
    parameters_t *parameters = (parameters_t *)malloc(sizeof(parameters_t));
    if (parameters == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for parameters_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    variable_t **variables = (variable_t **)malloc(0 * sizeof(variable_t *));
    if (variables == NULL) {
        free(parameters);
        fprintf(stderr,
                "Failed to allocate memory for variable_t* (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(variables);
        free(parameters);
        return NULL;
    }
    parameters->variables = variables;
    parameters->len = 0;
    parameters->program = iloc_program;
    return parameters;
}

variable_t *ast_variable_new(ast_type_t type, variable_names_t *names) {
    variable_t *variable = (variable_t *)malloc(sizeof(variable_t));
    if (variable == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for variable_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(variable);
        return NULL;
    }
    variable->type = type;
    variable->names = names;
    variable->program = iloc_program;
    return variable;
}

variable_names_t *ast_variable_names_new() {
    variable_names_t *variable_names = (variable_names_t*) malloc(sizeof(variable_names_t));
    if (variable_names == NULL) {
        fprintf(stderr, "Failed to allocate memory for variable_names_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        return NULL;
    }
    identifier_t** names = (identifier_t**) malloc(0*sizeof(identifier_t*));
    if (names == NULL) {
        free(variable_names);
        fprintf(stderr, "Failed to allocate memory for identifier_t* (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(names);
        free(variable_names);
        return NULL;
    }
    variable_names->names = names;
    variable_names->len = 0;
    variable_names->program = iloc_program;
    return variable_names;
}

command_t *ast_command_new_variable(variable_t *variable) {
    command_t *command = (command_t *)malloc(sizeof(command_t));
    if (command == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for command_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(command);
        return NULL;
    }
    command->variable = variable;
    command->attribution = NULL;
    command->call = NULL;
    command->_return = NULL;
    command->_if = NULL;
    command->_while = NULL;
    command->block = NULL;
    command->type = variable->type;
    command->program = iloc_program;
    return command;
}

command_t *ast_command_new_attribution(attribution_t *attribution) {
    command_t *command = (command_t *)malloc(sizeof(command_t));
    if (command == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for command_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(command);
        return NULL;
    }
    command->variable = NULL;
    command->attribution = attribution;
    command->call = NULL;
    command->_return = NULL;
    command->_if = NULL;
    command->_while = NULL;
    command->block = NULL;
    command->type = attribution->type;
    command->program = iloc_program;
    return command;
}

command_t *ast_command_new_call(call_t *call) {
    command_t *command = (command_t *)malloc(sizeof(command_t));
    if (command == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for command_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(command);
        return NULL;
    }
    command->variable = NULL;
    command->attribution = NULL;
    command->call = call;
    command->_return = NULL;
    command->_if = NULL;
    command->_while = NULL;
    command->block = NULL;
    command->type = call->type;
    command->program = iloc_program;
    return command;
}

command_t *ast_command_new_return(return_t *_return) {
    command_t *command = (command_t *)malloc(sizeof(command_t));
    if (command == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for command_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(command);
        return NULL;
    }
    command->variable = NULL;
    command->attribution = NULL;
    command->call = NULL;
    command->_if = NULL;
    command->_while = NULL;
    command->_return = _return;
    command->block = NULL;
    command->type = _return->type;
    command->program = iloc_program;
    return command;
}

command_t *ast_command_new_if(if_t *_if) {
    command_t *command = (command_t *)malloc(sizeof(command_t));
    if (command == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for command_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(command);
        return NULL;
    }
    command->variable = NULL;
    command->attribution = NULL;
    command->call = NULL;
    command->_return = NULL;
    command->_if = _if;
    command->_while = NULL;
    command->block = NULL;
    command->type = _if->type;
    command->program = iloc_program;
    return command;
}

command_t *ast_command_new_while(while_t *_while) {
    command_t *command = (command_t *)malloc(sizeof(command_t));
    if (command == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for command_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(command);
        return NULL;
    }
    command->variable = NULL;
    command->attribution = NULL;
    command->call = NULL;
    command->_return = NULL;
    command->_if = NULL;
    command->_while = _while;
    command->block = NULL;
    command->type = _while->type;
    command->program = iloc_program;
    return command;
}

command_t *ast_command_new_block(block_t *block) {
    command_t *command = (command_t *)malloc(sizeof(command_t));
    if (command == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for command_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(command);
        return NULL;
    }
    command->variable = NULL;
    command->attribution = NULL;
    command->call = NULL;
    command->_return = NULL;
    command->_if = NULL;
    command->_while = NULL;
    command->block = block;
    command->type = ast_type_undefined;
    command->program = iloc_program;
    return command;
}

attribution_t *ast_attribution_new(identifier_t *variable_name, expression_t *expression) {
    attribution_t *attribution = (attribution_t *)malloc(sizeof(attribution_t));
    if (attribution == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for attribution_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(attribution);
        return NULL;
    }
    attribution->variable_name = variable_name;
    attribution->expression = expression;
    attribution->type = ast_type_undefined;
    attribution->program = iloc_program;
    return attribution;
}

call_t *ast_call_new(identifier_t *function_name, arguments_t *arguments) {
    call_t *call = (call_t *)malloc(sizeof(call_t));
    if (call == NULL) {
        fprintf(
                stderr,
                "Failed to allocate memory for call_t (errno = %d) [at file \"" __FILE__
                "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(call);
        return NULL;
    }
    call->function_name = function_name;
    call->arguments = arguments;
    call->type = ast_type_undefined;
    call->program = iloc_program;
    return call;
}

arguments_t *ast_arguments_new() {
    arguments_t *arguments = (arguments_t *)malloc(sizeof(arguments_t));
    if (arguments == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for arguments_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    expression_t **argument_list =
        (expression_t **)malloc(0 * sizeof(expression_t *));
    if (argument_list == NULL) {
        free(arguments);
        fprintf(stderr,
                "Failed to allocate memory for expression_t* (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(arguments);
        free(arguments);
        return NULL;
    }
    arguments->arguments = argument_list;
    arguments->len = 0;
    arguments->program = iloc_program;
    return arguments;
}

return_t *ast_return_new(expression_t *expression) {
    return_t *_return = (return_t*) malloc(sizeof(return_t));
    if (_return == NULL) {
        fprintf(stderr, "Failed to allocate memory for return_t (errno = %d) [at file " "\"" __FILE__ "\", line %d]\n", errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(_return);
        return NULL;
    }
    _return->expression = expression;
    _return->type = expression->type;
    _return->program = iloc_program;
    return _return;
}

if_t *ast_if_new(expression_t *condition, block_t *then_block,
        block_t *else_block) {
    if_t *_if = (if_t *)malloc(sizeof(if_t));
    if (_if == NULL) {
        fprintf(
                stderr,
                "Failed to allocate memory for if_t (errno = %d) [at file \"" __FILE__
                "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(_if);
        return NULL;
    }
    _if->condition = condition;
    _if->then_block = then_block;
    _if->else_block = else_block;
    _if->type = condition->type;
    _if->program = iloc_program;
    return _if;
}

while_t *ast_while_new(expression_t *condition, block_t *block) {
    while_t *_while = (while_t *)malloc(sizeof(while_t));
    if (_while == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for while_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(_while);
        return NULL;
    }
    _while->condition = condition;
    _while->block = block;
    _while->type = condition->type;
    _while->program = iloc_program;
    return _while;
}

block_t *ast_block_new() {
    block_t *block = (block_t *)malloc(sizeof(block_t));
    if (block == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for block_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    command_t **commands = (command_t **)malloc(sizeof(command_t *));
    if (commands == NULL) {
        free(block);
        fprintf(stderr,
                "Failed to allocate memory for command_t* (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(commands);
        free(block);
        return NULL;
    }
    block->commands = commands;
    block->len = 0;
    block->program = iloc_program;
    return block;
}

expression_t *ast_expression_new_bin_op(bin_op_t *_bin_op) {
    expression_t *expression = (expression_t *)malloc(sizeof(expression_t));
    if (expression == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for expression_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(expression);
        return NULL;
    }
    expression->_bin_op = _bin_op;
    expression->_un_op = NULL;
    expression->call = NULL;
    expression->identifier = NULL;
    expression->literal = NULL;
    expression->type = _bin_op->type;
    expression->program = iloc_program;
    return expression;
}

expression_t *ast_expression_new_un_op(un_op_t *_un_op) {
    expression_t *expression = (expression_t *)malloc(sizeof(expression_t));
    if (expression == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for expression_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(expression);
        return NULL;
    }
    expression->_bin_op = NULL;
    expression->_un_op = _un_op;
    expression->call = NULL;
    expression->identifier = NULL;
    expression->literal = NULL;
    expression->type = _un_op->type;
    expression->program = iloc_program;
    return expression;
}

expression_t *ast_expression_new_call(call_t *call) {
    expression_t *expression = (expression_t *)malloc(sizeof(expression_t));
    if (expression == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for expression_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(expression);
        return NULL;
    }
    expression->_bin_op = NULL;
    expression->_un_op = NULL;
    expression->call = call;
    expression->identifier = NULL;
    expression->literal = NULL;
    expression->type = call->type;
    expression->program = iloc_program;
    return expression;
}

expression_t *ast_expression_new_identifier(identifier_t *identifier) {
    expression_t *expression = (expression_t *)malloc(sizeof(expression_t));
    if (expression == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for expression_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(expression);
        return NULL;
    }
    expression->_bin_op = NULL;
    expression->_un_op = NULL;
    expression->call = NULL;
    expression->identifier = identifier;
    expression->literal = NULL;
    expression->type = identifier->type;
    expression->program = iloc_program;
    return expression;
}

expression_t *ast_expression_new_literal(literal_t *literal) {
    expression_t *expression = (expression_t *)malloc(sizeof(expression_t));
    if (expression == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for expression_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(expression);
        return NULL;
    }
    expression->_bin_op = NULL;
    expression->_un_op = NULL;
    expression->call = NULL;
    expression->identifier = NULL;
    expression->literal = literal;
    expression->type = literal->type;
    expression->program = iloc_program;
    return expression;
}

bin_op_t *ast_bin_op_new(bin_op op, expression_t *left, expression_t *right) {
    bin_op_t *bin_op = (bin_op_t *)malloc(sizeof(bin_op_t));
    if (bin_op == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for bin_op_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(bin_op);
        return NULL;
    }
    bin_op->op = op;
    bin_op->left = left;
    bin_op->right = right;
    bin_op->type = infer_type(left->type, right->type);
    bin_op->program = iloc_program;
    return bin_op;
}

un_op_t *ast_un_op_new(un_op op, expression_t *expression) {
    un_op_t *un_op = (un_op_t *)malloc(sizeof(un_op_t));
    if (un_op == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for un_op_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(un_op);
        return NULL;
    }
    un_op->op = op;
    un_op->expression = expression;
    un_op->type = expression->type;
    un_op->program = iloc_program;
    return un_op;
};

literal_t *ast_literal_new_int(ast_int_t *_int) {
    literal_t *literal = (literal_t *)malloc(sizeof(literal_t));
    if (literal == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for literal_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(literal);
        return NULL;
    }
    literal->_int = _int;
    literal->_float = NULL;
    literal->_bool = NULL;
    literal->type = ast_int;
    literal->program = iloc_program;
    return literal;
}

literal_t *ast_literal_new_float(ast_float_t *_float) {
    literal_t *literal = (literal_t *)malloc(sizeof(literal_t));
    if (literal == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for literal_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(literal);
        return NULL;
    }
    literal->_int = NULL;
    literal->_float = _float;
    literal->_bool = NULL;
    literal->type = ast_float;
    literal->program = iloc_program;
    return literal;
}

literal_t *ast_literal_new_bool(ast_bool_t *_bool) {
    literal_t *literal = (literal_t *)malloc(sizeof(literal_t));
    if (literal == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for literal_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(literal);
        return NULL;
    }
    literal->_int = NULL;
    literal->_float = NULL;
    literal->_bool = _bool;
    literal->type = ast_bool;
    literal->program = iloc_program;
    return literal;
}

ast_int_t *ast_int_new(int value, lexeme_t lexeme) {
    ast_int_t *_int = (ast_int_t *)malloc(sizeof(ast_int_t));
    if (_int == NULL) {
        fprintf(
                stderr,
                "Failed to allocate memory for int_t (errno = %d) [at file \"" __FILE__
                "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(_int);
        return NULL;
    }
    _int->value = value;
    _int->lexeme = lexeme;
    _int->program = iloc_program;
    return _int;
}

ast_float_t *ast_float_new(float value, lexeme_t lexeme) {
    ast_float_t *_float = (ast_float_t *)malloc(sizeof(ast_float_t));
    if (_float == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for float_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(_float);
        return NULL;
    }
    _float->value = value;
    _float->lexeme = lexeme;
    _float->program = iloc_program;
    return _float;
}

ast_bool_t *ast_bool_new_true(lexeme_t lexeme) {
    ast_bool_t *_bool = (ast_bool_t *)malloc(sizeof(ast_bool_t));
    if (_bool == NULL) {
        fprintf(
                stderr,
                "Failed to allocate memory for bool_t (errno = %d) [at file \"" __FILE__
                "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(_bool);
        return NULL;
    }
    _bool->value = 1;
    _bool->lexeme = lexeme;
    _bool->program = iloc_program;
    return _bool;
}

ast_bool_t *ast_bool_new_false(lexeme_t lexeme) {
    ast_bool_t *_bool = (ast_bool_t *)malloc(sizeof(ast_bool_t));
    if (_bool == NULL) {
        fprintf(
                stderr,
                "Failed to allocate memory for bool_t (errno = %d) [at file \"" __FILE__
                "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(_bool);
        return NULL;
    }
    _bool->value = 0;
    _bool->lexeme = lexeme;
    _bool->program = iloc_program;
    return _bool;
}

identifier_t *ast_identifier_new(char *text, lexeme_t lexeme) {
    identifier_t *identifier = (identifier_t *)malloc(sizeof(identifier_t));
    if (identifier == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for identifier_t (errno = %d) [at file "
                "\"" __FILE__ "\", line %d]\n",
                errno, __LINE__ - 2);
        return NULL;
    }
    iloc_program_t *iloc_program = iloc_program_new();
    if (iloc_program == NULL) {
        free(identifier);
        return NULL;
    }
    identifier->text = strdup(text);
    identifier->len = strlen(text);
    identifier->lexeme = lexeme;
    identifier->type = ast_type_undefined;
    identifier->program = iloc_program;
    return identifier;
}

/* ###############
 * # Destructors #
 * ############### */
void ast_program_free(program_t *program) {
    for (uint64_t i = 0; i < program->len; i++) {
        ast_global_free(program->globals[i]);
    }
    iloc_program_free(program->program);
    free(program->globals);
    free(program);
};

void ast_global_free(global_t *global) {
    if (global->variable != NULL) {
        ast_variable_free(global->variable);
    }
    if (global->function != NULL) {
        ast_function_free(global->function);
    }
    iloc_program_free(global->program);
    free(global);
};

void ast_function_free(function_t *function) {
    iloc_program_free(function->program);
    ast_identifier_free(function->name);
    ast_parameters_free(function->parameters);
    ast_block_free(function->body);
    free(function);
};

void ast_parameters_free(parameters_t *parameters) {
    for (uint64_t i = 0; i < parameters->len; i++) {
        ast_variable_free(parameters->variables[i]);
    }
    iloc_program_free(parameters->program);
    free(parameters->variables);
    free(parameters);
};

void ast_variable_free(variable_t *variable) {
    iloc_program_free(variable->program);
    ast_variable_names_free(variable->names);
    free(variable);
};

void ast_variable_names_free(variable_names_t *names) {
    for (uint64_t i = 0; i < names->len; i++) {
        ast_identifier_free(names->names[i]);
    }
    iloc_program_free(names->program);
    free(names->names);
    free(names);
}

void ast_command_free(command_t *command) {
    if (command->variable) {
        ast_variable_free(command->variable);
    }
    if (command->attribution) {
        ast_attribution_free(command->attribution);
    }
    if (command->call) {
        ast_call_free(command->call);
    }
    if (command->_if) {
        ast_if_free(command->_if);
    }
    if (command->_while) {
        ast_while_free(command->_while);
    }
    if (command->block) {
        ast_block_free(command->block);
    }
    iloc_program_free(command->program);
    free(command);
};

void ast_attribution_free(attribution_t *attribution) {
    iloc_program_free(attribution->program);
    ast_identifier_free(attribution->variable_name);
    ast_expression_free(attribution->expression);
    free(attribution);
};

void ast_call_free(call_t *call) {
    iloc_program_free(call->program);
    ast_identifier_free(call->function_name);
    ast_arguments_free(call->arguments);
    free(call);
};

void ast_arguments_free(arguments_t *arguments) {
    for (uint64_t i = 0; i < arguments->len; i++) {
        ast_expression_free(arguments->arguments[i]);
    }
    iloc_program_free(arguments->program);
    free(arguments->arguments);
    free(arguments);
};

void ast_return_free(return_t *_return) {
    iloc_program_free(_return->program);
    ast_expression_free(_return->expression);
    free(_return);
};

void ast_if_free(if_t *_if) {
    iloc_program_free(_if->program);
    ast_expression_free(_if->condition);
    ast_block_free(_if->then_block);
    ast_block_free(_if->else_block);
    free(_if);
};

void ast_while_free(while_t *_while) {
    iloc_program_free(_while->program);
    ast_expression_free(_while->condition);
    ast_block_free(_while->block);
    free(_while);
};

void ast_block_free(block_t *block) {
    for (uint64_t i = 0; i < block->len; i++) {
        ast_command_free(block->commands[i]);
    }
    iloc_program_free(block->program);
    free(block->commands);
    free(block);
};

void ast_expression_free(expression_t *expression) {
    if (expression->_bin_op != NULL) {
        ast_bin_op_free(expression->_bin_op);
    }
    if (expression->_un_op != NULL) {
        ast_un_op_free(expression->_un_op);
    }
    if (expression->literal != NULL) {
        ast_literal_free(expression->literal);
    }
    if (expression->identifier != NULL) {
        ast_identifier_free(expression->identifier);
    }
    iloc_program_free(expression->program);
    free(expression);
};

void ast_bin_op_free(bin_op_t *bin_op) {
    iloc_program_free(bin_op->program);
    ast_expression_free(bin_op->left);
    ast_expression_free(bin_op->right);
    free(bin_op);
};

void ast_un_op_free(un_op_t *un_op) {
    iloc_program_free(un_op->program);
    ast_expression_free(un_op->expression);
    free(un_op);
};

void ast_literal_free(literal_t *literal) {
    if (literal->_int != NULL) {
        ast_int_free(literal->_int);
    }
    if (literal->_float != NULL) {
        ast_float_free(literal->_float);
    }
    if (literal->_bool != NULL) {
        ast_bool_free(literal->_bool);
    }
    iloc_program_free(literal->program);
    free(literal);
};

void ast_int_free(ast_int_t *_int) { 
    iloc_program_free(_int->program); 
    lexeme_free(_int->lexeme);
    free(_int); 
};

void ast_float_free(ast_float_t *_float) {
    iloc_program_free(_float->program);
    lexeme_free(_float->lexeme);
    free(_float);
};

void ast_bool_free(ast_bool_t *_bool) {
    iloc_program_free(_bool->program);
    lexeme_free(_bool->lexeme);
    free(_bool);
};

void ast_identifier_free(identifier_t *identifier) {
    iloc_program_free(identifier->program);
    lexeme_free(identifier->lexeme);
    free(identifier->text);
    free(identifier);
};

/* ###########
 * # Methods #
 * ########### */

void ast_program_add_child(program_t *program, global_t *global) {
    program->globals = (global_t **) realloc(program->globals, (program->len + 1) * sizeof(global_t *));
    program->globals[program->len] = global;
    program->len++;
}

void ast_variable_names_add_child(variable_names_t *variable_names, identifier_t *name) {
    variable_names->names = (identifier_t **) realloc(variable_names->names, (variable_names->len + 1) * sizeof(identifier_t *));
    variable_names->names[variable_names->len] = name;
    variable_names->len++;
}

void ast_parameters_add_child(parameters_t *parameters, variable_t *variable) {
    parameters->variables = (variable_t **)realloc(
            parameters->variables, (parameters->len + 1) * sizeof(variable_t *));
    parameters->variables[parameters->len] = variable;
    parameters->len++;
};

void ast_arguments_add_child(arguments_t *arguments, expression_t *argument) {
    arguments->arguments = (expression_t **)realloc(
            arguments->arguments, (arguments->len + 1) * sizeof(expression_t *));
    arguments->arguments[arguments->len] = argument;
    arguments->len++;
}

void ast_block_add_child(block_t *block, command_t *command) {
    block->commands = (command_t **)realloc(
            block->commands, (block->len + 1) * sizeof(command_t *));
    block->commands[block->len] = command;
    block->len++;
}

void *ast_program_export(program_t *program) {
    void *first_item = NULL;
    void *last_item = NULL;
    for (uint64_t i = 0; i < program->len; i++) {
        void *ptr = ast_global_export(program->globals[i]);
        if (ptr != NULL) {
            if (last_item != NULL) {
                printf("%p, %p\n", last_item, ptr);
            }
            last_item = ptr;
            if (first_item == NULL) {
                first_item = ptr;
            }
        }
    }
    return first_item;
}

void *ast_global_export(global_t *global) {
    if (global->variable != NULL) {
        return ast_variable_export(global->variable);
    } else {
        return ast_function_export(global->function);
    }
}

void *ast_function_export(function_t *function) {
    printf("%p [label=\"%s\"]\n", function, function->name->text);
    void *ptr = ast_block_export(function->body);
    if (ptr != NULL) {
        printf("%p, %p\n", function, ptr);
    }
    return function;
}

void *ast_parameters_export(parameters_t *parameters) {
    return NULL;
}

void *ast_variable_export(variable_t *variable) {
    return NULL;
}

void *ast_variable_names_export(variable_names_t *names) {
    return NULL;
}

void *ast_command_export(command_t *command) {
    void *ptr;
    if (command->attribution != NULL) {
        return ast_attribution_export(command->attribution);
    } else if (command->call != NULL) {
        return ast_call_export(command->call);
    } else if (command->_return != NULL) {
        return ast_return_export(command->_return);
    } else if (command->_if != NULL) {
        return ast_if_export(command->_if);
    } else if (command->_while != NULL) {
        return ast_while_export(command->_while);
    } else if (command->block != NULL) {
        return ast_block_export(command->block);
    } else {
        return ast_variable_export(command->variable);
    }
}

void *ast_attribution_export(attribution_t *attribution) {
    void *ptr;
    printf("%p [label=\"=\"]\n", attribution);
    ptr = ast_identifier_export(attribution->variable_name);
    printf("%p, %p\n", attribution, ptr);
    ptr = ast_expression_export(attribution->expression);
    printf("%p, %p\n", attribution, ptr);
    return attribution;
}

void *ast_call_export(call_t *call) {
    void *ptr;
    printf("%p [label=\"call %s\"]\n", call, call->function_name->text);
    ptr = ast_arguments_export(call->arguments);
    if (ptr != NULL) {
        printf("%p, %p\n", call, ptr);
    }
    return call;
}

void *ast_arguments_export(arguments_t *arguments) {
    void *first_item = NULL;
    void *last_item = NULL;
    for (uint64_t i = 0; i < arguments->len; i++) {
        void *ptr = ast_expression_export(arguments->arguments[i]);
        if (last_item != NULL) {
            printf("%p, %p\n", last_item, ptr);
        }
        last_item = ptr;
        if (first_item == NULL) {
            first_item = ptr;
        }
    }
    return first_item;
}

void *ast_return_export(return_t *_return) {
    void *ptr;
    printf("%p [label=\"return\"]\n", _return);
    ptr = ast_expression_export(_return->expression);
    printf("%p, %p\n", _return, ptr);
    return _return;
}

void *ast_if_export(if_t *_if) {
    void *ptr;
    printf("%p [label=\"if\"]\n", _if);
    ptr = ast_expression_export(_if->condition);
    printf("%p, %p\n", _if, ptr);
    ptr = ast_block_export(_if->then_block);
    if (ptr != NULL) {
        printf("%p, %p\n", _if, ptr);
    }
    ptr = ast_block_export(_if->else_block);
    if (ptr != NULL) {
        printf("%p, %p\n", _if, ptr);
    }
    return _if;
}

void *ast_while_export(while_t *_while) {
    void *ptr;
    printf("%p [label=\"while\"]\n", _while);
    ptr = ast_expression_export(_while->condition);
    printf("%p, %p\n", _while, ptr);
    ptr = ast_block_export(_while->block);
    if (ptr != NULL) {
        printf("%p, %p\n", _while, ptr);
    }
    return _while;
}

void *ast_block_export(block_t *block) {
    void *first_item = NULL;
    void *last_item = NULL;
    for (uint64_t i = 0; i < block->len; i++) {
        void *ptr = ast_command_export(block->commands[i]);
        //fprintf(stderr, "ast_command_export returned %p\n", ptr);
        if (ptr != NULL) {
            if (last_item != NULL) {
                printf("%p, %p\n", last_item, ptr);
            }
            last_item = ptr;
            if (first_item == NULL) {
                first_item = ptr;
            }
        }
    }
    return first_item;
}

void *ast_expression_export(expression_t *expression) {
    if (expression->literal != NULL) {
        return ast_literal_export(expression->literal);
    } else if (expression->call != NULL) {
        return ast_call_export(expression->call);
    } else if (expression->identifier != NULL) {
        return ast_identifier_export(expression->identifier);
    } else if (expression->_bin_op != NULL) {
        return ast_bin_op_export(expression->_bin_op);
    } else {
        return ast_un_op_export(expression->_un_op);
    }
}

char *bin_op_label(bin_op op) {
    switch (op) {
        case op_mul:
            return "*";
        case op_div:
            return "/";
        case op_mod:
            return "%";
        case op_add:
            return "+";
        case op_sub:
            return "-";
        case op_lt:
            return "<";
        case op_gt:
            return ">";
        case op_le:
            return "<=";
        case op_ge:
            return ">=";
        case op_eq:
            return "==";
        case op_ne:
            return "!=";
        case op_and:
            return "&";
        case op_or:
            return "|";
    }
}

char *un_op_label(un_op op) {
    switch (op) {
        case op_inv:
            return "-";
        case op_not:
            return "!";
    }
}

void *ast_bin_op_export(bin_op_t *_bin_op) {
    void *ptr;
    printf("%p [label=\"%s\"]\n", _bin_op, bin_op_label(_bin_op->op));
    ptr = ast_expression_export(_bin_op->left);
    printf("%p, %p\n", _bin_op, ptr);
    ptr = ast_expression_export(_bin_op->right);
    printf("%p, %p\n", _bin_op, ptr);
    return _bin_op;
}

void *ast_un_op_export(un_op_t *_un_op) {
    void *ptr;
    printf("%p [label=\"%s\"]\n", _un_op, un_op_label(_un_op->op));
    ptr = ast_expression_export(_un_op->expression);
    printf("%p, %p\n", _un_op, ptr);
    return _un_op;
}

void *ast_literal_export(literal_t *literal) {
    if (literal->_int != NULL) {
        return ast_int_export(literal->_int);
    } else if (literal->_float != NULL) {
        return ast_float_export(literal->_float);
    } else {
        return ast_bool_export(literal->_bool);
    }
}

void *ast_int_export(ast_int_t *_int) {
    printf("%p [label=\"%d\"]\n", _int, _int->value);
    return _int;
}

void *ast_float_export(ast_float_t *_float) {
    printf("%p [label=\"%f\"]\n", _float, _float->value);
    return _float;
}

void *ast_bool_export(ast_bool_t *_bool) {
    if (_bool->value == 0) {
        printf("%p [label=\"false\"]\n", _bool);
    } else {
        printf("%p [label=\"true\"]\n", _bool);
    }
    return _bool;
}

void *ast_identifier_export(identifier_t *identifier) {
    printf("%p [label=\"%s\"]\n", identifier, identifier->text);
    return identifier;
}

void ast_call_print(call_t *call) {
    fprintf(stderr, "%s(", call->function_name->text);
    for (uint64_t i = 0; i < call->arguments->len; i++) {
        if (i > 0) {
            fprintf(stderr, ", ");
        }
        ast_expression_print(call->arguments->arguments[i]);
    }
    fprintf(stderr, ")");
}

void ast_expression_print(expression_t *expression) {
    if (expression->literal != NULL) {
        return ast_literal_print(expression->literal);
    } else if (expression->call != NULL) {
        return ast_call_print(expression->call);
    } else if (expression->identifier != NULL) {
        return ast_identifier_print(expression->identifier);
    } else if (expression->_bin_op != NULL) {
        return ast_bin_op_print(expression->_bin_op);
    } else {
        return ast_un_op_print(expression->_un_op);
    }
}

void ast_bin_op_print(bin_op_t *_bin_op) {
    fprintf(stderr, "(");
    ast_expression_print(_bin_op->left);
    fprintf(stderr, ") %s (", bin_op_label(_bin_op->op));
    ast_expression_print(_bin_op->right);
    fprintf(stderr, ")");
}

void ast_un_op_print(un_op_t *_un_op) {
    fprintf(stderr, "%s", un_op_label(_un_op->op));
    ast_expression_print(_un_op->expression);
}

void ast_literal_print(literal_t *literal) {
    if (literal->_int != NULL) {
        fprintf(stderr, "%d", literal->_int->value);
    } else if (literal->_float != NULL) {
        fprintf(stderr, "%f", literal->_float->value);
    } else {
        fprintf(stderr, "%s", literal->_bool->value == 0 ? "false" : "true");
    }
}

void ast_identifier_print(identifier_t *identifier) {
    fprintf(stderr, "%s", identifier->text);
}

void ast_program_print_program(program_t *program) {
    iloc_program_to_string(program->program);
}

void ast_global_print_program(global_t *global) {
    iloc_program_to_string(global->program);
}

void ast_function_print_program(function_t *function) {
    iloc_program_to_string(function->program);
}

void ast_parameters_print_program(parameters_t *parameters) {
    iloc_program_to_string(parameters->program);
}

void ast_variable_print_program(variable_t *variable) {
    iloc_program_to_string(variable->program);
}

void ast_variable_names_print_program(variable_names_t *names) {
    iloc_program_to_string(names->program);
}

void ast_command_print_program(command_t *command) {
    iloc_program_to_string(command->program);
}

void ast_attribution_print_program(attribution_t *attribution) {
    iloc_program_to_string(attribution->program);
}

void ast_call_print_program(call_t *call) {
    iloc_program_to_string(call->program);
}

void ast_arguments_print_program(arguments_t *arguments) {
    iloc_program_to_string(arguments->program);
}

void ast_return_print_program(return_t *_return) {
    iloc_program_to_string(_return->program);
}

void ast_if_print_program(if_t *_if) {
    iloc_program_to_string(_if->program);
}

void ast_while_print_program(while_t *_while) {
    iloc_program_to_string(_while->program);
}

void ast_block_print_program(block_t *block) {
    iloc_program_to_string(block->program);
}

uint64_t ast_expression_print_program(expression_t *expression) {
    if (expression->_un_op != NULL) {
    } else if (expression->_bin_op != NULL) {
    } else if (expression->call != NULL) {
    } else if (expression->literal != NULL) {
    } else if (expression->identifier != NULL) {
    }
}

uint64_t ast_bin_op_print_program(bin_op_t *bin_op) {
    uint64_t left_value = ast_expression_print_program(bin_op->left);
    uint64_t right_value = ast_expression_print_program(bin_op->right);

    uint64_t value = iloc_next_id();

    uint64_t temp = iloc_next_id();
    uint64_t condition;
    uint64_t check_right = iloc_next_id();
    uint64_t case_true = iloc_next_id();
    uint64_t case_false = iloc_next_id();
    uint64_t done = iloc_next_id();

    iloc_program_append(bin_op->program, bin_op->left->program);

    switch (bin_op->op) {
        case op_mul:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(mult, left_value, right_value, value));
            break;
        case op_div:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(_div, left_value, right_value, value));
            break;
        case op_mod:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(_div, left_value, right_value, value));
            iloc_program_push(bin_op->program, iloc_instruction_new(mult, value, right_value, temp));
            iloc_program_push(bin_op->program, iloc_instruction_new(sub, left_value, temp, value));
            break;
        case op_add:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(add, left_value, right_value, value));
            break;
        case op_sub:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(sub, left_value, right_value, value));
            break;
        case op_lt:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_lt, left_value, right_value, value));
            break;
        case op_gt:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_gt, left_value, right_value, value));
            break;
        case op_le:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_le, left_value, right_value, value));
            break;
        case op_ge:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_ge, left_value, right_value, value));
            break;
        case op_eq:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_eq, left_value, right_value, value));
            break;
        case op_ne:
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_ne, left_value, right_value, value));
            break;
        case op_and:
            // Preparation
            iloc_program_push(bin_op->program, iloc_instruction_new(load_i, 0, temp, 0));
            // Compare left
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_eq, temp, left_value, condition));
            iloc_program_push(bin_op->program, iloc_instruction_new(cbr, condition, case_false, check_right));
            // Compare right
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(label, check_right, 0, 0));
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_eq, temp, right_value, condition));
            iloc_program_push(bin_op->program, iloc_instruction_new(cbr, condition, case_false, case_true));
            // If the expression evaluates to true, set value to true
            iloc_program_push(bin_op->program, iloc_instruction_new(label, case_true, 0, 0));
            iloc_program_push(bin_op->program, iloc_instruction_new(load_i, 1, value, 0));
            iloc_program_push(bin_op->program, iloc_instruction_new(jump_i, done, 0, 0));
            // If the expression evaluates to false, set value to false
            iloc_program_push(bin_op->program, iloc_instruction_new(label, case_false, 0, 0));
            iloc_program_push(bin_op->program, iloc_instruction_new(load_i, 0, value, 0));
            // Extra label to end the evaluation of the expression
            iloc_program_push(bin_op->program, iloc_instruction_new(label, done, 0, 0));
            break;

        case op_or:
            // Preparation
            iloc_program_push(bin_op->program, iloc_instruction_new(load_i, 0, temp, 0));
            // Compare left
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_eq, temp, left_value, condition));
            iloc_program_push(bin_op->program, iloc_instruction_new(cbr, condition, check_right, case_true));
            // Compare right
            iloc_program_append(bin_op->program, bin_op->right->program);
            iloc_program_push(bin_op->program, iloc_instruction_new(label, check_right, 0, 0));
            iloc_program_push(bin_op->program, iloc_instruction_new(cmp_eq, temp, right_value, condition));
            iloc_program_push(bin_op->program, iloc_instruction_new(cbr, condition, case_false, case_true));
            // If the expression evaluates to true, set value to true
            iloc_program_push(bin_op->program, iloc_instruction_new(label, case_true, 0, 0));
            iloc_program_push(bin_op->program, iloc_instruction_new(load_i, 1, value, 0));
            iloc_program_push(bin_op->program, iloc_instruction_new(jump_i, done, 0, 0));
            // If the expression evaluates to false, set value to false
            iloc_program_push(bin_op->program, iloc_instruction_new(label, case_false, 0, 0));
            iloc_program_push(bin_op->program, iloc_instruction_new(load_i, 0, value, 0));
            // Extra label to end the evaluation of the expression
            iloc_program_push(bin_op->program, iloc_instruction_new(label, done, 0, 0));
            break;
    }

    return value;
}

uint64_t ast_un_op_print_program(un_op_t *un_op) {
    uint64_t expr_value = ast_expression_print_program(un_op->expression);
    uint64_t value = iloc_next_id();
    uint64_t temp = iloc_next_id();
    uint64_t condition;
    uint64_t case_true = iloc_next_id();
    uint64_t case_false = iloc_next_id();
    uint64_t done = iloc_next_id();

    switch (un_op->op) {
        case op_inv:
            iloc_program_push(un_op->program, iloc_instruction_new(rsub_i, expr_value, 0, value));
            break;
        case op_not:
            iloc_program_push(un_op->program, iloc_instruction_new(load_i, 0, temp, 0));
            iloc_program_push(un_op->program, iloc_instruction_new(cmp_eq, temp, expr_value, condition));
            iloc_program_push(un_op->program, iloc_instruction_new(cbr, condition, case_false, case_true));
            iloc_program_push(un_op->program, iloc_instruction_new(label, case_true, 0, 0));
            iloc_program_push(un_op->program, iloc_instruction_new(load_i, 0, value, 0));
            iloc_program_push(un_op->program, iloc_instruction_new(jump_i, done, 0, 0));
            iloc_program_push(un_op->program, iloc_instruction_new(label, case_false, 0, 0));
            iloc_program_push(un_op->program, iloc_instruction_new(load_i, 1, value, 0));
            iloc_program_push(un_op->program, iloc_instruction_new(label, done, 0, 0));
            break;
    }

    return value;
}

uint64_t ast_literal_print_program(literal_t *literal) {
    iloc_program_t *inner_program;
    uint64_t value;
    if (literal->_bool != NULL) {
        inner_program = literal->_bool->program;
        value = ast_bool_print_program(literal->_bool);
    } else if (literal->_int != NULL) {
        inner_program = literal->_int->program;
        value = ast_int_print_program(literal->_int);
    } else {
        inner_program = literal->_float->program;
        value = ast_float_print_program(literal->_float);
    }
    iloc_program_append(literal->program, inner_program);
    return value;
}

uint64_t ast_int_print_program(ast_int_t *_int) {
    uint64_t value = iloc_next_id();
    iloc_program_push(_int->program, iloc_instruction_new(load_i, _int->value, value, 0));
    return value;
}

uint64_t ast_float_print_program(ast_float_t *_float) {
    uint64_t value = iloc_next_id();
    iloc_program_push(_float->program, iloc_instruction_new(load_i, (uint64_t) _float->value, value, 0));
    return value;
}

uint64_t ast_bool_print_program(ast_bool_t *_bool) {
    uint64_t value = iloc_next_id();
    iloc_program_push(_bool->program, iloc_instruction_new(load_i, _bool->value == 0 ? 0 : 1, value, 0));
    return value;
}

uint64_t ast_identifier_print_program(identifier_t *identifier) {
    uint64_t location, offset;
    uint64_t value = iloc_next_id();
    if (!get_variable(identifier->text, &location, &offset)) {
        fprintf(stderr, "ERROR: Semantic analysis failed. Variable \"%s\" accessed before being declared.", identifier->text);
        exit(ERR_UNDECLARED);
    }
    iloc_program_push(identifier->program, iloc_instruction_new(load_ai, location, offset, value));
    return value;
}

