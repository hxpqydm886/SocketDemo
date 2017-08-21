#include "stdafx.h"
#include "EventSelect.h"
#include "QQ_ServerDlg.h"
#include <vector>
#include<sstream>
#include<iostream>
using namespace std;


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
EventSelect::EventSelect()
{	
}


EventSelect::~EventSelect()
{
}

//�������
void EventSelect::define()
{
	m_nEventTotal = 0;	//ע����¼�����
	m_nPort = 8888; //�˷����������Ķ˿ں�
	msgNumber = 0; //��Ϣ����
	m_id = 1; //socket���
}


//�����Ͱ�Socket��������Ϊ����״̬
void EventSelect::initSocket()
{
	//��ʼ��windows Sockets ����
	WSAStartup(MAKEWORD(2, 2), &m_ws);
	//��������socket
	sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//���õ�ַ����socket
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_nPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	::bind(sListen, (struct sockaddr *)&sin, sizeof(struct sockaddr));
	::listen(sListen, 10);
}

//�����¼����󣬲�ע�������¼�
void EventSelect::initEvent()
{
	//�����¼����󣬲�����������Socket��ע��FD_ACCEPT��FD_CLOSE�����¼�
	WSAEVENT myevent = ::WSACreateEvent();
	::WSAEventSelect(sListen, myevent, FD_ACCEPT | FD_CLOSE);
	//���½���ʱ��myevent���浽eventArray������
	eventArray[m_nEventTotal] = myevent;
	//������socket slisten ���浽 sockArray������
	sockArray[m_nEventTotal] = sListen;
	m_nEventTotal++;
}


//���������¼�
void EventSelect::dealEvent()
{
	while (true)
	{
		//������ʱ������ϵȴ���ֻҪ��һ��ʱ������Ϊ������״̬���������أ������ڵȴ�
		int nIndex = ::WSAWaitForMultipleEvents(m_nEventTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		//������ʱ������������һ���Ǿ����������ǰ�����һ����Ȼ������ѭ�����δ��������¼�����
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		//��ÿ���¼�����WSAWaitForMultipleEvents�������Ա�ȷ������״̬
		for (int i = nIndex; i < m_nEventTotal; i++)
		{
			int ret;
			ret = :: WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 1000, FALSE);
			if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				//��ȡ������֪ͨ��Ϣ��WSAWaitForMultipleEvents�������Զ����������¼�
				::WSAEnumNetworkEvents(sockArray[i], eventArray[i], &event1);//event1���淢���������¼���¼��������Ĵ������
				if (event1.lNetworkEvents & FD_ACCEPT)  //����FD_ACCEPT�¼�
				{
					accept(i);
				}
				else if (event1.lNetworkEvents & FD_READ) //����FD_READ�¼�
				{
					read(i);
				}
				else if (event1.lNetworkEvents & FD_CLOSE) //����FD_CLOSE�¼�
				{
					close(i);
				}
				else if (event1.lNetworkEvents & FD_WRITE) //����FD_WRITE�¼�
				{
					write(i);
				}
			}
		}
	}
}


