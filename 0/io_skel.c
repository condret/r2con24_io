#include <r_io.h>
#include <r_lib.h>

static int __write(RIO* io, RIODesc* fd, const ut8* buf, int count) {
	return count;
}

static int __read(RIO* io, RIODesc* fd, ut8* buf, int count) {
	return count;
}

static bool __close(RIODesc* fd) {
	return true;
}

static ut64 __lseek(RIO* io, RIODesc* fd, ut64 offset, int whence) {
	return offset;
}

static RIODesc* __open(RIO* io, const char* pathname, int rwx, int mode) {
	return NULL;
}

static bool __check(RIO *io, const char *pathname, bool many) {
	return false;
}

RIOPlugin r_io_plugin_mbc2ram = {
	.meta = {
		.name = "bla",
		.desc = "bla",
		.author = "bla",
		.license = "bla",
	},
	.uris = "mbc2ram://",
	.open = __open,
	.close = __close,
	.read = __read,
	.check = __check,
	.seek = __lseek,
	.write = __write,
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_IO,
	.data = &r_io_plugin_mbc2ram,
	.version = R2_VERSION
};
#endif
