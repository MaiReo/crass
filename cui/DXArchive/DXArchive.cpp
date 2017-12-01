#include <windows.h>
#include <tchar.h>
#include <crass_types.h>
#include <acui.h>
#include <cui.h>
#include <package.h>
#include <resource.h>
#include <cui_error.h>
#include <utility.h>
#include <stdio.h>
#include <vector>

using namespace std;
using std::vector;

/*
找解密key的方法：
搜索常量aa 有push AA这句话就是KeyCreate。

Q:\asuhare
 */

/* 接口数据结构: 表示cui插件的一般信息 */
struct acui_information DXArchive_cui_information = {
	_T("山田 巧"),			/* copyright */
	_T("ＤＸライブラリ"),	/* system */
	_T(".dxa .dat .hud"),	/* package */
	_T("1.0.1"),			/* revision */
	_T("痴汉公贼"),			/* author */
	_T("2009-7-31 20:18"),	/* date */
	NULL,					/* notion */
	ACUI_ATTRIBUTE_LEVEL_UNSTABLE
};

/*
	データマップ
		
	DARC_HEAD
	ファイル実データ
	ファイル名テーブル
	DARC_FILEHEAD テーブル
	DARC_DIRECTORY テーブル
*/

/*
	ファイル名のデータ形式
	2byte:文字列の長さ(バイトサイズ÷４)
	2byte:文字列のパリティデータ(全ての文字の値を足したもの)
	英字は大文字に変換されたファイル名のデータ(４の倍数のサイズ)
	英字が大文字に変換されていないファイル名のデータ
*/

#define FILE_ATTRIBUTE_DIRECTORY		0x00000010

// struct ---------------------------------------

#pragma pack(push)
#pragma pack(1)
// アーカイブデータの最初のヘッダ
typedef struct tagDARC_HEAD {
	s8  Head[2];							// ヘッダ
	u16 Version;							// バージョン
	u32 HeadSize;							// ヘッダ情報の DARC_HEAD を抜いた全サイズ
	u32 DataStartAddress;					// 最初のファイルのデータが格納されているデータアドレス(ファイルの先頭アドレスをアドレス０とする)
	u32 FileNameTableStartAddress;			// ファイル名テーブルの先頭アドレス(ファイルの先頭アドレスをアドレス０とする)
	u32 FileTableStartAddress;				// ファイルテーブルの先頭アドレス(メンバ変数 FileNameTableStartAddress のアドレスを０とする)
	u32 DirectoryTableStartAddress;			// ディレクトリテーブルの先頭アドレス(メンバ変数 FileNameTableStartAddress のアドレスを０とする)
											// アドレス０から配置されている DARC_DIRECTORY 構造体がルートディレクトリ
} DARC_HEAD ;

// ファイルの時間情報
typedef struct tagDARC_FILETIME {
	u64 Create;			// 作成時間
	u64 LastAccess;		// 最終アクセス時間
	u64 LastWrite;		// 最終更新時間
} DARC_FILETIME;

// ファイル格納情報(Ver 0x0001)
typedef struct tagDARC_FILEHEAD_VER1 {
	u32 NameAddress ;			// ファイル名が格納されているアドレス( ARCHIVE_HEAD構造体 のメンバ変数 FileNameTableStartAddress のアドレスをアドレス０とする) 

	u32 Attributes ;			// ファイル属性
	DARC_FILETIME Time ;		// 時間情報
	u32 DataAddress ;			// ファイルが格納されているアドレス
								//			ファイルの場合：DARC_HEAD構造体 のメンバ変数 DataStartAddress が示すアドレスをアドレス０とする
								//			ディレクトリの場合：DARC_HEAD構造体 のメンバ変数 DirectoryTableStartAddress のが示すアドレスをアドレス０とする
	u32 DataSize ;				// ファイルのデータサイズ
} DARC_FILEHEAD_VER1;

