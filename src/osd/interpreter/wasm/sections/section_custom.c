#include "../load_wasm.h"
#include <inttypes.h>
#include <stdlib.h>
#include "../../wasm_shared.h"

void section_custom(buffer_t *bf, uint32_t size, wasm_t *wat) {
	size_t p = bf->pos;
	const char *name = read_string(bf);
	if (trace >= LogDetail)
		print("Custom section '%s' at 0x%07lX/\n", name, p);
//	uint32_t num_fields = read_leb128_32(bf);
	bf->pos = p + size;
}
