#pragma once



// FormView1 ������ͼ

class FormView1 : public CFormView
{
	DECLARE_DYNCREATE(FormView1)

protected:
	FormView1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~FormView1();

public:
	enum { IDD = IDD_FORMVIEW1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


