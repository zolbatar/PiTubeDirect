#include "../load_wasm.h"
#include <inttypes.h>
#include <stdlib.h>
#include "../../wasm_shared.h"

void section_code(buffer_t *bf, uint32_t size, wasm_t *wat) {
	size_t pos = bf->pos;
	wat->num_codes = read_leb128_32(bf);
	if (trace >= LogDetail)
		print("%d code section(s) at 0x%07lX\n", wat->num_codes, pos);

	// Allocate space
	wat->section_codes = malloc(sizeof(section_code_t) * wat->num_codes);

	// Import each in turn
	for (uint32_t i = 0; i < wat->num_codes; i++) {
		if (trace >= LogDetail)
			print("  [%d] code section at 0x%07lX\n", i, bf->pos);
		section_code_t *p = &wat->section_codes[i];
		uint32_t num_bytes = read_leb128_32(bf);

		// Get function for this block
		uint32_t fd = wat->section_functions[i];
		section_type_t *tt = &wat->section_types[fd];
		p->num_params = tt->num_params;
		p->params = tt->params;

		// Locals
		p->num_locals = read_leb128_32(bf);
		p->num_locals_total = 0;
		if (p->num_locals > 0) {
			p->locals = malloc(p->num_locals * sizeof(section_code_locals_t));
			for (uint32_t j = 0; j < p->num_locals; j++) {
				p->locals[j].count = read_byte_int(bf);
				if (trace >= LogTrace)
					print("--%d\n", p->locals[j].count);
				p->locals[j].type = read_byte_int(bf);
				validate_type(p->locals[j].type, wat);
				p->num_locals_total += p->locals[j].count;
			}
		}

		// Expression
		p->code = load_expression(bf);
	}
}
