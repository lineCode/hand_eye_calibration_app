#pragma once
#include "opencv2\opencv.hpp"
#include "afxwin.h"

using namespace std;
using namespace cv;

// CHandeyeParaDlg �Ի���

class CHandeyeParaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHandeyeParaDlg)

public:
	CHandeyeParaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHandeyeParaDlg();

	vector<Mat> A, B;
	int i;
	vector<CString> s_imageList;
	vector<Mat> rvecs, tvecs;
	vector<Mat> image_corner;

// �Ի�������
	enum { IDD = IDD_HANDEYEPARA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	CListBox m_listBox;
	afx_msg void OnBnClickedButton1();
};
