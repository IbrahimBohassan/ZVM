#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "zvm.h"

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

void zvm_init_io(zvm_vm_t *vm){
    vm->io_devices[0] = &keyboard;
    vm->io_devices[1] = &screen;
    vm->io_devices[2] = NULL;
    vm->io_devices[3] = NULL;
}

void zvm_init_program(zvm_vm_t* vm){
    #define VMP (vm->program)
    VMP.instructions_count = 0;
    VMP.data_count = 0;
    VMP.stack_counts = 0;

    memset(VMP.data, 0, ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE);
    memset(VMP.stack, 0, ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE);

    // LDI(VMP, R3, 'Z')
    // DEC(VMP, R3)
    
    LDI(VMP, R0, ZVM_IO_PORT_KEYBOARD)
    LDI(VMP, R1, ZVM_IO_SERVICE_KEYBOARD_READ_CHARACTER)
    LDI(VMP, R2, 0)

    IN(VMP, R0, R1, R2)

    PUSH(VMP, R3)

    LDI(VMP, R0, ZVM_IO_PORT_SCREEN)
    LDI(VMP, R1, ZVM_IO_SERVICE_SCREEN_WRITE_CHARACTER)
    LDI(VMP, R2, 1)

    OUT(VMP, R0, R1, R2)

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

uint8_t zvm_stack_pop(zvm_vm_t *vm)
{
    uint8_t top = vm->program.stack[(ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE - 1) - vm->cpu.SP];
    vm->cpu.SP--;
    return top;
}

/* TODO:
   - Remove zvm_init_program?
*/

int main(void){
    /* declaration */
    zvm_vm_t vm;

    /* initialization */
    zvm_init(&vm);
    zvm_init_io(&vm);
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