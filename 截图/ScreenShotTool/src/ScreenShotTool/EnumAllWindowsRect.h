#pragma once

#include <vector>
using namespace std;


class WindowRect
{
public:
    WindowRect();
    ~WindowRect();

    bool    GetRect(const POINT& point, RECT& rect);
    void    GetRect(vector<RECT>& rects);


    HWND    m_hwnd;
    CString m_className;
    RECT    m_rect;
    vector<WindowRect>     m_vecChildWindowRect;
};


class EnumAllWindowsRect
{
public:
    EnumAllWindowsRect(void);
    ~EnumAllWindowsRect(void);

    void EnumAllWindows();
    bool GetRect(const POINT& point, RECT& rect);
    void GetAllWindowsRect(vector<RECT>& rects);
    void ClearAllWindows();

private:
    void EnumWindowsCtrl();
    void EnumAllCtrl(WindowRect& win);
private:
    vector<WindowRect>  m_vectWindowsRect;
};
