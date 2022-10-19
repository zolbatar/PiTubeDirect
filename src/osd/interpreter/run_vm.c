#include "wasm/load_wasm.h"
#include "wasm_shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "../../cache.h"
#include "lightning/lightning.h"
#include "lightning/jit_private.h"
#include <capstone/capstone.h>
#include <capstone/platform.h>

size_t SP;
jit_state_t *_jit;
typedef int (*start)(void);

#define VERBOSE_COMPILE 1

static void setup_capstone()
{
#ifndef DISABLE_DISASM
    // Setup capstone
    cs_opt_mem setup;
    setup.malloc = malloc;
    setup.calloc = calloc;
    setup.realloc = realloc;
    setup.free = free;
    setup.vsnprintf = vsnprintf;
    cs_err err = cs_option(0, CS_OPT_MEM, (size_t)&setup);
    if (err != CS_ERR_OK)
    {
        print("Error (cs_option): %d\n", err);
    }
#endif
}

static void disassemble(start exec, jit_word_t sz)
{
#ifndef DISABLE_DISASM
    // Disassemble
    csh handle;
    cs_insn *insn;
    size_t count;
#ifdef CAPSTONE_HAS_X86
    print("Architecture: X86\n");
    cs_err err = cs_open(CS_ARCH_X86, CS_MODE_64, &handle);
#else
#ifdef PITUBE
    print("Architecture: ARM\n");
    cs_err err = cs_open(CS_ARCH_ARM, CS_MODE_ARM, &handle);
#else
    print("Architecture: AARCH64\n");
    cs_err err = cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle);
#endif
#endif
    if (err != CS_ERR_OK)
    {
        print("Disassemble error: %d\n", err);
    }
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    count = cs_disasm(handle, (const unsigned char *)exec, sz, (size_t)_jit->code.ptr, 0, &insn);
    print("There are %d CPU instructions\n", count);
    if (count > 0)
    {
        size_t j;
        for (j = 0; j < count; j++)
        {
#ifdef PITUBE
            print("0x%X:\t%s\t%s\n", (uint32_t)insn[j].address, insn[j].mnemonic, insn[j].op_str);
#else
            print("0x%" PRIx64 ":\t%s\t%s\n", insn[j].address, insn[j].mnemonic, insn[j].op_str);
#endif
        }
        cs_free(insn, count);
    }
    else
    {
        print("ERROR: Failed to disassemble given code!");
    }
    cs_close(&handle);
#endif
}

void printtest()
{
    print("Test\n");
}

void compile_function(wasm_t *wat, section_code_t *func)
{
    for (size_t i = 0; i < cvec_instruction_t_size(&func->code); i++)
    {
        const cvec_instruction_t_value *ins = cvec_instruction_t_at(&func->code, i);
        switch (ins->opcode)
        {
        case INSTRUCTION_END:
            break;
        case INSTRUCTION_CALL:
            break;
        case INSTRUCTION_I32_CONST:
            jit_movi(JIT_R0, ins->value_i32);
            stack_push_int(JIT_R0);
            break;

        default:
            print("Unknown opcode during native compile\n");
            exit(1);
        }
    }
}

void run_vm(wasm_t *wat, uint8_t *_RAM, size_t RAM_size, uint32_t heap_start)
{
    setup_capstone();
    init_jit("Daric");
    _jit = jit_new_state();

    jit_prolog();
    stack_init();

    // Compile all functions
    jit_movi(JIT_R0, (jit_word_t)&printtest);
    jit_prepare();
    jit_finishr(JIT_R0);
    for (size_t i = 0; i < wat->num_codes; i++)
    {
        print("Compiling function %d\n", i);
        compile_function(wat, &wat->section_codes[i]);
    }

    /* Remember to setup calls to ctor and start function
     *
     * wat->ctors_function
     * wat->start_function
     */

    // Code & data size
    jit_realize();
    if (!_jitc->realize)
    {
        print("Failed to realise");
    }

    // Do compile
    jit_word_t sz = _jit->code.length;
#ifndef RICH
    void *code = malloc(sz);

    // jit_set_code(code, sz);
    jit_set_data(NULL, 0, JIT_DISABLE_NOTE | JIT_DISABLE_NOTE);
#endif
    start exec = jit_emit_void();
    if (exec == NULL)
    {
        print("Code generation failed");
    }

    // Size?
    jit_word_t code_size;
    jit_get_code(&code_size);
#ifdef VERBOSE_COMPILE
    print("Code size: %ld [%ld] bytes\n", code_size, sz);
#endif

#ifndef DISABLE_DISASM
    disassemble(exec, code_size);
#endif

    jit_clear_state();
    print("Preparing to execute\n");
    // Invalidate entire data cache
#if defined(RPI2) || defined(RPI3) || defined(RPI4)
    asm volatile("isb" ::: "memory");
    InvalidateDataCache();
#else
    // invalidate data cache and flush prefetch buffer
    // NOTE: The below code seems to cause a Pi 2 to crash
    asm volatile("mcr p15, 0, %0, c7, c5,  4" ::"r"(0) : "memory");
    asm volatile("mcr p15, 0, %0, c7, c6,  0" ::"r"(0) : "memory");
#endif
    exec();
    print("Execution complete\n");
    jit_destroy_state();
#ifndef RICH
    // free(code);
#endif
    finish_jit();
    return;
}
