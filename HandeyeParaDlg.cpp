// HandeyeParaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hand_eye_calibration_app.h"
#include "HandeyeParaDlg.h"
#include "afxdialogex.h"
#include "RInputDlg.h"


// CHandeyeParaDlg �Ի���

IMPLEMENT_DYNAMIC(CHandeyeParaDlg, CDialogEx)

CHandeyeParaDlg::CHandeyeParaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHandeyeParaDlg::IDD, pParent)
{
	i = 0;
}

CHandeyeParaDlg::~CHandeyeParaDlg()
{
}

void CHandeyeParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HANDEYE_LIST, m_listBox);
}


BEGIN_MESSAGE_MAP(CHandeyeParaDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CHandeyeParaDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CHandeyeParaDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHandeyeParaDlg ��Ϣ�������


void CHandeyeParaDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nCurSel;
	vector<Mat>::iterator iter_A, iter_B;
	iter_A = A.begin();
	iter_B = B.begin();

	nCurSel = m_listBox.GetCurSel();
	A.erase(iter_A + nCurSel);
	B.erase(iter_B + nCurSel);

	m_listBox.DeleteString(nCurSel);
}


void CHandeyeParaDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CRInputDlg RInputDlg;       // ����Ի�����CRInputDlg��ʵ��

	RInputDlg.s_imageList = s_imageList;
	RInputDlg.image_corner = image_corner;
	RInputDlg.rvecs = rvecs;
	RInputDlg.tvecs = tvecs;
	nRes = RInputDlg.DoModal();  // �����Ի���
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
	UpdateData(TRUE);

	//ͨ������ͼ���RT�ͻ�е���˶�RT������AB

	Mat B1(4, 4, CV_32F, Scalar(0));
	Mat B2(4, 4, CV_32F, Scalar(0));
	Mat a(4, 4, CV_32F, Scalar(0));
	Mat b(4, 4, CV_32F, Scalar(0));
	Rect rect1(0, 0, 3, 3), rect2(3, 0, 1, 3);

	B1(rect1) = RInputDlg.R1;
	B1(rect2) = RInputDlg.T1;
	/*Mat B11(B1, rect1);
	Mat B12(B1, rect2);
	B11 = RInputDlg.R1;
	B12 = RInputDlg.T1;*/
	B1.at<float>(3, 3) = 1;

	/*Mat B21(B2, rect1);
	Mat B22(B2, rect2);
	B21 = RInputDlg.R2;
	B22 = RInputDlg.T2;*/
	B2(rect1) = RInputDlg.R2;
	B2(rect2) = RInputDlg.T2;
	B2.at<float>(3, 3) = 1;

	/*Mat a1(a, rect1);
	Mat a2(a, rect2);
	a1 = RInputDlg.R;
	a2 = RInputDlg.T;*/
	a(rect1) = RInputDlg.R;
	a(rect2) = RInputDlg.T;
	a.at<float>(3, 3) = 1;

	b = B1*B2.inv();

	//����AB�������б�����ʾ
	A.push_back(a);
	B.push_back(b);
	CString str;
	str.Format(_T("move %d"), ++i);
	m_listBox.AddString(str);
}
