#include "load_wasm.h"
#include <stdio.h>
#include <stdlib.h>

cvec_instruction_t load_expression(buffer_t *bf) {
	cvec_instruction_t code = cvec_instruction_t_init();

	// Process each in turn
	uint8_t opcode = INSTRUCTION_END;
	do {
		opcode = process_instruction(&code, bf);
	}
	while (opcode != INSTRUCTION_END);

	return code;
}

