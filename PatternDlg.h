#pragma once


// CPatternDlg �Ի���

class CPatternDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPatternDlg)

public:
	CPatternDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPatternDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float m_SquareSize;
	int m_PatternWidth;
	int m_PatternHeight;
	BOOL m_IsChess;
	BOOL m_IsWhite;
};
