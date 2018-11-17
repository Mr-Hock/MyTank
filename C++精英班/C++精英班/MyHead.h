#pragma comment(lib, "winmm.lib")
//界面操作宏
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define WALL_WIDE 50
#define WALL_HIGH 60

//获取路径宏
#define ROUTE_SAVEMAP 0
#define ROUTE_OPENMAP 1
#define ROUTE_SAVEGAME 2
#define ROUTE_LOADGAME 3

//坦克属性物品ID宏
#define BULLET_MY 0
#define BULLET_MY2 3
#define BULLET_PC 1
#define BULLET_CLEAN 88

#define OBSTACLE_NULL -1
#define OBSTACLE_WALL 0
#define OBSTACLE_EARTHWALL 1
#define OBSTACLE_GRASS 2
#define OBSTACLE_WATER 3
#define OBSTACLE_CAMP_A 4
#define OBSTACLE_CAMP_B 5
#define OBSTACLE_CAMP_PC 6

#define TANK_PC1 100
#define TANK_PC1_HP 2
#define TANK_PC1_SPEED 8
#define TANK_PC1_BULLETCD 1000

#define TANK_PC2 101
#define TANK_PC2_HP 4
#define TANK_PC2_SPEED 6
#define TANK_PC2_BULLETCD 500

#define TANK_PC3 102
#define TANK_PC3_HP 10
#define TANK_PC3_SPEED 4
#define TANK_PC3_BULLETCD 500

#define TANK_PC4 103
#define TANK_PC4_HP 20
#define TANK_PC4_SPEED 2
#define TANK_PC4_BULLETCD 300

#define TANK_PC5 104
#define TANK_PC5_HP 50
#define TANK_PC5_SPEED 3
#define TANK_PC5_BULLETCD 500

#define TANK_PA 110
#define TANK_PA_HP 10
#define TANK_PA_MP 100
#define TANK_PA_MAXHP 10
#define TANK_PA_MAXMP 100
#define TANK_PA_SPEED 2
#define TANK_PA_BULLETCD 200

#define TANK_PB 111
#define TANK_PB_HP 10
#define TANK_PB_MP 100
#define TANK_PB_MAXHP 10
#define TANK_PB_MAXMP 100
#define TANK_PB_SPEED 2
#define TANK_PB_BULLETCD 200

#define BUFFER_NULL -1
#define BUFFER_RANDBUFFER 10
#define BUFFER_ATTACK 1
#define BUFFER_BULLECTCD 2
#define BUFFER_NOHURT 3
#define BUFFER_STATICALL 4
#define BUFFER_KILLALL 5

//PC坦克行为宏
#define ACTION_DIR 1
#define ACTION_ATTACK 2
#define ACTION_MOV 3

//Ai移动状态宏
#define MOVE_CD 0
#define MOVE_OK 1
#define MOVE_NO 2

//框架区域宏
#define FRAME_DRAW 0
#define FRAME_TOOLS 1
#define FRAME_SAVEINFO 2

//鼠标按键宏
#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define KEY_SPACE 32
#define KEY_ESC 27
#define KEY_PA_UP 'W'
#define KEY_PA_DOWN 'S'
#define KEY_PA_LEFT 'A'
#define KEY_PA_RIGHT 'D'
#define KEY_PA_BULLET 'J'
#define KEY_PA_BULLETGROUP 'K'
#define KEY_PA_BULLETSPUER 'L'
#define KEY_PB_UP 38
#define KEY_PB_DOWN 40
#define KEY_PB_LEFT 37
#define KEY_PB_RIGHT 39
#define KEY_PB_BULLET 100
#define KEY_PB_BULLETGROUP 101
#define KEY_PB_BULLETSPUER 102

//音效播放宏
#define SOUND_OPEN 0
#define SOUND_ATTACK 1
#define SOUND_BUFFER 2
#define SOUND_WIN 3

//菜单按钮ID宏
#define MENU_HOMEPAGE 0
#define MENU_STARTGAMEPAGE 1
#define MENU_CUSTOMPAGE 2
#define MENU_DRAWMAPPAGE 3
#define MENU_GAMEPAGE 4
#define MENU_GAMEOVERPAGE 5
#define MENU_EDITMAPPAGE 6
#define MENU_LINKGAMEPAGE 7
#define MENU_LINKGAMEOVERPAGE 8

#define BUTTON_BACKHOMEPAGE 600

#define BUTTON_STARTGAME 100
#define BUTTON_SINGLE 101
#define BUTTON_PARTNER 102
#define BUTTON_CUSTOMGAME 103
#define BUTTON_LINKGAME 104

#define BUTTON_CREATROOM 105
#define BUTTON_ADDROOM 106

#define BUTTON_LOADGAME 200

#define BUTTON_CUSTOMMAP 300
#define BUTTON_NEWMAP 301
#define BUTTON_OPENMAP 302

#define BUTTON_ENDGAME 400

