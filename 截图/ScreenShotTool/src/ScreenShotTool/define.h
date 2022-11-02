#pragma once
#include "CRectTrackerEx.h"

#define Gray_Width      1           // ��������ȡ�
#define Gray_Height     1           // �������߶ȡ�

#define ALPHA_GRAY      50          // ��͸���ȡ�
#define ALPHA_NO        0           // ����͸����

#define MAGNIFY_BORDER  20          // ���Ŵ󾵱߿��С��
#define MAGNIFY_SIZE    150         // ���Ŵ󾵴�С��

#define MAGNIFY_MULTIPLE 2          // ���Ŵ󾵱�����

#define TOOLBAR_WIDTH   360         // ����������С��
#define TOOLBAR_SIZE    20          // ����������С��

#define BACKGROUND_COLOR      RGB(0,   0,   0)          // ��������ɫ��
#define RECTANGLE_COLOR       RGB(255, 127, 39 )        // ��������ɫ��
#define LINE_COLOR            RGB(0,   0,   255)        // ���ߵ���ɫ��
#define PEN_COLOR             RGB(255, 174, 201)        // ���ߵ���ɫ��

#define IDC_PEN             MAKEINTRESOURCE(32631)      // �����ι�꡿

enum SCREENSTATES
{
    SCREEN_UNDO  = 0,
    SCREEN_DOING,
    SCREEN_DONE,
};

// ����Ƥ�������롿
enum TRACKAREA
{
    AREA_OUTSIDE = -1,      //�������ⲿ��
    AREA_LEFTUP,            //�����ϡ�
    AREA_RIGHTUP,           //�����ϡ�
    AREA_RIGHTDOWN,         //�����¡�
    AREA_LETDOWN,           //�����¡�
    AREA_UP,                //���ϡ�
    AREA_RIGHT,             //���ҡ�
    AREA_DOWN,              //���¡�
    AREA_LEFT,              //����
    AREA_INSIDE,            //���ڲ���
};

enum GRAPH
{
    GRAPH_LINE = 0,         // ���ߡ�
    GRAPH_RECTANGLE,        // �����Ρ�
    GRAPH_PEN,              // �����ʡ�
};

//ֱ�߷���ʽax+by+c=0�Ĳ����ṹ
typedef struct _LineParam
{
    double a;   //x�Ĳ���
    double b;   //y�Ĳ���(Ϊ1��0)
    double c;   //����
}LineParam;

// ������������Ƶ�ͼ�ͣ����ڳ�����
typedef struct  _GraphParam
{
    int             m_nType;    // ��ͼ�����ͣ���or���Ρ�
    CPoint          m_pPosX;    // ���ߵ���㡿
    CPoint          m_pPosY;    // ���ߵ��յ㡿
    CRectTrackerEx  m_rt;       // �����Ρ�

    vector<CPoint>  m_vecPen;

    _GraphParam()
    {
        // �����ͳ�ʼ����
        m_nType  =  -1;

        // ���߳�ʼ����
        {
            m_pPosX.x = 0;
            m_pPosX.y = 0;
            m_pPosY.x = 0;
            m_pPosY.y = 0;
        }

        // �����γ�ʼ����
        {
            m_rt.m_rect.SetRectEmpty();
            m_rt.m_nStyle = CRectTracker::solidLine;
        }
        {
            m_vecPen.clear();
        }
    }
}GraphParam;