#pragma once
#include "afxext.h"

class CRectTrackerEx :
    public CRectTracker
{
public:
    CRectTrackerEx(void);
    ~CRectTrackerEx(void);

    void Draw(CDC* pDC, CPen * pen);    // 【橡皮筋扩展类，可以改变橡皮筋颜色】
};
