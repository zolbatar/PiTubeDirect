#include "../load_wasm.h"
#include <inttypes.h>
#include <stdlib.h>
#include "../../wasm_shared.h"

void section_custom(buffer_t *bf, uint32_t size, wasm_t *wat) {
	size_t pos = bf->pos;
	const char *name = read_string(bf);
	if (trace >= LogDetail)
		print("Custom section '%s' at 0x%07lX\n", name);
	bf->pos = pos + size;
}
