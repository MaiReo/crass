#include <tchar.h>
#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <vector>

#include "../common/SDK/include/crass/locale.h"
#include "../common/SDK/include/utility.h"

#if FALSE
#ifndef FIRST_BUILD_UTILITY
#define FIRST_BUILD_UTILITY
#endif // !FIRST_BUILD_UTILITY
#endif

using namespace std;
using std::vector;

static struct locale_configuration locale_current_configuration;
static const TCHAR **early_locale_string_table;

/****************** early locale API ******************/

static const TCHAR *default_early_strings[] = {
	_T("failed to search language module\n"),
	_T("%s: invalid loc path\n"),
	_T("%s: invalid loc name\n"),
	_T("%s: failed to load loc\n"),
	_T("%s: can\'t find the configuration information of loc\n"),
	_T("loaded %d language modules ...\n\n\n"),
	NULL
};

static const TCHAR *simplified_chinese_early_strings[] = {
	_T("搜索language组件时发生错误\n"),
	_T("%s: 无效的loc路径\n"),
	_T("%s: 无效的loc名称\n"),
	_T("%s: 加载loc时发生错误\n"),
	_T("%s: 没有找到loc配置信息\n"),
	_T("加载%d个language组件 ...\n\n\n"),
	NULL
};

static const TCHAR *traditional_chinese_early_strings[] = {
	_T("搜索language組件時發生錯誤\n"),
	_T("%s: 無效的loc路徑\n"),
	_T("%s: 無效的loc名稱\n"),
	_T("%s: 加載loc時發生錯誤\n"),
	_T("%s: 沒有找到loc配置信息\n"),
	_T("加載%d個language組件 ...\n\n\n"),
	NULL
};

static const TCHAR *japanese_early_strings[] = {
	_T("languageパーツの捜索時にエラーが発生します\n"),
	_T("%s: 無効なlocパス\n"),
	_T("%s: 無効なloc名称\n"),
	_T("%s: locの読み込み時にエラーが発生します\n"),
	_T("%s: locの配置メッセージが見つかりません\n"),
	_T("languageパーツ%d個を読み込む ...\n\n\n"),
	NULL
};

/* TODO: put new local locale string here */

static struct locale_configuration locale_local_configurations[] = {
	{ 936, simplified_chinese_early_strings, },
	{ 950, traditional_chinese_early_strings, },
	{ 932, japanese_early_strings, },
	/* TODO: put new local locale string here */
	{ -1, NULL, }
};

static void locale_init_early(void)
{
	struct locale_configuration *config = locale_local_configurations;
	locale_current_configuration.acp = GetACP();
	locale_current_configuration.string_table = default_early_strings;
	early_locale_string_table = default_early_strings;
	locale_current_configuration.module = NULL;
	while (config->string_table) {
		if (config->acp == locale_current_configuration.acp) {
			locale_current_configuration.string_table = config->string_table;
			early_locale_string_table = config->string_table;
			break;
		}
		++config;
	}
	const TCHAR **tbl = locale_current_configuration.string_table;
	DWORD k;
	for (k = 0; tbl[k]; ++k);
	locale_current_configuration.max_id = k;
}

/****************** general locale API ******************/

LOCALE_API const TCHAR *locale_load_string(DWORD id)
{
	if (id >= locale_current_configuration.max_id)
		return NULL;

	return locale_current_configuration.string_table[id];
}

#define lls(id)		locale_load_string(id)

