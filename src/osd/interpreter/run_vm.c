#include "wasm/load_wasm.h"
#include "wasm_shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "lightning/lightning.h"
#include "lightning/jit_private.h"
#include <capstone/capstone.h>
#include <capstone/platform.h>

size_t SP;
static uint8_t *heap_space;
jit_state_t *_jit;
typedef int (*start)(void);

/*static inline void push_int_stack(int32_t v) {
	SP -= 8;
	write_uint_simple(&heap_space[SP], v);
	if (trace >= LogTrace)
		print("Stack push: %d\n", v);
}

static inline int32_t pop_int_stack() {
	int32_t v = read_int_simple(&heap_space[SP]);
	SP += 8;
	if (trace >= LogTrace)
		print("Stack pop: %d\n", v);
	return v;
}

void API_Call() {
	int32_t p2 = pop_int_stack();
	int32_t p1 = pop_int_stack();
	switch (p1) {
		case 0:
			print(&heap_space[p2]);
			break;
		default:
			if (trace >= LogTrace)
				print("Call API %d %d\n", p1, p2);
			break;
	}
	push_int_stack(0);
}

void get_local(local_t *locals, wasm_t *wat, const cvec_instruction_t_value *t) {
	local_t *l = &locals[t->index];
	switch (l->type) {
		case TYPE_I32:
			push_int_stack(l->value_i32);
			break;
					case TYPE_I64:
						break;
					case TYPE_F32:
						break;
					case TYPE_F64:
						break;
default:
print("Unsupported");
exit(1);
}
}

void set_local(local_t *locals, wasm_t *wat, const cvec_instruction_t_value *t) {
	local_t *l = &locals[t->index];
	switch (l->type) {
		case TYPE_I32:
			l->value_i32 = pop_int_stack();
			break;
		case TYPE_I64:
			break;
		case TYPE_F32:
			break;
		case TYPE_F64:
			break;
		default:
			fprintf(stderr, "Unsupported");
			exit(1);
	}
}

void get_global(wasm_t *wat, const cvec_instruction_t_value *t) {
	section_global_t *g = &wat->section_globals[t->index];
	switch (g->type) {
		case TYPE_I32:
			push_int_stack(g->value_i32);
			break;
		case TYPE_I64:
			break;
		case TYPE_F32:
			break;
		case TYPE_F64:
			break;
		default:
			print("Unsupported");
			exit(1);
	}
}

void set_global(wasm_t *wat, const cvec_instruction_t_value *t) {
	section_global_t *g = &wat->section_globals[t->index];
	switch (g->type) {
		case TYPE_I32:
			g->value_i32 = pop_int_stack();
			break;
		case TYPE_I64:
			break;
		case TYPE_F32:
			break;
		case TYPE_F64:
			break;
		default:
			print("Unsupported");
			exit(1);
	}
}

void run_function(wasm_t *wat, section_code_t *func) {
	uint32_t PC = 0;

	// Locals?
	local_t *locals;
	uint32_t total = func->num_params + func->num_locals_total;

	if (total > 0) {
		size_t sz = sizeof(section_code_locals_t) * total;
		locals = malloc(sz);
		memset(locals, 0xFF, sz);

		size_t index = 0;

		// Params
		for (uint32_t i = 0; i < func->num_params; i++) {
			if (trace >= LogTrace)
				print("P %d/%d - %d\n", i, index, func->params[i]);
			locals[index++].type = func->params[i];
		}

		// Real locals
		for (uint32_t i = 0; i < func->num_locals; i++) {
			section_code_locals_t *locals_def = &func->locals[i];
			for (uint32_t j = 0; j < locals_def->count; j++) {
				if (trace >= LogTrace)
					print("%d/%d - %d\n", j, index, locals_def->type);
				locals[index++].type = locals_def->type;
			}
		}
	}
	else {
		locals = NULL;
	}

	cvec_instruction_t current_code_list = func->code;
	while (1) {
		const cvec_instruction_t_value *t = cvec_instruction_t_at(&current_code_list, PC++);
		switch (t->opcode) {
			case INSTRUCTION_NOP:
				break;
			case INSTRUCTION_END:
				if (locals != NULL)
					free(locals);
				return;
			case INSTRUCTION_RETURN:
				if (trace >= LogTrace)
					print("Stack pointer on exit: %ld\n", SP);
				break;
			case INSTRUCTION_CALL: {
				uint32_t index = t->index;
				if (index < wat->num_import_functions) {
					// HAL
					section_import_t *it = &wat->section_imports[index];
					switch (it->type) {
						case API_CALL:
							if (trace >= LogTrace)
								print("Stack pointer on entry: %ld\n", SP);
							API_Call();
							if (trace >= LogTrace)
								print("Stack pointer on exit: %ld\n", SP);
							break;
						default:
							print("Unknown API call index\n");
							exit(1);
					}
				}
				else {
					// Real
					if (trace >= LogTrace)
						print("Stack pointer on entry CALL: %ld\n", SP);
					index = index - wat->num_import_functions;
					run_function(wat, &wat->section_codes[index]);
					if (trace >= LogTrace)
						print("Stack pointer on exit CALL: %ld\n", SP);
				}
				break;
			}
			case INSTRUCTION_DROP:
				SP += 8;
				if (trace >= LogTrace)
					print("Stack pointer after drop: %ld\n", SP);
				break;
			case INSTRUCTION_GLOBAL_GET:
				get_global(wat, t);
				break;
			case INSTRUCTION_GLOBAL_SET:
				set_global(wat, t);
				break;
			case INSTRUCTION_LOCAL_GET:
				get_local(locals, wat, t);
				break;
			case INSTRUCTION_LOCAL_SET:
				set_local(locals, wat, t);
				break;
			case INSTRUCTION_I32_LOAD: {
				int32_t addr = pop_int_stack() + t->offset;
				int32_t v = read_int_simple(&heap_space[addr]);
				push_int_stack(v);
				break;
			}
			case INSTRUCTION_I32_STORE: {
				int32_t v = pop_int_stack();
				int32_t addr = pop_int_stack() + t->offset;
				write_int_simple(&heap_space[addr], v);
				break;
			}
			case INSTRUCTION_I32_CONST:
				push_int_stack(t->value_i32);
				break;
			case INSTRUCTION_I32_ADD: {
				int32_t v2 = pop_int_stack();
				int32_t v1 = pop_int_stack();
				push_int_stack(v1 + v2);
				break;
			}
			case INSTRUCTION_I32_SUB: {
				int32_t v2 = pop_int_stack();
				int32_t v1 = pop_int_stack();
				push_int_stack(v1 - v2);
				break;
			}
			case INSTRUCTION_UNREACHABLE:
				fprintf(stderr, "Unreachable hit\n");
				exit(1);
			case INSTRUCTION_I32_CONST: {
				int32_t v = read_int_simple(&RAM[PC]);
				push_int_stack(v);
				break;
			}
			default:
				print("Unknown opcode 0x%X\n", t->opcode);
				exit(1);
		}
	}
}
*/

