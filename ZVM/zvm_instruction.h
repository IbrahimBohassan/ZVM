#ifndef __ZVM_INSTRUCTION_H__
#define __ZVM_INSTRUCTION_H__
#include <stdint.h>
#include <stdbool.h>
#define ZVM_INSTRUCTION_MAX_OPCODE 255
#define zvm_is_valid_instruction(opcode) ((opcode) < ZVM_INSTRUCTION_HANDLERS_COUNT)

typedef struct zvm_vm_t zvm_vm_t;
typedef bool (*zfn_instruction_handler_t)(zvm_vm_t* vm, uint8_t, uint8_t, uint8_t);

typedef struct zvm_instruction_operand_t zvm_instruction_operand_t;
typedef struct zvm_instruction_t zvm_instruction_t;

typedef struct zvm_instruction_metadata_t zvm_instruction_metadata_t;
typedef struct zvm_operand_metadata_t zvm_operand_metadata_t;

#define ZVM_INSTRUCTION_HANDLER_FUNCTION_NAME(name)   zfnx_ ## name
#define ZVM_INSTRUCTION_HANDLER_FUNCTION_PARAM \
    zvm_vm_t* vm, uint8_t left, uint8_t right, uint8_t output

#define ZVM_INSTRUCTION_HANDLER_FUNCTION_ARGS \
    vm, left, right, output

#define ZVM_INSTRUCTION_HANDLER_FUNCTION(name)  \
    bool ZVM_INSTRUCTION_HANDLER_FUNCTION_NAME(name) (ZVM_INSTRUCTION_HANDLER_FUNCTION_PARAM)


#define ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(name) \
    ZVM_INSTRUCTION_HANDLER_FUNCTION(name) {

#define ZVM_INSTRUCTION_HANDLER_FUNCTION_END    \
        return true;                            \
    }

#define ZVM_INSTRUCTION_HANDLER_CALL(name) zfnx_ ## name(ZVM_INSTRUCTION_HANDLER_FUNCTION_ARGS)

#define ZVM_OPERAND_TYPE_REG 0
#define ZVM_OPERAND_TYPE_MEM 1
#define ZVM_OPERAND_TYPE_IMM 2

typedef struct zvm_operand_metadata_t {
    uint16_t type:2; /* ZVM_OPERAND_XXX */
}zvm_operand_metadata_t;

typedef struct zvm_instruction_operand_t{
    zvm_operand_metadata_t* metadata;
    uint8_t value;
}zvm_instruction_operand_t;

#define ZVM_INST_0_OP  0
#define ZVM_INST_1_OP  1
#define ZVM_INST_2_OP  2
#define ZVM_INST_3_OP  3

#define ZVM_INST_MAX_OPERANDS  3


/* instructions opcodes */
// #define ZVM_OPCODE_STM        5 /*  STM mem, imm       */

#define ZVM_OPCODE_ADD        0  /*  ADD reg, reg, reg  */
#define ZVM_OPCODE_SUB        1  /*  SUB reg, reg, reg  */
#define ZVM_OPCODE_LDI        2  /*  LDI reg, imm       */
#define ZVM_OPCODE_STR        3  /*  STR reg, mem       */
#define ZVM_OPCODE_LDM        4  /*  LDM reg, mem       */
#define ZVM_OPCODE_LDA        5  /*  LDA reg, reg       */
#define ZVM_OPCODE_STA        6  /*  STA reg, reg       */
#define ZVM_OPCODE_PUSH       7  /*  PUSH reg           */
#define ZVM_OPCODE_POP        8  /*  POP reg            */
#define ZVM_OPCODE_INC        9  /*  INC reg            */
#define ZVM_OPCODE_DEC        10 /*  DEC reg            */
#define ZVM_OPCODE_AND        11 /*  AND reg, reg, reg  */
#define ZVM_OPCODE_OR         12 /*  OR  reg, reg, reg  */
#define ZVM_OPCODE_XOR        13 /*  XOR reg, reg, reg  */
#define ZVM_OPCODE_NOT        14 /*  NOT reg, reg       */
#define ZVM_OPCODE_MUL        15 /*  MUL reg, reg, reg  */
#define ZVM_OPCODE_DIV        16 /*  DIV reg, reg, reg  */
#define ZVM_OPCODE_MOD        17 /*  MOD reg, reg, reg  */
#define ZVM_OPCODE_JMP        18 /*  JMP imm            */
#define ZVM_OPCODE_JZ         19 /*  JZ  reg, imm       */
#define ZVM_OPCODE_JNZ        20 /*  JNZ reg, imm       */
#define ZVM_OPCODE_JE         21 /*  JE  reg, reg, imm  */
#define ZVM_OPCODE_JNE        22 /*  JNE reg, reg, imm  */
#define ZVM_OPCODE_JG         23 /*  JG  reg, reg, imm  */
#define ZVM_OPCODE_JGE        24 /*  JGE reg, reg, imm  */
#define ZVM_OPCODE_JL         25 /*  JL  reg, reg, imm  */
#define ZVM_OPCODE_JLE        26 /*  JLE reg, reg, imm  */


