#pragma comment(lib,"ws2_32.lib")//���ÿ��ļ�
using std::vector;
using std::ostream;
using std::endl;

typedef struct MYDATA
{
	int m_State;//���ݰ�����
	//����Ϊ����Ļ�����Ϣ
	int m_X;
	int m_Y;
	int m_Dir;
	int m_Type;
	int m_Id;
	int m_Color;
	int m_Hp;
	int m_Mp;
	int m_Rank;
	int m_MaxHp;

	friend ostream& operator<<(ostream&os, MYDATA &obj)
	{
		os << "m_State:" << obj.m_State << endl;
		os << "m_X:" << obj.m_X << endl;
		os << "m_Y:" << obj.m_Y << endl;
		os << "m_Dir:" << obj.m_Dir << endl;
		os << "m_Type:" << obj.m_Type << endl;
		os << "m_Id:" << obj.m_Id << endl;
		return os;
	}
}MYDATA, *PMYDATA;

class TCP_SERVER
{
private:
	SOCKET sockConn;
	SOCKET sockSrv;
	vector<MYDATA>TempData;

public:
	TCP_SERVER(){}
	~TCP_SERVER()
	{
		closesocket(sockSrv);
		WSACleanup();
		TempData.clear();
		vector<MYDATA>().swap(TempData);
	}

	bool IntiSever(int port)
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			MessageBoxA(NULL, "��ʼ��ʧ��", "", NULL);
			return false;
		}

		//�������ڼ������׽���,������˵��׽���
		sockSrv = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN addrSrv;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ�������ַ��ʵ�ϱ�ʾ��ȷ����ַ�������е�ַ�����������ַ���� һ����˵���ڸ���ϵͳ�о������Ϊ0ֵ��
		addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
		if (retVal == SOCKET_ERROR) {
			MessageBoxA(NULL, "�󶨶˿�ʧ��", "", NULL);
			return false;
		}

		if (listen(sockSrv, 10) == SOCKET_ERROR) {
			MessageBoxA(NULL, "����ʧ��", "", NULL);
			return false;
		}
		return true;
	}
	bool WaitClient()
	{
		SOCKADDR_IN addrClient;
		int len = sizeof(SOCKADDR);

		if (MySelect(sockSrv))
		{
			//�ȴ��ͻ�������
			sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
			if (sockConn == SOCKET_ERROR) {
				MessageBoxA(NULL, "�ȴ�����ʧ��", "", NULL);
				return false;
			}
			return true;
		}
		return false;
	}
	int MySelect(SOCKET &socket)
	{
		fd_set fdread;
		timeval tv;
		FD_ZERO(&fdread);//��ʼ��fd_set
		FD_SET(socket, &fdread);//�����׽��־������Ӧ��fd_set
		tv.tv_sec = 0;//�������Ǵ�����select�ȴ�����󷵻أ����ⱻ����
		tv.tv_usec = 0;
		select(0, &fdread, NULL, NULL, &tv);
		return FD_ISSET(socket, &fdread);
	}

	int RecvData(PMYDATA &data)
	{
		if (MySelect(sockConn))
		{
			char *recvBuf = new char[65535]{};
			recv(sockConn, recvBuf, 65535, 0);

			int size = 0;
			memcpy(&size, recvBuf, sizeof(int));

			if (size)
			{
				data = new MYDATA[size];

				memcpy(data, recvBuf + sizeof(int), sizeof(MYDATA)*size);

				delete[]recvBuf;

				return size;
			}
		}

		return 0;
	}
	void AddData(MYDATA &data)
	{
		TempData.push_back(data);
	}
	void ClearData()
	{
		TempData.clear();
		vector<MYDATA>().swap(TempData);
	}
	bool SendData()
	{
		if (!TempData.size())return true;

		char *sendbuf = new char[sizeof(MYDATA)*TempData.size() + sizeof(int)]{};

		int size = TempData.size();
		memcpy(sendbuf, &size, sizeof(int));
		memcpy(sendbuf + sizeof(int), &TempData[0], sizeof(MYDATA)*TempData.size());

		int iSend = send(sockConn, sendbuf, sizeof(MYDATA)*TempData.size() + sizeof(int), 0);
		if (iSend == SOCKET_ERROR)
		{
			MessageBoxA(NULL, "����ʧ��", "", NULL);
			return false;
		}

		delete[]sendbuf;
		TempData.clear();
		vector<MYDATA>().swap(TempData);
		return true;
	}
};
class TCP_CLIENT
{
private:
	SOCKET sockClient;
	WSADATA wsaData;
	vector<MYDATA>TempData;

public:
	TCP_CLIENT(){}
	~TCP_CLIENT()
	{
		closesocket(sockClient);
		WSACleanup();//�ͷų�ʼ��Ws2_32.dll���������Դ��
		TempData.clear();
		vector<MYDATA>().swap(TempData);
	}