// ファイル格納情報
typedef struct tagDARC_FILEHEAD {
	u32 NameAddress;			// ファイル名が格納されているアドレス( ARCHIVE_HEAD構造体 のメンバ変数 FileNameTableStartAddress のアドレスをアドレス０とする) 

	u32 Attributes;				// ファイル属性
	DARC_FILETIME Time;			// 時間情報
	u32 DataAddress;			// ファイルが格納されているアドレス
								//			ファイルの場合：DARC_HEAD構造体 のメンバ変数 DataStartAddress が示すアドレスをアドレス０とする
								//			ディレクトリの場合：DARC_HEAD構造体 のメンバ変数 DirectoryTableStartAddress のが示すアドレスをアドレス０とする
	u32 DataSize;				// ファイルのデータサイズ
	u32 PressDataSize;			// 圧縮後のデータのサイズ( 0xffffffff:圧縮されていない ) ( Ver0x0002 で追加された )
} DARC_FILEHEAD;

// ディレクトリ格納情報
typedef struct tagDARC_DIRECTORY {
	u32 DirectoryAddress;			// 自分の DARC_FILEHEAD が格納されているアドレス( DARC_HEAD 構造体 のメンバ変数 FileTableStartAddress が示すアドレスをアドレス０とする)
	u32 ParentDirectoryAddress;		// 親ディレクトリの DARC_DIRECTORY が格納されているアドレス( DARC_HEAD構造体 のメンバ変数 DirectoryTableStartAddress が示すアドレスをアドレス０とする)
	u32 FileHeadNum;				// ディレクトリ内のファイルの数
	u32 FileHeadAddress;			// ディレクトリ内のファイルのヘッダ列が格納されているアドレス( DARC_HEAD構造体 のメンバ変数 FileTableStartAddress が示すアドレスをアドレス０とする) 
} DARC_DIRECTORY;
#pragma pack(pop)

typedef struct {
	s8 name[MAX_PATH];
	u32 offset;
	u32 comprlen;
	u32 uncomprlen;
} my_dxa_entry_t;

#define DXA_HEAD			"DX"		// ヘッダ
#define DXA_VER				0x0003		// バージョン
#define DXA_KEYSTR_LENGTH	12			// 鍵文字列の長さ

static const char *key_list[] = {
	"hug_001",
	"FbavPKCS7",
	"hug_001_DEMO",
	"jirotsuke",
	NULL
};

// ファイル名データから元のファイル名の文字列を取得する
static const char *GetOriginalFileName(u8 *FileNameTable)
{
	return (char *)FileNameTable + *((u16 *)&FileNameTable[0]) * 4 + 4;
}

#define MIN_COMPRESS		(4)						// 最低圧縮バイト数
#define MAX_SEARCHLISTNUM	(64)					// 最大一致長を探す為のリストを辿る最大数
#define MAX_SUBLISTNUM		(65536)					// 圧縮時間短縮のためのサブリストの最大数
#define MAX_COPYSIZE 		(0x1fff + MIN_COMPRESS)	// 参照アドレスからコピー出切る最大サイズ( 圧縮コードが表現できるコピーサイズの最大値 + 最低圧縮バイト数 )
#define MAX_ADDRESSLISTNUM	(1024 * 1024 * 1)		// スライド辞書の最大サイズ
#define MAX_POSITION		(1 << 24)				// 参照可能な最大相対アドレス( 16MB )

