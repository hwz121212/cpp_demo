// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SYSTOOLS_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SYSTOOLS_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SYSTOOLS_EXPORTS
#define SYSTOOLS_API __declspec(dllexport)
#else
#define SYSTOOLS_API __declspec(dllimport)
#endif

SYSTOOLS_API vector<wstring> OpenFileDlg(HWND hParentWnd = NULL, const char* lpszDefExt = NULL, const char* lpszFileName = NULL, const char* lpszFilter = NULL, bool bAllowMultiSelect = false);
SYSTOOLS_API vector<wstring> SaveFileDlg(HWND hParentWnd, const char* lpszDefExt, const char* lpszFileName, const char* lpszFilter);

EXTERN_C SYSTOOLS_API void ShowCaptureWnd(HWND hWnd);