#define BUTTON_SAVEMAP 500
#define BUTTON_ENDEDITMAP 501

#define BUTTON_CONTIUNEGAME 700
#define BUTTON_SAVEGAME 701
#define BUTTON_RESTGAME 702
#define BUTTON_NEXTGAME 703

//通信与数据包标识宏
#define TCP_LINK_NULL 0
#define TCP_LINK_SERVER 1
#define TCP_LINK_CLIENT 2

#define DATA_MOVE 1//移动行为
#define DATA_MOVE_REFRESH 11//刷新移动
#define DATA_DIR 2//改变方向
#define DATA_BULLET 3//发炮1行为
#define DATA_BULLET_REFRESH 33//刷新炮弹
#define DATA_BULLETGROUP 4//发炮2行为
#define DATA_BULLETSUPER 5//发泡3行为
#define DATA_BUFFER 6//生成食物行为
#define DATA_BUFFER_REFRESH 66//刷新食物
#define DATA_BUFFER_REFRESH_CLEAR 666//清除食物
#define DATA_CREATTANK 7//创造坦克
#define DATA_CREATTANK_REFRESH 77//刷新坦克
#define DATA_CREATTANK_REFRESH_CLEAR 777//清除坦克
#define DATA_OBSTACLE_GRASS_REFRESH 8//刷新草丛
#define DATA_INTIDATA 100//初始化开始游戏
#define DATA_GAMEOVER 200//游戏结束


int GetRandNum(int Min, int Max);

class POINTDATA
{
public:
	int m_x;//X坐标
	int m_y;//Y坐标
	int m_color;//该点颜色
	char* m_data;//该点显示的图案
	int m_type;//扩展：类型
	int m_dir;//方向
	int m_time;//自动生成时间

	POINTDATA() {};//带参构造函数初始化各成员值
	POINTDATA(int x, int y) :m_x(x), m_y(y) {};
	POINTDATA(int x, int y, int color, char* data, int dir, int type) :
		m_x(x), m_y(y), m_color(color), m_data(data), m_dir(dir), m_type(type), m_time((DWORD)time(NULL)+ GetRandNum(1, (DWORD)time(NULL))) {}
};
struct POINTSS
{
	int thisX;
	int thisY;
	int lastX;
	int lastY;
};
struct POINTSSS
{
	int nMinF;
	int nG;
	int nH;
	int thisX;
	int thisY;
	int lastX;
	int lastY;
};
struct POINTXY
{
	int m_x;
	int m_y;
};
struct TankInfo
{
	int x;
	int y ;
	int color ;
	int dir ;
	int speed ;
	int bulletcd ;
	int hp ;
	int mp ;
	int rank ;
	int maxhp;
	int maxmp;
};

