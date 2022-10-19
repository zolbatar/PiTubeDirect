#include "../load_wasm.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "../../wasm_shared.h"

#define IMPORT_TYPE_FUNC 0
#define IMPORT_TYPE_TABLE 1
#define IMPORT_TYPE_MEM 2
#define IMPORT_TYPE_GLOBAL 3

void section_import(buffer_t *bf, uint32_t size, wasm_t *wat) {
	size_t pos = bf->pos;
	wat->num_imports = read_leb128_32(bf);
	if (trace >= LogDetail)
		print("%d import section(s) at 0x%07lX\n", wat->num_imports, pos);

	// Allocate space
	wat->section_imports = malloc(sizeof(section_import_t) * wat->num_imports);

	// Import each in turn
	wat->num_import_functions = 0;
	for (uint32_t i = 0; i < wat->num_imports; i++) {
		section_import_t *p = &wat->section_imports[i];
		p->module = read_string(bf);
		p->name = read_string(bf);
		p->tag = read_byte_int(bf);
		switch (p->tag) {
			case IMPORT_TYPE_FUNC:
				p->index = read_leb128_32(bf);
				wat->num_import_functions++;
				if (trace >= LogTrace)
					print("Import tag type FUNC: '%s, %d'\n", p->name, p->index);
/*				if (strcmp(p->name, "call_OSD_API") == 0) {
					p->type = API_CALL;
				}*/
				break;
			case IMPORT_TYPE_TABLE:
				p->index = read_leb128_32(bf);
				if (trace >= LogTrace)
					print("Import tag type TABLE: '%s', %d\n", p->name, p->index);
				break;
			case IMPORT_TYPE_MEM: {
				uint8_t has_max = read_byte_int(bf);
				uint32_t min = read_leb128_32(bf);
				uint32_t max = 0;
				if (has_max)
					max = read_leb128_32(bf);
				if (trace >= LogTrace)
					print("Import tag type MEM: '%s', %d/%d\n", p->name, min, max);
				break;
			}
			case IMPORT_TYPE_GLOBAL:
				p->index = read_leb128_32(bf);
				if (trace >= LogTrace)
					print("Import tag type GLOBAL: '%s', %d\n", p->name, p->index);
				break;
			default:
				print("Unknown tag type in import: %d\n", p->tag);
				exit(1);
		}
	}
}
