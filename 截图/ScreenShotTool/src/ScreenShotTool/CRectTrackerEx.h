#pragma once
#include "afxext.h"

class CRectTrackerEx :
    public CRectTracker
{
public:
    CRectTrackerEx(void);
    ~CRectTrackerEx(void);

    void Draw(CDC* pDC, CPen * pen);    // ����Ƥ����չ�࣬���Ըı���Ƥ����ɫ��
};