// デコード( 戻り値:解凍後のサイズ  -1 はエラー  Dest に NULL を入れることも可能 )
static int Decode(void *Src, void *Dest)
{
	u32 srcsize, destsize, code, indexsize, keycode, conbo, index ;
	u8 *srcp, *destp, *dp, *sp ;

	destp = (u8 *)Dest ;
	srcp  = (u8 *)Src ;
	
	// 解凍後のデータサイズを得る
	destsize = *((u32 *)&srcp[0]) ;

	// 圧縮データのサイズを得る
	srcsize = *((u32 *)&srcp[4]) - 9 ;

	// キーコード
	keycode = srcp[8] ;
	
	// 出力先がない場合はサイズだけ返す
	if( Dest == NULL )
		return destsize ;
	
	// 展開開始
	sp  = srcp + 9 ;
	dp  = destp ;
	while( srcsize )
	{
		// キーコードか同かで処理を分岐
		if( sp[0] != keycode )
		{
			// 非圧縮コードの場合はそのまま出力
			*dp = *sp ;
			dp      ++ ;
			sp      ++ ;
			srcsize -- ;
			continue ;
		}
	
		// キーコードが連続していた場合はキーコード自体を出力
		if( sp[1] == keycode )
		{
			*dp = (u8)keycode ;
			dp      ++ ;
			sp      += 2 ;
			srcsize -= 2 ;
			
			continue ;
		}

		// 第一バイトを得る
		code = sp[1] ;

		// もしキーコードよりも大きな値だった場合はキーコード
		// とのバッティング防止の為に＋１しているので－１する
		if( code > keycode ) code -- ;

		sp      += 2 ;
		srcsize -= 2 ;

		// 連続長を取得する
		conbo = code >> 3 ;
		if( code & ( 0x1 << 2 ) )
		{
			conbo |= *sp << 5 ;
			sp      ++ ;
			srcsize -- ;
		}
		conbo += MIN_COMPRESS ;	// 保存時に減算した最小圧縮バイト数を足す

		// 参照相対アドレスを取得する
		indexsize = code & 0x3 ;
		switch( indexsize )
		{
		case 0 :
			index = *sp ;
			sp      ++ ;
			srcsize -- ;
			break ;
			
		case 1 :
			index = *((u16 *)sp) ;
			sp      += 2 ;
			srcsize -= 2 ;
			break ;
			
		case 2 :
			index = *((u16 *)sp) | ( sp[2] << 16 ) ;
			sp      += 3 ;
			srcsize -= 3 ;
			break ;
		}
		index ++ ;		// 保存時に－１しているので＋１する

		// 展開
		if( index < conbo )
		{
			u32 num ;

			num  = index ;
			while( conbo > num )
			{
				memcpy( dp, dp - num, num ) ;
				dp    += num ;
				conbo -= num ;
				num   += num ;
			}
			if( conbo != 0 )
			{
				memcpy( dp, dp - num, conbo ) ;
				dp += conbo ;
			}
		}
		else
		{
			memcpy( dp, dp - index, conbo ) ;
			dp += conbo ;
		}
	}

	// 解凍後のサイズを返す
	return (int)destsize ;
}

static void KeyCreate(const char *Source, unsigned char *Key)
{
	int Len ;

	if (Source == NULL) {
		memset(Key, 0xaaaaaaaa, DXA_KEYSTR_LENGTH);
	} else {
		Len = strlen(Source) ;
		if (Len > DXA_KEYSTR_LENGTH) {
			memcpy(Key, Source, DXA_KEYSTR_LENGTH);
		} else {
			int i ;

			for (i = 0; i + Len <= DXA_KEYSTR_LENGTH; i += Len)
				memcpy(Key + i, Source, Len);
			if (i < DXA_KEYSTR_LENGTH)
				memcpy(Key + i, Source, DXA_KEYSTR_LENGTH - i) ;
		}
	}

	Key[0] = ~Key[0];
	Key[1] = (Key[1] >> 4) | (Key[1] << 4);
	Key[2] = Key[2] ^ 0x8a;
	Key[3] = ~((Key[3] >> 4) | (Key[3] << 4));
	Key[4] = ~Key[4];
	Key[5] = Key[5] ^ 0xac;
	Key[6] = ~Key[6];
	Key[7] = ~((Key[7] >> 3) | (Key[7] << 5));
	Key[8] = (Key[8] >> 5) | (Key[8] << 3);
	Key[9] = Key[9] ^ 0x7f;
	Key[10] = ((Key[10] >> 4) | (Key[10] << 4)) ^ 0xd6;
	Key[11] = Key[11] ^ 0xcc;
}

static void KeyConv(void *Data, int Size, int Position, unsigned char *Key)
{
	int i, j;

	Position %= DXA_KEYSTR_LENGTH;
	j = Position;
	for (i = 0; i < Size; i++) {
		((u8 *)Data)[i] ^= Key[j];
		j++;
		if (j == DXA_KEYSTR_LENGTH) j = 0;
	}
}

