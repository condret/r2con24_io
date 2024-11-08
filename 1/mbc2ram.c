#include <stdio.h>
#include <r_io.h>
#include <r_util.h>

typedef struct gb_mbc2_ram_data {
	ut64 seek;
	ut8 buf[512];
} MBC2Ram;

RIOPlugin r_io_plugin_mbc2ram;

static bool __ram_check(RIO *io, const char *pathname, bool many) {
	return r_str_startswith (pathname, "mbc2ram://");
}

static RIODesc *__ram_open(RIO *io, const char *pathname, int rw, int mode) {
	if (!r_str_startswith (pathname, "mbc2ram://")) {
		return NULL;
	}
	MBC2Ram *ram = R_NEW0 (MBC2Ram);
	if (!ram) {
		return NULL;
	}
	RIODesc *desc = r_io_desc_new (io, &r_io_plugin_mbc2ram, pathname,
		R_PERM_RWX, mode, ram);
	if (!desc) {
		free (ram);
		return NULL;
	}
	memset (ram->buf, 0xf0, 512 * sizeof (ut8));
	return desc;
}

static bool __ram_close(RIODesc *desc) {
	free (desc->data);
	return true;
}

static ut64 __ram_lseek(RIO* io, RIODesc *desc, ut64 offset, int whence) {
	MBC2Ram *ram = (MBC2Ram *)desc->data;
	switch (whence) {
	case R_IO_SEEK_SET:
		return ram->seek = R_MIN (0x200, offset);
	case R_IO_SEEK_CUR:
		return ram->seek = R_MIN (0x200, ram->seek + offset);
	case R_IO_SEEK_END:
		return ram->seek = 0x200;
	}
	return ram->seek;
}

static int __ram_read(RIO *io, RIODesc *desc, ut8 *buf, int len) {
	MBC2Ram *ram = (MBC2Ram *)desc->data;
	len = R_MIN (len, 0x200 - ram->seek);
	if (len < 0) {
		return -1;
	}
	memcpy (buf, &ram->buf[ram->seek], len);
	ram->seek += len;
	return len;
}

static int __ram_write(RIO *io, RIODesc *desc, const ut8 *buf, int len) {
	MBC2Ram *ram = (MBC2Ram *)desc->data;
	const int ret = len = R_MIN (len, 0x200 - ram->seek);
	if (len < 0) {
		return -1;
	}
	memcpy (&ram->buf[ram->seek], buf, len);
	if (R_UNLIKELY (len > 7)) {
		ut32 ut64len = len >> 3;
		do {
			ut64 *buf64 = (ut64 *)(&ram->buf[ram->seek]);
			*buf64 |= 0xf0f0f0f0f0f0f0f0;
			ram->seek += 8;
		} while (--ut64len);
		len &= 7;
	}
	do {
		ram->buf[ram->seek] |= 0xf0;
		ram->seek++;
	} while (--len);
	return ret;
}

RIOPlugin r_io_plugin_mbc2ram = {
	.meta = {
		.name = "mbc2ram",
		.desc = "mbc2ram",
		.license = "LGPL",
	},
	.uris = "mbc2ram://",
	.open = __ram_open,
	.close = __ram_close,
	.read = __ram_read,
	.check = __ram_check,
	.seek = __ram_lseek,
	.write = __ram_write,
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_IO,
	.data = &r_io_plugin_mbc2ram,
	.version = R2_VERSION
};
#endif