void EventSelect::accept(int i)
{
	struct sockaddr_in from; //���ܽ����ַ

	//�������FD_ACCEPT��Ϣʱû�д���
	if (event1.iErrorCode[FD_ACCEPT_BIT] == 0)
	{
		//����̫�࣬��ʱ������
		if (m_nEventTotal > WSA_MAXIMUM_WAIT_EVENTS)
		{
			cout << "�������������" << endl;
			return;
		}
		//�����������󣬵õ���ͻ��˽���ͨ�ŵ�socket snew
		int len = sizeof(from);
		SOCKET sNew = ::accept(sockArray[i], (struct sockaddr*)&from, &len);
		WSAEVENT newEvent = ::WSACreateEvent(); //Ϊ���׽��ִ����¼�����
		//���½����¼�����newEvent������socket snew��
		//ע��FD_READ|FD_CLOSE|FD_WRITE�����¼�
		::WSAEventSelect(sNew, newEvent, FD_READ | FD_CLOSE | FD_WRITE);
		//���½����¼�newEvent���浽eventArray������ 
		eventArray[m_nEventTotal] = newEvent;
		//���½���socketsnew���浽sockarray������
		sockArray[m_nEventTotal] = sNew;

		client c;
		string ip = inet_ntoa(from.sin_addr);
		int port = from.sin_port;
		ostringstream os;
		string str_port;
		os << port;
		str_port = os.str();
		ostringstream oss;
		string str_id;
		oss << m_id;
		str_id = oss.str();
		c.id = str_id;
		c.ip = ip;
		c.port = str_port;
		clients.push_back(c);
		
		//��ʾ��ǰ�ͻ����ӳɹ�
		CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2);
		list->InsertItem(msgNumber, 0);
		//���뷢�Ͷ�
		list->SetItemText(msgNumber, 0, _T("0"));
		//������ն�
		list->SetItemText(msgNumber, 1, _T("0"));
		CString msg;
		msg += "�ͻ���";
		CString id;
		id.Format(_T("%d"), m_id);
		msg += id;
		msg += "���ӵ�������";
		list->SetItemText(msgNumber, 2, msg);
		msgNumber++;
		//���Կͻ��б�
		sendlisttoclient();
		//�������
		TRACE("This is a TRACE statement\n");
		TRACE("The value of ip is %d\n", from.sin_addr);
		TRACE("The value of port is %d\n", from.sin_port);

		m_nEventTotal++;
		m_id++;
		//���µ�ǰ���Ӵ���
		updateList();
	}
}


void EventSelect::read(int i)
{
	//�������FD_READ��Ϣʱû�д���
	if (event1.iErrorCode[FD_READ_BIT] == 0)
	{
		//������Ϣ  �û��б�1|id|id|id|.....
		//����  0|1|2|msg
		vector<string> lines;
		char recvBuf[1024];
		int nrev = ::recv(sockArray[i], recvBuf, sizeof(recvBuf), 0);
		if (nrev > 0)
		{
			string s;
			s = recvBuf;
			lines = split(s, "|");
			//ȷ�����ն�socketarray
			char *p = new char[1024];
			int n1, n2, n3, n;
			CString str1, str2, str3, str;
			str1 = lines[0].c_str();
			str2 = lines[1].c_str();
			str3 = lines[2].c_str();
			str = lines[3].c_str();
			n1 = str1.GetLength();
			n2 = str2.GetLength();
			n3 = str3.GetLength();
			n = str.GetLength();
			int i;
			/*for (i = 0; i < n1; i++)
			{
				*(p + i) = str1.GetAt(i);
			}*/
			*p = str1.GetAt(0);
			*(p + 1) = '|';
			for (i = 0; i < n2; i++)
			{
				*(p + 2 + i) = str2.GetAt(i);
			}
			*(p + 2 + n2) = '|';
			for (i = 0; i < n3; i++)
			{
				*(p + 3 + n2 + i) = str3.GetAt(i);
			}
			*(p + 3 + n2 + n3) = '|';
			for (i = 0; i < n; i++)
			{
				*(p + n2 + n3 + 4 + i) = str.GetAt(i);
			}
			*(p + n2 + n3 + 4 + n) = '|';
			*(p + n2 + n3 + 5 + n) = '\0';
			if (lines[2] == "0")//Ⱥ��
			{
				for (int j = 1; j < clients.size() + 1; j++)
				{
					send(sockArray[j], p, strlen(p), 0);
				}
			}
			else//ת��
			{
				int n;
				for (int j = 0; j < clients.size(); j++)
				{
					if (lines[2] == clients[j].id)
					{
						n = j+1;
						break;
					}
				}
				send(sockArray[n], p, strlen(p), 0);
			}
		}
		CString str_send, str_recv, str;
		str_send = lines[1].c_str();
		str_recv = lines[2].c_str();
		str = lines[3].c_str();
		//��ʾ���˳�
		CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2);
		list->InsertItem(msgNumber, 0);
		//���뷢�Ͷ�
		list->SetItemText(msgNumber, 0, str_send);
		//������ն�
		list->SetItemText(msgNumber, 1, str_recv);
		list->SetItemText(msgNumber, 2, str);
		msgNumber++;
		lines.clear();
	}
	
}

