#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "zvm.h"

// void * zinout_keyboard(zvm_vm_t *vm, uint8_t port, uint8_t operation_type, void *args){
//     return NULL;
// }

void zvm_init(zvm_vm_t *vm){
    vm->cpu.IP = 0;
    vm->cpu.IR = NULL;

    vm->cpu.R[0] = 0;
    vm->cpu.R[1] = 0;
    vm->cpu.R[2] = 0;
    vm->cpu.R[3] = 0;

    vm->cpu.OUTPUT = 0;
    vm->cpu.FLAGS  = 0;

    vm->cpu.DR = NULL;
    vm->cpu.CR = NULL;
    vm->cpu.SR = NULL;
    vm->cpu.SP = -1;
}

void zvm_init_program(zvm_vm_t* vm){
    #define VMP (vm->program)
    VMP.instructions_count = 0;
    VMP.data_count = 0;
    VMP.stack_counts = 0;

    memset(VMP.data, 0, ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE);
    memset(VMP.stack, 0, ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE);

    LDI(VMP, R0, 55)
    INC(VMP, R0)
    INC(VMP, R0)
    INC(VMP, R0)
    DEC(VMP, R0)
    INC(VMP, R0)
    DEC(VMP, R0)
    INC(VMP, R0)

    LDI(VMP, R0, 10)
    LDI(VMP, R1, 5)
    ADD(VMP, R2, R0, R1)  
    SUB(VMP, R2, R0, R1)  
    MUL(VMP, R2, R0, R1)  
    DIV(VMP, R2, R0, R1)  
    MOD(VMP, R2, R0, R1)  

    LDI(VMP, R0, 0x0F)
    LDI(VMP, R1, 0x03)
    AND(VMP, R2, R0, R1)  
    OR(VMP, R2, R0, R1)   
    XOR(VMP, R2, R0, R1)  
    NOT(VMP, R2, R0)      

    LDI(VMP, R0, 0)
    LDI(VMP, R1, 5)
    
   
    LDI(VMP, R0, 0)
    JZ(VMP, R0, 30)        
    LDI(VMP, R0, 99)       
    
    LDI(VMP, R0, 0)
    JNZ(VMP, R0, 30)       
    LDI(VMP, R0, 42)       
    
    LDI(VMP, R0, 10)
    LDI(VMP, R1, 10)
    JE(VMP, R0, R1, 35)    
    LDI(VMP, R0, 99)       
    
    LDI(VMP, R0, 10)
    LDI(VMP, R1, 20)
    JNE(VMP, R0, R1, 40)   
    LDI(VMP, R0, 99)       
    
    LDI(VMP, R0, 20)
    LDI(VMP, R1, 10)
    JG(VMP, R0, R1, 45)    
    LDI(VMP, R0, 99)       
    
    LDI(VMP, R0, 10)
    LDI(VMP, R1, 10)
    JGE(VMP, R0, R1, 50)   
    LDI(VMP, R0, 99)       
    
    LDI(VMP, R0, 5)
    LDI(VMP, R1, 10)
    JL(VMP, R0, R1, 55)    
    LDI(VMP, R0, 99)       
    
    LDI(VMP, R0, 10)
    LDI(VMP, R1, 10)
    JLE(VMP, R0, R1, 60)   
    LDI(VMP, R0, 99)       
    
    LDI(VMP, R0, 0)        
    LDI(VMP, R0, 0)        

    #undef VMP
}

/* raise exception */
static bool fetch(zvm_vm_t *vm){
    vm->cpu.IR = &vm->program.instructions[vm->cpu.IP];
    vm->cpu.IP++;
    return true;
}

static bool decode(zvm_vm_t* vm){
    // TODO:
    // if(!zvm_is_valid_instruction(vm->cpu.IR->metadata->opcode)){
    //     zvm_raise(vm, DECODE, DECODE)
    //     return false;
    // }

    /* input */
    uint8_t val0 = vm->cpu.IR->operands[0].value;
    uint8_t val1 = vm->cpu.IR->operands[1].value;
    uint8_t val2 = vm->cpu.IR->operands[2].value;

    switch(vm->cpu.IR->metadata->handler.type){
        case ZVM_INST_HANDLER_TYPE_RRR:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 >= ZVM_RX_REGISTERS_COUNT
               || val2 >= ZVM_RX_REGISTERS_COUNT){
                zvm_raise(vm, DECODE, INVALID_REGISTER)
                return false;
            }
        break;
         
        case ZVM_INST_HANDLER_TYPE_RI:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
            //    || val1 >= 256
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        case ZVM_INST_HANDLER_TYPE_RM:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 >= ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        case ZVM_INST_HANDLER_TYPE_RR:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 >= ZVM_RX_REGISTERS_COUNT
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        case ZVM_INST_HANDLER_TYPE_R:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 != 0
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        case ZVM_INST_HANDLER_TYPE_I:
            if(val0 >= ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE
               || val1 != 0
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        case ZVM_INST_HANDLER_TYPE_RRI:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 >= ZVM_RX_REGISTERS_COUNT
               || val2 >= ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        default:
            zvm_raise(vm, DECODE, DECODE)
            return false;
    }

    return true;
}

static bool execute(zvm_vm_t* vm){

    int8_t instruction_index = vm->cpu.IR->metadata->handler.type;
    if(instruction_index >= ZVM_INSTRUCTION_HANDLERS_COUNT){
        zvm_raise(vm, EXECUTE, EXECUTE);
        return false;
    }
    
    zfn_instruction_handler_t handler = (zfn_instruction_handler_t)vm->cpu.IR->metadata->handler.action;
    bool instruction_result = false;

    instruction_result = handler(vm, vm->cpu.IR->operands[0].value,
                                 vm->cpu.IR->operands[1].value,
                                 vm->cpu.IR->operands[2].value);

    return instruction_result;
}

bool except(zvm_vm_t* vm){
    if(vm->has_exception){
        int8_t code = zvm_exception_get_code(vm);

        if(zvm_exception_is_handler(code)){
            return exception_handlers[code]
                            .handler(ZVM_EXCEPTION_HANDLER_FUNCTION_ARGS);
        }else{
            fprintf(stderr, "Invalid exception handler code\n");
            return false;
        }
    }
    
    return true;
}


/* TODO:
   - Remove zvm_init_program?
*/

int main(void){
    /* declaration */
    zvm_vm_t vm;

    /* initialization */
    zvm_init(&vm);
    zvm_init_program(&vm);
    
    /* execution */
    while(zvm_has_next_instruction(&vm)){
        /* fetch */
        if(!fetch(&vm)){
            zvm_raise(&vm, FETCH, FETCH)
            goto zvm_catch;
        }
        /* decode  */
        if(!decode(&vm)){
            zvm_raise(&vm, DECODE, DECODE)
            goto zvm_catch;
        }
        /* execute */
        if(!execute(&vm)){
            goto zvm_catch;
        }
        continue;
    /* exception handling */
    zvm_catch:
        if(!except(&vm)){
            break;
        }else{
            zvm_exception_reset(&vm)
        }
    }

    if(vm.has_exception){
        fprintf(stderr, "Exception(%d): %s\n", vm.exception_code,
                    exception_handlers[vm.exception_code].message);
    }

    /* finalization */

    return 0;
}