//其他操作函数
int GetRandNum(int Min, int Max)
{
	//srand(time(NULL));
	return rand() % Max + Min;
}
int GetRandTankId()
{
	return clock() + GetRandNum(1, clock());
}
char* GetRoute(int type)
{
	static char szFileName[MAX_PATH] = { 0 };
	OPENFILENAMEA openFileName = { 0 };
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.nMaxFile = MAX_PATH;  //这个必须设置，不设置的话不会出现打开文件对话框
	openFileName.lpstrFile = (LPSTR)szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST |OFN_NOCHANGEDIR;


	char *nEndName=nullptr;
	strcpy_s(szFileName,2, " ");//将文件名置空，防止出现上一次名字

	if (type == ROUTE_SAVEMAP || type == ROUTE_OPENMAP)
	{
		openFileName.lpstrInitialDir = "map";
		openFileName.lpstrFilter = "地图文件(*.map)\0*.map\0";
		nEndName = ".map";
	}
	else if (type == ROUTE_SAVEGAME || type == ROUTE_LOADGAME)
	{
		openFileName.lpstrInitialDir = "sav";
		openFileName.lpstrFilter = "存档文件(*.sav)\0*.sav\0";
		nEndName = ".sav";
	}

	switch (type)
	{
	case ROUTE_SAVEMAP:case ROUTE_SAVEGAME:
		if (GetSaveFileNameA(&openFileName))
		{
			if (strstr(openFileName.lpstrFile, nEndName) == NULL)
				strcat_s(openFileName.lpstrFile,strlen(openFileName.lpstrFile)+strlen(nEndName)+1, nEndName);
			return openFileName.lpstrFile;
		}
		break;

	case ROUTE_OPENMAP:case ROUTE_LOADGAME:
		if (GetOpenFileNameA(&openFileName))
		{
			return openFileName.lpstrFile;
		}
		break;
	}
	return NULL;
}
char* GetProgramDir()
{
	static char exeFullPath[MAX_PATH]; // Full path
	char *nWeak;

	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	nWeak =strrchr(exeFullPath,'\\');

	nWeak[1] = 0;
	return exeFullPath;
	system("pause");

}
bool GetKey(int vk_code)
{
	return (GetAsyncKeyState(vk_code) & 0x8000) ? true : false;
}
int IsNextStage(int nStage,int nRank)
{
	switch (nStage)
	{
	case 1:
		if (nRank >= 5)return 0;
		return 5- nRank;
	case 2:
		if (nRank >= 10)return 0;
		return 10 - nRank;
	case 3:
		if (nRank >= 15)return 0;
		return 15 - nRank;
	case 4:
		if (nRank >= 20)return 0;
		return 20 - nRank;
	case 5:
		if (nRank >= 8888888)return 0;
		return 8888 - nRank;
	}
	return 8888;
}
void PlayMusic(int type)
{
	char *nSoundRoute = GetProgramDir();

	switch (type)
	{
	case SOUND_OPEN:
		strcat_s(nSoundRoute,strlen(nSoundRoute)+strlen("sound/sound.wav")+1, "sound/sound.wav");;
		PlaySoundA(nSoundRoute, NULL, SND_ASYNC | SND_NODEFAULT);
		break;
	case SOUND_ATTACK:
		strcat_s(nSoundRoute, strlen(nSoundRoute) + strlen("sound/attack.wav") + 1, "sound/attack.wav");;
		PlaySoundA(nSoundRoute, NULL, SND_ASYNC | SND_NODEFAULT);
		break;
	case SOUND_WIN:
		strcat_s(nSoundRoute, strlen(nSoundRoute) + strlen("sound/win.wav") + 1, "sound/win.wav");;
		PlaySoundA(nSoundRoute, NULL, SND_ASYNC | SND_NODEFAULT);
		break;
	case SOUND_BUFFER:
		strcat_s(nSoundRoute, strlen(nSoundRoute) + strlen("sound/buffer.wav") + 1, "sound/buffer.wav");;
		PlaySoundA(nSoundRoute, NULL, SND_ASYNC | SND_NODEFAULT);
		break;
	case 88:
		break;
	default:
		break;
	}

}
void ReadServerIni(int &port)
{
	char nTemp[256]{};//储存配置文件全路径
	char nRoute[256]{};//保存当前运行目录
	strcpy_s(nRoute, GetProgramDir());//取出当前运行目录
	strcat_s(nRoute, "TankLink.ini");//组成配置文件全路径
	//开始读取TankLink.ini配置文件中的信息
	GetPrivateProfileStringA("Server", "Port", "6666", nTemp, _countof(nTemp), nRoute);
	sscanf_s(nTemp, "%d", &port);//将文本转换为整数型
}
void ReadClientIni(char *ip,int &port)
{
	char nTemp[256]{};
	char nRoute[256]{};
	strcpy_s(nRoute, GetProgramDir());
	strcat_s(nRoute, "TankLink.ini");
	GetPrivateProfileStringA("Client", "Ip", "127.0.0.1", nTemp, _countof(nTemp), nRoute);
	strcpy_s(ip,strlen(nTemp)+1 ,nTemp);
	GetPrivateProfileStringA("Client", "Port", "6666", nTemp, _countof(nTemp), nRoute);
	sscanf_s(nTemp, "%d", &port);

}

