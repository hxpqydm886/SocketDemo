
// QQ_ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQ_Server.h"
#include "QQ_ServerDlg.h"
#include "afxdialogex.h"
#include "EventSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQQ_ServerDlg �Ի���

EventSelect myserver;

CQQ_ServerDlg::CQQ_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQQ_ServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQ_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT1, m_string);
}

BEGIN_MESSAGE_MAP(CQQ_ServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CQQ_ServerDlg::OnEnChangeEdit1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CQQ_ServerDlg::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BUTTON2, &CQQ_ServerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CQQ_ServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CQQ_ServerDlg ��Ϣ�������

BOOL CQQ_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	LONG lStyle;
	lStyle = GetWindowLong(m_list1.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_list1.m_hWnd, GWL_STYLE, lStyle);//����style
	SetWindowLong(m_list2.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_list1.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_list1.SetExtendedStyle(dwStyle); //������չ���
	m_list2.SetExtendedStyle(dwStyle); //������չ���

	m_list1.InsertColumn(0, _T("���"), LVCFMT_LEFT, 50);//�����У���������ó�0��������ϾͿ��������У�������Ż�Ա��
	m_list1.InsertColumn(1, _T("IP"), LVCFMT_LEFT, 80);
	m_list1.InsertColumn(2, _T("�˿�"), LVCFMT_LEFT, 80);

	m_list2.InsertColumn(0, _T("���Ͷ˱��"), LVCFMT_LEFT, 100);//�����У���������ó�0��������ϾͿ��������У�������Ż�Ա��
	m_list2.InsertColumn(1, _T("���ն˱��"), LVCFMT_LEFT, 100);
	m_list2.InsertColumn(2, _T("��Ϣ"), LVCFMT_LEFT, 280);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQQ_ServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CQQ_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CQQ_ServerDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CQQ_ServerDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


UINT ThreadFunc(LPVOID lpParam)
{
	myserver.define();
	myserver.initSocket();
	myserver.initEvent();
	myserver.dealEvent();
	return 0;
}

void CQQ_ServerDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	pServerThread = AfxBeginThread(ThreadFunc, this);
}



//����ϵͳ�㲥0|0|0|��Ϣ
void CQQ_ServerDlg::OnBnClickedButton1()
{
	//��ȡeditctrl���ı�
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	int n = str.GetLength();
	int len = myserver.clients.size();
	int i;
	char *p = new char[1024];
	*p = '0';
	*(p + 1) = '|';
	*(p + 2) = '0';
	*(p + 3) = '|';
	*(p + 4) = '0';
	*(p + 5) = '|';
	for (i = 0; i < n; i++)
	{
		*(p + 6 + i) = str.GetAt(i);
	}
	*(p + 6 + n) = '|';
	*(p + 7 + n) = '\0';
	//���͵��ͻ���	
	for (int i = 1; i < len+1; i++)
	{
		send(myserver.sockArray[i], p, strlen(p), 0);
	}

	//������ʾ
	CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2);
	list->InsertItem(myserver.msgNumber, 0);
	//���뷢�Ͷ�
	list->SetItemText(myserver.msgNumber, 0, _T("0"));
	//������ն�
	list->SetItemText(myserver.msgNumber, 1, _T("0"));
	//������Ϣ
	list->SetItemText(myserver.msgNumber, 2, str);
	delete p;
	myserver.msgNumber++;

}

