// RInputDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hand_eye_calibration_app.h"
#include "RInputDlg.h"
#include "afxdialogex.h"


// CRInputDlg �Ի���

IMPLEMENT_DYNAMIC(CRInputDlg, CDialogEx)

CRInputDlg::CRInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRInputDlg::IDD, pParent)
	, R(3, 3, CV_32F, Scalar(0))
	, T(3, 1, CV_32F, Scalar(0))
{
}

CRInputDlg::~CRInputDlg()
{
}

BOOL CRInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	auto iter_image_list = s_imageList.begin();
	for (; iter_image_list != s_imageList.end(); iter_image_list++){
		m_listBox.AddString(*iter_image_list);
	}
	return TRUE;
}

void CRInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Control(pDX, IDC_IMAGE1, m_image1);
	DDX_Control(pDX, IDC_IMAGE2, m_image2);
	DDX_Text(pDX, IDC_EDIT1, R.at<float>(0, 0));
	DDX_Text(pDX, IDC_EDIT2, R.at<float>(0, 1));
	DDX_Text(pDX, IDC_EDIT3, R.at<float>(0, 2));
	DDX_Text(pDX, IDC_EDIT4, R.at<float>(1, 0));
	DDX_Text(pDX, IDC_EDIT5, R.at<float>(1, 1));
	DDX_Text(pDX, IDC_EDIT6, R.at<float>(1, 2));
	DDX_Text(pDX, IDC_EDIT7, R.at<float>(2, 0));
	DDX_Text(pDX, IDC_EDIT8, R.at<float>(2, 1));
	DDX_Text(pDX, IDC_EDIT9, R.at<float>(2, 2));
	DDX_Text(pDX, IDC_EDIT14, T.at<float>(0));
	DDX_Text(pDX, IDC_EDIT15, T.at<float>(1));
	DDX_Text(pDX, IDC_EDIT16, T.at<float>(2));
}


BEGIN_MESSAGE_MAP(CRInputDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRInputDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRInputDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CRInputDlg ��Ϣ�������


void CRInputDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nCursel;
	nCursel = m_listBox.GetCurSel();
	Rodrigues(rvecs[nCursel], R1);
	T1 = tvecs[nCursel];

	//��ʾͼƬ

	CImage c_image;
	Mat image = image_corner[nCursel];

	//create new CImage  
	int width = image.cols;
	int height = image.rows;
	int channels = image.channels();

	c_image.Destroy(); //clear  
	c_image.Create(width,
		height, //positive: left-bottom-up   or negative: left-top-down  
		8 * channels); //numbers of bits per pixel  

	//copy values  
	uchar* ps;
	uchar* pimg = (uchar*)c_image.GetBits(); //A pointer to the bitmap buffer  

	//The pitch is the distance, in bytes. represent the beginning of   
	// one bitmap line and the beginning of the next bitmap line  
	int step = c_image.GetPitch();

	for (int i = 0; i < height; ++i)
	{
		ps = (image.ptr<uchar>(i));
		for (int j = 0; j < width*3; ++j)
		{
			*(pimg + i*step + j) = ps[j];
		}
	}

	CRect rect;//���������  
	int cx = c_image.GetWidth();//��ȡͼƬ���  
	int cy = c_image.GetHeight();//��ȡͼƬ�߶�  

	GetDlgItem(IDC_IMAGE1)->GetWindowRect(&rect);//�����ھ���ѡ�е�picture�ؼ���  
	ScreenToClient(&rect);//���ͻ���ѡ�е�Picture�ؼ���ʾ�ľ���������  
	GetDlgItem(IDC_IMAGE1)->MoveWindow(rect, TRUE);//�������ƶ���Picture�ؼ���ʾ�ľ�������  

	CWnd *pWnd = GetDlgItem(IDC_IMAGE1);//���pictrue�ؼ����ڵľ��  

	pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������  

	CDC *pDC = pWnd->GetDC();//���pictrue�ؼ���DC  

	pDC->SetStretchBltMode(COLORONCOLOR);
//	img.Draw(pDC->m_hDC, dispRect);

	c_image.Draw(pDC->m_hDC, rect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
	ReleaseDC(pDC);//�ͷ�picture�ؼ���DC  
}


void CRInputDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nCursel;
	nCursel = m_listBox.GetCurSel();
	Rodrigues(rvecs[nCursel], R2);
	T2 = tvecs[nCursel];

	//��ʾͼƬ

	CImage c_image;
	Mat image = image_corner[nCursel];

	//create new CImage  
	int width = image.cols;
	int height = image.rows;
	int channels = image.channels();

	c_image.Destroy(); //clear  
	c_image.Create(width,
		height, //positive: left-bottom-up   or negative: left-top-down  
		8 * channels); //numbers of bits per pixel  

	//copy values  
	uchar* ps;
	uchar* pimg = (uchar*)c_image.GetBits(); //A pointer to the bitmap buffer  

	//The pitch is the distance, in bytes. represent the beginning of   
	// one bitmap line and the beginning of the next bitmap line  
	int step = c_image.GetPitch();

	for (int i = 0; i < height; ++i)
	{
		ps = (image.ptr<uchar>(i));
		for (int j = 0; j < width; ++j)
		{
			if (channels == 1) //gray  
			{
				*(pimg + i*step + j) = ps[j];
			}
			else if (channels == 3) //color  
			{
				for (int k = 0; k < 3; ++k)
				{
					*(pimg + i*step + j * 3 + k) = ps[j * 3 + k];
				}
			}
		}
	}

	CRect rect;//���������  
	int cx = c_image.GetWidth();//��ȡͼƬ���  
	int cy = c_image.GetHeight();//��ȡͼƬ�߶�  

	GetDlgItem(IDC_IMAGE2)->GetWindowRect(&rect);//�����ھ���ѡ�е�picture�ؼ���  
	ScreenToClient(&rect);//���ͻ���ѡ�е�Picture�ؼ���ʾ�ľ���������  
	GetDlgItem(IDC_IMAGE2)->MoveWindow(rect, TRUE);//�������ƶ���Picture�ؼ���ʾ�ľ�������  

	CWnd *pWnd = GetDlgItem(IDC_IMAGE2);//���pictrue�ؼ����ڵľ��  

	pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������  

	CDC *pDC = pWnd->GetDC();//���pictrue�ؼ���DC  

	pDC->SetStretchBltMode(COLORONCOLOR);

	c_image.Draw(pDC->m_hDC, rect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
	ReleaseDC(pDC);//�ͷ�picture�ؼ���DC  
}
