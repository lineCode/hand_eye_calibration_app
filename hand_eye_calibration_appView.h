// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// hand_eye_calibration_appView.h : Chand_eye_calibration_appView ��Ľӿ�
//

#pragma once
#include "hand_eye_calibration_appDoc.h"

class Chand_eye_calibration_appView : public CView
{
protected: // �������л�����
	Chand_eye_calibration_appView();
	DECLARE_DYNCREATE(Chand_eye_calibration_appView)

// ����
public:
	Chand_eye_calibration_appDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Chand_eye_calibration_appView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	
};

#ifndef _DEBUG  // hand_eye_calibration_appView.cpp �еĵ��԰汾
inline Chand_eye_calibration_appDoc* Chand_eye_calibration_appView::GetDocument() const
   { return reinterpret_cast<Chand_eye_calibration_appDoc*>(m_pDocument); }
#endif

