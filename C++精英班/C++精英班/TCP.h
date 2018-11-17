#pragma comment(lib,"ws2_32.lib")//引用库文件
using std::vector;
using std::ostream;
using std::endl;

typedef struct MYDATA
{
	int m_State;//数据包类型
	//以下为传输的基本信息
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
			MessageBoxA(NULL, "初始化失败", "", NULL);
			return false;
		}

		//创建用于监听的套接字,即服务端的套接字
		sockSrv = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN addrSrv;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		//INADDR_ANY就是指定地址为0.0.0.0的地址，这个地址事实上表示不确定地址，或“所有地址”、“任意地址”。 一般来说，在各个系统中均定义成为0值。
		addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
		if (retVal == SOCKET_ERROR) {
			MessageBoxA(NULL, "绑定端口失败", "", NULL);
			return false;
		}

		if (listen(sockSrv, 10) == SOCKET_ERROR) {
			MessageBoxA(NULL, "监听失败", "", NULL);
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
			//等待客户请求到来
			sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
			if (sockConn == SOCKET_ERROR) {
				MessageBoxA(NULL, "等待请求失败", "", NULL);
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
		FD_ZERO(&fdread);//初始化fd_set
		FD_SET(socket, &fdread);//分配套接字句柄到相应的fd_set
		tv.tv_sec = 0;//这里我们打算让select等待两秒后返回，避免被锁死
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
			MessageBoxA(NULL, "发送失败", "", NULL);
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
		WSACleanup();//释放初始化Ws2_32.dll所分配的资源。
		TempData.clear();
		vector<MYDATA>().swap(TempData);
	}

	bool IntiClient()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			MessageBoxA(NULL, "初始化Winsock失败", "", NULL);
			return false;
		}
		return true;
	}
	bool LineServer(char *ip, int port)
	{
		SOCKADDR_IN addrSrv;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);//端口号
		//addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);//IP地址 使用老函数警告
		inet_pton(AF_INET, ip, &addrSrv.sin_addr.S_un.S_addr);//IP地址
		//inet_ntop(AF_INET,&addrSrv.sin_addr.S_un.S_addr,ip,sizeof(ip));//反转方法

		//创建套接字
		sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (SOCKET_ERROR == sockClient)
		{
			MessageBoxA(NULL, "创建套接字失败", "", NULL);
			return false;
		}

		if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET)
		{
			MessageBoxA(NULL, "连接失败", "", NULL);
			return false;
		}

		return true;

	}
	int MySelect(SOCKET &socket)
	{
		fd_set fdread;
		timeval tv;
		FD_ZERO(&fdread);//初始化fd_set
		FD_SET(socket, &fdread);//分配套接字句柄到相应的fd_set
		tv.tv_sec = 0;//这里我们打算让select等待两秒后返回，避免被锁死
		tv.tv_usec = 0;
		select(0, &fdread, NULL, NULL, &tv);
		return FD_ISSET(socket, &fdread);
	}

	int RecvData(PMYDATA &data)
	{
		if (MySelect(sockClient))//是否有数据包到达，否则直接跳过
		{
			int size = 0; //数据包内包含的结构体数据数量
			char *recvBuf = new char[65535]{};//申请堆空间接受数据
			recv(sockClient, recvBuf, 65535, 0);//将接收到数据拷贝到堆空间
			memcpy(&size, recvBuf, sizeof(int));//首先取出前4字节，数据包的数量

			if (size)//如果数量不为0
			{
				data = new MYDATA[size];//申请指定数量的结构体空间
				//从第4个字节后开始拷贝
				memcpy(data, recvBuf + sizeof(int), sizeof(MYDATA)*size);
				delete[]recvBuf;//释放堆空间
				return size;//返回数据包的数量
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
		if (!TempData.size())return true;//如果待发送数据为空就返回

		//申请指定大小的对空间存放待发送数据
		char *sendbuf = new char[sizeof(MYDATA)*TempData.size() + sizeof(int)]{};

		//取出待发送数据包的数量
		int size = TempData.size();
		//把数量写入堆空间首4字节
		memcpy(sendbuf, &size, sizeof(int));
		//再往后4字节开始拷贝所有待发送的结构体数组数据
		memcpy(sendbuf + sizeof(int), &TempData[0], sizeof(MYDATA)*TempData.size());
		//开始发送数据包
		int iSend = send(sockClient, sendbuf, sizeof(MYDATA)*TempData.size() + sizeof(int), 0);
		if (iSend == SOCKET_ERROR)
		{
			MessageBoxA(NULL, "发送失败", "", NULL);
			return false;
		}
		delete[]sendbuf;//发送完毕释放对空间
		TempData.clear();//清空待发送数据
		vector<MYDATA>().swap(TempData);
		return true;

	}
};