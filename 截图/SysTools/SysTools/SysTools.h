// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SYSTOOLS_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SYSTOOLS_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SYSTOOLS_EXPORTS
#define SYSTOOLS_API __declspec(dllexport)
#else
#define SYSTOOLS_API __declspec(dllimport)
#endif

SYSTOOLS_API vector<wstring> OpenFileDlg(HWND hParentWnd = NULL, const char* lpszDefExt = NULL, const char* lpszFileName = NULL, const char* lpszFilter = NULL, bool bAllowMultiSelect = false);
SYSTOOLS_API vector<wstring> SaveFileDlg(HWND hParentWnd, const char* lpszDefExt, const char* lpszFileName, const char* lpszFilter);

EXTERN_C SYSTOOLS_API void ShowCaptureWnd(HWND hWnd);