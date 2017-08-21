
// QQ_ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQ_Client.h"
#include "QQ_ClientDlg.h"
#include "afxdialogex.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQQ_ClientDlg �Ի���

threadInfo info;
int m_msgNumber = 0;
string m_id="1";
//void updatelist();
vector<string>m_ids;//�����б�
int flag = false;
//�ַ����ָ��
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
CQQ_ClientDlg::CQQ_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQQ_ClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQ_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_LIST1, m_list2);
	DDX_Control(pDX, IDC_LIST2, m_list1);
	DDX_Text(pDX, IDC_EDIT1, m_string);
}

BEGIN_MESSAGE_MAP(CQQ_ClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CQQ_ClientDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON4, &CQQ_ClientDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

//�㲥�����߳�
UINT ThreadFunc(LPVOID lpParam)
{
	threadInfo* info = (threadInfo*)lpParam;

	while (true)
	{
		//��������
		vector<string> lines;
		char recvBuf[1024];
		memset(recvBuf, sizeof(recvBuf), '\0');  //��ջ�����

		int nRecv = ::recv(info->sClient, recvBuf, sizeof(recvBuf), 0);
		if (nRecv == SOCKET_ERROR)
		{
			//cout << "recv Error!" << endl;
			break;
		}
		if (nRecv > 0)
		{
			string str;
			str = recvBuf;
			//ostringstream os;
			//os << nRecv;
			//str = os.str();
			////����
			//CString cs;
			//cs = str.c_str();
			//CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2);
			//list->DeleteAllItems(); // ȫ����� 
			//list->InsertItem(0, 0);
			////������
			//list->SetItemText(0, 0, cs);
			////��������
			//list->SetItemText(0, 1, _T("����"));

			lines = split(str, "|");
			if (lines[0] == "1")//�û��б���Ϣ
			{
				if (flag == false)
				{
					CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2);
					list->DeleteAllItems(); // ȫ�����  
					for (int i = 1; i < nRecv/2; i++)
					{
						CString str_id;
						str_id = lines[i].c_str();
						list->InsertItem(i-1, 0);
						//������
						list->SetItemText(i-1, 0, str_id);
						//��������
						list->SetItemText(i-1, 1, _T("����"));
					}
					m_id = lines[lines.size() - 2];
					flag = true;
				}
				else
				{
					
					CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2);
					list->DeleteAllItems(); // ȫ�����
					int len = lines.size();
					for (int i = 1; i < nRecv/2; i++)
					{
						CString str_id;
						str_id = lines[i].c_str();
						list->InsertItem(i-1, 0);
						//������
						list->SetItemText(i-1, 0, str_id);
						//��������
						list->SetItemText(i-1, 1, _T("����"));
					}
				}
			}
			else//������Ϣ
			{
				if (lines[1] != m_id)
				{
					CString send_id, recv_id, msg;
					//TRACE(lines[);
					send_id = lines[1].c_str();//.....
					recv_id = lines[2].c_str();
					msg = lines[3].c_str();
					//�������
					CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST1);
					list->InsertItem(m_msgNumber, 0);
					//���뷢�Ͷ�
					list->SetItemText(m_msgNumber, 0, send_id);
					//������ն�
					list->SetItemText(m_msgNumber, 1, recv_id);
					//������Ϣ
					list->SetItemText(m_msgNumber, 2, msg);
					m_msgNumber++;
				}
			}		
		}
		lines.clear();
	}
	
	return 0;
}

// CQQ_ClientDlg ��Ϣ�������

BOOL CQQ_ClientDlg::OnInitDialog()
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

	m_list2.InsertColumn(0, _T("���Ͷ˱��"), LVCFMT_LEFT, 80);//�����У���������ó�0��������ϾͿ��������У�������Ż�Ա��
	m_list2.InsertColumn(1, _T("���ܶ˱��"), LVCFMT_LEFT, 80);
	m_list2.InsertColumn(2, _T("��Ϣ"), LVCFMT_LEFT, 200);

	m_list1.InsertColumn(0, _T("���"), LVCFMT_LEFT, 100);//�����У���������ó�0��������ϾͿ��������У�������Ż�Ա��
	m_list1.InsertColumn(1, _T("������Ϣ"), LVCFMT_LEFT, 100);

	//���ӵ�������
	connectToServer();
	pClientThread = AfxBeginThread(ThreadFunc, &info);
	//Ĭ��ѡ�������
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);//ѡ��
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);//  ��ѡ��


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQQ_ClientDlg::OnPaint()
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
HCURSOR CQQ_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CQQ_ClientDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CQQ_ClientDlg::connectToServer()
{
	//��ʼ��WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		//cout << "��ʼ��ʧ��" << endl;
		return;
	}

	info.sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (info.sClient == INVALID_SOCKET)
	{
		//printf("Failed socket()\n");
		return;
	}
	//Ҫ���ӵķ�������ַ
	info.addrRemote.sin_family = AF_INET;
	info.addrRemote.sin_port = htons(8888);
	info.addrRemote.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(info.sClient, (sockaddr *)&info.addrRemote, sizeof(info.addrRemote)) == SOCKET_ERROR)
	{
		printf("Failed connect()\n");
		return;
	}
}