//打印文字，图案，边框函数
void PrintPoint(SHORT x, SHORT y, char* data, int color)
{
	using std::cout;

	HANDLE HND;
	HND = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 1;
	SetConsoleCursorInfo(HND, &cci);

	COORD pos = { x * 2,y };
	SetConsoleCursorPosition(HND, pos);


	SetConsoleTextAttribute(HND, color);
	//SetConsoleTextAttribute(HND, rand()%16);

	cout << data;

}
void PrintPoint(SHORT x, SHORT y, char* data, int num, int color)
{
	using std::cout;

	HANDLE HND;
	HND = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 1;
	SetConsoleCursorInfo(HND, &cci);

	SetConsoleTextAttribute(HND, color);

	COORD pos = { x * 2 ,y };
	SetConsoleCursorPosition(HND, pos);
	cout << data << num<<"    ";
}
void PrintTank(SHORT x, SHORT y, char* data, int color,int dir)
{
	switch (dir)
	{
	case DIR_UP:
		PrintPoint(x - 1, y - 1, "  ", color);
		PrintPoint(x, y - 1, data, color);
		PrintPoint(x + 1, y - 1, "  ", color);

		PrintPoint(x - 1, y, data, color);
		PrintPoint(x, y, data, color);
		PrintPoint(x + 1, y, data, color);

		PrintPoint(x - 1, y + 1, data, color);
		PrintPoint(x, y + 1, "  ", color);
		PrintPoint(x + 1, y + 1, data, color);
		break;
	case DIR_DOWN:
		PrintPoint(x - 1, y - 1, data, color);
		PrintPoint(x, y - 1, "  ", color);
		PrintPoint(x + 1, y - 1, data, color);

		PrintPoint(x - 1, y, data, color);
		PrintPoint(x, y, data, color);
		PrintPoint(x + 1, y, data, color);

		PrintPoint(x - 1, y + 1, "  ", color);
		PrintPoint(x, y + 1, data, color);
		PrintPoint(x + 1, y + 1, "  ", color); 
		break;
	case DIR_LEFT:
		PrintPoint(x - 1, y - 1,"  " , color);
		PrintPoint(x, y - 1, data, color);
		PrintPoint(x + 1, y - 1, data, color);

		PrintPoint(x - 1, y, data, color);
		PrintPoint(x, y, data, color);
		PrintPoint(x + 1, y, "  ", color);

		PrintPoint(x - 1, y + 1, "  ", color);
		PrintPoint(x, y + 1, data, color);
		PrintPoint(x + 1, y + 1, data, color); 
		break;
	case DIR_RIGHT:
		PrintPoint(x - 1, y - 1, data, color);
		PrintPoint(x, y - 1, data, color);
		PrintPoint(x + 1, y - 1, "  ", color);

		PrintPoint(x - 1, y, "  ", color);
		PrintPoint(x, y, data, color);
		PrintPoint(x + 1, y, data, color);

		PrintPoint(x - 1, y + 1, data, color);
		PrintPoint(x, y + 1, data, color);
		PrintPoint(x + 1, y + 1, "  ", color); 
		break;
	default:
		PrintPoint(x - 1, y - 1, "  ", color);
		PrintPoint(x, y - 1, "  ", color);
		PrintPoint(x + 1, y - 1, "  ", color);

		PrintPoint(x - 1, y, "  ", color);
		PrintPoint(x, y, "  ", color);
		PrintPoint(x + 1, y, "  ", color);

		PrintPoint(x - 1, y + 1, "  ", color);
		PrintPoint(x, y + 1, "  ", color);
		PrintPoint(x + 1, y + 1, "  ", color);
		break;
	}


}
void PrintEmptyFrame(SHORT x, SHORT y, int wide, int higt, int color, char*data = "■")
{
	for (SHORT w = 0; w <= wide; w++)
	{
		for (SHORT h = 0; h <= higt; h++)
		{
			if (h == 0 || h == higt)
			{
				PrintPoint(w + x, h + y, data, color);
				Sleep(3);
			}
			else if (w == 0 || w == wide)
			{
				PrintPoint(w + x, h + y, data, color);
				Sleep(3);
			}
		}
	}

}
void PrintEmptyFrameEx(SHORT x, SHORT y, int wide, int higt, int color, char*data = "■")
{
	for (SHORT w = 0; w <= wide; w++)
	{
		for (SHORT h = 0; h <= higt; h++)
		{
			if (h == 0 || h == higt)
			{
				PrintPoint(w + x, h + y, data, color);
			}
			else if (w == 0 || w == wide)
			{
				PrintPoint(w + x, h + y, data, color);
			}
		}
	}

}
void PrintTilteText(SHORT x, SHORT y)
{
	using std::cout;
	using std::endl;

	PrintPoint(x, y,     "■■■■   ■■■■■      ■■■■■     ■■■■      ■     ■     ■   ■  ", 0x9);
	PrintPoint(x, y + 1, "■     ■      ■              ■        ■      ■     ■■   ■     ■  ■   ", 0x2);
	PrintPoint(x, y + 2, "■    ■       ■              ■        ■      ■     ■ ■  ■     ■ ■    ", 0x6);
	PrintPoint(x, y + 3, "■■■■       ■              ■        ■■■■■     ■  ■ ■     ■■     ", 0xc);
	PrintPoint(x, y + 4, "■     ■      ■              ■        ■      ■     ■   ■■     ■ ■    ", 0xa);
	PrintPoint(x, y + 5, "■     ■      ■              ■        ■      ■     ■     ■     ■  ■   ", 0x5);
	PrintPoint(x, y + 6, "■■■■       ■              ■        ■      ■     ■     ■     ■   ■  ", 0xe);
																		     
}
void PrintTilteTextRandColor(SHORT x, SHORT y)
{
	using std::cout;
	using std::endl;

	PrintPoint(x, y, "■■■■   ■■■■■      ■■■■■     ■■■■      ■     ■     ■   ■  ", GetRandNum(1,15));
	PrintPoint(x, y + 1, "■     ■      ■              ■        ■      ■     ■■   ■     ■  ■   ", GetRandNum(1, 15));
	PrintPoint(x, y + 2, "■    ■       ■              ■        ■      ■     ■ ■  ■     ■ ■    ", GetRandNum(1, 15));
	PrintPoint(x, y + 3, "■■■■       ■              ■        ■■■■■     ■  ■ ■     ■■     ", GetRandNum(1, 15));
	PrintPoint(x, y + 4, "■     ■      ■              ■        ■      ■     ■   ■■     ■ ■    ", GetRandNum(1, 15));
	PrintPoint(x, y + 5, "■     ■      ■              ■        ■      ■     ■     ■     ■  ■   ", GetRandNum(1, 15));
	PrintPoint(x, y + 6, "■■■■       ■              ■        ■      ■     ■     ■     ■   ■  ", GetRandNum(1, 15));

}
void ClearTilteTextRandColor(SHORT x, SHORT y)
{
	using std::cout;
	using std::endl;

	PrintPoint(x, y,     "                                                                               ", GetRandNum(1, 15));
	PrintPoint(x, y + 1, "                                                                               ", GetRandNum(1, 15));
	PrintPoint(x, y + 2, "                                                                               ", GetRandNum(1, 15));
	PrintPoint(x, y + 3, "                                                                               ", GetRandNum(1, 15));
	PrintPoint(x, y + 4, "                                                                               ", GetRandNum(1, 15));
	PrintPoint(x, y + 5, "                                                                               ", GetRandNum(1, 15));
	PrintPoint(x, y + 6, "                                                                               ", GetRandNum(1, 15));

}
void PrintGameTips(SHORT x, SHORT y)
{

		PrintPoint(x, y + 1, "道具说明", 0xb);
		PrintPoint(x, y + 3, "■ 钢墙", 0x8);
		PrintPoint(x, y + 4, "■ 土墙", 0x6);
		PrintPoint(x, y + 5, "○ 河流", 0x9);
		PrintPoint(x, y + 6, "△ 草丛", 0x2);

		PrintPoint(x, y + 7, "★ 攻击翻倍", 0xc);
		PrintPoint(x, y + 8, "★ 无限子弹", 0x9);
		PrintPoint(x, y + 9, "☆ 自身无敌", 0xc);
		PrintPoint(x, y + 10, "☆ 冰冻敌军", 0x9);
		PrintPoint(x, y + 11, "卍 全屏秒杀", 0xc);
}
void PrintStageText(SHORT x, SHORT y,int nStage)
{
	using std::cout;
	using std::endl;
	char nText[255] = {0};
	system("cls");
	if (nStage != NULL)sprintf_s(nText,_countof(nText),"                     第 %d 关！准备开始！                      ", nStage);
	else strcpy_s(nText, _countof(nText), "                     刺激坦克！即将开始！                      ");
	PrintPoint(x, y,     " ■■■    ■■■■■     ■■■■        ■■■       ■■■■  ", 0xf);
	PrintPoint(x, y + 1, "■    ■       ■        ■      ■      ■     ■     ■        ", 0xf);
	PrintPoint(x, y + 2, "■             ■        ■      ■     ■             ■        ", 0xf);
	PrintPoint(x, y + 3, " ■■■        ■        ■■■■■     ■  ■■■     ■■■■  ", 0xf);
	PrintPoint(x, y + 4, "      ■       ■        ■      ■     ■      ■     ■        ", 0xf);
	PrintPoint(x, y + 5, "■    ■       ■        ■      ■      ■    ■      ■        ", 0xf);
	PrintPoint(x, y + 6, " ■■■        ■        ■      ■       ■■■       ■■■■  ", 0xf);
	PrintPoint(x, y + 7, "                                                                 ", 0xf);
	PrintPoint(x, y + 8, nText, 0xa);
	PrintPoint(x, y + 9, "                                                                 ", 0xf);
	PlayMusic(SOUND_OPEN);

	PrintEmptyFrameEx(6, 37, 55, 6, 0xb, "□");//打印菜单框架
	SHORT nTankX = 8;
	SHORT nTankY = 40;
	for (int i=0;i<50;i++)
	{
		PrintPoint(x, y + 10, "                      正在努力加载：", i * 2, 0xb);
		PrintTank(nTankX, nTankY, "  ", 0xe, DIR_RIGHT);
		nTankX++;
		PrintTank(nTankX, nTankY, "■", 0xe, DIR_RIGHT);
		Sleep(100);
	}
}
void PrintNextStageText(SHORT x, SHORT y, int nStage)
{
	using std::cout;
	using std::endl;
	char nText[255];
	system("cls");
	sprintf_s(nText, _countof(nText), "              恭喜你通过了第 %d 关！准备开始第 %d 关！          ", nStage,nStage+1);
	PrintPoint(x, y,     " ■■■       ■■■       ■■■       ■■■       ■■■   ", 0xe);
	PrintPoint(x, y + 1, "■    ■     ■    ■     ■    ■     ■    ■     ■    ■     ", 0xe);
	PrintPoint(x, y + 2, "■           ■           ■           ■           ■         ", 0xe);
	PrintPoint(x, y + 3, "■■■■     ■■■■     ■■■■     ■■■■     ■■■■   ", 0xe);
	PrintPoint(x, y + 4, "■    ■     ■    ■     ■    ■     ■    ■     ■    ■     ", 0xe);
	PrintPoint(x, y + 5, "■    ■     ■    ■     ■    ■     ■    ■     ■    ■       ", 0xe);
	PrintPoint(x, y + 6, " ■■■       ■■■       ■■■       ■■■       ■■■   ", 0xe);
	PrintPoint(x, y + 7, "                                                                 ", 0xe);
	PrintPoint(x, y + 8, nText, 0xa);
	PrintPoint(x, y + 9, "                                                                 ", 0xe);
	PlayMusic(SOUND_WIN);
	Sleep(3000);
}

