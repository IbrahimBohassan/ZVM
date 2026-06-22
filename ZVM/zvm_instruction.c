#include <stdio.h>
#include "zvm_instruction.h"
#include "zvm.h"

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(add)
    vm->cpu.R[output] = vm->cpu.R[left] + vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(sub)
    vm->cpu.R[output] = vm->cpu.R[left] - vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(ldi)
    vm->cpu.R[left] = right;
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(str)
    /* STR reg, mem */
    vm->program.data[right] = vm->cpu.R[left];
    printf("MEM=%u\n", vm->program.data[right]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(ldm)
    /* LDM reg, mem */
    vm->cpu.R[left] = vm->program.data[right];
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(lda)
    uint8_t address = vm->cpu.R[left];
    if(!zvm_is_address(address)){
        zvm_raise(vm, EXECUTE, BAD_MEMORY_ADDRESS)
        return false;
    }
    vm->cpu.R[right] = vm->program.data[address];
    printf("R=%u\n", vm->cpu.R[right]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(sta)
    /* STA reg, reg */
    uint8_t address = vm->cpu.R[left];
    if(!zvm_is_address(address)){
        zvm_raise(vm, EXECUTE, BAD_MEMORY_ADDRESS)
        return false;
    }
    vm->program.data[address] = vm->cpu.R[right];
    printf("address=%u\n", vm->program.data[address]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(push)
    uint8_t value;
    if(vm->cpu.SP >= ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE){
        zvm_raise(vm, EXECUTE, STACK_OVERFLOW)
        return false;
    }

    vm->cpu.SP++;
    value = vm->cpu.R[left];
    vm->program.stack[(ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE - 1) - vm->cpu.SP] = value;
    printf("stack=%u\n", value);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(pop)
    uint8_t value;
    if(vm->cpu.SP < 0){
        zvm_raise(vm, EXECUTE, STACK_UNDERFLOW)
        return false;
    }

    value = vm->program.stack[(ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE - 1) - vm->cpu.SP];
    vm->cpu.SP--;
    
    vm->cpu.R[left] = value;
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(inc)
    /* INC reg*/
    vm->cpu.R[left]++;
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(dec)
    /* DEC reg*/
    vm->cpu.R[left]--;
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(and)
    vm->cpu.R[output] = vm->cpu.R[left] & vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(or)
    vm->cpu.R[output] = vm->cpu.R[left] | vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(xor)
    vm->cpu.R[output] = vm->cpu.R[left] ^ vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(not)
    vm->cpu.R[right] = ~vm->cpu.R[left];
    printf("R=%u\n", vm->cpu.R[right]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(mul)
    vm->cpu.R[output] = vm->cpu.R[left] * vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(div)
    if(vm->cpu.R[right] == 0){
        zvm_raise(vm, EXECUTE, EXECUTE)
        return false;
    }
    vm->cpu.R[output] = vm->cpu.R[left] / vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(mod)
    if(vm->cpu.R[right] == 0){
        zvm_raise(vm, EXECUTE, EXECUTE)
        return false;
    }
    vm->cpu.R[output] = vm->cpu.R[left] % vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(jmp)
    /* JMP imm */
    if(left >= vm->program.instructions_count){
        zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
        return false;
    }
    vm->cpu.IP = left - 1;  // Subtract 1 because IP will be incremented after execute
    printf("JMP to %u\n", left);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(jz)
    /* JZ reg, imm */
    if(vm->cpu.R[left] == 0){
        if(right >= vm->program.instructions_count){
            zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
            return false;
        }
        vm->cpu.IP = right - 1;  
        printf("JZ to %u\n", right);
    }
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(jnz)
    /* JNZ reg, imm */
    if(vm->cpu.R[left] != 0){
        if(right >= vm->program.instructions_count){
            zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
            return false;
        }
        vm->cpu.IP = right - 1;  
        printf("JNZ to %u\n", right);
    }
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(je)
    /* JE reg, reg, imm */
    if(vm->cpu.R[left] == vm->cpu.R[right]){
        if(output >= vm->program.instructions_count){
            zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
            return false;
        }
        vm->cpu.IP = output - 1;  
        printf("JE to %u\n", output);
    }
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(jne)
    /* JNE reg, reg, imm */
    if(vm->cpu.R[left] != vm->cpu.R[right]){
        if(output >= vm->program.instructions_count){
            zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
            return false;
        }
        vm->cpu.IP = output - 1;  
        printf("JNE to %u\n", output);
    }
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(jg)
    /* JG reg, reg, imm (signed comparison) */
    if((int8_t)vm->cpu.R[left] > (int8_t)vm->cpu.R[right]){
        if(output >= vm->program.instructions_count){
            zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
            return false;
        }
        vm->cpu.IP = output - 1;  
        printf("JG to %u\n", output);
    }
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(jge)
    /* JGE reg, reg, imm */
    if((int8_t)vm->cpu.R[left] >= (int8_t)vm->cpu.R[right]){
        if(output >= vm->program.instructions_count){
            zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
            return false;
        }
        vm->cpu.IP = output - 1;  
        printf("JGE to %u\n", output);
    }
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(jl)
    /* JL reg, reg, imm */
    if((int8_t)vm->cpu.R[left] < (int8_t)vm->cpu.R[right]){
        if(output >= vm->program.instructions_count){
            zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
            return false;
        }
        vm->cpu.IP = output - 1;  
        printf("JL to %u\n", output);
    }
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(jle)
    /* JLE reg, reg, imm */
    if((int8_t)vm->cpu.R[left] <= (int8_t)vm->cpu.R[right]){
        if(output >= vm->program.instructions_count){
            zvm_raise(vm, EXECUTE, BAD_INSTRUCTION)
            return false;
        }
        vm->cpu.IP = output - 1;  
        printf("JLE to %u\n", output);
    }
ZVM_INSTRUCTION_HANDLER_FUNCTION_END