void CQQ_ClientDlg::closeSocket()
{
	closesocket(info.sClient);
}
//������Ϣ
void CQQ_ClientDlg::OnBnClickedButton4()
{
	//��ȡeditctrl���ı�
	CString str;//�뷢�͵��ı�
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	int str_n = str.GetLength();
	CString str_m_id;
	str_m_id = m_id.c_str();
	int m_id_n = str_m_id.GetLength();
	int i;
	char *p = new char[1024];

	//���ѡ�пͻ��� 0|���Ͷ�|���ն�|��Ϣ
	if (((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck() == 1)
	{
		*p = '0';
		*(p + 1) = '|';
		for (i = 0; i < m_id_n; i++)
		{
			*(p + 2 + i) = str_m_id.GetAt(i);
		}
		//*(p + 2) = '1';
		*(p + 2 + m_id_n) = '|';
		CString str_id;
		GetDlgItem(IDC_EDIT2)->GetWindowText(str_id);
		int str_id_n = str_id.GetLength();
		for (i = 0; i < str_id_n; i++)
		{
			*(p + 3 + m_id_n + i) = str_id.GetAt(i);
		}
		*(p + 3 + m_id_n + str_id_n) = '|';
		for (i = 0; i < str_n; i++)
		{
			*(p + 4 + m_id_n + str_id_n + i) = str.GetAt(i);
		}
		*(p + 4 + m_id_n + str_id_n + str_n) = '|';
		*(p + 5 + m_id_n + str_id_n + str_n) = '\0';
		//���͵�������
		send(info.sClient, p, strlen(p), 0);
		//�������
		CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST1);
		list->InsertItem(m_msgNumber, 0);
		//���뷢�Ͷ�
		list->SetItemText(m_msgNumber, 0, str_m_id);
		//������ն�
		list->SetItemText(m_msgNumber, 1, str_id);
		//������Ϣ
		list->SetItemText(m_msgNumber, 2, str);
		delete p;
		m_msgNumber++;
	}
	//ѡ����Ƿ�����
	else if (((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck() == 1)
	{
		*p = '0';
		*(p + 1) = '|';
		for (i = 0; i < m_id_n; i++)
		{
			*(p + 2 + i) = str_m_id.GetAt(i);
		}
		//*(p + 2) = '1';
		*(p + 2 + m_id_n) = '|';
		*(p + 3 + m_id_n) = '0';
		*(p + 4 + m_id_n) = '|';
		for (i = 0; i < str_n; i++)
		{
			*(p + 5 + m_id_n + i) = str.GetAt(i);
		}
		*(p + 5 + m_id_n + str_n) = '|';
		*(p + 6 + m_id_n + str_n) = '\0';
		//���͵�������
		send(info.sClient, p, strlen(p), 0);
		//�������
		CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST1);
		list->InsertItem(m_msgNumber, 0);
		//���뷢�Ͷ�
		list->SetItemText(m_msgNumber, 0, str_m_id);
		//������ն�
		list->SetItemText(m_msgNumber, 1, _T("0"));
		//������Ϣ
		list->SetItemText(m_msgNumber, 2, str);
		delete p;
		m_msgNumber++;
	}
}

//�����û��б���
void CQQ_ClientDlg::updatelist()
{
    CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2);
	list->DeleteAllItems(); // ȫ�����  
	for (int i = 0; i < m_ids.size(); i++)
	{
		CString str_id;
		str_id = m_ids[i].c_str();
		list->InsertItem(i, 0);
		//������
		list->SetItemText(i, 0, str_id);
		//��������
		list->SetItemText(i, 1, _T("����"));
	}
}