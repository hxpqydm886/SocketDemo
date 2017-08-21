
// QQ_ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"
#include <vector>
#include<iostream>
using namespace std;

struct threadInfo{
	SOCKET sClient;
	//string m_id;
	sockaddr_in addrRemote;
};

UINT ThreadFunc(LPVOID lpParam);
// CQQ_ClientDlg �Ի���
class CQQ_ClientDlg : public CDialogEx
{
// ����
public:
	CQQ_ClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QQ_CLIENT_DIALOG };

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

public :
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton4();
	CEdit m_edit;
	CWinThread *pClientThread;
	void connectToServer();
	void closeSocket();
	CListCtrl m_list1;
	CListCtrl m_list2;
	CString m_string;
	void updatelist();
	//vector<string>m_ids;//�����б�
	//string m_id;//
	afx_msg void OnEnChangeEdit3();
};

