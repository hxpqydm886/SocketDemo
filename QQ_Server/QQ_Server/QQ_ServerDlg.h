
// QQ_ServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "EventSelect.h"
#include "Resource.h"


// CQQ_ServerDlg �Ի���
class CQQ_ServerDlg : public CDialogEx
{
// ����
public:
	CQQ_ServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QQ_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_list1;
	CListCtrl m_list2;
	CEdit m_edit;
	CString m_string;
	CWinThread *pServerThread;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