// 指定のディレクトリデータにあるファイルを展開する
static void DirectoryDecode(u8 *NameP, u8 *DirP, u8 *FileP, 
						   DARC_HEAD *Head, DARC_DIRECTORY *Dir, struct package *pkg, 
						   unsigned char *Key, char *ParentDirPath, 
						   vector<my_dxa_entry_t> &dxa_index)
{
	char DirPath[MAX_PATH];
	
	// ディレクトリ情報がある場合は、まず展開用のディレクトリを作成する
	if (Dir->DirectoryAddress != 0xffffffff && Dir->ParentDirectoryAddress != 0xffffffff) {
		DARC_FILEHEAD *DirFile;
		
		// DARC_FILEHEAD のアドレスを取得
		DirFile = (DARC_FILEHEAD * )(FileP + Dir->DirectoryAddress) ;
		
		// ディレクトリの作成
		sprintf(DirPath, "%s%s\\", ParentDirPath, 
			GetOriginalFileName(NameP + DirFile->NameAddress));
		//CreateDirectory(GetOriginalFileName(NameP + DirFile->NameAddress), NULL);
		
		// そのディレクトリにカレントディレクトリを移す
		//SetCurrentDirectory( GetOriginalFileName( NameP + DirFile->NameAddress ) ) ;
		//printf("switch dir %s\n", DirPath);
	} else
		strcpy(DirPath, ParentDirPath);

	// 展開処理開始
	{
		u32 FileHeadSize;
		DARC_FILEHEAD *File;

		// 格納されているファイルの数だけ繰り返す
		FileHeadSize = Head->Version >= 0x0002 ? sizeof(DARC_FILEHEAD) : sizeof(DARC_FILEHEAD_VER1);
		File = (DARC_FILEHEAD *)(FileP + Dir->FileHeadAddress);
		for (u32 i = 0; i < Dir->FileHeadNum; ++i, File = (DARC_FILEHEAD *)((u8 *)File + FileHeadSize)) {
			// ディレクトリかどうかで処理を分岐
			if (File->Attributes & FILE_ATTRIBUTE_DIRECTORY) {
				// ディレクトリの場合は再帰をかける
				DirectoryDecode(NameP, DirP, FileP, Head, 
					(DARC_DIRECTORY * )(DirP + File->DataAddress), pkg, 
					Key, DirPath, dxa_index);
			} else {
				my_dxa_entry_t entry;

				sprintf(entry.name, "%s%s", DirPath, 
					GetOriginalFileName(NameP + File->NameAddress));
				entry.offset = Head->DataStartAddress + File->DataAddress;

				// データが圧縮されているかどうかで処理を分岐
				if(Head->Version >= 0x0002 && File->PressDataSize != -1) {
					entry.uncomprlen = File->DataSize;
					entry.comprlen = File->PressDataSize;
				} else {
					entry.uncomprlen = 0;
					entry.comprlen = File->DataSize;
				}
				dxa_index.push_back(entry);
			}
		}
	}
}

/********************* dxa *********************/

/* 封包匹配回调函数 */
static int DXArchive_dxa_match(struct package *pkg)
{
	DARC_HEAD Head;	

	if (pkg->pio->open(pkg, IO_READONLY))
		return -CUI_EOPEN;

	if (pkg->pio->read(pkg, &Head, sizeof(Head))) {
		pkg->pio->close(pkg);
		return -CUI_EREAD;
	}

	u8 *Key = NULL;
	if (strncmp(Head.Head, DXA_HEAD, 2)) {
		Key = (u8 *)malloc(DXA_KEYSTR_LENGTH);
		if (!Key) {
			pkg->pio->close(pkg);
			return -CUI_EMEM;
		}

		KeyCreate(NULL, Key);
		KeyConv(&Head, sizeof(Head), 0, Key);
		if (!strncmp(Head.Head, DXA_HEAD, 2)) {
			//printf("Using default key\n");
			goto ver_check;
		}

		if (pkg->pio->readvec(pkg, &Head, sizeof(Head), 0, IO_SEEK_SET)) {
			free(Key);
			pkg->pio->close(pkg);
			return -CUI_EREADVEC;
		}

		memset(Key, 0xffffffff, DXA_KEYSTR_LENGTH);
		KeyConv(&Head, sizeof(Head), 0, Key);
		if (!strncmp(Head.Head, DXA_HEAD, 2)) {
			//printf("Using default key\n");
			goto ver_check;
		}

		const char *KeyString = get_options("key");
		if (KeyString) {
			if (pkg->pio->readvec(pkg, &Head, sizeof(Head), 0, IO_SEEK_SET)) {
				free(Key);
				pkg->pio->close(pkg);
				return -CUI_EREADVEC;
			}

			KeyCreate(KeyString, Key);
			KeyConv(&Head, sizeof(Head), 0, Key);

			if (!strncmp(Head.Head, DXA_HEAD, 2)) {
				//printf("Using specified key %s\n", KeyString);
				goto ver_check;
			}
		}

		int ret = -CUI_EMATCH;
		for (int k = 0; key_list[k]; k++) {
			if (pkg->pio->readvec(pkg, &Head, sizeof(Head), 0, IO_SEEK_SET)) {
				ret = -CUI_EREADVEC;
				break;
			}

			KeyCreate(key_list[k], Key);
			KeyConv(&Head, sizeof(Head), 0, Key);

			if (!strncmp(Head.Head, DXA_HEAD, 2)) {
				//printf("Using embedded key %s\n", key_list[k]);
				ret = 0;
				break;
			}
		}
		if (ret) {
			free(Key);
			pkg->pio->close(pkg);
			return ret;	
		}
	}

ver_check:
	if (Head.Version > DXA_VER) {
		free(Key);
		pkg->pio->close(pkg);
		return -CUI_EMATCH;
	}

	package_set_private(pkg, Key);

	return 0;	
}

