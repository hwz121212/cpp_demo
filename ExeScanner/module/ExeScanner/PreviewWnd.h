/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PreviewWnd.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class CPreviewWnd.
*
* NOTES:		Preview window for the resource Images.
*
******************************************************************************/
#pragma once


// CPreviewWnd
class CImageImportDlg;

class CPreviewWnd : public CWnd
{
	DECLARE_DYNAMIC(CPreviewWnd)

public:
	CPreviewWnd();
	virtual ~CPreviewWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:

	int m_DispW; //Width of window
	int m_DispH;//Height of window
	int m_iScrollX;//current scroll position (horizontal)
	int m_iScrollY;//current scroll position (vertical)
	int m_iWidth;//width of image
	int m_iHeight;//height of image
	float m_movX;//when scroll position moves by one position, how many pixels should be moved?
	float m_movY;//when scroll position moves by one position, how many pixels should be moved?
	CDC *m_pDC;

	CImageImportDlg * m_pDlg;//Parent who generated me.
	afx_msg void OnPaint();
	void SetDlg(CImageImportDlg * pDlg, CDC* pDC,int iWidth,int iHeight);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


