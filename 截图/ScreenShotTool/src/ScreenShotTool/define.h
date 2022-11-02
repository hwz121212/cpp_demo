#pragma once
#include "CRectTrackerEx.h"

#define Gray_Width      1           // 【画布宽度】
#define Gray_Height     1           // 【画布高度】

#define ALPHA_GRAY      50          // 【透明度】
#define ALPHA_NO        0           // 【不透明】

#define MAGNIFY_BORDER  20          // 【放大镜边框大小】
#define MAGNIFY_SIZE    150         // 【放大镜大小】

#define MAGNIFY_MULTIPLE 2          // 【放大镜倍数】

#define TOOLBAR_WIDTH   360         // 【工具条大小】
#define TOOLBAR_SIZE    20          // 【工具条大小】

#define BACKGROUND_COLOR      RGB(0,   0,   0)          // 【背景颜色】
#define RECTANGLE_COLOR       RGB(255, 127, 39 )        // 【矩形颜色】
#define LINE_COLOR            RGB(0,   0,   255)        // 【线的颜色】
#define PEN_COLOR             RGB(255, 174, 201)        // 【线的颜色】

#define IDC_PEN             MAKEINTRESOURCE(32631)      // 【笔形光标】

enum SCREENSTATES
{
    SCREEN_UNDO  = 0,
    SCREEN_DOING,
    SCREEN_DONE,
};

// 【橡皮筋区域码】
enum TRACKAREA
{
    AREA_OUTSIDE = -1,      //【区域外部】
    AREA_LEFTUP,            //【左上】
    AREA_RIGHTUP,           //【右上】
    AREA_RIGHTDOWN,         //【右下】
    AREA_LETDOWN,           //【左下】
    AREA_UP,                //【上】
    AREA_RIGHT,             //【右】
    AREA_DOWN,              //【下】
    AREA_LEFT,              //【左】
    AREA_INSIDE,            //【内部】
};

enum GRAPH
{
    GRAPH_LINE = 0,         // 【线】
    GRAPH_RECTANGLE,        // 【矩形】
    GRAPH_PEN,              // 【画笔】
};

//直线方程式ax+by+c=0的参数结构
typedef struct _LineParam
{
    double a;   //x的参数
    double b;   //y的参数(为1或0)
    double c;   //常数
}LineParam;

// 【用来保存绘制的图型，便于撤销】
typedef struct  _GraphParam
{
    int             m_nType;    // 【图像类型，线or矩形】
    CPoint          m_pPosX;    // 【线的起点】
    CPoint          m_pPosY;    // 【线的终点】
    CRectTrackerEx  m_rt;       // 【矩形】

    vector<CPoint>  m_vecPen;

    _GraphParam()
    {
        // 【类型初始化】
        m_nType  =  -1;

        // 【线初始化】
        {
            m_pPosX.x = 0;
            m_pPosX.y = 0;
            m_pPosY.x = 0;
            m_pPosY.y = 0;
        }

        // 【矩形初始化】
        {
            m_rt.m_rect.SetRectEmpty();
            m_rt.m_nStyle = CRectTracker::solidLine;
        }
        {
            m_vecPen.clear();
        }
    }
}GraphParam;