// LDI R0, 15
// TODO: PUSH POP

/* instructions handlers */
ZVM_INSTRUCTION_HANDLER_FUNCTION(add);
ZVM_INSTRUCTION_HANDLER_FUNCTION(sub);
ZVM_INSTRUCTION_HANDLER_FUNCTION(ldi);
ZVM_INSTRUCTION_HANDLER_FUNCTION(str);
ZVM_INSTRUCTION_HANDLER_FUNCTION(ldm);
ZVM_INSTRUCTION_HANDLER_FUNCTION(lda);
ZVM_INSTRUCTION_HANDLER_FUNCTION(sta);

ZVM_INSTRUCTION_HANDLER_FUNCTION(push);
ZVM_INSTRUCTION_HANDLER_FUNCTION(pop);

ZVM_INSTRUCTION_HANDLER_FUNCTION(inc);
ZVM_INSTRUCTION_HANDLER_FUNCTION(dec);

ZVM_INSTRUCTION_HANDLER_FUNCTION(and);
ZVM_INSTRUCTION_HANDLER_FUNCTION(or);
ZVM_INSTRUCTION_HANDLER_FUNCTION(xor);
ZVM_INSTRUCTION_HANDLER_FUNCTION(not);
ZVM_INSTRUCTION_HANDLER_FUNCTION(mul);
ZVM_INSTRUCTION_HANDLER_FUNCTION(div);
ZVM_INSTRUCTION_HANDLER_FUNCTION(mod);

ZVM_INSTRUCTION_HANDLER_FUNCTION(jmp);
ZVM_INSTRUCTION_HANDLER_FUNCTION(jz);
ZVM_INSTRUCTION_HANDLER_FUNCTION(jnz);
ZVM_INSTRUCTION_HANDLER_FUNCTION(je);
ZVM_INSTRUCTION_HANDLER_FUNCTION(jne);
ZVM_INSTRUCTION_HANDLER_FUNCTION(jg);
ZVM_INSTRUCTION_HANDLER_FUNCTION(jge);
ZVM_INSTRUCTION_HANDLER_FUNCTION(jl);
ZVM_INSTRUCTION_HANDLER_FUNCTION(jle);



#define ZVM_INST_HANDLER_TYPE_RRR  0
#define ZVM_INST_HANDLER_TYPE_RI   1
#define ZVM_INST_HANDLER_TYPE_RM   2
#define ZVM_INST_HANDLER_TYPE_RR   3
#define ZVM_INST_HANDLER_TYPE_R    4
#define ZVM_INST_HANDLER_TYPE_I    5
#define ZVM_INST_HANDLER_TYPE_RRI  6




typedef struct zvm_instruction_handler_t zvm_instruction_handler_t;

struct zvm_instruction_handler_t{
    int8_t type;
    void *action;
};

typedef struct zvm_instruction_metadata_t{
    char *mnemonic;
    uint8_t opcode;
    uint8_t operands_count;
    zvm_operand_metadata_t operands[ZVM_INST_MAX_OPERANDS];
    zvm_instruction_handler_t handler;
}zvm_instruction_metadata_t;


#define ZVM_INSTRUCTION_HANDLERS_COUNT (sizeof(instruction_handlers) / sizeof(zvm_instruction_metadata_t))

// ZVM_INSTRUCTION_HANDLER_ENTRY(ADD, add, 3, RRR)
#define ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, argc) \
    [ZVM_OPCODE_ ## name] = {#name, ZVM_OPCODE_ ## name, ZVM_INST_ ## argc ## _OP, {

#define ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, type) \
    },{ZVM_INST_HANDLER_TYPE_ ## type, ZVM_INSTRUCTION_HANDLER_FUNCTION_NAME(fn)}},

#define ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(type) \
    {ZVM_OPERAND_TYPE_ ## type},

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 3) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RRR)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RI(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 2) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(IMM) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RI)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RM(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 2) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(MEM) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RM)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RR(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 2) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RR)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_R(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 1) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, R)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 3) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(IMM) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RRI)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_I(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 1) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(IMM) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, I)

static const zvm_instruction_metadata_t instruction_handlers [ZVM_INSTRUCTION_MAX_OPCODE + 1] = {
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(ADD, add)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(SUB, sub)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RI(LDI, ldi)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RM(STR, str)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RM(LDM, ldm)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RR(LDA, lda)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RR(STA, sta)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(PUSH, push)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(POP, pop)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(INC, inc)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(DEC, dec)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(AND, and)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(OR, or)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(XOR, xor)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RR(NOT, not)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(MUL, mul)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(DIV, div)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(MOD, mod)
    ZVM_INSTRUCTION_HANDLER_ENTRY_I(JMP, jmp)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(JZ, jz)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(JNZ, jnz)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(JE, je)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(JNE, jne)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(JG, jg)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(JGE, jge)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(JL, jl)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRI(JLE, jle)

};