void EventSelect::write(int i)
{
	//�������FD_WRITE��Ϣʱû�д���
	if (event1.iErrorCode[FD_WRITE_BIT] == 0)
	{
		//������Ϣ
		char sendBuf[256] = "�ɹ����ӷ�����";
	}
}

void EventSelect::close(int i)
{
	//�ر�socket��ɾ�������ж�Ӧ�ļ�¼
	
	if (event1.iErrorCode[FD_CLOSE_BIT] != 0)
	{
		//��ʾ���˳�
		CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2);
		list->InsertItem(msgNumber, 0);
		//���뷢�Ͷ�
		list->SetItemText(msgNumber, 0, _T("0"));
		//������ն�
		list->SetItemText(msgNumber, 1, _T("0"));
		CString msg;
		msg += "�ͻ���";
		CString id;
		//id.Format(_T("%d"), clients[i-1].id);
		id = clients[i - 1].id.c_str();
		msg += id;
		msg += "�˳�������";
		list->SetItemText(msgNumber, 2, msg);
		msgNumber++;

		//ɾ���ͻ�����Ϣ
		vector<client>::iterator it = clients.begin() + i - 1;
		clients.erase(clients.begin() + i - 1);
		//���Կͻ��б�
		//sendlisttoclient();
		int k, j;
		int sum = 0;
		int str_id_n;
		CString str_id;
		char p[1024];
		memset(p, sizeof(p), '\0');
		p[0] = '1';
		p[1] = '|';
		for (k = 0; k < clients.size(); k++)
		{
			str_id = clients[k].id.c_str();
			str_id_n = str_id.GetLength();

			for (j = 0; j < str_id_n; j++)
			{
				p[2 + sum + j] = str_id.GetAt(j);
			}
			p[2 + sum + str_id_n] = '|';
			sum += str_id_n + 1;
		}
		p[2 + sum] = '\0';
		//����
		//list->InsertItem(msgNumber, 0);
		////���뷢�Ͷ�
		//list->SetItemText(msgNumber, 0, _T("0"));
		////������ն�
		//list->SetItemText(msgNumber, 1, _T("0"));
		//string s = p;
		//msg= s.c_str();
		//list->SetItemText(msgNumber, 2, msg);
		//msgNumber++;

		int len = clients.size();
		for (k = 1; k < len + 2; k++)
		{
			send(sockArray[k], p, strlen(p), 0);
		}
		//delete p;
		//�����б�
		updateList();
		::closesocket(sockArray[i]);
		for (int j = i; j < m_nEventTotal -1; j++)
		{
			sockArray[j] = sockArray[j + 1];
			eventArray[j] = eventArray[j + 1];
		}
		m_nEventTotal--;
	}
}

void EventSelect::updateList()
{
	CListCtrl* list = (CListCtrl*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST1);
	list->DeleteAllItems(); // ȫ�����  
	for (int i = 0; i < clients.size(); i++)
	{
		CString str_id,str_ip,str_port;
		str_id = clients[i].id.c_str();
		str_ip = clients[i].ip.c_str();
		str_port = clients[i].port.c_str();
		list->InsertItem(i, 0);
		//������
		list->SetItemText(i, 0, str_id);
		//����ip
		list->SetItemText(i, 1, str_ip);
		//����port
		list->SetItemText(i, 2, str_port);
	}
}

//�ѵ�ǰ�û���Ϣ���͸��ͻ���1|���|���|��������|
void EventSelect::sendlisttoclient()
{
	int k, j;
	int sum = 0;
	int str_id_n;
	CString str_id;
	char *p = new char[1024];
	*p = '1';
	*(p + 1) = '|';
	for (k = 0; k < clients.size(); k++)
	{
		str_id = clients[k].id.c_str();
		str_id_n = str_id.GetLength();

		for (j = 0; j < str_id_n; j++)
		{
			*(p + 2 + sum + j) = str_id.GetAt(j);
		}
		*(p + 2 + sum + str_id_n) = '|';
		sum += str_id_n + 1;
	}
	*(p + 2 + sum) = '\0';
	int len = clients.size();
	for (k = 1; k < len + 1; k++)
	{
		send(sockArray[k], p, strlen(p), 0);
	}
}