//取出当前点击位置的按钮信息
int GetTilteButton( int x, int y,int mousex,int mousey)
{

	if (mousey == y && mousex >= x && mousex <= x + 10)return BUTTON_STARTGAME;
	else if (mousey == y + 2 && mousex >= x && mousex <= x + 10)return BUTTON_LOADGAME;
	else if (mousey == y + 4 && mousex >= x && mousex <= x + 10)return BUTTON_CUSTOMMAP;
	else if (mousey == y + 6 && mousex >= x && mousex <= x + 10)return BUTTON_ENDGAME;

	return -1;

}
int GetStartGameButton(int x, int y, int mousex, int mousey)
{

	if (mousey == y && mousex >= x && mousex <= x + 10)return BUTTON_SINGLE;
	else if (mousey == y + 2 && mousex >= x && mousex <= x + 10)return BUTTON_PARTNER;
	else if (mousey == y + 4 && mousex >= x && mousex <= x + 10)return BUTTON_CUSTOMGAME;
	else if (mousey == y + 6 && mousex >= x && mousex <= x + 10)return BUTTON_LINKGAME;
	else if (mousey == y + 8 && mousex >= x && mousex <= x + 10)return BUTTON_BACKHOMEPAGE;

	return -1;

}
int GetLinkGameButton(int x, int y, int mousex, int mousey)
{

	if (mousey == y && mousex >= x && mousex <= x + 10)return BUTTON_CREATROOM;
	else if (mousey == y + 2 && mousex >= x && mousex <= x + 10)return BUTTON_ADDROOM;
	else if (mousey == y + 4 && mousex >= x && mousex <= x + 10)return BUTTON_BACKHOMEPAGE;

	return -1;

}
int GetCustomButton(int x, int y, int mousex, int mousey)
{

	if (mousey == y && mousex >= x && mousex <= x + 10)return BUTTON_NEWMAP;
	else if (mousey == y + 2 && mousex >= x && mousex <= x + 10)return BUTTON_OPENMAP;
	else if (mousey == y + 4 && mousex >= x && mousex <= x + 10)return BUTTON_BACKHOMEPAGE;
	return -1;

}
int GetPauseButton(int x, int y, int mousex, int mousey)
{

	if (mousey == y && mousex >= x && mousex <= x + 10)return BUTTON_CONTIUNEGAME;
	else if (mousey == y + 2 && mousex >= x && mousex <= x + 10)return BUTTON_SAVEGAME;
	else if (mousey == y + 4 && mousex >= x && mousex <= x + 10)return BUTTON_RESTGAME;
	else if (mousey == y + 6 && mousex >= x && mousex <= x + 10)return BUTTON_BACKHOMEPAGE;
	return -1;

}
int GetGameOverButton(int x, int y, int mousex, int mousey)
{

	if (mousey == y+9 && mousex >= x && mousex <= x + 10)return BUTTON_RESTGAME;
	else if (mousey == y + 10 && mousex >= x && mousex <= x + 10)return BUTTON_BACKHOMEPAGE;
	return -1;

}

