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

// hand_eye_calibration_appView.cpp : Chand_eye_calibration_appView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "hand_eye_calibration_app.h"
#endif

#include "hand_eye_calibration_appDoc.h"
#include "hand_eye_calibration_appView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Chand_eye_calibration_appView

IMPLEMENT_DYNCREATE(Chand_eye_calibration_appView, CView)

BEGIN_MESSAGE_MAP(Chand_eye_calibration_appView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Chand_eye_calibration_appView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Chand_eye_calibration_appView ����/����

Chand_eye_calibration_appView::Chand_eye_calibration_appView()
{
	// TODO:  �ڴ˴���ӹ������

}

Chand_eye_calibration_appView::~Chand_eye_calibration_appView()
{
}

BOOL Chand_eye_calibration_appView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Chand_eye_calibration_appView ����

void Chand_eye_calibration_appView::OnDraw(CDC* /*pDC*/)
{
	Chand_eye_calibration_appDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Chand_eye_calibration_appView ��ӡ


void Chand_eye_calibration_appView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Chand_eye_calibration_appView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Chand_eye_calibration_appView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Chand_eye_calibration_appView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}

void Chand_eye_calibration_appView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Chand_eye_calibration_appView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Chand_eye_calibration_appView ���

#ifdef _DEBUG
void Chand_eye_calibration_appView::AssertValid() const
{
	CView::AssertValid();
}

void Chand_eye_calibration_appView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Chand_eye_calibration_appDoc* Chand_eye_calibration_appView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Chand_eye_calibration_appDoc)));
	return (Chand_eye_calibration_appDoc*)m_pDocument;
}
#endif //_DEBUG


// Chand_eye_calibration_appView ��Ϣ�������


