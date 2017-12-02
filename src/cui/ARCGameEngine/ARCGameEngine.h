#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "../../common/SDK/include/crass_types.h"
#include "../../common/SDK/include/acui.h"
#include "../../common/SDK/include/cui.h"
#include "../../common/SDK/include/package.h"
#include "../../common/SDK/include/resource.h"
#include "../../common/SDK/include/cui_error.h"
#include "../../common/SDK/include/utility.h"

/* 所有的封包特定的数据结构都要放在这个#pragma段里 */
#pragma pack (1)
typedef struct {
	s8 magic[4];		// "S3IC", "S3IN", "S4IC", "S4IN"
	s8 version[4];		// "XXX "
	u8 unknown[0x124];	// 可能是一些配置用信息（运行时）
} BIN_header_t;

typedef struct {
	u32	actual_length;
	u32	uncomprlen;
	u32	comprlen;
} BIN_COMPRESSINFO_t;

typedef struct {
	u32 ALF_count;		// ALF封包的个数
} BIN_index_header_t;

typedef struct {
	s8 ALF_name[256];
} BIN_index_entry_t;

typedef struct {
	u32 entries;		// 资源总个数
} BIN_entry_header_t;

typedef struct {
	s8 name[64];
	u32 ALF_id;		// 所属的ALF封包编号（ALF名称命名按照BIN_index_entry_t中的命名）
	u32 id;			// 没掌握到分配规则
	u32 offset;
	u32 length;
} BIN_entry_t;

typedef struct {
	s8 magic[4];	// "ACGF"
	u32 type;		// 1 or 2, etc是非法(把type理解为额外压缩块的个数是否更好些?)
	u32 zero;
	BIN_COMPRESSINFO_t info;
} AGF_header_t;

typedef struct {
	s8 magic[2];	// 0000
	u32 total_size;
	u32 reserved;
	u32 offset;		// 0x36
	u16 pad;
	BITMAPINFOHEADER info;
} BM_header_t;

typedef struct {
	s8 magic[4];	// "ACIF"
	u32 type;		// 2
	u32 zero;
	u32 length;
	u32 width;
	u32 height;
	BIN_COMPRESSINFO_t info;
} ACIF_header_t;

typedef struct {
	s8 magic[4];		// "S3AB", "S4AB"
	u32 unknown0;		// 1
	u32 unknown1;		// 0
	u32 act_length;
	u8 unknown[16];
	BIN_COMPRESSINFO_t info;
} AB_header_t;

typedef struct {
	s8 magic[4];		// "0.03"
	u32 unknown[2];
	u32 index_entries;
} idx_header_t;

typedef struct {
	s8 name[64];
	u32 offset_lo;
	u32 offset_hi;
	u32 length_lo;
	u32 length_hi;
} idx_entry_t;
#pragma pack ()