//清除菜单文字与边框
void ClsTilteMenu(SHORT x, SHORT y)
{
		PrintPoint(x, y, "                    ", 3);
		PrintPoint(x, y + 2, "                    ", 3);
		PrintPoint(x, y + 4, "                    ", 3);
		PrintPoint(x, y + 6, "                    ", 3);
		PrintPoint(x, y + 8, "                    ", 3);


		int wide = 18;
		int higt = 12;
		SHORT x2 = 23;
		SHORT y2 = 41;
		for (SHORT w = 0; w <= wide; w++)
		{
			for (SHORT h = 0; h <= higt; h++)
			{
				if (h == 0 || h == higt)
				{
					PrintPoint(w + x2, h + y2, "  ", 0);
				}
				else if (w == 0 || w == wide)
				{
					PrintPoint(w + x2, h + y2, "  ", 0);
				}
			}
		}
}
void ClsPuaseMenu(SHORT x, SHORT y)
{
	PrintPoint(x, y, "                    ", 3);
	PrintPoint(x, y + 2, "                    ", 3);
	PrintPoint(x, y + 4, "                    ", 3);
	PrintPoint(x, y + 6, "                    ", 3);

	int wide = 14;
	int higt = 10;
	SHORT x2 = WALL_WIDE;
	SHORT y2 = 48;

	for (SHORT w = 0; w <= wide; w++)
	{
		for (SHORT h = 0; h <= higt; h++)
		{
			if (h == 0 || h == higt)
			{
				PrintPoint(w + x2, h + y2, "  ", 0);
			}
			else if (w == 0 || w == wide)
			{
				PrintPoint(w + x2, h + y2, "  ", 0);
			}
		}
	}
}

