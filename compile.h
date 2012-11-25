#ifndef COMPILE_H
#define COMPILE_H
#include <stdint.h>
#include "bytecode.h"
#include "opcode.h"
#include "locfile.h"

struct inst;
typedef struct inst inst;


typedef struct block {
  inst* first;
  inst* last;
} block;

block gen_location(location, block);

block gen_noop();
block gen_op_simple(opcode op);
block gen_op_const(opcode op, jv constant);
block gen_op_target(opcode op, block target);
block gen_op_var_unbound(opcode op, const char* name);
block gen_op_var_bound(opcode op, block binder);
block gen_op_block_defn(opcode op, const char* name, block block);
block gen_op_block_defn_rec(opcode op, const char* name, block block);
block gen_op_block_unbound(opcode op, const char* name);
block gen_op_call(opcode op, block arglist);
block gen_op_symbol(opcode op, const char* name);

block gen_subexp(block a);
block gen_both(block a, block b);
block gen_collect(block expr);
block gen_assign(block expr);
block gen_definedor(block a, block b);
block gen_condbranch(block iftrue, block iffalse);
block gen_and(block a, block b);
block gen_or(block a, block b);

block gen_cond(block cond, block iftrue, block iffalse);

block gen_cbinding(struct symbol_table* functions, block b);

void block_append(block* b, block b2);
block block_join(block a, block b);
int block_has_only_binders(block, int bindflags);
block block_bind(block binder, block body, int bindflags);

int block_compile(block, struct locfile*, struct bytecode**);

void block_free(block);



// Here's some horrible preprocessor gunk so that code
// sequences can be contructed as BLOCK(block1, block2, block3)

#define BLOCK_1(b1) (b1)
#define BLOCK_2(b1,b2) (block_join((b1),(b2)))
#define BLOCK_3(b1,b2,b3) (block_join(BLOCK_2(b1,b2),(b3)))
#define BLOCK_4(b1,b2,b3,b4) (block_join(BLOCK_3(b1,b2,b3),(b4)))
#define BLOCK_5(b1,b2,b3,b4,b5) (block_join(BLOCK_4(b1,b2,b3,b4),(b5)))
#define BLOCK_6(b1,b2,b3,b4,b5,b6) (block_join(BLOCK_5(b1,b2,b3,b4,b5),(b6)))
#define BLOCK_7(b1,b2,b3,b4,b5,b6,b7) (block_join(BLOCK_6(b1,b2,b3,b4,b5,b6),(b7)))

#define BLOCK_IDX(_1,_2,_3,_4,_5,_6,_7,NAME,...) NAME
#define BLOCK(...) \
  BLOCK_IDX(__VA_ARGS__, BLOCK_7, BLOCK_6, BLOCK_5, BLOCK_4, BLOCK_3, BLOCK_2, BLOCK_1)(__VA_ARGS__)


#endif