static void setup_capstone() {
#ifndef DISABLE_DISASM
	// Setup capstone
	cs_opt_mem setup;
	setup.malloc = malloc;
	setup.calloc = calloc;
	setup.realloc = realloc;
	setup.free = free;
	setup.vsnprintf = vsnprintf;
	cs_err err = cs_option(0, CS_OPT_MEM, (size_t)&setup);
	if (err != CS_ERR_OK) {
		printf("Error (cs_option): %d\n", err);
	}
#endif
}

static void disassemble(start exec, jit_word_t sz) {
#ifndef DISABLE_DISASM
	// Disassemble
	csh handle;
	cs_insn *insn;
	size_t count;
#ifdef CAPSTONE_HAS_X86
	cs_err err = cs_open(CS_ARCH_X86, CS_MODE_64, &handle);
#else
#ifdef PITUBE
	cs_err err = cs_open(CS_ARCH_ARM, CS_MODE_ARM, &handle);
#else
	cs_err err = cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle);
#endif
#endif
	if (err != CS_ERR_OK) {
		printf("Disassemble error: %d\n", err);
	}
	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
	count = cs_disasm(handle, (const unsigned char *)exec, sz, (size_t)_jit->code.ptr, 0, &insn);
	if (count > 0) {
		size_t j;
		for (j = 0; j < count; j++) {
#ifdef PITUBE
			printf("0x%X:\t%s\t%s\n", insn[j].address, insn[j].mnemonic, insn[j].op_str);
#else
			printf("0x%" PRIx64 ":\t%s\t%s\n", insn[j].address, insn[j].mnemonic, insn[j].op_str);
#endif
		}
		cs_free(insn, count);
	}
	else {
		printf("ERROR: Failed to disassemble given code!");
	}
	cs_close(&handle);
#endif
}

void compile_function(wasm_t *wat, section_code_t *func) {
	for (size_t i = 0; i < cvec_instruction_t_size(&func->code); i++) {
		const cvec_instruction_t_value *ins = cvec_instruction_t_at(&func->code, i);
		switch (ins->opcode) {
			case INSTRUCTION_END:
				break;
			case INSTRUCTION_CALL:
				break;
			case INSTRUCTION_I32_CONST:
				jit_movi(JIT_R0, ins->value_i32);
				stack_push_int(JIT_R0);
				break;


			default:
				printf("Unknown opcode during native compile\n");
				exit(1);
		}
	}
}

void run_vm(wasm_t *wat, uint8_t *_RAM, size_t RAM_size, uint32_t heap_start) {
	setup_capstone();
	init_jit("Dork");
	_jit = jit_new_state();

	jit_prolog();
	stack_init();

	// Compile all functions
	for (size_t i = 0; i < wat->num_codes; i++) {
		compile_function(wat, &wat->section_codes[i]);
	}

	/* Remember to setup calls to ctor and start function
	 *
	 * wat->ctors_function
	 * wat->start_function
	 */
	printf("Compilation complete\n");

	// Code & data size
	jit_realize();
	if (!_jitc->realize) {
		printf("Failed to realise");
	}

	// Do compile
	jit_word_t sz = _jit->code.length;
#ifndef RICH
	void *code = malloc(sz);
	jit_set_code(code, sz);
	jit_set_data(NULL, 0, JIT_DISABLE_NOTE | JIT_DISABLE_NOTE);
#endif
	start exec = jit_emit_void();
	if (exec == NULL) {
		printf("Code generation failed");
	}

	// Size?
	jit_word_t code_size;
	jit_get_code(&code_size);
#ifdef VERBOSE_COMPILE
	printf("Code size: %ld [%ld] bytes\n", code_size, sz);
#endif

#ifndef DISABLE_DISASM
	disassemble(exec, code_size);
#endif

	jit_clear_state();
	exec();
	jit_destroy_state();
#ifndef RICH
	free(code);
#endif
	printf("Execution complete\n");
	finish_jit();
	return;







	// Reset state
	/*heap_space = _RAM;
	SP = heap_start;
	if (trace >= LogTrace)
		print("Stack pointer at start: %ld\n", SP);
	if (trace >= LogInfo)
		print("Calling global constructors...\n");
	run_function(wat, &wat->section_codes[wat->ctors_function]);
	if (trace >= LogInfo)
		print("Calling start...\n");
	run_function(wat, &wat->section_codes[wat->start_function]);
	if (trace >= LogInfo)
		print("Finished\n");*/
}