struct zvm_instruction_t {
    zvm_instruction_metadata_t *metadata;
    zvm_instruction_operand_t operands[ZVM_INST_MAX_OPERANDS];
};


#define zvm_has_next_instruction(vm) \
  ((vm)->cpu.IP < (vm)->program.instructions_count)

#define zvm_set_instruction(p, i, opcode, v1, v2, v3)\
    (p).instructions[(i)].metadata = (zvm_instruction_metadata_t*)&instruction_handlers[ZVM_OPCODE_ ## opcode];\
    (p).instructions[(i)].operands[0].metadata = (zvm_operand_metadata_t*)&instruction_handlers[ZVM_OPCODE_ ## opcode].operands[0];\
    (p).instructions[(i)].operands[1].metadata = (zvm_operand_metadata_t*)&instruction_handlers[ZVM_OPCODE_ ## opcode].operands[1];\
    (p).instructions[(i)].operands[2].metadata = (zvm_operand_metadata_t*)&instruction_handlers[ZVM_OPCODE_ ## opcode].operands[2];\
    (p).instructions[(i)].operands[0].value = (v1); \
    (p).instructions[(i)].operands[1].value = (v2); \
    (p).instructions[(i)].operands[2].value = (v3);

#define R0  0
#define R1  1
#define R2  2
#define R3  3

#define INST(p, opcode, v1, v2, v3) \
    zvm_set_instruction(p, ((p).instructions_count), opcode, v1, v2, v3) \
    (p).instructions_count++;


// #define ADD(p, v1, v2, v3) INST(p, ADD, v1, v2, v3)
// #define SUB(p, v1, v2, v3) INST(p, SUB, v1, v2, v3)
#define LDI(p, v1, v2) INST(p, LDI, v1, v2, 0)

#define LDM(p, v1, v2) INST(p, LDM, v1, v2, 0)
#define STR(p, v1, v2) INST(p, STR, v1, v2, 0)

#define LDA(p, v1, v2) INST(p, LDA, v1, v2, 0)
#define STA(p, v1, v2) INST(p, STA, v1, v2, 0)

// #define PUSH(p, v1) INST(p, PUSH, v1, 0, 0)
// #define POP(p, v1) INST(p, POP, v1, 0, 0)

// #define INC(p, v1) INST(p, INC, v1, 0, 0)
// #define DEC(p, v1) INST(p, DEC, v1, 0, 0)

#define ADD(p, v1, v2, v3) INST(p, ADD, v2, v3, v1)  // left=v2, right=v3, output=v1
#define SUB(p, v1, v2, v3) INST(p, SUB, v2, v3, v1)
#define AND(p, v1, v2, v3) INST(p, AND, v2, v3, v1)
#define OR(p, v1, v2, v3)  INST(p, OR, v2, v3, v1)
#define XOR(p, v1, v2, v3) INST(p, XOR, v2, v3, v1)
#define MUL(p, v1, v2, v3) INST(p, MUL, v2, v3, v1)
#define DIV(p, v1, v2, v3) INST(p, DIV, v2, v3, v1)
#define MOD(p, v1, v2, v3) INST(p, MOD, v2, v3, v1)

// For 2-operand instructions: NOT, LDA, STA, etc.
#define NOT(p, v1, v2) INST(p, NOT, v1, v2, 0)  // left=v1, right=v2
#define LDA(p, v1, v2) INST(p, LDA, v1, v2, 0)  // left=v1, right=v2
#define STA(p, v1, v2) INST(p, STA, v1, v2, 0)  // left=v1, right=v2

// For jump instructions
#define JZ(p, v1, v2)  INST(p, JZ, v1, v2, 0)
#define JNZ(p, v1, v2) INST(p, JNZ, v1, v2, 0)
#define JE(p, v1, v2, v3)  INST(p, JE, v1, v2, v3)
#define JNE(p, v1, v2, v3) INST(p, JNE, v1, v2, v3)
#define JG(p, v1, v2, v3)  INST(p, JG, v1, v2, v3)
#define JGE(p, v1, v2, v3) INST(p, JGE, v1, v2, v3)
#define JL(p, v1, v2, v3)  INST(p, JL, v1, v2, v3)
#define JLE(p, v1, v2, v3) INST(p, JLE, v1, v2, v3)

// 1-operand instructions
#define INC(p, v1) INST(p, INC, v1, 0, 0)
#define DEC(p, v1) INST(p, DEC, v1, 0, 0)
#define PUSH(p, v1) INST(p, PUSH, v1, 0, 0)
#define POP(p, v1) INST(p, POP, v1, 0, 0)
#define JMP(p, v1) INST(p, JMP, v1, 0, 0)

#endif