	bool IntiClient()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			MessageBoxA(NULL, "��ʼ��Winsockʧ��", "", NULL);
			return false;
		}
		return true;
	}
	bool LineServer(char *ip, int port)
	{
		SOCKADDR_IN addrSrv;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);//�˿ں�
		//addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);//IP��ַ ʹ���Ϻ�������
		inet_pton(AF_INET, ip, &addrSrv.sin_addr.S_un.S_addr);//IP��ַ
		//inet_ntop(AF_INET,&addrSrv.sin_addr.S_un.S_addr,ip,sizeof(ip));//��ת����

		//�����׽���
		sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (SOCKET_ERROR == sockClient)
		{
			MessageBoxA(NULL, "�����׽���ʧ��", "", NULL);
			return false;
		}

		if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET)
		{
			MessageBoxA(NULL, "����ʧ��", "", NULL);
			return false;
		}

		return true;

	}
	int MySelect(SOCKET &socket)
	{
		fd_set fdread;
		timeval tv;
		FD_ZERO(&fdread);//��ʼ��fd_set
		FD_SET(socket, &fdread);//�����׽��־������Ӧ��fd_set
		tv.tv_sec = 0;//�������Ǵ�����select�ȴ�����󷵻أ����ⱻ����
		tv.tv_usec = 0;
		select(0, &fdread, NULL, NULL, &tv);
		return FD_ISSET(socket, &fdread);
	}

	int RecvData(PMYDATA &data)
	{
		if (MySelect(sockClient))//�Ƿ������ݰ��������ֱ������
		{
			int size = 0; //���ݰ��ڰ����Ľṹ����������
			char *recvBuf = new char[65535]{};//����ѿռ��������
			recv(sockClient, recvBuf, 65535, 0);//�����յ����ݿ������ѿռ�
			memcpy(&size, recvBuf, sizeof(int));//����ȡ��ǰ4�ֽڣ����ݰ�������

			if (size)//���������Ϊ0
			{
				data = new MYDATA[size];//����ָ�������Ľṹ��ռ�
				//�ӵ�4���ֽں�ʼ����
				memcpy(data, recvBuf + sizeof(int), sizeof(MYDATA)*size);
				delete[]recvBuf;//�ͷŶѿռ�
				return size;//�������ݰ�������
			}
		}
		return 0;
	}
	void AddData(MYDATA &data)
	{
		TempData.push_back(data);
	}
	void ClearData()
	{
		TempData.clear();
		vector<MYDATA>().swap(TempData);
	}
	bool SendData()
	{
		if (!TempData.size())return true;//�������������Ϊ�վͷ���

		//����ָ����С�ĶԿռ��Ŵ���������
		char *sendbuf = new char[sizeof(MYDATA)*TempData.size() + sizeof(int)]{};

		//ȡ�����������ݰ�������
		int size = TempData.size();
		//������д��ѿռ���4�ֽ�
		memcpy(sendbuf, &size, sizeof(int));
		//������4�ֽڿ�ʼ�������д����͵Ľṹ����������
		memcpy(sendbuf + sizeof(int), &TempData[0], sizeof(MYDATA)*TempData.size());
		//��ʼ�������ݰ�
		int iSend = send(sockClient, sendbuf, sizeof(MYDATA)*TempData.size() + sizeof(int), 0);
		if (iSend == SOCKET_ERROR)
		{
			MessageBoxA(NULL, "����ʧ��", "", NULL);
			return false;
		}
		delete[]sendbuf;//��������ͷŶԿռ�
		TempData.clear();//��մ���������
		vector<MYDATA>().swap(TempData);
		return true;

	}
};