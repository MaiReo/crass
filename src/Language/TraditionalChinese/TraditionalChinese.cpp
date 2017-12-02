#include <tchar.h>
#include <windows.h>
#include <crass/locale.h>

static const TCHAR *string_table[] = {
	_T("程序：\tCrage - 使用cui插件擴展的通用遊戲資源提取器\n"),
	_T("作者：\t%s\n"),	
	_T("版本：\t%s\n"),
	_T("日期：\t%s\n"),
	_T("發佈：\t%s\n"),
	_T("語法：\n\t%s %s\n\n"),
	_T("語法錯誤，請閱讀FAQ和INSTALL獲得更多的幫助信息\n\n"),
	_T("%s: 獲取directory信息失敗(%d)\n"),
	_T("要繼續提取下一個封包嗎？(y/n)\n"),
	_T("%s：無效的directory信息\n"),
	_T("%s：不正確的directory信息\n"),
	_T("%s: 準備提取封包文件（含%d個資源文件） ...\n"),
	_T("%s: 準備提取資源封包文件 ...\n"),
	_T("%s: 解析resource信息失敗(%d)\n"),
	_T("要繼續提取封包內的其他資源嗎？(y/n)\n"),
	_T("%s: 無效的resource名稱\n"),
	_T("%s: 無效的resource\n"),
	_T("%s: 資源文件名轉換失敗\n"),
	_T("%s: 無效的資源文件(%d)\n"),
	_T("%s: 提取資源文件失敗(%d)\n"),
	_T("%s: 定位資源文件失敗\n"),
	_T("%s: 構造資源文件存儲路徑失敗\n"),
	_T("%s: 提取資源文件時發生錯誤(%d)\n"),
	_T("%s: 保存失敗\n"),
 	_T("磁盤空間不足，要重試嗎？(y/n)\n"),
 	_T("%s: 指定的資源提取成功\n"),
 	_T("%s: 成功提取%ld / %ld 個資源文件 %c\r"),
 	_T("%s: 成功提取%ld / %ld 個資源文件\n"),
 	_T("%s：提取第%d個時失敗\n"),
	_T("\n%s：提取封包%s時發生錯誤(%d)\n"),
 	_T("%s：成功提取%d個%s資源封包文件 %c\r"),
 	_T("%s：成功提取%d個%s封包文件          \n\n"),
 	_T("%s: 指定的cui不存在\n"),
 	_T("\n\n初始化cui core ...\n"),
 	_T("沒有找到任何cui\n"),
 	_T("加載%d個cui ...\n"),
 	_T("%s: 支持"),
	_T("\n初始化package core ...\n"),
	_T("%s: 沒有找到任何package\n"),
	_T("讀取%d個封包文件信息 ...\n"),
	_T("\n開始執行解包操作 ...\n\n"),
	_T("\n\n\t\t\t\t\t\t\t... 解包操作執行完成\n\n"),
	_T("版權：\t%s\n"),
	_T("系統：\t%s\n"),
	_T("封包：\t%s\n"),
	_T("版本：\t%s\n"),
	_T("作者：\t%s\n"),
	_T("時間：\t%s\n"),
	_T("注意：\t%s\n"),
	_T("%s: 搜尋單個package時發生錯誤\n"),
	_T("%s: 搜尋第一個package時發生錯誤 (%d)\n"),
	NULL
};

__declspec(dllexport) struct locale_configuration TraditionalChinese_locale_configuration = {
	950,
	string_table
};
;