static int locale_module_register(const TCHAR *path)
{
	TCHAR *name;
	TCHAR loc_name[MAX_PATH];

	name = PathFindFileName(path);
	if (!name) {
#ifndef FIRST_BUILD_UTILITY
		crass_printf(early_locale_string_table[1], loc_name);
#endif
		return -1;
	}
	_tcscpy(loc_name, name);
	if (!PathRenameExtension(loc_name, _T(""))) {
#ifndef FIRST_BUILD_UTILITY
		crass_printf(early_locale_string_table[2], loc_name);
#endif
		return -1;
	}

	HMODULE loc_mod = LoadLibrary(path);
	if (!loc_mod) {
#ifndef FIRST_BUILD_UTILITY
		crass_printf(early_locale_string_table[3], loc_name);
#endif
		return -1;
	}

	TCHAR loc_config_name[MAX_PATH];
	_stprintf(loc_config_name, _T("%s_locale_configuration"), loc_name);

#ifdef UNICODE
	char mbcs_config_name[MAX_PATH];
	wcstombs(mbcs_config_name, loc_config_name, sizeof(mbcs_config_name));
	struct locale_configuration *loc_cfg = (struct locale_configuration *)GetProcAddress(loc_mod, mbcs_config_name);
#else
	struct locale_configuration *loc_cfg = (struct locale_configuration *)GetProcAddress(loc_mod, loc_config_name);	
#endif

	if (!loc_cfg) {
		FreeLibrary(loc_mod);
#ifndef FIRST_BUILD_UTILITY
		crass_printf(early_locale_string_table[3], loc_name);
#endif // !FIRST_BUILD_UTILITY	
		return -1;	
	}

	int ret = -1;
	if (loc_cfg->acp == locale_current_configuration.acp)
		ret = 1;
	else if (!loc_cfg->acp)
		ret = 0;

	if (ret >= 0) {
		if (locale_current_configuration.module)
			FreeLibrary(locale_current_configuration.module);
		locale_current_configuration.string_table = loc_cfg->string_table;
		DWORD k;
		for (k = 0; loc_cfg->string_table[k]; ++k);
		locale_current_configuration.max_id = k;
		locale_current_configuration.module = loc_mod;
	} else
		FreeLibrary(loc_mod);

	return ret;
}

LOCALE_API int locale_init(void)
{
	HANDLE find_file;
	WIN32_FIND_DATA find_data;
	int count;

	locale_init_early();
	
	find_file = FindFirstFile(_T("language\\*.loc"), &find_data);
	if (find_file == INVALID_HANDLE_VALUE) {
#ifndef FIRST_BUILD_UTILITY
		locale_printf(0, NULL);
#endif
		return -1;
	}

	count = 0;

	TCHAR loc_path[MAX_PATH];
	_stprintf(loc_path, _T("language\\%s"), find_data.cFileName);

	const TCHAR *last_msg = locale_current_configuration.string_table[locale_current_configuration.max_id-1];

	/* 忽略注册失败的loc */
	int ret = locale_module_register(loc_path);
	if (ret >= 0) {
		++count;
		if (ret)
			goto out;
	}

	while (FindNextFile(find_file, &find_data)) {
		_stprintf(loc_path, _T("language\\%s"), find_data.cFileName);
		int ret = locale_module_register(loc_path);
		if (ret >= 0) {
			++count;
			if (ret)
				goto out;
		}
	}
out:
	FindClose(find_file);
#ifndef FIRST_BUILD_UTILITY
	crass_printf(last_msg, count);
#endif

	return count;
}

static vector<struct locale_configuration> locale_app_configurations;

LOCALE_API const TCHAR *locale_app_load_string(DWORD cid, DWORD id)
{
	if (cid >= locale_app_configurations.size())
		return NULL;

	if (id >= locale_app_configurations[cid].max_id)
		return NULL;

	return locale_app_configurations[cid].string_table[id];
}

LOCALE_API int locale_app_register(struct locale_configuration *config, DWORD count)
{
	struct locale_configuration *def_config;

	if (!config || !count)
		return -1;

	def_config = NULL;
	for (DWORD i = 0; i < count; ++i) {
		if (!config->max_id) {
			DWORD k;
			for (k = 0; config->string_table[k]; ++k);
			config->max_id = k;
		}
		if (config->acp == GetACP()) {
			locale_app_configurations.push_back(*config);
			goto out;
		} else if (!config->acp)
			def_config = config;
		++config;
	}
	if (def_config)
		locale_app_configurations.push_back(*def_config);
out:
	return locale_app_configurations.size() - 1;
}