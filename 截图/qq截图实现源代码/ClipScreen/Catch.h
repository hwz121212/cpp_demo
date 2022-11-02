// Catch.h: interface for the CCatch class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __MAPOUT_H_     /* prevent multiple includes */
#define __MAPOUT_H_


#ifndef __DISPLAY_BMP_
#define OUTEXPORT  __declspec(dllimport) 
#else
#define OUTEXPORT  __declspec(dllexport)
#endif

#define SCREEN_SHOT NULL//ÆÁÄ»µÄ½ØÆÁ
#define DllImport  extern "C" __declspec( dllimport )
#define DllExport  extern "C" __declspec( dllexport )

extern "C"
{
	OUTEXPORT int WINAPI  StartCatchScreen(CWnd *pSnapshotWnd = SCREEN_SHOT);
}
#endif