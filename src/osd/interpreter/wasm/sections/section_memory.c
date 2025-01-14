#include "../load_wasm.h"
#include <inttypes.h>
#include <stdlib.h>
#include "../../wasm_shared.h"

void section_memory(buffer_t *bf, uint32_t size, wasm_t *wat) {
	size_t pos = bf->pos;
	wat->num_memories = read_leb128_32(bf);
	if (trace >= LogDetail)
		print("%d memory section(s) at 0x%07lX\n", wat->num_memories, pos);

	// Allocate space
	wat->section_memories = malloc(sizeof(section_memory_t) * wat->num_memories);

	// Import each in turn
	for (uint32_t i = 0; i < wat->num_memories; i++) {
		section_memory_t *p = &wat->section_memories[i];
		p->has_max = read_byte_int(bf);
		p->min = read_leb128_32(bf);
		if (p->has_max)
			p->max = read_leb128_32(bf);
		print("Memory section %u: %u/%u\n", i, p->min, p->max);
	}
}
