#include <r_core.h>

void entry(RCore *core) {
	r_io_map_add (core->io, 3, R_PERM_RWX, 0, 0xa000, 512);
}