/* 封包索引目录提取函数 */
static int DXArchive_dxa_extract_directory(struct package *pkg,
										   struct package_directory *pkg_dir)
{
	DARC_HEAD Head;

	if (pkg->pio->readvec(pkg, &Head, sizeof(Head), 0, IO_SEEK_SET)) 
		return -CUI_EREADVEC;

	u8 *Key = (u8 *)package_get_private(pkg);
	KeyConv(&Head, sizeof(Head), 0, Key);

	u8 *HeadBuffer = (u8 *)malloc(Head.HeadSize);
	if (!HeadBuffer)
		return -CUI_EMEM;
		
	// ヘッダパックをメモリに読み込む
	if (pkg->pio->seek(pkg, Head.FileNameTableStartAddress, IO_SEEK_SET)) {
		free(HeadBuffer);
		return -CUI_ESEEK;
	}

	if (pkg->pio->read(pkg, HeadBuffer, Head.HeadSize)) {
		free(HeadBuffer);
		return -CUI_EREAD;
	}
	KeyConv(HeadBuffer, Head.HeadSize, Head.FileNameTableStartAddress, Key);

	// 各アドレスをセットする
	u8 *NameP = HeadBuffer;
	u8 *FileP = NameP + Head.FileTableStartAddress;
	u8 *DirP = NameP + Head.DirectoryTableStartAddress;

	vector<my_dxa_entry_t> my_dxa_index;

	// アーカイブの展開を開始する
	DirectoryDecode(NameP, DirP, FileP, &Head, (DARC_DIRECTORY *)DirP, pkg, Key, "", my_dxa_index);
	
	my_dxa_entry_t *index = new my_dxa_entry_t[my_dxa_index.size()];
	if (!index)
		return -CUI_EMEM;

	for (DWORD i = 0; i < my_dxa_index.size(); ++i)
		index[i] = my_dxa_index[i];

	pkg_dir->index_entries = my_dxa_index.size();
	pkg_dir->directory = index;
	pkg_dir->directory_length = sizeof(my_dxa_entry_t) * my_dxa_index.size();
	pkg_dir->index_entry_length = sizeof(my_dxa_entry_t);

	return 0;
}

/* 封包索引项解析函数 */
static int DXArchive_dxa_parse_resource_info(struct package *pkg,
											 struct package_resource *pkg_res)
{
	my_dxa_entry_t *my_dxa_entry;

	my_dxa_entry = (my_dxa_entry_t *)pkg_res->actual_index_entry;
	strcpy(pkg_res->name, my_dxa_entry->name);
	pkg_res->name_length = -1;			/* -1表示名称以NULL结尾 */
	pkg_res->raw_data_length = my_dxa_entry->comprlen;
	pkg_res->actual_data_length = my_dxa_entry->uncomprlen;
	pkg_res->offset = my_dxa_entry->offset;