//打印高亮时菜单的文本
void PrintTilteMenu(int nbutton, SHORT x, SHORT y)
{

	switch (nbutton)
	{
	case BUTTON_STARTGAME:
		PrintPoint(x, y, "开始游戏", 0xb);
		PrintPoint(x, y + 2, "载入存档", 3);
		PrintPoint(x, y + 4, "编辑地图", 3);
		PrintPoint(x, y + 6, "结束游戏", 3);
		break;
	case BUTTON_LOADGAME:
		PrintPoint(x, y, "开始游戏", 3);
		PrintPoint(x, y + 2, "载入存档", 0xb);
		PrintPoint(x, y + 4, "编辑地图", 3);
		PrintPoint(x, y + 6, "结束游戏", 3);
		break;
	case BUTTON_CUSTOMMAP:
		PrintPoint(x, y, "开始游戏", 3);
		PrintPoint(x, y + 2, "载入存档", 3);
		PrintPoint(x, y + 4, "编辑地图", 0xb);
		PrintPoint(x, y + 6, "结束游戏", 3);
		break;
	case BUTTON_ENDGAME:
		PrintPoint(x, y, "开始游戏", 3);
		PrintPoint(x, y + 2, "载入存档", 3);
		PrintPoint(x, y + 4, "编辑地图", 3);
		PrintPoint(x, y + 6, "结束游戏", 0xb);
		break;
	default:
		break;
	}

}
void PrintStartGameMenu(int nbutton, SHORT x, SHORT y)
{

	switch (nbutton)
	{
	case BUTTON_SINGLE:
		PrintPoint(x, y, "单人游戏", 0xb);
		PrintPoint(x, y + 2, "双人游戏", 3);
		PrintPoint(x, y + 4, "自定义游戏", 3);
		PrintPoint(x, y + 6, "联机游戏", 3);
		PrintPoint(x, y + 8, "返回主菜单", 3);
		break;
	case BUTTON_PARTNER:
		PrintPoint(x, y, "单人游戏", 3);
		PrintPoint(x, y + 2, "双人游戏", 0xb);
		PrintPoint(x, y + 4, "自定义游戏", 3);
		PrintPoint(x, y + 6, "联机游戏", 3);
		PrintPoint(x, y + 8, "返回主菜单", 3);
		break;
	case BUTTON_CUSTOMGAME:
		PrintPoint(x, y, "单人游戏", 3);
		PrintPoint(x, y + 2, "双人游戏", 3);
		PrintPoint(x, y + 4, "自定义游戏", 0xb);
		PrintPoint(x, y + 6, "联机游戏", 3);
		PrintPoint(x, y + 8, "返回主菜单", 3);
		break;
	case BUTTON_LINKGAME:
		PrintPoint(x, y, "单人游戏", 3);
		PrintPoint(x, y + 2, "双人游戏", 3);
		PrintPoint(x, y + 4, "自定义游戏",3 );
		PrintPoint(x, y + 6, "联机游戏", 0xb);
		PrintPoint(x, y + 8, "返回主菜单", 3);
		break;
	case BUTTON_BACKHOMEPAGE:
		PrintPoint(x, y, "单人游戏", 3);
		PrintPoint(x, y + 2, "双人游戏", 3);
		PrintPoint(x, y + 4, "自定义游戏", 3);
		PrintPoint(x, y + 6, "联机游戏", 3);
		PrintPoint(x, y + 8, "返回主菜单", 0xb);		break;
	}

}
void PrintLinkGameMenu(int nbutton, SHORT x, SHORT y)
{

	switch (nbutton)
	{
	case BUTTON_CREATROOM:
		PrintPoint(x, y, "创建房间", 0xb);
		PrintPoint(x, y + 2, "加入房间", 3);
		PrintPoint(x, y + 4, "返回主菜单", 3);
		break;
	case BUTTON_ADDROOM:
		PrintPoint(x, y, "创建房间", 3);
		PrintPoint(x, y + 2, "加入房间", 0xb);
		PrintPoint(x, y + 4, "返回主菜单", 3);
		break;
	case BUTTON_BACKHOMEPAGE:
		PrintPoint(x, y, "创建房间", 3);
		PrintPoint(x, y + 2, "加入房间", 3);
		PrintPoint(x, y + 4, "返回主菜单", 0xb);
		break;
	}

}
void PrintCustomMenu(int nbutton, SHORT x, SHORT y)
{

	switch (nbutton)
	{
	case BUTTON_NEWMAP:
		PrintPoint(x, y, "新建地图", 0xb);
		PrintPoint(x, y + 2, "打开地图", 3);
		PrintPoint(x, y + 4, "返回主菜单", 3);
		break;
	case BUTTON_OPENMAP:
		PrintPoint(x, y, "新建地图", 3);
		PrintPoint(x, y + 2, "打开地图", 0xb);
		PrintPoint(x, y + 4, "返回主菜单", 3);
		break;
	case BUTTON_BACKHOMEPAGE:
		PrintPoint(x, y, "新建地图", 3);
		PrintPoint(x, y + 2, "打开地图", 3);
		PrintPoint(x, y + 4, "返回主菜单", 0xb);
		break;
	}

}
void PrintPauseMenu(SHORT x, SHORT y, int nbutton)
{
	switch (nbutton)
	{
	case BUTTON_CONTIUNEGAME:
		PrintPoint(x, y, "【继续游戏】", 0xa);
		PrintPoint(x, y + 2, "【保存游戏】", 2);
		PrintPoint(x, y + 4, "【重新开始】", 2);
		PrintPoint(x, y + 6, "【回到主页】", 2);
		break;
	case BUTTON_SAVEGAME:
		PrintPoint(x, y, "【继续游戏】", 2);
		PrintPoint(x, y + 2, "【保存游戏】", 0xa);
		PrintPoint(x, y + 4, "【重新开始】", 2);
		PrintPoint(x, y + 6, "【回到主页】", 2);
		break;
	case BUTTON_RESTGAME:
		PrintPoint(x, y, "【继续游戏】", 2);
		PrintPoint(x, y + 2, "【保存游戏】", 2);
		PrintPoint(x, y + 4, "【重新开始】", 0xa);
		PrintPoint(x, y + 6, "【回到主页】", 2);
		break;
	case BUTTON_BACKHOMEPAGE:
		PrintPoint(x, y, "【继续游戏】", 2);
		PrintPoint(x, y + 2, "【保存游戏】", 2);
		PrintPoint(x, y + 4, "【重新开始】", 2);
		PrintPoint(x, y + 6, "【回到主页】", 0xa);
		break;
		/*
		case 4:
		PrintPoint(x, y, "             ", 0);
		PrintPoint(x, y + 2, "             ", 0);
		PrintPoint(x, y + 4, "             ", 0);
		PrintPoint(x, y + 6, "             ", 0);
		break;*/

	}
}
void PrintGameOverMenu(SHORT x, SHORT y, int nbutton)
{
	using std::cout;
	using std::endl;
	int color = 0xf;

	PrintPoint(x, y, "       ■■■■■", color);
	PrintPoint(x, y + 1, "    ■■■■■■■■ ", color);
	PrintPoint(x, y + 2, "  ■■■ ■■■ ■■■ ", color);
	PrintPoint(x, y + 3, " ■■■    ■    ■■■ ", color);
	PrintPoint(x, y + 4, "  ■■■  ■■  ■■■", color);
	PrintPoint(x, y + 5, "   ■■■■■■■■■", color);
	PrintPoint(x, y + 6, "     ■■■■■■■", color);
	PrintPoint(x, y + 7, "     ■■ ■■ ■■", color);
	PrintPoint(x, y + 8, "       Game Over!        ", 0xa);
	if (nbutton == BUTTON_RESTGAME)
	{
		PrintPoint(x, y + 9, "        重新开始        ", 0x8b);
		PrintPoint(x, y + 10, "        回到主页        ", 0xb);
	}
	else if (nbutton == BUTTON_BACKHOMEPAGE)
	{
		PrintPoint(x, y + 9, "        重新开始        ", 0xb);
		PrintPoint(x, y + 10, "        回到主页        ", 0x8b);
	}
	PrintPoint(x, y + 11, "     ■■ ■■ ■■ ", color);
	PrintPoint(x, y + 12, "      ■■■■■■", color);
}
void PrintLinkGameOverMenu(SHORT x, SHORT y, int nbutton)
{
	using std::cout;
	using std::endl;
	int color = 0xf;

	PrintPoint(x, y, "       ■■■■■", color);
	PrintPoint(x, y + 1, "    ■■■■■■■■ ", color);
	PrintPoint(x, y + 2, "  ■■■ ■■■ ■■■ ", color);
	PrintPoint(x, y + 3, " ■■■    ■    ■■■ ", color);
	PrintPoint(x, y + 4, "  ■■■  ■■  ■■■", color);
	PrintPoint(x, y + 5, "   ■■■■■■■■■", color);
	PrintPoint(x, y + 6, "     ■■■■■■■", color);
	PrintPoint(x, y + 7, "     ■■ ■■ ■■", color);
	PrintPoint(x, y + 8, "       Game Over!        ", 0xa);
	if (nbutton == BUTTON_RESTGAME)
	{
		PrintPoint(x, y + 9, "        联机结束        ", 0x8b);
		PrintPoint(x, y + 10, "        回到主页        ", 0xb);
	}
	else if (nbutton == BUTTON_BACKHOMEPAGE)
	{
		PrintPoint(x, y + 9, "        联机结束        ", 0xb);
		PrintPoint(x, y + 10, "        回到主页        ", 0x8b);
	}
	PrintPoint(x, y + 11, "     ■■ ■■ ■■ ", color);
	PrintPoint(x, y + 12, "      ■■■■■■", color);
}

