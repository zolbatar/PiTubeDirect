#include "wasm/load_wasm.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "wasm_shared.h"

uint32_t start_of_heap(wasm_t *wat) {
	for (uint32_t i = 0; i < wat->num_exports; i++) {
		section_export_t *t = &wat->section_exports[i];

		if (strcmp(t->name, "__heap_base") == 0) {
			section_global_t *g = &wat->section_globals[t->index];
			assert(cvec_instruction_t_size(&g->init) > 0);
			return cvec_instruction_t_at(&g->init, 0)->value_i32;
		}
	}
	return 0;
}

void build_vm(wasm_t *wat, uint8_t *RAM, size_t RAM_size) {
	// Copy static data
	for (uint32_t i = 0; i < wat->num_datas; i++) {
		section_data_t *t = &wat->section_datas[i];
		uint32_t offset = cvec_instruction_t_at(&t->offset, 0)->value_i32;
		memcpy(&RAM[offset], t->data, t->data_size);
	}

	// Compile all init sections for globals first
	for (uint32_t i = 0; i < wat->num_globals; i++) {
		section_global_t *t = &wat->section_globals[i];
		const cvec_instruction_t_value *instruction = cvec_instruction_t_at(&t->init, 0);
		switch (t->type) {
			case TYPE_I32:
				t->value_i32 = instruction->value_i32;
				break;
			case TYPE_I64:
				t->value_i64 = instruction->value_i64;
				break;
			case TYPE_F32:
				t->value_f32 = instruction->value_f32;
				break;
			case TYPE_F64:
				t->value_f64 = instruction->value_f64;
				break;
			default:
				print("Unknown type in global init\n");
				exit(1);
		}
	}
}