	return 0;
}

/* 封包资源提取函数 */
static int DXArchive_dxa_extract_resource(struct package *pkg,
									   struct package_resource *pkg_res)
{
	DWORD comprlen = pkg_res->raw_data_length;
	BYTE *compr = new BYTE[comprlen];
	if (!compr)
		return -CUI_EMEM;

	if (pkg->pio->readvec(pkg, compr, comprlen, pkg_res->offset, IO_SEEK_SET)) {
		delete [] compr;
		return -CUI_EREADVEC;
	}

	u8 *Key = (u8 *)package_get_private(pkg);
	KeyConv(compr, comprlen, pkg_res->offset, Key);

	if (pkg_res->actual_data_length) {
		BYTE *uncompr = new BYTE[pkg_res->actual_data_length];
		if (!uncompr) {
			delete [] compr;
			return -CUI_EMEM;
		}

		// 解凍
		Decode(compr, uncompr);

		pkg_res->actual_data = uncompr;
	}

	pkg_res->raw_data = compr;

	return 0;
}

/* 资源保存函数 */
static int DXArchive_dxa_save_resource(struct resource *res, 
									struct package_resource *pkg_res)
{
	if (res->rio->create(res))
		return -CUI_ECREATE;

	if (pkg_res->actual_data && pkg_res->actual_data_length) {
		if (res->rio->write(res, pkg_res->actual_data, pkg_res->actual_data_length)) {
			res->rio->close(res);
			return -CUI_EWRITE;
		}
	} else if (pkg_res->raw_data && pkg_res->raw_data_length) {
		if (res->rio->write(res, pkg_res->raw_data, pkg_res->raw_data_length)) {
			res->rio->close(res);
			return -CUI_EWRITE;
		}
	}

	res->rio->close(res);
	
	return 0;
}

/* 封包资源释放函数 */
static void DXArchive_dxa_release_resource(struct package *pkg, 
										   struct package_resource *pkg_res)
{
	if (pkg_res->actual_data) {
		delete [] pkg_res->actual_data;
		pkg_res->actual_data = NULL;
	}

	if (pkg_res->raw_data) {
		delete [] pkg_res->raw_data;
		pkg_res->raw_data = NULL;
	}
}

/* 封包卸载函数 */
static void DXArchive_dxa_release(struct package *pkg, 
								  struct package_directory *pkg_dir)
{
	void *priv = (void *)package_get_private(pkg);
	if (priv) {
		free(priv);
		package_set_private(pkg, NULL);
	}

	if (pkg_dir->directory) {
		delete [] pkg_dir->directory;
		pkg_dir->directory = NULL;
	}

	pkg->pio->close(pkg);
}

/* 封包处理回调函数集合 */
static cui_ext_operation DXArchive_dxa_operation = {
	DXArchive_dxa_match,				/* match */
	DXArchive_dxa_extract_directory,	/* extract_directory */
	DXArchive_dxa_parse_resource_info,	/* parse_resource_info */
	DXArchive_dxa_extract_resource,		/* extract_resource */
	DXArchive_dxa_save_resource,		/* save_resource */
	DXArchive_dxa_release_resource,		/* release_resource */
	DXArchive_dxa_release				/* release */
};

/* 接口函数: 向cui_core注册支持的封包类型 */
int CALLBACK DXArchive_register_cui(struct cui_register_callback *callback)
{
	/* 注册cui插件支持的扩展名、资源放入扩展名、处理回调函数和封包属性 */
	if (callback->add_extension(callback->cui, _T(".dxa"), NULL, 
		NULL, &DXArchive_dxa_operation, CUI_EXT_FLAG_PKG | CUI_EXT_FLAG_DIR))
			return -1;

	if (callback->add_extension(callback->cui, _T(".hud"), NULL, 
		NULL, &DXArchive_dxa_operation, CUI_EXT_FLAG_PKG | CUI_EXT_FLAG_DIR))
			return -1;

	if (callback->add_extension(callback->cui, _T(".dat"), NULL, 
		NULL, &DXArchive_dxa_operation, CUI_EXT_FLAG_PKG | CUI_EXT_FLAG_DIR))
			return -1;

	return 0;
}
}
