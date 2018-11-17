#include "stdafx.h"
#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
#include <vector>
#include <time.h>
#include <math.h>
#include "MyHead.h"
#include "TCP.h"

using namespace std;

class Map;//自定义地图类
class Tank;//坦克类
class Obstacle;//障碍物类
class Bullet;//炮弹类
class AutoTank;//控制PC所有坦克行为类
class Buffer;//食物加成类

int MessageLoop(int nPage);
int MouseEvenProc(MOUSE_EVENT_RECORD mer, int nPage,Map &MyMap);

vector <POINTDATA> pObstacle;//储存所有障碍物的坐标类数组
vector <POINTDATA> pBullet;//储存所有炮弹的坐标类数组
vector <Tank> pPcTank;//储存所有PC坦克的数组
vector <POINTDATA> pAllBuffer;//储存所有加成的数组

vector <POINTDATA> pMap;//储存自定义地图时绘画图案的坐标类数组
vector <POINTDATA> pPcTankConfig;//储存读取地图时所有PC坦克的坐标类数组
vector <POINTDATA> pPlayerTankConfig;//储存读取地图时玩家坦克的坐标类数组
vector <TankInfo> pPlayerTankConfigLoad;//储存读取存档时玩家坦克的具体信息
vector <POINTDATA> pBufferConfig;//储存当前食物加成的分布坐标信息

TCP_SERVER MySever;//定义TCP服务端类
TCP_CLIENT MyClient;//定义TCP客户端类
MYDATA SendMyData{};//初始化发包数据结构体
PMYDATA RecvMyData = nullptr;//初始化接受数据包结构体指针
int LinkType = TCP_LINK_NULL;//连接类型默认单机
int TankAId = 0;//联机坦克A生成的ID
int TankBId = 0;//联机坦克B生成的ID

class Obstacle
{
private:
	vector<POINTDATA>pObstacleGrass;

public:
	Obstacle(){ CreatWall(0, 0, WALL_WIDE-1, WALL_HIGH-1, false); PrintObstaclePoint(); }
	Obstacle(int wide, int higt) 
	{ 
		CreatWall(0,0,wide-1, higt-1,false); 

		CreatWall(20, 53, 6, 6, false);
		CreatCamp(22, 55, 2, 2, true, OBSTACLE_CAMP_A);

		PrintObstaclePoint();
	}
	~Obstacle()
	{
		pObstacleGrass.clear();
		pObstacleGrass.swap(vector<POINTDATA>());
	}
	void CreatWall(int x, int y, int wide, int higt, bool fill)
	{

		for (int w = 0; w <= wide; w++)
		{
			for (int h = 0; h <= higt; h++)
			{
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 8, "■", 0, OBSTACLE_WALL));
				else
				{
					if (h == 0 || h == higt)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 8, "■", 0, OBSTACLE_WALL));

					}
					else if (w == 0 || w == wide)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 8, "■", 0, OBSTACLE_WALL));
					}
				}
			}
		}
	}
	void CreatEarthWall(int x, int y, int wide, int higt,bool fill)
	{

		for (int w = 0; w <= wide; w++)
		{
			for (int h = 0; h <= higt; h++)
			{
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 6, "■", 0, OBSTACLE_EARTHWALL));
				else
				{
					if (h == 0 || h == higt)
					{
					pObstacle.push_back(POINTDATA(w + x, h + y, 6, "■", 0, OBSTACLE_EARTHWALL));

					}
					else if (w == 0 || w == wide)
					{
					pObstacle.push_back(POINTDATA(w + x, h + y, 6, "■", 0, OBSTACLE_EARTHWALL));
					}
				}

			}
		}
	}
	void CreatWater(int x, int y, int wide, int higt, bool fill)
	{

		for (int w = 0; w <= wide; w++)
		{
			for (int h = 0; h <= higt; h++)
			{
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 9, "○", 0, OBSTACLE_WATER));
				else
				{
					if (h == 0 || h == higt)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 9, "○", 0, OBSTACLE_WATER));

					}
					else if (w == 0 || w == wide)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 9, "○", 0, OBSTACLE_WATER));
					}
				}

			}
		}
	}
	void CreatGrass(int x, int y, int wide, int higt, bool fill)
	{

		for (int w = 0; w <= wide; w++)
		{
			for (int h = 0; h <= higt; h++)
			{
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 2, "△", 0, OBSTACLE_GRASS));
				else
				{
					if (h == 0 || h == higt)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 2, "△", 0, OBSTACLE_GRASS));

					}
					else if (w == 0 || w == wide)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 2, "△", 0, OBSTACLE_GRASS));
					}
				}

			}
		}
	}
	void CreatCamp(int x, int y, int wide, int higt, bool fill,int type)
	{

		for (int w = 0; w <= wide; w++)
		{
			for (int h = 0; h <= higt; h++)
			{
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 14, "★", 0, type));
				else
				{
					if (h == 0 || h == higt)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 14, "★", 0, type));

					}
					else if (w == 0 || w == wide)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 14, "★", 0, type));
					}
				}

			}
		}
	}

	bool IsTouchObstacle(SHORT x, SHORT y)
	{
		for (auto &val : pObstacle) { if (val.m_x == x && val.m_y == y) return true; }
		return false;
	}
	void PrintObstaclePoint()
	{
		for (auto &val : pObstacle) {
			PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
		}
	}
	void PrintWaterPoint()
	{
		for (auto &val : pObstacle) {
			if (val.m_type==OBSTACLE_WATER)PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
		}
	}
	void PrintGrassPoint()
	{
		if (pObstacleGrass.size() == 0)
		{
			for (auto &val : pObstacle)
			{
				if (val.m_type == OBSTACLE_GRASS)
					pObstacleGrass.push_back(val);
			}
		}
		for (auto &val : pObstacleGrass)PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
	}
};
class Bullet
{
	int m_bulletmovtime = clock();//炮弹移动时间记录
public:
	Bullet() {}
	~Bullet()
	{
		vector <POINTDATA>().swap(pBullet);
	}

	void SetBullet(int x , int y ,int color,char *data, int dir, int type,int tankid)
	{
		SendMyData.m_State = DATA_BULLET;
		SendMyData.m_X = x;;
		SendMyData.m_Y = y;
		SendMyData.m_Dir = dir;
		SendMyData.m_Type = type;
		SendMyData.m_Id = tankid;
		SendMyData.m_Color = color;
		if (LinkType == TCP_LINK_SERVER && TankBId != tankid)MySever.AddData(SendMyData);
		else if (LinkType == TCP_LINK_CLIENT && TankBId == tankid)MyClient.AddData(SendMyData);

		int nOType;
		switch (dir)
		{
		case DIR_UP:
			nOType = GetObstacleType(x, y - 2, dir);
			if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return;
			y = y - 2;
			PrintPoint(x, y, data, color);
			break;
		case DIR_DOWN:
			nOType = GetObstacleType(x, y + 2, dir);
			if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return;
			y = y + 2;
			PrintPoint(x, y, data, color);
			break;
		case DIR_LEFT:
			nOType = GetObstacleType(x-2, y, dir);
			if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return;
			x = x - 2;
			PrintPoint(x, y, data, color);
			break;
		case DIR_RIGHT:
			nOType = GetObstacleType(x + 2, y, dir);
			if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return;
			x = x + 2;
			PrintPoint(x, y, data, color);
			break;
		}

		pBullet.push_back(POINTDATA(x, y, color, data, dir, type));


	}
	void SetBulletGroup(int x, int y, int color, char *data, int dir, int type, int tankid)
	{
		SendMyData.m_State = DATA_BULLETGROUP;
		SendMyData.m_X = x;;
		SendMyData.m_Y = y;
		SendMyData.m_Dir = dir;
		SendMyData.m_Type = type;
		SendMyData.m_Id = tankid;
		SendMyData.m_Color = color;
		if (LinkType == TCP_LINK_SERVER && TankBId != tankid)MySever.AddData(SendMyData);
		else if (LinkType == TCP_LINK_CLIENT && TankBId == tankid)MyClient.AddData(SendMyData);

		int nOType[3];
		switch (dir)
		{
		case DIR_UP:
			nOType[0] = GetObstacleType(x-1, y - 2, dir);
			nOType[1] = GetObstacleType(x, y - 2, dir);
			nOType[2] = GetObstacleType(x+1, y - 2, dir);
			if (IsSetBullet(nOType[0]))
			pBullet.push_back(POINTDATA(x - 1, y - 2, color, data, dir, type));
			if (IsSetBullet(nOType[1]))
			pBullet.push_back(POINTDATA(x , y - 2, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
			pBullet.push_back(POINTDATA(x + 1, y - 2, color, data, dir, type));
			break;
		case DIR_DOWN:
			nOType[0] = GetObstacleType(x - 1, y + 2, dir);
			nOType[1] = GetObstacleType(x, y + 2, dir);
			nOType[2] = GetObstacleType(x + 1, y + 2, dir);
			if (IsSetBullet(nOType[0]))
				pBullet.push_back(POINTDATA(x - 1, y + 2, color, data, dir, type));
			if (IsSetBullet(nOType[1]))
				pBullet.push_back(POINTDATA(x, y + 2, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
				pBullet.push_back(POINTDATA(x + 1, y + 2, color, data, dir, type));
			break;
		case DIR_LEFT:
			nOType[0] = GetObstacleType(x - 2, y -1, dir);
			nOType[1] = GetObstacleType(x-2, y, dir);
			nOType[2] = GetObstacleType(x -2, y +1, dir);
			if (IsSetBullet(nOType[0]))
				pBullet.push_back(POINTDATA(x -2, y-1 , color, data, dir, type));
			if (IsSetBullet(nOType[1]))
				pBullet.push_back(POINTDATA(x - 2, y, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
				pBullet.push_back(POINTDATA(x - 2, y+1, color, data, dir, type));
			break;
		case DIR_RIGHT:
			nOType[0] = GetObstacleType(x + 2, y - 1, dir);
			nOType[1] = GetObstacleType(x + 2, y, dir);
			nOType[2] = GetObstacleType(x + 2, y + 1, dir);
			if (IsSetBullet(nOType[0]))
				pBullet.push_back(POINTDATA(x + 2, y - 1, color, data, dir, type));
			if (IsSetBullet(nOType[1]))
				pBullet.push_back(POINTDATA(x + 2, y, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
				pBullet.push_back(POINTDATA(x + 2, y + 1, color, data, dir, type));
			break;
		}


	}
	void SetBulletSpuer(int x, int y, int color, char *data, int dir, int type, int tankid)
	{
		//联机游戏发包
		{
			SendMyData.m_State = DATA_BULLETSUPER;
			SendMyData.m_X = x;;
			SendMyData.m_Y = y;
			SendMyData.m_Dir = dir;
			SendMyData.m_Type = type;
			SendMyData.m_Id = tankid;
			SendMyData.m_Color = color;
			if (LinkType == TCP_LINK_SERVER && TankBId != tankid)MySever.AddData(SendMyData);
			else if (LinkType == TCP_LINK_CLIENT && TankBId == tankid)MyClient.AddData(SendMyData);
		}

		int nOType[3];
			dir = DIR_UP;//向上发3个炮弹
			//判断炮弹待发的坐标有没有障碍物，返回类型
			nOType[0] = GetObstacleType(x - 1, y - 2, dir);
			nOType[1] = GetObstacleType(x, y - 2, dir);
			nOType[2] = GetObstacleType(x + 1, y - 2, dir);
			//判断是否能在这个位置发出炮弹
			if (IsSetBullet(nOType[0]))
				pBullet.push_back(POINTDATA(x - 1, y - 2, color, data, dir, type));
			if (IsSetBullet(nOType[1]))
				pBullet.push_back(POINTDATA(x, y - 2, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
				pBullet.push_back(POINTDATA(x + 1, y - 2, color, data, dir, type));

			dir = DIR_DOWN;//向下发3个炮弹
			nOType[0] = GetObstacleType(x - 1, y + 2, dir);
			nOType[1] = GetObstacleType(x, y + 2, dir);
			nOType[2] = GetObstacleType(x + 1, y + 2, dir);
			if (IsSetBullet(nOType[0]))
				pBullet.push_back(POINTDATA(x - 1, y + 2, color, data, dir, type));
			if (IsSetBullet(nOType[1]))
				pBullet.push_back(POINTDATA(x, y + 2, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
				pBullet.push_back(POINTDATA(x + 1, y + 2, color, data, dir, type));

			dir = DIR_LEFT;
			nOType[0] = GetObstacleType(x - 2, y - 1, dir);
			nOType[1] = GetObstacleType(x - 2, y, dir);
			nOType[2] = GetObstacleType(x - 2, y + 1, dir);
			if (IsSetBullet(nOType[0]))
				pBullet.push_back(POINTDATA(x - 2, y - 1, color, data, dir, type));
			if (IsSetBullet(nOType[1]))
				pBullet.push_back(POINTDATA(x - 2, y, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
				pBullet.push_back(POINTDATA(x - 2, y + 1, color, data, dir, type));

			dir = DIR_RIGHT;
			nOType[0] = GetObstacleType(x + 2, y - 1, dir);
			nOType[1] = GetObstacleType(x + 2, y, dir);
			nOType[2] = GetObstacleType(x + 2, y + 1, dir);
			if (IsSetBullet(nOType[0]))
				pBullet.push_back(POINTDATA(x + 2, y - 1, color, data, dir, type));
			if (IsSetBullet(nOType[1]))
				pBullet.push_back(POINTDATA(x + 2, y, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
				pBullet.push_back(POINTDATA(x + 2, y + 1, color, data, dir, type));



	}
	bool IsSetBullet(int type)
	{
		if (type != OBSTACLE_WALL && type != OBSTACLE_EARTHWALL && type != OBSTACLE_CAMP_A && type != OBSTACLE_CAMP_B && type != OBSTACLE_CAMP_PC)
			return true;
		return false;
	}
	void RefreshObstacle(int x, int y)
	{
		for (auto &val : pObstacle) {
			if (val.m_type == OBSTACLE_WATER || val.m_type == OBSTACLE_GRASS )
				if( val.m_x == x && val.m_y == y)
				PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
		}
	}
	int MoveBullet(int bulletmovtime)
	{
		if (clock() - m_bulletmovtime < bulletmovtime)return-1;
		m_bulletmovtime = clock();

		if (pBullet.size() == 0)return -1;

		int nOType;

		for (auto &val: pBullet)//遍历所有活动的炮弹
		{
				if (val.m_dir == DIR_UP)//按方向判断
				{
					val.m_y--;//移动炮弹的坐标
					//取出炮弹所在位置是什么障碍物
					nOType = GetObstacleType(val.m_x, val.m_y, val.m_dir);
					if (nOType==OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)
					{	//如果是钢墙，土墙，基地
						PrintPoint(val.m_x, val.m_y + 1, "  ", val.m_color);//擦除前一个子弹坐标图案
						RefreshObstacle(val.m_x, val.m_y + 1);//刷新被擦除的草丛和河流
						val.m_type = BULLET_CLEAN;//将子弹删除
						//如果打到基地就直接返回，不继续游戏了
						if (nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return nOType;
					}
					else if(nOType==OBSTACLE_WATER || nOType == OBSTACLE_GRASS)
					{	//如果打到水或者草丛
						PrintPoint(val.m_x, val.m_y+1, "  ", val.m_color);//擦除前一个子弹坐标图案
						RefreshObstacle(val.m_x, val.m_y + 1);//刷新被擦除的草丛与河流
					}
					else if (nOType == OBSTACLE_NULL)
					{	//如果前面没有障碍物
						PrintPoint(val.m_x, val.m_y + 1, "  ", val.m_color);//擦除前一个子弹坐标图案
						RefreshObstacle(val.m_x, val.m_y + 2);//刷新被擦除的草丛与河流
					}
				}

				else if (val.m_dir == DIR_DOWN)
				{
					val.m_y++;
					nOType = GetObstacleType(val.m_x, val.m_y, val.m_dir);
					if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)
					{
						PrintPoint(val.m_x, val.m_y-1, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x, val.m_y - 1);//刷新被擦除的Obstacle
						val.m_type = BULLET_CLEAN;

						if (nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return nOType;

					}
					else if (nOType == OBSTACLE_WATER || nOType == OBSTACLE_GRASS)
					{
						PrintPoint(val.m_x, val.m_y - 1, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x, val.m_y - 1);//刷新被擦除的Obstacle
					}
					else if (nOType == OBSTACLE_NULL)
					{
						PrintPoint(val.m_x, val.m_y - 1, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x, val.m_y - 2);//刷新被擦除的Obstacle
					}
				}

				else if (val.m_dir == DIR_LEFT)
				{
					val.m_x--;
					nOType = GetObstacleType(val.m_x, val.m_y, val.m_dir);
					if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)
					{
						PrintPoint(val.m_x+1, val.m_y, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x + 1, val.m_y);//刷新被擦除的Obstacle
						val.m_type = BULLET_CLEAN;

						if (nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return nOType;

					}
					else if (nOType == OBSTACLE_WATER || nOType == OBSTACLE_GRASS)
					{
						PrintPoint(val.m_x + 1, val.m_y, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x + 1, val.m_y);//刷新被擦除的Obstacle
					}
					else if (nOType == OBSTACLE_NULL)
					{
						PrintPoint(val.m_x + 1, val.m_y, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x + 2, val.m_y);//刷新被擦除的Obstacle
					}
				}

				else if (val.m_dir == DIR_RIGHT)
				{
					val.m_x++;
					nOType = GetObstacleType(val.m_x, val.m_y, val.m_dir);
					if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)
					{
						PrintPoint(val.m_x-1, val.m_y, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x - 1, val.m_y);//刷新被擦除的Obstacle
						val.m_type = BULLET_CLEAN;

						if (nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return nOType;

					}
					else if (nOType == OBSTACLE_WATER || nOType == OBSTACLE_GRASS)
					{
						PrintPoint(val.m_x - 1, val.m_y, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x - 1, val.m_y);//刷新被擦除的Obstacle
					}
					else if (nOType == OBSTACLE_NULL)
					{
						PrintPoint(val.m_x - 1, val.m_y, "  ", val.m_color);//擦除前一个坐标图案
						RefreshObstacle(val.m_x - 2, val.m_y);//刷新被擦除的Obstacle
					}
				}

				//val为当前子弹，val2遍历全部子弹
				for (auto &val2 : pBullet)
				{	//判断当前子弹与遍历的所有子弹坐标，排除自身，用生成时间判断time
					if (val.m_x == val2.m_x && val.m_y == val2.m_y && val.m_time != val2.m_time)
					{
						val.m_type = BULLET_CLEAN;//标记为删除属性
						val2.m_type = BULLET_CLEAN;//标记为删除属性
					}
				}
		}

		//要用倒序循环，不然删除的时候会影响后续判断的数组元素位置
		for (int i = pBullet.size() - 1; i >= 0; i--)
		{
			if (pBullet[i].m_type == BULLET_CLEAN) pBullet.erase(pBullet.begin() + i);
		}


		return nOType;

	}
	int GetObstacleType(int x, int y,int dir)//是否打到障碍物了
	{
		for (int i= pObstacle.size()-1;i>=0;i--)
		{
			if (pObstacle[i].m_x == x && pObstacle[i].m_y == y)
			{
				if (pObstacle[i].m_type == OBSTACLE_EARTHWALL)//打到土墙并擦除
				{
					pObstacle.erase(pObstacle.begin() + i);
					PrintPoint(x, y, "  ", 6);
					EraserEarthWall(x,y, dir);//擦除炮弹相邻的两边土墙
					return OBSTACLE_EARTHWALL;
				}
				else if (pObstacle[i].m_type == OBSTACLE_WATER)//打到水
				{
					return OBSTACLE_WATER;
				}
				else if (pObstacle[i].m_type == OBSTACLE_WALL)//打到墙
				{
					return OBSTACLE_WALL;
				}
				else if (pObstacle[i].m_type == OBSTACLE_GRASS)//打到草
				{
					return OBSTACLE_GRASS;
				}
				return pObstacle[i].m_type;
			}
		}
		//for (auto &val : pObstacle) { if (val.m_x == x && val.m_y == y) return true; }
		return OBSTACLE_NULL;
	}
	void EraserEarthWall(int x,int y,int dir)
	{
		int nEWx[2] = { 0 };
		int nEWy[2] = { 0 };

		switch (dir)
		{
		case DIR_UP:case DIR_DOWN:
			nEWx[0] = x - 1;
			nEWx[1] = x + 1;
			nEWy[0] = y;
			nEWy[1] = y;
			break;
		case DIR_LEFT:case DIR_RIGHT:
			nEWx[0] = x ;
			nEWx[1] = x ;
			nEWy[0] = y-1;
			nEWy[1] = y+1;
			break;
		}

		for (int i = pObstacle.size() - 1; i >= 0; i--)
		{
			if (pObstacle[i].m_type == OBSTACLE_EARTHWALL)//打到土墙并擦除
			{
				for (int j=0;j<_countof(nEWx);j++)
				{
					if (pObstacle[i].m_x== nEWx[j]&& pObstacle[i].m_y == nEWy[j])
					{
						PrintPoint(pObstacle[i].m_x, pObstacle[i].m_y, "  ", 6);
						pObstacle.erase(pObstacle.begin() + i);
						break;
					}
				}
			}
		}
	}
	void PrintBullet()
	{
		for (auto &val: pBullet)
		{
			PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
		}
	
	}
};
class Buffer
{
	vector <POINTDATA> pBufferTeam;

	int m_foodnum = 0;
	int m_foodcd = 5000;//ms单位
	int m_food_time  ;

public:
	Buffer():m_food_time(clock())
	{
		pBufferTeam.assign(pBufferConfig.begin(), pBufferConfig.end());
		m_foodnum = pBufferTeam.size();
	}

	void RefreshBuffer()
	{
		if (pAllBuffer.size() >= (DWORD)m_foodnum)return;

		if(clock()- m_food_time<= m_foodcd)return;
		m_food_time = clock();

		int nType, nRandBuffer, nX, nY;
		do 
		{
			 nType = GetRandNum(BUFFER_ATTACK, BUFFER_KILLALL);
			 nRandBuffer = GetRandNum(1, m_foodnum) - 1;
			 nX = pBufferTeam[nRandBuffer].m_x;
			 nY = pBufferTeam[nRandBuffer].m_y;
		} while (IsHaveBuffer(nX,nY));

		if (LinkType == 2)return;
		AddBuffer(nX, nY, nType);
	}
	void ClearBuffer()
	{
		for (int i= pAllBuffer.size()-1;i>=0;i--)
		{
			PrintPoint(pAllBuffer[i].m_x, pAllBuffer[i].m_y, "  ", 0);
			pAllBuffer.erase(pAllBuffer.begin() + i);
		}
	}
	void AddBuffer(int x,int y,int type)	
	{
		int ncolor = 0;
		char *data=nullptr;

		switch (type)
		{
		case BUFFER_ATTACK:
			data = "★";
			ncolor = 0xc;
			break;
		case BUFFER_BULLECTCD:
			data = "★";
			ncolor = 0x9;
			break;
		case BUFFER_NOHURT:
			data = "☆";
			ncolor = 0xc;
			break;
		case BUFFER_STATICALL:
			data = "☆";
			ncolor = 0x9;
			break;
		case BUFFER_KILLALL:
			data = "卍";
			ncolor = 0xc;
			break;
		}

		pAllBuffer.push_back(POINTDATA(x, y, ncolor, data, 0, type));

		SendMyData.m_State = DATA_BUFFER;
		SendMyData.m_X = x;
		SendMyData.m_Y = y;
		SendMyData.m_Type = type;
		if (LinkType == TCP_LINK_SERVER)MySever.AddData(SendMyData);
	}
	bool IsHaveBuffer(int x, int y)
	{
		for (auto &val: pAllBuffer)if (val.m_x == x&& val.m_y == y)return true;
		return false;
	}
	int IsTouchBuffer(int x, int y)
	{
		for (int i= pAllBuffer.size()-1;i>=0;i--)
		{
			if (pAllBuffer[i].m_y == y - 1 && pAllBuffer[i].m_x >= x - 1 && pAllBuffer[i].m_x <= x + 1 ||
				pAllBuffer[i].m_y == y && pAllBuffer[i].m_x >= x - 1 && pAllBuffer[i].m_x <= x + 1 ||
				pAllBuffer[i].m_y == y + 1 && pAllBuffer[i].m_x >= x - 1 && pAllBuffer[i].m_x <= x + 1)
			{
				int nRet = pAllBuffer[i].m_type;
				pAllBuffer.erase(pAllBuffer.begin() + i);
				m_food_time = clock();
				PlayMusic(SOUND_BUFFER); 
				return nRet;
			}
		}
		return BUFFER_NULL;
	}
	void PrintBuffer()
	{
		for (auto &val : pAllBuffer)
		{
			PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
		}

	}
};
class Tank
{
	int m_x=2;//坦克坐标
	int m_y=2;

	int m_maxhp = 10;//坦克最大hp
	int m_maxmp = 100;//坦克最大mp

	int m_hp = 10;//坦克hp
	int m_mp = 100;//坦克mp

	int m_revhp = 1;//每次恢复Hp
	int m_revmp = 3;//每次恢复Mp

	int m_hp_time = 1;//恢复时间1秒
	int m_mp_time = 1;//恢复时间1秒

	int m_dir = 0;//坦克方向
	int m_color = 0;//坦克颜色
	int m_tankid = 0;//坦克id
	int m_rank = 0;//坦克计分

	int m_speed = 1;//坦克速度
	int m_speed_time = 1;

	int m_bulletcd = 100;//ms 发炮延迟
	int m_bullet_time = 0;

	int m_attack = 1;//攻击力
	int m_attack_buff = 10000;//ms
	int m_attack_time = 0;

	int m_bullercd_buff = 10000;//ms
	int m_bullercd_time = 0;

	int m_nohurt_buff = 10000;//ms
	int m_nohurt_time = 0;
	char *m_tank_data="■";
	char m_tank_hp_data[3] = "■";

	int m_staticall_buff = 5000;//ms
	int m_staticall_time = 0;

	int m_killall_buff = 1000;//ms
	int m_killall_time = 0;

	int m_Ai = 10000;//ms
	int m_Ai_time = 0;
	int m_Ai_type = TANK_PA;

	vector <POINTXY> pPointXY;//储存A星算法路线坐标

public:
	Tank(){}
	Tank(int x, int y, int color, int dir, int m_speed, int bulletcd, int hp, int mp,int revhp,int revmp,int tankid,int maxhp, int maxmp) :
		m_x(x), m_y(y), m_color(color), m_dir(dir), m_speed(m_speed), m_bulletcd(bulletcd),
		m_tankid(tankid),m_hp(hp),m_mp(mp), m_revhp(revhp), m_revmp(revmp),m_maxhp(maxhp), m_maxmp(maxmp)
	{
	}
	int GetTankType()
	{
		switch (m_maxhp)
		{
		case TANK_PC1_HP:
			return TANK_PC1;
		case TANK_PC2_HP:
			return TANK_PC2;
		case TANK_PC3_HP:
			return TANK_PC3;
		case TANK_PC4_HP:
			return TANK_PC4;
		case TANK_PC5_HP:
			return TANK_PC5;
		}
		return TANK_PC1;
	}
	int GetTankX()
	{
		return m_x;
	}
	void SetTankX(int x)
	{
		m_x = x;
	}
	int GetTankY()
	{
		return m_y;
	}
	void SetTankY(int y)
	{
		m_y = y;
	}
	int GetDir()
	{
		return m_dir;
	}
	void SetDir(int dir)
	{
		if (m_speed_time++ < m_speed)return;
		m_speed_time = 1;

		m_dir = dir;

		SendMyData.m_State = DATA_MOVE;
		SendMyData.m_X = m_x;;
		SendMyData.m_Y = m_y;
		SendMyData.m_Dir = m_dir;
		SendMyData.m_Type = 0;
		SendMyData.m_Id = m_tankid;
		if (LinkType == TCP_LINK_SERVER)MySever.AddData(SendMyData);
		else if (LinkType == TCP_LINK_CLIENT)MyClient.AddData(SendMyData);
	}

	void SubHp(int hp)
	{
		m_hp = m_hp-hp;
	}
	int GetHp()
	{
		return m_hp;
	}
	void SetHp(int hp)
	{
		m_hp=hp;
	}
	void SubMp(int mp)
	{
		m_mp = m_mp - mp;
	}
	int GetMp()
	{
		return m_mp;
	}
	void SetMp(int mp)
	{
		m_mp = mp;
	}

	int GetMaxHp()
	{
		return m_maxhp;
	}
	void SetMaxHp(int mp)
	{
		m_maxhp =  mp;
	}
	int GetMaxMp()
	{
		return m_maxmp;
	}
	void SetMaxMp(int mp)
	{
		m_maxmp = mp;
	}

	void AddRank()
	{
		m_rank = m_rank + 1;
	}
	int GetRank()
	{
		return m_rank;
	}
	void SetRank(int rank)
	{
		m_rank = rank;
	}
	int GetTankId()
	{
		return m_tankid;
	}
	void SetTankId(int tankid)
	{
		m_tankid = tankid;
	}

	int GetAttack()
	{
		return m_attack;
	}
	void SetAttack(int attcak)
	{
		m_attack = attcak;
	}
	int GetColor()
	{
		return m_color;
	}
	void SetColor(int color)
	{
		m_color=color;
	}
	int GetSpeed()
	{
		return m_speed;
	}
	void SetSpeed(int speed)
	{
		m_speed = speed;
	}
	int GetBulletCd()
	{
		return m_bulletcd;
	}
	int SetBulletCd(int bulletcd)
	{
		bulletcd = bulletcd;
	}

	void SetAttackBuffer()
	{
		m_attack_time = clock();
	}
	void SetBulletCdBuffer()
	{
		m_bullercd_time = clock();
	}
	void SetNoHurtBuffer()
	{	//记录当前时间
		m_nohurt_time = clock();
	}
	void SetStaticAll()
	{
		m_staticall_time = clock();
	}
	void SetKillAll()
	{
		m_killall_time = clock();
	}
	void SetAi(int type)
	{
		m_Ai_time = clock();
		m_Ai_type = type;
		pPointXY.clear();
		vector <POINTXY>().swap(pPointXY);
	}
	int GetAiType()
	{
		return m_Ai_type;
	}

	void IsAttackBuffer()
	{
		if (clock() <= m_attack_buff)m_attack = 50;
		else if (clock() - m_attack_time > m_attack_buff)m_attack = 1;
		else m_attack = 2;
	}
	bool IsBulletCdBuffer()
	{
		if (clock() <= m_bullercd_buff)return false;
		else if (clock() - m_bullercd_time > m_bullercd_buff)return false;
		return true;
	}
	bool IsNoHurtBuffer()
	{
		m_tank_data = "■";//坦克普通状态的实心正方形
		//如果现在时间小于设置时间则返回false
		if (clock() <= m_nohurt_buff)return false;
		//如果现在时间减去设置无敌的时间，大于10秒钟，返回false
		else if (clock() - m_nohurt_time > m_nohurt_buff)return false;
		m_tank_data = "□";//否则修改为空心正方形表示无敌
		return true;//返回true
	}
	bool IsStaticAllBuffer()
	{
		if (clock() <= m_staticall_buff)return false;
		else if (clock() - m_staticall_time > m_staticall_buff)return false;
		return true;
	}
	bool IsKillAllBuffer()
	{
		if (clock() <= m_killall_buff)return false;
		else if (clock() - m_killall_time > m_killall_buff)return false;
		return true;
	}
	bool IsAi()
	{
		if (clock() <= m_Ai)return false;
		else if (clock() - m_Ai_time > m_Ai)
		{
			m_speed = TANK_PC5_SPEED;
			m_bulletcd = TANK_PC5_BULLETCD;
			return false;
		}
		m_speed = LinkType == TCP_LINK_NULL ? TANK_PC5_SPEED : TANK_PC5_SPEED - 1;
		m_bulletcd = LinkType == TCP_LINK_NULL ? TANK_PC5_BULLETCD : 100;
		return true;
	}

	bool IsDie()
	{
		return m_hp <= 0 ? true : false;
	}
	void RevHp()
	{
		if (clock()- m_hp_time<=3000)return;
		if (m_hp >= m_maxhp)return;
		m_hp_time = clock();
		m_hp = m_hp + (m_hp>=m_maxhp ? 0 :  m_revhp);
	}
	void RevMp()
	{
		if (clock() - m_mp_time <= 1000)return;
		if (m_mp >= m_maxmp)return;
		m_mp_time = clock();
		m_mp = m_mp + (m_mp>=m_maxmp ? 0 :  m_revmp);
		m_mp = m_mp > 100 ? m_maxmp : m_mp;
	}

	void MoveTank()
	{
		if (m_speed_time++ < m_speed)return;
		m_speed_time = 1;

		CleanTank();
		if (m_dir == DIR_UP)
		{
			int nObstacleType = IsTouchObstacle(m_x, m_y - 2, DIR_UP);
			if (nObstacleType != OBSTACLE_NULL && nObstacleType != OBSTACLE_GRASS)return;
			m_y--;
		}
		else if (m_dir == DIR_DOWN)
		{
			int nObstacleType = IsTouchObstacle(m_x, m_y + 2, DIR_DOWN);
			if (nObstacleType != OBSTACLE_NULL && nObstacleType != OBSTACLE_GRASS)return;
			m_y++;
		}
		else if (m_dir == DIR_LEFT)
		{
			int nObstacleType = IsTouchObstacle(m_x - 2, m_y, DIR_LEFT);
			if (nObstacleType != OBSTACLE_NULL && nObstacleType != OBSTACLE_GRASS)return;

			m_x--;
		}
		else if (m_dir == DIR_RIGHT)
		{
			int nObstacleType = IsTouchObstacle(m_x + 2, m_y, DIR_RIGHT);
			if (nObstacleType != OBSTACLE_NULL && nObstacleType != OBSTACLE_GRASS)return;
			m_x++;
		}

		SendMyData.m_State = DATA_MOVE;//数据包类型为：移动
		SendMyData.m_X = m_x;//传入坐标
		SendMyData.m_Y = m_y;
		SendMyData.m_Dir = m_dir;//传入方向
		SendMyData.m_Id = m_tankid;//传入移动的坦克ID
		//将结构体内的数据加入待发送区
		if (LinkType == TCP_LINK_SERVER)MySever.AddData(SendMyData);
		else if (LinkType == TCP_LINK_CLIENT)MyClient.AddData(SendMyData);

		//PrintTank();
	}
	void MoveTankLink(int x,int y,int dir)
	{
		CleanTank();

		m_x = x;
		m_y = y;
		m_dir = dir;

		if (m_dir == DIR_UP)
			IsTouchObstacle(m_x, m_y + 2, DIR_UP);
		else if (m_dir == DIR_DOWN)
			IsTouchObstacle(m_x, m_y - 2, DIR_DOWN);
		else if (m_dir == DIR_LEFT)
			IsTouchObstacle(m_x + 2, m_y, DIR_LEFT);
		else if (m_dir == DIR_RIGHT)
			IsTouchObstacle(m_x - 2, m_y, DIR_RIGHT);

	}

	void HideTank(int *color)
	{
		int arryx[9] = { m_x - 1 ,m_x ,m_x + 1 ,m_x - 1 ,m_x ,m_x + 1 ,m_x - 1 ,m_x ,m_x + 1 };
		int arryy[9] = { m_y - 1 ,m_y - 1 ,m_y - 1,m_y,m_y,m_y,  m_y + 1 , m_y + 1 , m_y + 1 };

		for (auto &val : pObstacle)
		{
			if (val.m_type == OBSTACLE_GRASS)
			{
				for (int i=0;i<9;i++)
				{
					if (arryx[i]==val.m_x && arryy[i] == val.m_y)
					{
						color[i] = 2;
						break;
					}
				}

			}
		}
	}
	void RefreshObstacle(int *x, int *y,int num)
	{
		for (auto &val : pObstacle) {
			if (val.m_type == OBSTACLE_GRASS)
				for (int i=0;i<num;i++)
				{
					if (val.m_x == x[i] && val.m_y == y[i])
					{
						PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
						break;
					}
				}

		}
	}
	void PrintTank()
	{
		IsAttackBuffer();
		IsNoHurtBuffer();

		if (m_maxhp == TANK_PC5_HP) m_color = IsAi() ? GetRandNum(3, 0xf) : 0x9;

		int ncolor[9] = { m_color,m_color ,m_color ,m_color ,m_color ,m_color ,m_color ,m_color ,m_color };
		int nX[3] = {};
		int nY[3] = {};

		sprintf_s(m_tank_hp_data,_countof(m_tank_hp_data), "%2d", m_hp);



		if (m_dir == DIR_UP)
		{

			HideTank( ncolor);
			PrintPoint(m_x - 1, m_y - 1, ncolor[0] == 2 ? "△" : "  ", ncolor[0]);
			PrintPoint(m_x, m_y - 1, ncolor[1] == 2 ? "□" : m_tank_data, ncolor[1]);
			PrintPoint(m_x + 1, m_y - 1, ncolor[2] == 2 ? "△" : "  ", ncolor[2] );


			PrintPoint(m_x - 1, m_y, ncolor[3] == 2 ? "□" : m_tank_data, ncolor[3]);
			PrintPoint(m_x, m_y, ncolor[4] == 2 ? "□" : m_tank_hp_data, ncolor[4]);
			PrintPoint(m_x + 1, m_y, ncolor[5] == 2 ? "□" : m_tank_data, ncolor[5]);


			PrintPoint(m_x - 1, m_y + 1, ncolor[6] == 2 ? "□" : m_tank_data, ncolor[6]);
			PrintPoint(m_x, m_y + 1, ncolor[7] == 2 ? "△" : "  ", ncolor[7]);
			PrintPoint(m_x + 1, m_y + 1, ncolor[8] == 2 ? "□" : m_tank_data, ncolor[8]);

			nX[0] = m_x - 1;
			nX[1] = m_x ;
			nX[2] = m_x + 1;
			nY[0] = m_y + 2;
			nY[1] = m_y + 2;
			nY[2] = m_y + 2;
			RefreshObstacle(nX, nY,_countof(nX));//刷新被擦除的Obstacle


		}
		else if (m_dir == DIR_DOWN)
		{
			HideTank( ncolor);

			PrintPoint(m_x - 1, m_y - 1, ncolor[0] == 2 ? "□" : m_tank_data, ncolor[0]);
			PrintPoint(m_x, m_y - 1, ncolor[1] == 2 ? "△" : "  ", ncolor[1]);
			PrintPoint(m_x + 1, m_y - 1, ncolor[2] == 2 ? "□" : m_tank_data, ncolor[2]);

			PrintPoint(m_x - 1, m_y, ncolor[3] == 2 ? "□" : m_tank_data, ncolor[3]);
			PrintPoint(m_x, m_y, ncolor[4] == 2 ? "□" : m_tank_hp_data, ncolor[4]);
			PrintPoint(m_x + 1, m_y, ncolor[5] == 2 ? "□" : m_tank_data, ncolor[5]);

			PrintPoint(m_x - 1, m_y + 1, ncolor[6] == 2 ? "△" : "  ", ncolor[6]);
			PrintPoint(m_x, m_y + 1, ncolor[7] == 2 ? "□" : m_tank_data, ncolor[7]);
			PrintPoint(m_x + 1, m_y + 1, ncolor[8] == 2 ? "△" : "  ", ncolor[8]);

			nX[0] = m_x - 1;
			nX[1] = m_x;
			nX[2] = m_x + 1;
			nY[0] = m_y - 2;
			nY[1] = m_y - 2;
			nY[2] = m_y - 2; 
			RefreshObstacle(nX, nY, _countof(nX));//刷新被擦除的Obstacle

		}
		else if (m_dir == DIR_LEFT)
		{
			HideTank( ncolor);
			PrintPoint(m_x - 1, m_y - 1, ncolor[0] == 2 ? "△" : "  ", ncolor[0]);
			PrintPoint(m_x, m_y - 1, ncolor[1] == 2 ? "□" : m_tank_data, ncolor[1]);
			PrintPoint(m_x + 1, m_y - 1, ncolor[2] == 2 ? "□" : m_tank_data, ncolor[2]);

			PrintPoint(m_x - 1, m_y, ncolor[3] == 2 ? "□" : m_tank_data, ncolor[3]);
			PrintPoint(m_x, m_y, ncolor[4] == 2 ? "□" : m_tank_hp_data, ncolor[4]);
			PrintPoint(m_x + 1, m_y, ncolor[5] == 2 ? "△" : "  ", ncolor[5]);

			PrintPoint(m_x - 1, m_y + 1, ncolor[6] == 2 ? "△" : "  ", ncolor[6]);
			PrintPoint(m_x, m_y + 1, ncolor[7] == 2 ? "□" : m_tank_data, ncolor[7]);
			PrintPoint(m_x + 1, m_y + 1, ncolor[8] == 2 ? "□" : m_tank_data, ncolor[8]);

			nX[0] = m_x +2;
			nX[1] = m_x + 2;
			nX[2] = m_x + 2;
			nY[0] = m_y - 1;
			nY[1] = m_y ;
			nY[2] = m_y +1;
			RefreshObstacle(nX, nY, _countof(nX));//刷新被擦除的Obstacle

		}
		else if (m_dir == DIR_RIGHT)
		{
			HideTank( ncolor);
			PrintPoint(m_x - 1, m_y - 1, ncolor[0] == 2 ? "□" : m_tank_data, ncolor[0]);
			PrintPoint(m_x, m_y - 1, ncolor[1] == 2 ? "□" : m_tank_data, ncolor[1]);
			PrintPoint(m_x + 1, m_y - 1, ncolor[2] == 2 ? "△" : "  ", ncolor[2]);

			PrintPoint(m_x - 1, m_y, ncolor[3] == 2 ? "△" : "  ", ncolor[3]);
			PrintPoint(m_x, m_y, ncolor[4] == 2 ? "□" : m_tank_hp_data, ncolor[4]);
			PrintPoint(m_x + 1, m_y, ncolor[5] == 2 ? "□" : m_tank_data, ncolor[5]);

			PrintPoint(m_x - 1, m_y + 1, ncolor[6] == 2 ? "□" : m_tank_data, ncolor[6]);
			PrintPoint(m_x, m_y + 1, ncolor[7] == 2 ? "□" : m_tank_data, ncolor[7]);
			PrintPoint(m_x + 1, m_y + 1, ncolor[8] == 2 ? "△" : "  ", ncolor[8]);

			nX[0] = m_x - 2;
			nX[1] = m_x - 2;
			nX[2] = m_x - 2;
			nY[0] = m_y - 1;
			nY[1] = m_y;
			nY[2] = m_y + 1;
			RefreshObstacle(nX, nY, _countof(nX));//刷新被擦除的Obstacle

		}
	}
	void CleanTank()
	{
		PrintPoint(m_x - 1, m_y - 1, "  ", 11);
		PrintPoint(m_x, m_y - 1, "  ", 11);
		PrintPoint(m_x + 1, m_y - 1, "  ", 11);

		PrintPoint(m_x - 1, m_y, "  ", 11);
		PrintPoint(m_x, m_y, "  ", 11);
		PrintPoint(m_x + 1, m_y, "  ", 11);

		PrintPoint(m_x - 1, m_y + 1, "  ", 11);
		PrintPoint(m_x, m_y + 1, "  ", 11);
		PrintPoint(m_x + 1, m_y + 1, "  ", 11);

	}

	bool IsLaunchBullet(int mp)
	{
		if(m_mp<mp)return false;

		if (clock() -m_bullet_time>= m_bulletcd)
		{
			m_bullet_time = clock();
			return true;
		}
		return false;
	}
	int IsTouchObstacle(int x, int y, int dir)
	{
		for (auto &val : pObstacle) 
		{ 
			switch (dir)
			{
			case DIR_UP:case DIR_DOWN:
				if (y == val.m_y && x == val.m_x || y == val.m_y && x == val.m_x - 1 || y == val.m_y  && x == val.m_x + 1)return val.m_type;;
				break;
			case DIR_LEFT:case DIR_RIGHT:
				if (y == val.m_y && x == val.m_x || y == val.m_y-1 && x == val.m_x || y == val.m_y+1  && x == val.m_x )return val.m_type;;
				break;
			}
		}
		return OBSTACLE_NULL;
	}
	bool IsTouchBullet(int x, int y,int type)
	{
		for (auto &val : pBullet) 
		{ 
			if (val.m_x >= x - 1 && val.m_x <= x + 1 && val.m_y >= y - 1 && val.m_y <= y + 1 && val.m_type == type)
			{
				val.m_type = BULLET_CLEAN;
				return true;
			}
		}
		return false;
	}
	bool IsTouchPcTank(int x, int y, int dir, int tankid)
	{
		for (auto &val : pPcTank)
		{
			switch (dir)
			{
			case DIR_UP:
				if (y - 2 == val.m_y + 1 && x + 1 >= val.m_x - 1 && x - 1 <= val.m_x + 1 && tankid!= val.GetTankId())return true;
				break;
			case DIR_DOWN:
				if (y + 2 == val.m_y - 1 && x + 1 >= val.m_x - 1 && x - 1 <= val.m_x + 1 && tankid != val.GetTankId())return true;
				break;
			case DIR_LEFT:
				if (x - 2 == val.m_x + 1 && y - 1 <= val.m_y + 1 && y + 1 >= val.m_y - 1 && tankid != val.GetTankId())return true;
				break;
			case DIR_RIGHT:
				if (x + 2 == val.m_x - 1 && y - 1 <= val.m_y + 1 && y + 1 >= val.m_y - 1 && tankid != val.GetTankId())return true;
				break;
			}
		}
		return false;
	}
	bool IsTouchMyTank(Tank &MyTank, int x, int y, int dir)
	{
		switch (dir)
		{
		case DIR_UP:
			if (y - 2 == MyTank.m_y + 1 && x + 1 >= MyTank.m_x - 1 && x - 1 <= MyTank.m_x + 1)return true;
			break;
		case DIR_DOWN:
			if (y + 2 == MyTank.m_y - 1 && x + 1 >= MyTank.m_x - 1 && x - 1 <= MyTank.m_x + 1)return true;
			break;
		case DIR_LEFT:
			if (x - 2 == MyTank.m_x + 1 && y - 1 <= MyTank.m_y + 1 && y + 1 >= MyTank.m_y - 1)return true;
			break;
		case DIR_RIGHT:
			if (x + 2 == MyTank.m_x - 1 && y - 1 <= MyTank.m_y + 1 && y + 1 >= MyTank.m_y - 1)return true;
			break;
		}
		return false;
	}

	//NewA*=================================================================
	int MoveTankAi(int endx, int endy)
	{
		if (m_speed_time++ < m_speed)return MOVE_CD;
		m_speed_time = 1;



		if (pPointXY.size() <= 0 && IsAi())
		{
			NewRun({ m_x,m_y }, { endx,endy });

			if (pPointXY.size() <= 6) //距离小于6点，不移动
			{
				pPointXY.clear();
				vector <POINTXY>().swap(pPointXY);
				return MOVE_NO;
			}

			pPointXY.pop_back();//去除自身起点坐标
				
			if (pPointXY.size()>=6)//去除距离目标6点坐标，保持距离
			{
				pPointXY.erase(pPointXY.begin());
				pPointXY.erase(pPointXY.begin());
				pPointXY.erase(pPointXY.begin());
				pPointXY.erase(pPointXY.begin());
				pPointXY.erase(pPointXY.begin());
			}
		}

		if (pPointXY.size() > 0)//如果有路线则改变方向
		{
			int n_x = pPointXY[pPointXY.size() - 1].m_x;
			int n_y = pPointXY[pPointXY.size() - 1].m_y;

			if (n_x > m_x)
				m_dir=(DIR_RIGHT);
			else if (n_x < m_x)
				m_dir=(DIR_LEFT);
			else if (n_y < m_y)
				m_dir=(DIR_UP);
			else if (n_y > m_y)
				m_dir=(DIR_DOWN);

			pPointXY.erase(pPointXY.end() - 1);
		}


		CleanTank();//擦除坦克开始移动
		if (m_dir == DIR_UP)
		{
			m_y--;
		}
		if (m_dir == DIR_DOWN)
		{
			m_y++;
		}
		if (m_dir == DIR_LEFT)
		{
			m_x--;
		}
		if (m_dir == DIR_RIGHT)
		{
			m_x++;
		}

		SendMyData.m_State = DATA_MOVE;
		SendMyData.m_X = m_x;;
		SendMyData.m_Y = m_y;
		SendMyData.m_Dir = m_dir;
		SendMyData.m_Type = 0;
		SendMyData.m_Id = m_tankid;
		if (LinkType == TCP_LINK_SERVER)MySever.AddData(SendMyData);
		return MOVE_OK;

	}
	bool NewRun(POINTXY start, POINTXY end)
	{
		POINTXY NowPoint;
		vector <POINTSSS> pOpenPoint;//打开列表
		vector <POINTSSS> pClosePoint;//关闭列表
		POINTSSS pPointA = {};
		POINTSSS pPointT = {};
		int nMinF = 0;
		int nMinNum = 0;
		int nG = 0;
		bool bOpen = false;
		bool bClose = false;

		pPointT.thisX = start.m_x;
		pPointT.thisY = start.m_y;


		pOpenPoint.push_back(POINTSSS{ pPointT.nMinF, pPointT.nG,pPointT.nH, pPointT.thisX,pPointT.thisY,pPointT.lastX,pPointT.lastY });

		do 
		{
			nMinF = 0;
			for (int i=0; (DWORD)i<pOpenPoint.size();i++)
			{
				if(nMinF == 0)
				{
					nMinF = pOpenPoint[0].nMinF;
					nMinNum =i;
				}
				if (pOpenPoint[i].nMinF<nMinF)
				{
					nMinF = pOpenPoint[i].nMinF;
					nMinNum = i;
				}
			}


			memcpy(&pPointT, &pOpenPoint[nMinNum], sizeof(POINTSSS));
			pOpenPoint.erase(pOpenPoint.begin() + nMinNum);
			pClosePoint.push_back(POINTSSS{ pPointT.nMinF, pPointT.nG,pPointT.nH, pPointT.thisX,pPointT.thisY,pPointT.lastX,pPointT.lastY });



			for (DWORD i=0;i<pClosePoint.size();i++)
			{
				if (pClosePoint[i].thisX==end.m_x &&pClosePoint[i].thisY==end.m_y)
				{
					memcpy(&pPointA, &pClosePoint[i], sizeof(POINTSSS));
					//pClosePoint.push_back(&pPointA);

					do 
					{
						for (DWORD i2=0; i2<pClosePoint.size();i2++)
						{
							if (pClosePoint[i2].thisX== pPointA.lastX && pClosePoint[i2].thisY == pPointA.lastY)
							{
								memcpy(&pPointA, &pClosePoint[i2], sizeof(POINTSSS));
							}
						}

						if (pPointA.thisX != start.m_y || pPointA.thisY != start.m_y)
						{
							//AddPoints(pPointA.thisX ,pPointA.thisY );
							pPointXY.push_back( POINTXY{ pPointA.thisX,pPointA.thisY });

						}

					} while (pPointA.thisX!=start.m_x || pPointA.thisY != start.m_y);

					return true;
				}
			}



			for (int i=0;i<4;i++)
			{
				switch (i)
				{
				case 0:
					NowPoint.m_x = pPointT.thisX;
					NowPoint.m_y = pPointT.thisY-1;
					break;
				case 1:
					NowPoint.m_x = pPointT.thisX-1;
					NowPoint.m_y = pPointT.thisY;
					break;
				case 2:
					NowPoint.m_x = pPointT.thisX+1;
					NowPoint.m_y = pPointT.thisY;
					break;
				case 3:
					NowPoint.m_x = pPointT.thisX;
					NowPoint.m_y = pPointT.thisY+1;
					break;
				}

				if (NowPoint.m_x <= 0 || NowPoint.m_x > WALL_WIDE - 3 || NowPoint.m_y <= 0 || NowPoint.m_y > WALL_HIGH - 3)continue;
				if (!IsFree(NowPoint))continue;
				if (NowPoint.m_x!= pPointT.thisX && NowPoint.m_y != pPointT.thisY){}

				bClose = false;

				for (DWORD i2=0;i2<pClosePoint.size();i2++)
				{
					if (pClosePoint[i2].thisX== NowPoint.m_x && pClosePoint[i2].thisY == NowPoint.m_y)
					{
						bClose = true;
						continue;
					}
				}

				if(bClose)continue;

				bOpen = false;

				for (DWORD i2 = 0; i2 < pOpenPoint.size(); i2++)
				{
					if (pOpenPoint[i2].thisX == NowPoint.m_x && pOpenPoint[i2].thisY == NowPoint.m_y)
					{
						if (NowPoint.m_x != pPointT.thisX || NowPoint.m_y != pPointT.thisY)nG = 14;
						else  nG = 10; 


						if (pPointT.nG + nG < pOpenPoint[i2].nG)
						{
							pOpenPoint[i2].lastX = pPointT.thisX;
							pOpenPoint[i2].lastY = pPointT.thisY;
						}
						bOpen = true;
						continue;
					}

				}

				if (!bOpen)
				{
					if (NowPoint.m_x == pPointT.thisX || NowPoint.m_y == pPointT.thisY)nG = 10;
					else  nG = 14;
				}

				pPointA.nG = pPointT.nG + nG;
				pPointA.nH = abs(end.m_x - NowPoint.m_x) * 10 + abs(end.m_y - NowPoint.m_y) * 10;
				pPointA.nMinF = pPointA.nG + pPointA.nH;
				pPointA.thisX = NowPoint.m_x;
				pPointA.thisY = NowPoint.m_y;
				pPointA.lastX = pPointT.thisX;
				pPointA.lastY = pPointT.thisY;
				
				pOpenPoint.push_back(pPointA);

			}

		} while (pOpenPoint.size()!=0);
		return false;
	}
	void AddPoints(int x,int y)
	{
		pPointXY.push_back(POINTXY{ x,y });
	}
	bool IsFree(POINTXY &PD)
	{
		int x = PD.m_x;
		int y = PD.m_y;
		for (auto &val : pObstacle) 
		{ 
			if (val.m_x == x + 1 && val.m_y == y)
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}
			else if (val.m_x == x && val.m_y == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)
				return false;
			}
			else if (val.m_x == x - 1 && val.m_y == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}
			else if (val.m_x == x && val.m_y - 1 == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}
			else if (val.m_x == x && val.m_y == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}
			else if (val.m_x == x && val.m_y + 1 == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}

			else if (val.m_x -1  == x && val.m_y - 1 == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}
			else if (val.m_x - 1 == x && val.m_y + 1 == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}

			else if (val.m_x + 1 == x && val.m_y - 1 == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}
			else if (val.m_x + 1 == x && val.m_y + 1 == y) 
			{
				if (val.m_type == OBSTACLE_GRASS)return true;
				return false;
			}
		}
		return true;
	}
};
class AutoTank
{
	Tank &MyTank;
	Tank &MyTank2;
	Bullet &MyBullet;
	int m_tanknum = 1;
	bool m_creatboss = false;

	int m_tankcd_time = 3000;//生成新坦克的cd
	int m_addtank_time;
	vector <POINTDATA> pPcTankTeam;//当前需要生成的坦克信息

public:
	AutoTank(Tank &tank, Tank &tank2, Bullet &bullet,bool loading) : MyTank(tank), MyTank2(tank2), MyBullet(bullet), m_addtank_time(clock())
	{ 
		if (LinkType == TCP_LINK_CLIENT)return;//如果为联机游戏客户端，不初始化生成坦克列表

		pPcTankTeam.assign(pPcTankConfig.begin(), pPcTankConfig.end());//vector赋值
		
		if (IsHaveBossTank() != -1)m_tanknum = pPcTankTeam.size() - 1;//如果有boos就减去1，给boos预留1个位置
		else m_tanknum = pPcTankTeam.size();

		if (loading)return;

	}
	AutoTank(Tank &tank, Tank &tank2, Bullet &bullet, int tanknum) :MyTank(tank), MyTank2(tank2), MyBullet(bullet), m_tanknum(tanknum){}

	void GetRandNature(int &x, int &y, int &dir)
	{
		do
		{
			x = GetRandNum(2, WALL_WIDE - 10);
			y = GetRandNum(2, WALL_HIGH - 10);
			dir = GetRandNum(DIR_UP + 1, DIR_RIGHT + 1) - 1;
		} while (Tank().IsTouchPcTank(x, y, dir, 0) || Tank().IsTouchMyTank(MyTank, x, y, dir) || Tank().IsTouchObstacle(x, y, dir)!=OBSTACLE_NULL);
	}
	bool IsObstacleNull(int x, int y)
	{

		if (x >= MyTank.GetTankX() - 2 && x <= MyTank.GetTankX() + 2 && y >= MyTank.GetTankY() - 2 && y <= MyTank.GetTankY() + 2)return false;
		else if(x >= MyTank2.GetTankX() - 2 && x <= MyTank2.GetTankX() + 2 && y >= MyTank2.GetTankY() - 2 && y <= MyTank2.GetTankY() + 2)return false;
		else
		{
			for (auto &val:pPcTank)
			{
				if (x >= val.GetTankX() - 2 && x <= val.GetTankX() + 2 && y >= val.GetTankY() - 2 && y <= val.GetTankY() + 2)return false;
			}
		}
		return true;
	}
	int IsHaveBossTank()
	{
		int i = 0;
		for (i = 0; (DWORD)i < pPcTankTeam.size();i++)
		{ if (pPcTankTeam[i].m_type == TANK_PC5)
			return i; }
		return -1;
	}
	bool IsBossTankDie()
	{
		if (m_creatboss == false)return false;
		for (auto &val: pPcTank)if (val.GetMaxHp() == TANK_PC5_HP)return false;
		return true;
	}
	void CreatTank(int x, int y, int color, int dir, int m_speed, int bulletcd, int hp, int mp, int revhp, int revmp,int tankid,int maxhp,int maxmp)
	{//创造特定属性坦克
		pPcTank.push_back(Tank(x, y, color, dir, m_speed, bulletcd, hp, mp, revhp, revmp,tankid, maxhp, maxmp));
	}
	void CreatRandTank()
	{
		if (clock() - m_addtank_time <= m_tankcd_time)return;
		m_addtank_time = clock();


		//创造随机坦克
		int nTankNum = m_creatboss ? pPcTank.size() - 1 : pPcTank.size();
		if (nTankNum < m_tanknum)//坦克数量是否达到
		{
			int nRandTank = 0;
			int nSpeed = 0;
			int nBulletcd = 0;
			int nHp = 0;
			int nTankId = GetRandTankId();
			do
			{
				do 
				{
					nRandTank = GetRandNum(1, m_tanknum) - 1;
				} while  (pPcTankTeam[nRandTank].m_type == TANK_PC5);
				
				switch (pPcTankTeam[nRandTank].m_type)//判断要生成的坦克类型
				{
				case TANK_PC1:
					nSpeed = TANK_PC1_SPEED;
					nBulletcd = TANK_PC1_BULLETCD;
					nHp = TANK_PC1_HP;
					break;
				case TANK_PC2:
					nSpeed = TANK_PC2_SPEED;
					nBulletcd = TANK_PC2_BULLETCD;
					nHp = TANK_PC2_HP;
					break;
				case TANK_PC3:
					nSpeed = TANK_PC3_SPEED;
					nBulletcd = TANK_PC3_BULLETCD;
					nHp = TANK_PC3_HP;
					break;
				case TANK_PC4:
					nSpeed = TANK_PC4_SPEED;
					nBulletcd = TANK_PC4_BULLETCD;
					nHp = TANK_PC4_HP;
					break;
				case TANK_PC5:
					nSpeed = TANK_PC5_SPEED;
					nBulletcd = TANK_PC5_BULLETCD;
					nHp = TANK_PC5_HP;
					break;
				}
				//生成的点是否有障碍物，其他坦克阻挡？有的话就返回，等待3秒后再判断生成
				if (!IsObstacleNull(pPcTankTeam[nRandTank].m_x, pPcTankTeam[nRandTank].m_y))return;

			} while (!IsObstacleNull(pPcTankTeam[nRandTank].m_x, pPcTankTeam[nRandTank].m_y));

			//创造一辆新的坦克
			CreatTank(pPcTankTeam[nRandTank].m_x, pPcTankTeam[nRandTank].m_y, pPcTankTeam[nRandTank].m_color, pPcTankTeam[nRandTank].m_dir, nSpeed, nBulletcd, nHp, 100, 1, 1,nTankId, nHp,100);
		
			if (LinkType == TCP_LINK_SERVER)
			{
				SendMyData.m_State = DATA_CREATTANK;
				SendMyData.m_X = pPcTankTeam[nRandTank].m_x;;
				SendMyData.m_Y = pPcTankTeam[nRandTank].m_y;
				SendMyData.m_Dir = pPcTankTeam[nRandTank].m_dir;
				SendMyData.m_Type = pPcTankTeam[nRandTank].m_type;
				SendMyData.m_Id = nTankId;
				MySever.AddData(SendMyData);
			}
		
		}
		else if (m_creatboss == false)CreatBossTank();

	}
	void CreatBossTank()
	{
		if (m_creatboss)return;//已经创造了boss就返回
		int nBossTankWeak = IsHaveBossTank();//是否有需要创建boss信息
		if (IsHaveBossTank() == -1) { m_creatboss = true; return; }//如果没有需要创建boss信息，就返回

		int nSpeed = TANK_PC5_SPEED;
		int nBulletcd = TANK_PC5_BULLETCD;
		int nHp = TANK_PC5_HP;
		int nTankId = GetRandTankId();
		m_creatboss = true;
		//x,y,color,dir,speed,bulletcd,hp,mp,revhp,revmp
		CreatTank(pPcTankTeam[nBossTankWeak].m_x, pPcTankTeam[nBossTankWeak].m_y,
			pPcTankTeam[nBossTankWeak].m_color, pPcTankTeam[nBossTankWeak].m_dir, nSpeed, nBulletcd, nHp, 100, 1, 1, nTankId, nHp, 100);

		if (LinkType == TCP_LINK_SERVER)
		{
			SendMyData.m_State = DATA_CREATTANK;
			SendMyData.m_X = pPcTankTeam[nBossTankWeak].m_x;;
			SendMyData.m_Y = pPcTankTeam[nBossTankWeak].m_y;
			SendMyData.m_Dir = pPcTankTeam[nBossTankWeak].m_dir;
			SendMyData.m_Type = TANK_PC5;
			SendMyData.m_Id = nTankId;
			MySever.AddData(SendMyData);
		}
	}
	void ClearAllTank(int tanktype)
	{
		for (int i = pPcTank.size() - 1; i >= 0; i--)
		{
			if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)continue;//全屏秒杀对boss无效

			pPcTank[i].CleanTank();

			int tX = pPcTank[i].GetTankX();
			int tY = pPcTank[i].GetTankY();

			int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
			int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
			pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));//刷新草丛

			pPcTank.erase(pPcTank.begin() + i);
			if (tanktype == 0)MyTank.AddRank();
			else MyTank2.AddRank();
		}
		m_addtank_time = clock() - 5000;
	}
	
	//客户端接收服务端数据新建坦克
	void CreatRandTankLink(int x,int y,int dir,int type,int tankid)
	{
		int nSpeed = 0;
		int nBulletcd = 0;
		int nHp = 0;
		int nColor = 0;
				switch (type)
				{
				case TANK_PC1:
					nSpeed = TANK_PC1_SPEED;
					nBulletcd = TANK_PC1_BULLETCD;
					nHp = TANK_PC1_HP;
					nColor = 0xf;
					break;
				case TANK_PC2:
					nSpeed = TANK_PC2_SPEED;
					nBulletcd = TANK_PC2_BULLETCD;
					nHp = TANK_PC2_HP;
					nColor = 0xe;
					break;
				case TANK_PC3:
					nSpeed = TANK_PC3_SPEED;
					nBulletcd = TANK_PC3_BULLETCD;
					nHp = TANK_PC3_HP;
					nColor = 0xd;
					break;
				case TANK_PC4:
					nSpeed = TANK_PC4_SPEED;
					nBulletcd = TANK_PC4_BULLETCD;
					nHp = TANK_PC4_HP;
					nColor = 0xc;
					break;
				case TANK_PC5:
					nSpeed = TANK_PC5_SPEED;
					nBulletcd = TANK_PC5_BULLETCD;
					nHp = TANK_PC5_HP;
					nColor = 0x9;
					break;
				}

			CreatTank(x, y, nColor, dir, nSpeed, nBulletcd, nHp, 100, 1, 1, tankid, nHp,100);

	}
	//客户端接收服务端数据强制同步刷新坦克
	void RefreshTankLink(int x, int y, int dir, int type, int tankid, int hp,int maxhp)
	{
		int nSpeed = 0;
		int nBulletcd = 0;
		int nHp = 0;
		int nColor = 0;
		switch (type)
		{
		case TANK_PC1:
			nSpeed = TANK_PC1_SPEED;
			nBulletcd = TANK_PC1_BULLETCD;
			nHp = hp;
			nColor = 0xf;
			break;
		case TANK_PC2:
			nSpeed = TANK_PC2_SPEED;
			nBulletcd = TANK_PC2_BULLETCD;
			nHp = hp;
			nColor = 0xe;
			break;
		case TANK_PC3:
			nSpeed = TANK_PC3_SPEED;
			nBulletcd = TANK_PC3_BULLETCD;
			nHp = hp;
			nColor = 0xd;
			break;
		case TANK_PC4:
			nSpeed = TANK_PC4_SPEED;
			nBulletcd = TANK_PC4_BULLETCD;
			nHp = hp;
			nColor = 0xc;
			break;
		case TANK_PC5:
			nSpeed = TANK_PC5_SPEED;
			nBulletcd = TANK_PC5_BULLETCD;
			nHp = hp;
			nColor = 0x9;
			break;
		}

		CreatTank(x, y, nColor, dir, nSpeed, nBulletcd, nHp, 100, 1, 1, tankid, maxhp,100);
	}
	//客户端接收服务端数据清空所有坦克
	void ClearAllTankLink()
	{
		for (int i = pPcTank.size() - 1; i >= 0; i--)
		{
			pPcTank[i].CleanTank();

			int tX = pPcTank[i].GetTankX();
			int tY = pPcTank[i].GetTankY();

			int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
			int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
			pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));//刷新草丛

			pPcTank.erase(pPcTank.begin() + i);
		}
		m_addtank_time = clock() - 5000;
	}

	//负责所有PC坦克的行为
	void MoveTank()
	{//负责所有PC坦克的行为

		if (MyTank.IsKillAllBuffer()) { ClearAllTank(0); return; }//如果PlayerA吃了全屏秒杀buff就清空全部坦克
		if (MyTank2.IsKillAllBuffer()) { ClearAllTank(1); return; }//如果PlayerB吃了全屏秒杀buff就清空全部坦克

		for (int i = pPcTank.size() - 1; i >= 0; i--)
		{	//如果吃了冰冻buff就跳过移动操作
			if (MyTank.IsStaticAllBuffer()|| MyTank2.IsStaticAllBuffer())goto DontMove;
			//生成PC坦克随机行为
			int nGet = GetRandNum(ACTION_DIR, ACTION_MOV + 3);
			//改变方向
			if (nGet == ACTION_DIR)
			{
				pPcTank[i].SetDir(GetRandNum(DIR_UP + 1, DIR_RIGHT + 1) - 1);//生成随机方向
			}
			//发射炮弹
			else if (nGet == ACTION_ATTACK)
			{
				if (pPcTank[i].IsLaunchBullet(1))
					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)//如果是boss
					{
						int nBulletType = GetRandNum(1,5);//生成随机的发炮类型
						if (nBulletType==1) MyBullet.SetBulletGroup(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), 0x9, "●", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
						else MyBullet.SetBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), 0x9, "●", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
					}
					else MyBullet.SetBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), 12, "●", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());

					
			}
			//移动
			else
			{
			//ReStart://是否坦克重叠

				if (!pPcTank[i].IsTouchPcTank(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), pPcTank[i].GetDir(), pPcTank[i].GetTankId())
					&& !pPcTank[i].IsTouchMyTank(MyTank, pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), pPcTank[i].GetDir())
					&& !pPcTank[i].IsTouchMyTank(MyTank2, pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), pPcTank[i].GetDir()))
				{
					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP && pPcTank[i].IsAi())//如果是boss则判断是否Ai状态
					{
						int nEndX = pPcTank[i].GetAiType()==TANK_PA? MyTank.GetTankX(): MyTank2.GetTankX();//是否为Ai状态（自动寻找玩家作为目标）
						int nEndY = pPcTank[i].GetAiType() == TANK_PA ? MyTank.GetTankY() : MyTank2.GetTankY();//是否为Ai状态（自动寻找玩家作为目标）
						int nMov = pPcTank[i].MoveTankAi(nEndX, nEndY);//移动后返回移动结果
						if (nMov == MOVE_NO)//如果在范围内不使用A星寻路，将原地发炮
						{
							int nButtle = pPcTank[i].IsBulletCdBuffer();
							if(!nButtle)MyBullet.SetBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), 0x9, "●", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
						}

					}
					else pPcTank[i].MoveTank();//普通坦克随机移动
					
				}
				else//重叠则改变方向，跳回判断是否坦克重叠
				{
					pPcTank[i].SetDir(GetRandNum(DIR_UP + 1, DIR_RIGHT + 1) - 1);//生成随机方向
					//goto ReStart;
				}
			}

		DontMove:
			//是否碰到玩家A的炮弹
			if (pPcTank[i].IsTouchBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), BULLET_MY))
			{
				if (pPcTank[i].GetMaxHp() == TANK_PC5_HP || pPcTank[i].GetMaxHp() == TANK_PC4_HP)//如果是boss或者炼狱敌人
				{
					int nBulletColor = pPcTank[i].GetMaxHp() == TANK_PC5_HP ? 0x9 : 0xc;//判断炮弹颜色
					MyBullet.SetBulletSpuer(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), nBulletColor, "●", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());//反弹炮弹

					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)pPcTank[i].SetAi(TANK_PA);//如果是boss则设置10秒Ai状态，寻找玩家
				}

				pPcTank[i].SubHp(MyTank.GetAttack());//减去hp
				if (pPcTank[i].IsDie())//判断是否死亡
				{
					MyTank.AddRank();//加分
					pPcTank[i].CleanTank();//擦除PC坦克

					int tX = pPcTank[i].GetTankX();
					int tY = pPcTank[i].GetTankY();

					int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
					int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
					pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));//如果在草丛，则刷新草丛图案

					pPcTank.erase(pPcTank.begin() + i);//擦除坦克成员
					continue;
				}
			}
			//是否碰到玩家B的炮弹
			if (pPcTank[i].IsTouchBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), BULLET_MY2))
			{
				if (pPcTank[i].GetMaxHp() == TANK_PC5_HP || pPcTank[i].GetMaxHp() == TANK_PC4_HP)
				{
					int nBulletColor = pPcTank[i].GetMaxHp() == TANK_PC5_HP ? 0x9 : 0xc;
					MyBullet.SetBulletSpuer(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), nBulletColor, "●", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)pPcTank[i].SetAi(TANK_PB);

				}

				pPcTank[i].SubHp(MyTank2.GetAttack());//减去hp
				if (pPcTank[i].IsDie())//判断是否死亡
				{
					MyTank2.AddRank();
					pPcTank[i].CleanTank();

					int tX = pPcTank[i].GetTankX();
					int tY = pPcTank[i].GetTankY();

					int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
					int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
					pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));

					pPcTank.erase(pPcTank.begin() + i);
					continue;
				}
			}
			pPcTank[i].PrintTank();//打印坦克
		}

		if (!MyTank.IsStaticAllBuffer()||!MyTank2.IsStaticAllBuffer())CreatRandTank();//如果不是冻结状态，调用函数生成新的坦克
		
	}
	void MoveTankLink()
	{//负责所有PC坦克的行为

		if (MyTank.IsKillAllBuffer()) { ClearAllTank(0); return; }//如果PlayerA吃了全屏秒杀buff就清空全部坦克
		if (MyTank2.IsKillAllBuffer()) { ClearAllTank(1); return; }//如果PlayerB吃了全屏秒杀buff就清空全部坦克

		for (int i = pPcTank.size() - 1; i >= 0; i--)
		{
			if (pPcTank[i].IsTouchBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), BULLET_MY))//是否碰到我的炮弹
			{
				if (pPcTank[i].GetMaxHp() == TANK_PC5_HP || pPcTank[i].GetMaxHp() == TANK_PC4_HP)//如果是boss或者炼狱敌人
				{
					int nBulletColor = pPcTank[i].GetMaxHp() == TANK_PC5_HP ? 0x9 : 0xc;//判断炮弹颜色
					MyBullet.SetBulletSpuer(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), nBulletColor, "●", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());//反弹炮弹

					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)pPcTank[i].SetAi(TANK_PA);//如果是boss则设置10秒Ai状态，寻找玩家
				}

				pPcTank[i].SubHp(MyTank.GetAttack());//减去hp
				if (pPcTank[i].IsDie())//判断是否死亡
				{
					MyTank.AddRank();//加分
					pPcTank[i].CleanTank();//擦除PC坦克

					int tX = pPcTank[i].GetTankX();
					int tY = pPcTank[i].GetTankY();

					int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
					int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
					pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));//如果在草丛，则刷新草丛图案

					pPcTank.erase(pPcTank.begin() + i);//擦除坦克成员
					continue;
				}
			}
			if (pPcTank[i].IsTouchBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), BULLET_MY2))//是否碰到我的炮弹
			{
				if (pPcTank[i].GetMaxHp() == TANK_PC5_HP || pPcTank[i].GetMaxHp() == TANK_PC4_HP)
				{
					int nBulletColor = pPcTank[i].GetMaxHp() == TANK_PC5_HP ? 0x9 : 0xc;
					MyBullet.SetBulletSpuer(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), nBulletColor, "●", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)pPcTank[i].SetAi(TANK_PB);

				}

				pPcTank[i].SubHp(MyTank2.GetAttack());//减去hp
				if (pPcTank[i].IsDie())//判断是否死亡
				{
					MyTank2.AddRank();
					pPcTank[i].CleanTank();

					int tX = pPcTank[i].GetTankX();
					int tY = pPcTank[i].GetTankY();

					int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
					int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
					pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));

					pPcTank.erase(pPcTank.begin() + i);
					continue;
				}
			}
			pPcTank[i].PrintTank();//打印坦克
		}

	}

};
class Map
{
	int nType = 0;
	int nTypeArry[12] = { OBSTACLE_WALL ,OBSTACLE_EARTHWALL ,OBSTACLE_WATER ,OBSTACLE_GRASS,BUFFER_RANDBUFFER,TANK_PC1 ,TANK_PC2,TANK_PC3 ,TANK_PC4,TANK_PC5,TANK_PA,TANK_PB};
	int nColor[12] = { 0x8 ,0x6 ,0x9 ,0x2,0xc,0xf ,0xe,0xd ,0xc,0x9,0xb,0xa };

public:
	Map(){}
	Map(char *filename=NULL) 
	{
		bool nRet = EditMap(filename);
		if (nRet)
		{
			CreatBasicWall(WALL_WIDE, WALL_HIGH, false);//打印绘图区四周基本墙壁
			PrintEmptyFrame(WALL_WIDE, 0, 14, 50, 0xa, "□");//打印工具框
			PrintEmptyFrame(WALL_WIDE, 52, 14, 6, 0xa, "□");//打印保存框

			PrintTools();//打印障碍物选项
			PrintPaintTips();//打印操作提示
			PrintButton();//打印保存和退出按钮
			PrintBasicWallPoint();
		}
	}
	Map(bool creatbasic)
	{
		if (!creatbasic)return;

		CreatBasicWall(WALL_WIDE, WALL_HIGH, true);//打印绘图区四周基本墙壁
		PrintEmptyFrame(WALL_WIDE, 0, 14, 50, 0xa, "□");//打印工具框
		PrintEmptyFrame(WALL_WIDE, 52, 14, 6, 0xa, "□");//打印保存框

		PrintTools();//打印障碍物选项
		PrintPaintTips();//打印操作提示
		PrintButton();//打印保存和退出按钮
		PrintBasicWallPoint();
	}
	~Map()
	{
		vector <POINTDATA>().swap(pMap);
	}
	void PrintTools()
	{
		int px = WALL_WIDE + 3;
		int py = 2;
		PrintPoint(px, py, "地图障碍类", 0xc);

		PrintPoint(px, py + 2, "→   ■ 钢铁墙壁  ", 0x8);
		PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
		PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
		PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
		PrintPoint(px, py + 10, "物品加成类", 0xc);
		PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
		PrintPoint(px, py + 14, "敌人类", 0xc);
		PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
		PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
		PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
		PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
		PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);

		PrintPoint(px, py + 26, "玩家类", 0xc);
		PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
		PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);

	}
	int PrintTools(int x, int y)
	{
		int px = WALL_WIDE+3;
		int py = 2;

		if (x >= px && y == py + 2)nType = 0;
		else if (x >= px && y == py + 4)nType = 1;
		else if (x >= px && y == py + 6)nType = 2;
		else if (x >= px && y == py + 8)nType = 3;
		else if (x >= px && y == py + 12)nType = 4;
		else if (x >= px && y == py + 16)nType = 5;
		else if (x >= px && y == py + 18)nType = 6;
		else if (x >= px && y == py + 20)nType = 7;
		else if (x >= px && y == py + 22)nType = 8;
		else if (x >= px && y == py + 24)nType = 9;
		else if (x >= px && y == py + 28)nType = 10;
		else if (x >= px && y == py + 30)nType = 11;


		switch (nType)
		{
		case 0:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "→   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa); 
			break;

		case 1:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "→   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa); 
			break;

		case 2:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "→   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 3:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "→   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 4:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "→   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 5:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "→   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 6:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "→   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 7:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "→   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 8:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "→   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 9:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "→   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 10:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "→   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "   ■ 玩家2  ", 0xa);
			break;
		case 11:
			PrintPoint(px, py, "地图障碍类", 0xc);

			PrintPoint(px, py + 2, "   ■ 钢铁墙壁  ", 0x8);
			PrintPoint(px, py + 4, "   ■ 土质墙壁  ", 0x6);
			PrintPoint(px, py + 6, "   ○ 河流  ", 0x9);
			PrintPoint(px, py + 8, "   △ 草丛  ", 0x2);
			PrintPoint(px, py + 10, "物品加成类", 0xc);
			PrintPoint(px, py + 12, "   卍 随机物品  ", 0xc);
			PrintPoint(px, py + 14, "敌人类", 0xc);
			PrintPoint(px, py + 16, "   ① 初级敌人  ", 0xf);
			PrintPoint(px, py + 18, "   ② 中级敌人  ", 0xe);
			PrintPoint(px, py + 20, "   ③ 高级敌人  ", 0xd);
			PrintPoint(px, py + 22, "   ④ 炼狱敌人  ", 0xc);
			PrintPoint(px, py + 24, "   ⑤ 终级BOSS  ", 0x9);
			PrintPoint(px, py + 26, "玩家类", 0xc);
			PrintPoint(px, py + 28, "   ■ 玩家1  ", 0xb);
			PrintPoint(px, py + 30, "→   ■ 玩家2  ", 0xa);
			break;
		default:
			break;
		}
		return nType;
	}
	void PrintPaintTips()
	{
		int px = WALL_WIDE+3;
		int py = 35;
		PrintPoint(px, py, "绘画方法", 0xc);

		PrintPoint(px, py + 2, "① 右侧选择类块", 0xa);
		PrintPoint(px, py + 4, "② 按鼠标左键绘图", 0xa);
		PrintPoint(px, py + 6, "③ 按鼠标右键擦除", 0xa);

	}
	void PrintButton()
	{
		int px = WALL_WIDE+3;
		int py = 54;
		PrintPoint(px, py, "保存到地图    ", 0xa);
		PrintPoint(px, py + 2, "退出自定义编辑", 0x2);

	}
	void PrintButton(int nButton)
	{
		int px = WALL_WIDE+3;
		int py = 54;

		switch (nButton)
		{
		case BUTTON_SAVEMAP:
			PrintPoint(px, py, "保存到地图    ", 0xa);
			PrintPoint(px, py + 2, "退出自定义编辑", 0x2);
			break;
		case BUTTON_ENDEDITMAP:
			PrintPoint(px, py, "保存到地图    ", 0x2);
			PrintPoint(px, py + 2, "退出自定义编辑", 0xa);
			break;

		}
	}
	int GetButton(int x,int y)
	{
		int px = WALL_WIDE + 3;
		int py = 54;

		if (x >= px && y == py)return BUTTON_SAVEMAP;
		else if (x >= px && y == py + 2)return BUTTON_ENDEDITMAP;
		return -1;
	}
	void CreatBasicWall(int wide, int higt, bool creatcamp)
	{
		for (int w = 0; w < wide; w++)
		{
			for (int h = 0; h < higt; h++)
			{
				if (h == 0 || h == higt - 1)
				{
					pMap.push_back(POINTDATA(w, h, 8, "■",0, 88888));
				}
				else if (w == 0 || w == wide - 1)
				{
					pMap.push_back(POINTDATA(w, h, 8, "■", 0, 88888));
				}

			}
		}

		if (creatcamp)
		{
			CreatEarthWall(22, 54, 1, 4);
			CreatEarthWall(24, 54, 2, 1);
			CreatEarthWall(27, 54, 1, 4);
			CreatCamp(24, 56, 2, 2);
		}


	}		
	void CreatEarthWall(int x, int y, int wide, int higt)
	{

		for (int w = 0; w <= wide; w++)
		{
			for (int h = 0; h <= higt; h++)
			{

					if (h == 0 || h == higt)
					{
						pMap.push_back(POINTDATA(w + x, h + y, 6, "■", 0, OBSTACLE_EARTHWALL));

					}
					else if (w == 0 || w == wide)
					{
						pMap.push_back(POINTDATA(w + x, h + y, 6, "■", 0, OBSTACLE_EARTHWALL));
					}
			}
		}
	}
	void CreatCamp(int x, int y, int wide, int higt)
	{

		for (int w = 0; w <= wide; w++)
		{
			for (int h = 0; h <= higt; h++)
			{
				pMap.push_back(POINTDATA(w + x, h + y, 14, "★", 0, OBSTACLE_CAMP_A));
			}
		}
	}

	void PaintMap(int x, int y)
	{
		if (IsMapPoint(x, y, nType))return;

		if (nTypeArry[nType] == OBSTACLE_WALL)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "■", 0, nTypeArry[nType]));
			PrintPoint(x, y, "■", nColor[nType]);
		}
		else if (nTypeArry[nType]==OBSTACLE_EARTHWALL)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "■", 0, nTypeArry[nType]));
			PrintPoint(x, y, "■", nColor[nType]);
		}
		else if (nTypeArry[nType] == OBSTACLE_WATER)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "○", 0, nTypeArry[nType]));
			PrintPoint(x, y, "○", nColor[nType]);
		}
		else if (nTypeArry[nType] == OBSTACLE_GRASS)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "△", 0, nTypeArry[nType]));
			PrintPoint(x, y, "△", nColor[nType]);
		}
		else if (nTypeArry[nType] == BUFFER_RANDBUFFER)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "卍", 0, nTypeArry[nType]));
			PrintPoint(x, y, "卍", nColor[nType]);
		}
		else if (nTypeArry[nType] >= TANK_PC1 && nTypeArry[nType] <= TANK_PB)
		{

			if (nTypeArry[nType] == TANK_PA&&IsHavePlayerTank(1))return;
			if (nTypeArry[nType] == TANK_PB&&IsHavePlayerTank(2))return;
			if (nTypeArry[nType] == TANK_PC5&&IsHaveBossTank())return;

			


			pMap.push_back(POINTDATA(x, y, nColor[nType], "■", 0, nTypeArry[nType]));
			PrintPoint(x-1, y-1, "  ", nColor[nType]);
			PrintPoint(x, y - 1, "■", nColor[nType]);
			PrintPoint(x+1, y - 1, "  ", nColor[nType]);

			PrintPoint(x - 1, y, "■", nColor[nType]);
			PrintPoint(x, y, "■", nColor[nType]);
			PrintPoint(x + 1, y, "■", nColor[nType]);

			PrintPoint(x - 1, y + 1, "■", nColor[nType]);
			PrintPoint(x, y + 1, "  ", nColor[nType]);
			PrintPoint(x + 1, y + 1, "■", nColor[nType]);
		}
	}
	void EraseWall(int x, int y)
	{
		if (IsMapPoint(x, y, nType) == false)return;

		int nRet = DeleteWallPoint(x, y);
		if (nRet==1)PrintPoint(x, y, "  ", 0);
		else if(nRet==9)
		{
			PrintPoint(x - 1, y - 1, "  ", 0);
			PrintPoint(x, y - 1, "  ", 0);
			PrintPoint(x + 1, y - 1, "  ", 0);

			PrintPoint(x - 1, y, "  ", 0);
			PrintPoint(x, y, "  ", 0);
			PrintPoint(x + 1, y, "  ", 0);

			PrintPoint(x - 1, y + 1, "  ", 0);
			PrintPoint(x, y + 1, "  ", 0);
			PrintPoint(x + 1, y + 1, "  ", 0);
		}
	}
	int DeleteWallPoint(int x, int y)
	{
		if (pMap.size() == 0)return false;

		for (int i = pMap.size() - 1; i >= 0; i--)
		{
			if (pMap[i].m_type < 88888)
			{
				if (pMap[i].m_x == x && pMap[i].m_y == y)
				{
					int nRet = 1;
					if (pMap[i].m_type >= TANK_PC1 && pMap[i].m_type <= TANK_PB)nRet= 9;
					pMap.erase(pMap.begin() + i);
					return nRet;
				}
			}
		}
		return 0;
	}
	bool IsMapPoint(int x, int y,int type)//是否已画图
	{
		int nDrawType = 0;
		if (nTypeArry[type]>=TANK_PC1 && nTypeArry[type] <= TANK_PB)nDrawType = 1;//要画坦克
		else nDrawType = 0;//要画普通障碍物

		for (auto &val : pMap)
		{
			if (val.m_type>=TANK_PC1 && val.m_type <= TANK_PB)//这是坦克的点
			{
				if (nDrawType==0)//如果要画普通障碍物
				{
					if (x >= val.m_x - 1 && x <= val.m_x + 1 && y >= val.m_y - 1 && y <= val.m_y + 1)return true;
				}
				else if(nDrawType == 1)//如果要画坦克
				{
					if (x >= val.m_x - 2 && x <= val.m_x + 2 && y >= val.m_y - 2 && y <= val.m_y + 2)return true;
				}
			}
			else //这是一个普通的障碍物点
			{ 
				if (nDrawType == 0)//如果要画障碍物
				{
					if (val.m_x == x && val.m_y == y)return true;
				}
				else if (nDrawType == 1)//如果要画坦克
				{
					if (x >= val.m_x - 1 && x <= val.m_x + 1 && y >= val.m_y - 1 && y <= val.m_y + 1)return true;
				}
			}
		}
		return false;
	}
	bool IsHavePcTank()
	{
		for (auto &val : pMap)
		{
			if (val.m_type >= TANK_PC1 && val.m_type <= TANK_PC5)return true;
		}
		return false;
	}
	bool IsHaveBossTank()
	{
		for (auto &val : pMap)
		{
			if (val.m_type == TANK_PC5)return true;
		}
		return false;
	}

	bool IsHavePlayerTank(int player)
	{
		for (auto &val : pMap)
		{
			if (player == 1)
			{
				if (val.m_type == TANK_PA)return true;
			}
			else if (player == 2) 
			{ 
				if (val.m_type == TANK_PB)return true; 
			}
		}
		return false;
	}
	int GetClickFrame(int x, int y)//返回0：绘画界面 1：工具界面 2：保存界面 -1：无效区域
	{
		if (x > 0 && x < WALL_WIDE && y >0 && y < WALL_HIGH)return FRAME_DRAW;
		else if (x > WALL_WIDE && x < WALL_WIDE + 14 && y >0 && y < 48)return FRAME_TOOLS;
		else if (x > WALL_WIDE && x < WALL_WIDE + 14 && y >48 && y < WALL_HIGH)return FRAME_SAVEINFO;
		return -1;
	}
	void PrintBasicWallPoint()//打印全部绘画出来的点
	{
		for (auto &val : pMap) {
			if (val.m_type>=TANK_PC1&&val.m_type <=TANK_PB)//还原坦克六点图案
			{
				PrintPoint(val.m_x, val.m_y-1, val.m_data, val.m_color);
				PrintPoint(val.m_x-1, val.m_y, val.m_data, val.m_color);
				PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
				PrintPoint(val.m_x + 1, val.m_y, val.m_data, val.m_color);
				PrintPoint(val.m_x - 1, val.m_y+1, val.m_data, val.m_color);
				PrintPoint(val.m_x + 1, val.m_y + 1, val.m_data, val.m_color);
			}
			else PrintPoint(val.m_x, val.m_y, val.m_data, val.m_color);
		}
	}
	void ClsAll(int wide, int higt)//清除所有点
	{
		for (int w = 1; w < wide - 1; w++)
		{
			for (int h = 1; h < higt - 1; h++)
			{
				PrintPoint(w, h, "  ", 0);
			}
		}
	}
	bool SaveGame(Tank &MyTank, Tank &MyTank2)
	{
		//动态数组类型进行保存，先储存数组元素的数量，然后再接着储存数组内的数据。
		FILE *pfile;
		char *szFileName = GetRoute(ROUTE_SAVEGAME);
		if (szFileName == NULL)return false;
		int nsize = 0;

		fopen_s(&pfile, szFileName, "wb");//先用写出方式打开，覆盖原文件
		if (pfile == NULL) { MessageBoxA(NULL, "写出文件失败！", "保存失败", MB_OK + MB_ICONERROR); return false; }
		if (MyTank2.GetColor() == 0)nsize = 1;//判断是否有坦克B玩家
		else nsize = 2;
		fwrite(&nsize, 1, sizeof(int), pfile);
		fclose(pfile);


		//储存MyTank数据
		vector <TankInfo> pMyTankInfo;

		pMyTankInfo.push_back(TankInfo{ MyTank.GetTankX(),MyTank.GetTankY() ,MyTank.GetColor() ,MyTank.GetDir() ,MyTank.GetSpeed() ,MyTank.GetBulletCd(),MyTank.GetHp(),MyTank.GetMp(),MyTank.GetRank() ,MyTank.GetMaxHp() ,MyTank.GetMaxMp() });
		if(nsize==2)pMyTankInfo.push_back(TankInfo{ MyTank2.GetTankX(),MyTank2.GetTankY() ,MyTank2.GetColor() ,MyTank2.GetDir() ,MyTank2.GetSpeed() ,MyTank2.GetBulletCd(),MyTank2.GetHp(),MyTank2.GetMp(),MyTank2.GetRank(),MyTank2.GetMaxHp() ,MyTank2.GetMaxMp() });
		for (auto &val: pMyTankInfo)
		{
			fopen_s(&pfile, szFileName, "ab");//先用写出方式打开，覆盖原文件
			fwrite(&val, 1, sizeof(TankInfo), pfile);
			fclose(pfile);
		}

		//储存pObstacle数据
		nsize = pObstacle.size();//取出障碍物数量
		fopen_s(&pfile, szFileName, "ab");
		fwrite(&nsize, 1, sizeof(int), pfile);//数量写在前4字节
		fclose(pfile);
		for (auto &val : pObstacle)
		{
			fopen_s(&pfile, szFileName, "ab");
			//接着循环写入每个障碍物的坐标数据
			fwrite(&val, 1, sizeof(POINTDATA), pfile);
			fclose(pfile);
		}

		//储存pBullet数据
		nsize = pBullet.size();
		fopen_s(&pfile, szFileName, "ab");
		fwrite(&nsize, 1, sizeof(int), pfile);
		fclose(pfile);
		for (auto &val : pBullet)
		{
			fopen_s(&pfile, szFileName, "ab");
			fwrite(&val, 1, sizeof(POINTDATA), pfile);
			fclose(pfile);
		}

		//储存pPcTankConfig数据
		nsize = pPcTankConfig.size();
		fopen_s(&pfile, szFileName, "ab");
		fwrite(&nsize, 1, sizeof(int), pfile);
		fclose(pfile);
		for (auto &val : pPcTankConfig)
		{
			fopen_s(&pfile, szFileName, "ab");
			fwrite(&val, 1, sizeof(POINTDATA), pfile);
			fclose(pfile);
		}


		vector <TankInfo> pPcTankInfo;
		for (auto &val : pPcTank)
		{
			pPcTankInfo.push_back(TankInfo{ val.GetTankX(),val.GetTankY() ,val.GetColor() ,val.GetDir() ,val.GetSpeed() ,val.GetBulletCd(),val.GetHp(),val.GetMp() });
		}


		//储存pPcTank数据
		nsize = pPcTankInfo.size();
		fopen_s(&pfile, szFileName, "ab");
		fwrite(&nsize, 1, sizeof(int), pfile);
		fclose(pfile);
		for (auto &val : pPcTankInfo)
		{
			fopen_s(&pfile, szFileName, "ab");
			fwrite(&val, 1, sizeof(TankInfo), pfile);
			fclose(pfile);
		}


		//储存pBuffer数据
		nsize = pBufferConfig.size();
		fopen_s(&pfile, szFileName, "ab");
		fwrite(&nsize, 1, sizeof(int), pfile);
		fclose(pfile);
		for (auto &val : pBufferConfig)
		{
			fopen_s(&pfile, szFileName, "ab");
			fwrite(&val, 1, sizeof(POINTDATA), pfile);
			fclose(pfile);
		}

		MessageBoxA(NULL, "存档成功！", "提示", MB_OK + MB_ICONINFORMATION);
		return true;
	}
	bool LoadGame(char *filename = NULL)
	{
		if (filename == NULL) filename = GetRoute(ROUTE_LOADGAME);

		char *szFileName = filename;
		if (szFileName == NULL)return false;

		//初始化所有保存数据的动态数组
		pPlayerTankConfig.clear();//存档内玩家坦克的坐标信息
		pPlayerTankConfigLoad.clear();//存档内玩家坦克的剩余HP等信息
		pPcTankConfig.clear();//存档内生成坦克的规则信息
		pBufferConfig.clear();//加成食物道具的所在坐标信息
		vector <POINTDATA>().swap(pPlayerTankConfig);
		vector <TankInfo>().swap(pPlayerTankConfigLoad);
		vector <POINTDATA>().swap(pPcTankConfig);
		vector <POINTDATA>().swap(pBufferConfig);
		//初始化所有开始游戏所需要读取的动态数组数组
		pObstacle.clear();//障碍物类
		pBullet.clear();//炮弹类
		pPcTank.clear();//PC坦克类
		vector <POINTDATA>().swap(pObstacle);
		vector <POINTDATA>().swap(pBullet);
		vector <Tank>().swap(pPcTank);


		FILE *pfile;
		int nsize = 0;

		//读取MyTank数据
		fopen_s(&pfile, szFileName, "rb");
		if (pfile == NULL) { MessageBoxA(NULL, "打开文件失败！", "保存失败", MB_OK + MB_ICONERROR); return false; }
		fread(&nsize, 1, sizeof(int), pfile);

		TankInfo *mytank = new TankInfo;
		for (int i = 0; i < nsize; i++)
		{
			fread(mytank, 1, sizeof(TankInfo), pfile);
			pPlayerTankConfigLoad.push_back(TankInfo{ mytank->x, mytank->y, mytank->color,mytank->dir,mytank->speed,mytank->bulletcd, mytank->hp, mytank->mp, mytank->rank, mytank->maxhp, mytank->maxmp });
		}
		delete mytank;


		POINTDATA *temp = new POINTDATA;//申请一个临时堆空间
		//前4个字节是读取cObstacle内的数量
		fread(&nsize, 1, sizeof(int), pfile);
		//循环读取cObstacle的所有数据
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//读取的数据放入该内存空间
			switch (temp->m_type)//按类型选择
			{
			case OBSTACLE_WALL:case OBSTACLE_EARTHWALL:
				temp->m_data = "■";//重新赋值图案
				break;
			case OBSTACLE_GRASS:
				temp->m_data = "△";
				break;
			case OBSTACLE_WATER:
				temp->m_data = "○";
				break;
			case OBSTACLE_CAMP_A:
				temp->m_data = "★";
				break;
			}
			pObstacle.push_back(*temp);//加入pObstacle动态数组内
		}

		//读取pBullet数据
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//读取的数据放入该内存空间
			if(temp->m_type==BULLET_MY)temp->m_data = "◎";
			else temp->m_data = "●";
			pBullet.push_back(*temp);
		}

		//读取pPcTankConfig数据
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//读取的数据放入该内存空间
			temp->m_data = "■";
			pPcTankConfig.push_back(*temp);
		}

		vector <TankInfo*> pPcTankInfo;
		TankInfo *tank = new TankInfo;
		
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(tank, 1, sizeof(TankInfo), pfile);//读取的数据放入该内存空间

			pPcTank.push_back(Tank(tank->x, tank->y, tank->color, tank->dir, tank->speed, tank->bulletcd, tank->hp, tank->mp, 1, 3,GetRandTankId(), tank->hp, tank->mp));
		}
		delete tank;


		//读取pBufferConfig数据
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//读取的数据放入该内存空间
			temp->m_data = "卍";
			pBufferConfig.push_back(*temp);
		}
		delete temp;
		fclose(pfile);
		return true;
	}
	bool SaveMap()
	{
		if (IsHavePlayerTank(1) == false) { MessageBoxA(NULL,"至少初始化PlayerA(玩家1)位置！","保存失败", MB_OK + MB_ICONERROR);return false; }

		vector <POINTDATA> cObstacle;
		vector <POINTDATA> cBuffer;
		vector <POINTDATA> cTank;

		//保存前先把坐标分类，压入不同类型的动态数组内
		for (auto &val : pMap)//遍历画的地图内所有坐标点数据
		{
			if (val.m_type ==OBSTACLE_WALL)//钢墙类
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "■",0, val.m_type));
			}
			else if (val.m_type == OBSTACLE_EARTHWALL)//土墙
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "■",0, val.m_type));
			}
			else if (val.m_type == OBSTACLE_WATER)//河流
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "○", 0, val.m_type));
			}
			else if (val.m_type == OBSTACLE_GRASS)//草丛
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "△", 0, val.m_type));
			}
			else if (val.m_type == BUFFER_RANDBUFFER)//食物
			{
				cBuffer.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "卍", 0, val.m_type));
			}
			else if (val.m_type == OBSTACLE_CAMP_A)//基地
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "★", 0, val.m_type));
			}
			else if (val.m_type >= TANK_PC1&& val.m_type <= TANK_PB)//坦克
			{
				cTank.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "■", 0, val.m_type));
			}

		}

		//然后再按动态数组类型进行保存，先储存数组元素的数量，然后再接着储存数组内的数据。
		FILE *pfile;
		char *szFileName = GetRoute(ROUTE_SAVEMAP);
		if (szFileName == NULL)return false;
		int nsize = 0;

		//储存cObstacle数据
		nsize = cObstacle.size();
		fopen_s(&pfile, szFileName, "wb");//先用写出方式打开，覆盖原文件
		if (pfile == NULL) { MessageBoxA(NULL, "写出文件失败！", "保存失败", MB_OK + MB_ICONERROR); return false; }
		fwrite(&nsize, 1, sizeof(int), pfile);//前4个字节先写出数组的数量
		fclose(pfile);
		for (auto &val : cObstacle)
		{
			fopen_s(&pfile, szFileName, "ab");//再用追加方式打开，在文件后面继续追加数组数据
			fwrite(&val, 1, sizeof(POINTDATA), pfile);//开始写入数据
			fclose(pfile);
		}

		//储存cBuffer数据
		nsize = cBuffer.size();
		fopen_s(&pfile, szFileName, "ab");
		fwrite(&nsize, 1, sizeof(int), pfile);
		fclose(pfile);
		for (auto &val : cBuffer)
		{
			fopen_s(&pfile, szFileName, "ab");
			fwrite(&val, 1, sizeof(POINTDATA), pfile);
			fclose(pfile);
		}


		//储存cTank数据
		nsize = cTank.size();
		fopen_s(&pfile, szFileName, "ab");
		fwrite(&nsize, 1, sizeof(int), pfile);
		fclose(pfile);
		for (auto &val : cTank)
		{
			fopen_s(&pfile, szFileName, "ab");
			fwrite(&val, 1, sizeof(POINTDATA), pfile);
			fclose(pfile);
		}

		MessageBoxA(NULL, "保存成功！", "提示", MB_OK + MB_ICONINFORMATION);
		return true;
	}
	bool OpenMap(char *filename=NULL)
	{
		if (filename == NULL) filename = GetRoute(ROUTE_OPENMAP);

		char *szFileName = filename;
		if (szFileName == NULL)return false;

		pPlayerTankConfig.clear();
		pPlayerTankConfigLoad.clear();
		pPcTankConfig.clear();
		pBufferConfig.clear();
		vector <TankInfo>().swap(pPlayerTankConfigLoad);
		vector <POINTDATA>().swap(pPlayerTankConfig);
		vector <POINTDATA>().swap(pPcTankConfig);
		vector <POINTDATA>().swap(pBufferConfig);

		pObstacle.clear();
		pBullet.clear();
		pPcTank.clear();
		vector <POINTDATA>().swap(pObstacle);
		vector <POINTDATA>().swap(pBullet);
		vector <Tank>().swap(pPcTank);


		vector <POINTDATA>pPlayerATankConfig;
		vector <POINTDATA>pPlayerBTankConfig;


		FILE *pfile;


		int nsize = 0;

		//读取cObstacle数据
		fopen_s(&pfile, szFileName, "rb");
		if (pfile == NULL) //使用文件对话框后需要绝对路径才能打开关卡地图文件
		{ 
			char *tRoute = GetProgramDir();
			strcat_s(tRoute,strlen(szFileName)+1, szFileName);
			fopen_s(&pfile, tRoute, "rb");
			if (pfile == NULL) { MessageBoxA(NULL, "打开文件失败！", "保存失败", MB_OK + MB_ICONERROR); return false; }
		}
	

		fread(&nsize, 1, sizeof(int), pfile);
		//先读取前4字节,数组的数量，然后for循环依次读入指定数量的数据，放入临时内存里

		POINTDATA *temp = new POINTDATA;//申请一个临时内存空间
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//读取的数据放入该内存空间

			switch (temp->m_type)
			{
			case OBSTACLE_WALL:case OBSTACLE_EARTHWALL:
				temp->m_data = "■";
				break;
			case OBSTACLE_GRASS:
				temp->m_data = "△";
				break;
			case OBSTACLE_WATER:
				temp->m_data = "○";
				break;
			case OBSTACLE_CAMP_A:
				temp->m_data = "★";
				break;
			}
			pObstacle.push_back(*temp);
		}

		//读取pBufferConfig
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);
			temp->m_data = "卍";
			pBufferConfig.push_back(*temp);
			
		}

		//读取pPlayerTankConfig,pPcTankConfig
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);
			temp->m_data = "■";

			switch (temp->m_type)
			{
			case TANK_PA:
				pPlayerATankConfig.push_back(*temp);
				break;
			case TANK_PB:
				pPlayerBTankConfig.push_back(*temp);
				break;
			case TANK_PC1:case TANK_PC2:case TANK_PC3:case TANK_PC4:case TANK_PC5:
				pPcTankConfig.push_back(*temp);
				break;
			}
		}
		delete temp;

		pPlayerTankConfig.assign(pPlayerATankConfig.begin(), pPlayerATankConfig.end());
		if (pPlayerBTankConfig.size()>0)pPlayerTankConfig.insert(pPlayerTankConfig.begin()+1,pPlayerBTankConfig.begin(), pPlayerBTankConfig.end());


		fclose(pfile);
		return true;
	}
	bool EditMap(char *filename = NULL)
	{
		if (filename == NULL) filename = GetRoute(ROUTE_OPENMAP);
		char *szFileName = filename;
		if (szFileName == NULL)return false;

		system("cls");
		FILE *pfile;
		int nsize = 0;
		//读取cObstacle数据的数量
		fopen_s(&pfile, szFileName, "rb");
		if (pfile == NULL) { MessageBoxA(NULL, "打开文件失败！", "保存失败", MB_OK + MB_ICONERROR); return false; }
		fread(&nsize, 1, sizeof(int), pfile);
		//先读取前4字节,数组的数量，然后for循环依次读入指定数量的数据，放入临时内存里

		POINTDATA *temp = new POINTDATA;//申请一个临时内存空间
		for (int i = 0; i < nsize; i++)
		{
			//读取的数据放入该内存空间
			fread(temp, 1, sizeof(POINTDATA), pfile);
			switch (temp->m_type)//按类型选择
			{
			case OBSTACLE_WALL:case OBSTACLE_EARTHWALL:
				temp->m_data = "■";//重新赋值图案
				break;
			case OBSTACLE_GRASS:
				temp->m_data = "△";
				break;
			case OBSTACLE_WATER:
				temp->m_data = "○";
				break;
			case OBSTACLE_CAMP_A:
				temp->m_data = "★";
				break;
			}
			pMap.push_back(POINTDATA(*temp));//将数据加入动态数组Map内
		}

		//读取pBufferConfig
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);
			temp->m_data = "卍";
			pMap.push_back(POINTDATA(*temp));
		}


		//读取pPlayerTankConfig,pPcTankConfig
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);
			temp->m_data = "■";
			pMap.push_back(POINTDATA(*temp));
		}

		delete temp;
		fclose(pfile);
		return true;
	}
};

int StartGame(int nStage=NULL)
{
	system("cls");
	bool PlayerB = false;//是否双人模式
	bool Suspend = false;//是否暂停游戏
	bool Loading = false;//是否为载入存档

	Obstacle MyObstacle;
	Bullet MyBullet;
	Buffer MyBuffer;

	//PlayerA，B初始化时的参数声明
	int nPlayerX ;
	int nPlayerY ;
	int nPlayerColor;
	int nPlayerDir ;
	int nPlayerSpeed;
	int nBulletcd;
	int nHp;
	int nMp;
	int nRank;
	int nMaxHp;
	int nMaxMp;

	//如果Load坦克大于数量0，就是读取存档
	if (pPlayerTankConfigLoad.size()>0)Loading = true;
	else Loading = false;
	
	//开始载入PlayerA坦克的初始化信息
	switch (Loading)
	{
	case true:
		if (pPlayerTankConfigLoad.size() >= 2)PlayerB = true;
		else PlayerB = false;

		nPlayerX = pPlayerTankConfigLoad[0].x;
		nPlayerY = pPlayerTankConfigLoad[0].y;
		nPlayerColor = pPlayerTankConfigLoad[0].color;
		nPlayerDir = pPlayerTankConfigLoad[0].dir;
		nPlayerSpeed = pPlayerTankConfigLoad[0].speed;
		nBulletcd = pPlayerTankConfigLoad[0].bulletcd;
		nHp = pPlayerTankConfigLoad[0].hp;
		nMp = pPlayerTankConfigLoad[0].mp;
		nRank= pPlayerTankConfigLoad[0].rank;
		nMaxHp= pPlayerTankConfigLoad[0].maxhp;
		nMaxMp= pPlayerTankConfigLoad[0].maxmp;
		break;;
	case false:
		if (pPlayerTankConfig.size() >= 2)PlayerB = true;
		else PlayerB = false;

		nPlayerX = pPlayerTankConfig[0].m_x;
		nPlayerY = pPlayerTankConfig[0].m_y;
		nPlayerColor = pPlayerTankConfig[0].m_color;
		nPlayerDir = pPlayerTankConfig[0].m_dir;
		nPlayerSpeed = TANK_PA_SPEED;
		nBulletcd = TANK_PA_BULLETCD;
		nHp = TANK_PA_HP;
		nMp = TANK_PA_MP;
		nRank = 0;
		nMaxHp = TANK_PA_MAXHP;
		nMaxMp = TANK_PA_MAXMP;
		break;
	}
	Tank MyTank(nPlayerX, nPlayerY, nPlayerColor, nPlayerDir, nPlayerSpeed, nBulletcd, nHp, nMp,1, 3, GetRandTankId(), nMaxHp, nMaxMp);//x,t,color,dir,speed,bulletcd,hp,mp,revhp,revmp
	MyTank.SetRank(nRank);

	//开始载入PlayerB坦克的初始化信息
	switch (PlayerB)
	{
	case true:
		if(Loading)
		{
			nPlayerX = pPlayerTankConfigLoad[1].x;
			nPlayerY = pPlayerTankConfigLoad[1].y;
			nPlayerColor = pPlayerTankConfigLoad[1].color;
			nPlayerDir = pPlayerTankConfigLoad[1].dir;
			nPlayerSpeed = pPlayerTankConfigLoad[1].speed;
			nBulletcd = pPlayerTankConfigLoad[1].bulletcd;
			nHp = pPlayerTankConfigLoad[1].hp;
			nMp = pPlayerTankConfigLoad[1].mp;
			nRank = pPlayerTankConfigLoad[1].rank;
			nMaxHp = pPlayerTankConfigLoad[1].maxhp;
			nMaxMp = pPlayerTankConfigLoad[1].maxmp;
		}
		else
		{
			nPlayerX = pPlayerTankConfig[1].m_x;
			nPlayerY = pPlayerTankConfig[1].m_y;
			nPlayerColor = pPlayerTankConfig[1].m_color;
			nPlayerDir = pPlayerTankConfig[1].m_dir;
			nPlayerSpeed = TANK_PB_SPEED;
			nBulletcd = TANK_PB_BULLETCD;
			nHp = TANK_PB_HP;
			nMp = TANK_PB_MP;
			nRank = 0;
			nMaxHp = TANK_PB_MAXHP;
			nMaxMp = TANK_PB_MAXMP;
		}
		break;
	case false:
		nPlayerColor = 0;
		break;
	}
	Tank MyTank2(nPlayerX, nPlayerY, nPlayerColor, nPlayerDir, nPlayerSpeed, nBulletcd, nHp, nMp, 1, 3, GetRandTankId(), nMaxHp, nMaxMp);//x,t,color,dir,speed,bulletcd,hp,mp,revhp,revmp
	MyTank2.SetRank(nRank);

	AutoTank MyAutoTank(MyTank, MyTank2, MyBullet, Loading);


	//打印游戏右上框，中框
	PrintEmptyFrame(WALL_WIDE, 0, 14, 30, 0xa, "□");
	PrintEmptyFrame(WALL_WIDE, 32, 14, 14, 0xa, "□");

	while (true)
	{
		//取按键状态，使用GetAsyncKeyState函数
		{
			if (GetKey(KEY_PA_UP))
			{
				if (MyTank.GetDir() == DIR_UP)
				{
					if (PlayerB)
					{
						//判断我的坦克是否与PC坦克和PlayerB坦克重叠
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId())
							&& !MyTank.IsTouchMyTank(MyTank2, MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir()))
							MyTank.MoveTank();
					}
					else
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId()))
							MyTank.MoveTank();
					}

				}
				else MyTank.SetDir(DIR_UP);
			}
			if (GetKey(KEY_PA_DOWN))
			{
				if (MyTank.GetDir() == DIR_DOWN)
				{
					if (PlayerB)
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId())
							&& !MyTank.IsTouchMyTank(MyTank2, MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir()))
							MyTank.MoveTank();
					}
					else
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId()))
							MyTank.MoveTank();
					}
				}
				else MyTank.SetDir(DIR_DOWN);
			}
			if (GetKey(KEY_PA_LEFT))
			{
				if (MyTank.GetDir() == DIR_LEFT)
				{
					if (PlayerB)
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId())
							&& !MyTank.IsTouchMyTank(MyTank2, MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir()))
							MyTank.MoveTank();
					}
					else
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId()))
							MyTank.MoveTank();
					}
				}
				else MyTank.SetDir(DIR_LEFT);
			}
			if (GetKey(KEY_PA_RIGHT))
			{
				if (MyTank.GetDir() == DIR_RIGHT)
				{
					if (PlayerB)
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId())
							&& !MyTank.IsTouchMyTank(MyTank2, MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir()))
							MyTank.MoveTank();
					}
					else
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId()))
							MyTank.MoveTank();
					}

				}
				else MyTank.SetDir(DIR_RIGHT);
			}
			if (GetKey(KEY_PA_BULLET))
			{
				if (MyTank.IsLaunchBullet(1))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBullet(MyTank.GetTankX(), MyTank.GetTankY(), 11, "◎", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
					if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(1);
				}
			}
			if (GetKey(KEY_PA_BULLETGROUP))
			{
				if (MyTank.IsLaunchBullet(5))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBulletGroup(MyTank.GetTankX(), MyTank.GetTankY(), 11, "◎", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
					if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(5);
				}
			}
			if (GetKey(KEY_PA_BULLETSPUER))
			{
				if (MyTank.IsLaunchBullet(20))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBulletSpuer(MyTank.GetTankX(), MyTank.GetTankY(), 11, "◎", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
					if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(20);
				}
			}

			if (GetKey(KEY_PB_UP))
			{
				if (MyTank2.GetDir() == DIR_UP)
				{
					if (!MyTank2.IsTouchPcTank(MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir(), MyTank2.GetTankId())
						&& !MyTank2.IsTouchMyTank(MyTank, MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir()))
						MyTank2.MoveTank();
				}
				else MyTank2.SetDir(DIR_UP);
			}
			if (GetKey(KEY_PB_DOWN))
			{
				if (MyTank2.GetDir() == DIR_DOWN)
				{
					if (!MyTank2.IsTouchPcTank(MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir(), MyTank2.GetTankId())
						&& !MyTank2.IsTouchMyTank(MyTank, MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir()))
						MyTank2.MoveTank();
				}
				else MyTank2.SetDir(DIR_DOWN);
			}
			if (GetKey(KEY_PB_LEFT))
			{
				if (MyTank2.GetDir() == DIR_LEFT)
				{
					if (!MyTank2.IsTouchPcTank(MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir(), MyTank2.GetTankId())
						&& !MyTank2.IsTouchMyTank(MyTank, MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir()))
						MyTank2.MoveTank();
				}
				else MyTank2.SetDir(DIR_LEFT);
			}
			if (GetKey(KEY_PB_RIGHT))
			{
				if (MyTank2.GetDir() == DIR_RIGHT)
				{
					if (!MyTank2.IsTouchPcTank(MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir(), MyTank2.GetTankId())
						&& !MyTank2.IsTouchMyTank(MyTank, MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir()))
						MyTank2.MoveTank();
				}
				else MyTank2.SetDir(DIR_RIGHT);
			}
			if (GetKey(KEY_PB_BULLET))
			{
				if (MyTank2.IsLaunchBullet(1))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBullet(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "◎", MyTank2.GetDir(), BULLET_MY2, MyTank2.GetTankId());
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(1);
				}
			}
			if (GetKey(KEY_PB_BULLETGROUP))
			{
				if (MyTank2.IsLaunchBullet(5))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBulletGroup(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "◎", MyTank2.GetDir(), BULLET_MY2, MyTank2.GetTankId());
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(5);
				}
			}
			if (GetKey(KEY_PB_BULLETSPUER))
			{
				if (MyTank2.IsLaunchBullet(20))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBulletSpuer(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "◎", MyTank2.GetDir(), BULLET_MY, MyTank2.GetTankId());
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(20);
				}
			}

			if (GetKey(KEY_SPACE) || GetKey(KEY_ESC))Suspend = true;//空格键或ESC键暂停游戏
		}

		//判断PlayerA，B是否碰到电脑的炮弹
		if (MyTank.IsTouchBullet(MyTank.GetTankX(), MyTank.GetTankY(), BULLET_PC))
		{
			if (!MyTank.IsNoHurtBuffer())MyTank.SubHp(1);//减去hp
			if (MyTank.IsDie())goto PrintGameOver;
		}
		if (PlayerB && MyTank2.IsTouchBullet(MyTank2.GetTankX(), MyTank2.GetTankY(), BULLET_PC))//是否碰到电脑的炮弹
		{
			if (!MyTank2.IsNoHurtBuffer())MyTank2.SubHp(1);
			if (MyTank2.IsDie())goto PrintGameOver;
		}

		//调用AutoTank类，负责所有PC坦克的行为
		MyAutoTank.MoveTank();

		MyTank.PrintTank();//打印坦克
		MyTank.RevHp();//自动恢复HP
		MyTank.RevMp();//自动恢复MP
		if (PlayerB)
		{
			MyTank2.PrintTank();//打印坦克2
			MyTank2.RevHp();//自动恢复HP
			MyTank2.RevMp();//自动恢复MP
		}

		//炮弹打到的障碍物，炮弹移动速度
		int nOType = MyBullet.MoveBullet(10);

		//判断是否被打到基地了
		if (nOType == OBSTACLE_CAMP_A)
		{
			PrintGameOver:
			int nButton = BUTTON_RESTGAME;
			int nPage = MENU_GAMEOVERPAGE;
			PrintGameOverMenu(22,20, nButton);//打印游戏结束文字与菜单
			nButton = MessageLoop(nPage);//进入鼠标消息循环
			if (nButton == BUTTON_RESTGAME) { return nButton; }//点击 重新开始 按钮返回消息
			else if (nButton == BUTTON_BACKHOMEPAGE) { return nButton; }//点击 回到主页 按钮返回消息
		}

		//判断PlayerA，B吃到食物设置状态
		{
		int nBufferType = MyBuffer.IsTouchBuffer(MyTank.GetTankX(), MyTank.GetTankY());
		if (nBufferType ==BUFFER_ATTACK)MyTank.SetAttackBuffer();//加攻击
		if (nBufferType == BUFFER_BULLECTCD)MyTank.SetBulletCdBuffer();//无限子弹
		if (nBufferType == BUFFER_NOHURT)MyTank.SetNoHurtBuffer();//无敌
		if (nBufferType == BUFFER_STATICALL)MyTank.SetStaticAll();//冰冻敌人
		if (nBufferType == BUFFER_KILLALL)MyTank.SetKillAll();//全屏秒杀
		if (PlayerB)
		{
			nBufferType = MyBuffer.IsTouchBuffer(MyTank2.GetTankX(), MyTank2.GetTankY());
			if (nBufferType == BUFFER_ATTACK)MyTank2.SetAttackBuffer();
			if (nBufferType == BUFFER_BULLECTCD)MyTank2.SetBulletCdBuffer();
			if (nBufferType == BUFFER_NOHURT)MyTank2.SetNoHurtBuffer();
			if (nBufferType == BUFFER_STATICALL)MyTank2.SetStaticAll();
			if (nBufferType == BUFFER_KILLALL)MyTank2.SetKillAll();
		}
		}


		MyBuffer.RefreshBuffer();//固定时间随机地点刷新食物
		MyBuffer.PrintBuffer();//打印食物
		MyBullet.PrintBullet();//打印炮弹


		PrintGameTips(WALL_WIDE + 4, 33);//打印游戏操作提示，道具说明

		//打印玩家游戏信息
		{
		PrintPoint(WALL_WIDE + 3, 2, "玩家1", 0xb);
		PrintPoint(WALL_WIDE + 4, 4, "当前得分：", MyTank.GetRank(), 10);
		PrintPoint(WALL_WIDE + 4, 6, "当前HP：", MyTank.GetHp(), 12);
		PrintPoint(WALL_WIDE + 4, 7, "HP回复率：1HP/秒", 12);
		PrintPoint(WALL_WIDE + 4, 9, "当前MP：", MyTank.GetMp(), 9);
		PrintPoint(WALL_WIDE + 4, 10, "MP回复率：3MP/秒", 9);
		PrintPoint(WALL_WIDE + 4, 12, "当前攻击力：", MyTank.GetAttack(), 0xc);
		if (PlayerB)
		{
			PrintPoint(WALL_WIDE + 3, 15, "玩家2", 0xb);
			PrintPoint(WALL_WIDE + 4, 17, "当前得分：", MyTank2.GetRank(), 10);
			PrintPoint(WALL_WIDE + 4, 19, "当前HP：", MyTank2.GetHp(), 12);
			PrintPoint(WALL_WIDE + 4, 20, "HP回复率：1HP/秒", 12);
			PrintPoint(WALL_WIDE + 4, 22, "当前MP：", MyTank2.GetMp(), 9);
			PrintPoint(WALL_WIDE + 4, 23, "MP回复率：3MP/秒", 9);
			PrintPoint(WALL_WIDE + 4, 25, "当前攻击力：", MyTank2.GetAttack(), 0xc);
		}
		}
		
		//判断是否关卡模式并下一关
		if (nStage!=NULL)
		{
			char nText[255] = {0};
			int nPlayerARank = IsNextStage(nStage, MyTank.GetRank());
			int nPlayerBRank = IsNextStage(nStage, MyTank2.GetRank());

			PrintPoint(WALL_WIDE + 3, 27, "过关条件", 0xb);
			sprintf_s(nText,_countof(nText), "消灭%d个敌军", nPlayerARank > nPlayerBRank ? nPlayerBRank : nPlayerARank);

			if (MyAutoTank.IsHaveBossTank() != -1)//如果有boss
			{
				strcpy_s(nText,strlen("消灭终极BOSS")+1, "消灭终极BOSS") ;
				if (MyAutoTank.IsBossTankDie())goto NextStage;//如果boss死亡
			}
			else if (nPlayerARank <=0 || nPlayerBRank <=0)//没有boss就判断分数
			{
			NextStage:
				PrintNextStageText(18, 20, nStage);//打印恭喜过关文字
				return BUTTON_NEXTGAME;
			}
			
			PrintPoint(WALL_WIDE + 4, 28, nText, 0xa);

		}

		//是否暂停了游戏
		while (Suspend)
		{
			int nButton = BUTTON_CONTIUNEGAME;
			int nPage = MENU_GAMEPAGE;
			PrintEmptyFrame(WALL_WIDE, 48, 14, 10, 0xa, "□");//打印菜单框架
			PrintPauseMenu(WALL_WIDE + 4, 50, nButton);//打印菜单文字
			nButton = MessageLoop(nPage);//进入鼠标消息循环

			if (nButton == BUTTON_CONTIUNEGAME) { ClsPuaseMenu(WALL_WIDE + 4, 50); Suspend = false; }//点击 继续游戏 按钮，清楚菜单文字与框架
			else if (nButton == BUTTON_SAVEGAME) //点击 保存游戏 按钮
			{
				Map MyMap(false);
				MyMap.SaveGame(MyTank,MyTank2);//调用保存游戏方法
			}
			else if (nButton == BUTTON_RESTGAME) { return nButton; }//点击 重新开始 按钮，返回按钮消息
			else if (nButton == BUTTON_BACKHOMEPAGE) { return nButton;}//点击 回到主页 按钮，返回按钮消息
		}
		Sleep(10);
	}
	
}
int StartLinkGame()
{
	system("cls");

	Obstacle MyObstacle;
	Bullet MyBullet;
	Buffer MyBuffer;
	int nNowTime = clock();//强制同步时间

	//PlayerA，B初始化时的参数声明
	int nPlayerX;
	int nPlayerY;
	int nPlayerColor;
	int nPlayerDir;
	int nPlayerSpeed;
	int nBulletcd;
	int nHp;
	int nMp;
	int nRank;
	int nMaxHp;
	int nMaxMp;
	int nTankId;

	//开始载入PlayerA坦克的初始化信息
	{
	nPlayerX = pPlayerTankConfig[0].m_x;
	nPlayerY = pPlayerTankConfig[0].m_y;
	nPlayerColor = pPlayerTankConfig[0].m_color;
	nPlayerDir = pPlayerTankConfig[0].m_dir;
	nPlayerSpeed = TANK_PA_SPEED + 1;
	nBulletcd = TANK_PA_BULLETCD + 100;
	nHp = TANK_PA_HP;
	nMp = TANK_PA_MP;
	nRank = 0;
	nMaxHp = TANK_PA_MAXHP;
	nMaxMp = TANK_PA_MAXMP;
	nTankId = TankAId == 0 ? GetRandTankId() : TankAId;
	}

	Tank MyTank(nPlayerX, nPlayerY, nPlayerColor, nPlayerDir, nPlayerSpeed, nBulletcd, nHp, nMp, 1, 3, nTankId, nMaxHp, nMaxMp);//x,t,color,dir,speed,bulletcd,hp,mp,revhp,revmp
	MyTank.SetRank(nRank);

	//开始载入PlayerB坦克的初始化信息
	{
	nPlayerX = pPlayerTankConfig[1].m_x;
	nPlayerY = pPlayerTankConfig[1].m_y;
	nPlayerColor = pPlayerTankConfig[1].m_color;
	nPlayerDir = pPlayerTankConfig[1].m_dir;
	nPlayerSpeed = TANK_PB_SPEED + 1;
	nBulletcd = TANK_PB_BULLETCD + 100;
	nHp = TANK_PB_HP;
	nMp = TANK_PB_MP;
	nRank = 0;
	nMaxHp = TANK_PB_MAXHP;
	nMaxMp = TANK_PB_MAXMP;
	nTankId = TankBId == 0 ? GetRandTankId() : TankBId;
	}

	Tank MyTank2(nPlayerX, nPlayerY, nPlayerColor, nPlayerDir, nPlayerSpeed, nBulletcd, nHp, nMp, 1, 3, nTankId, nMaxHp, nMaxMp);//x,t,color,dir,speed,bulletcd,hp,mp,revhp,revmp
	MyTank2.SetRank(nRank);

	AutoTank MyAutoTank(MyTank, MyTank2, MyBullet, false);


	//打印游戏右上框，中框
	PrintEmptyFrame(WALL_WIDE, 0, 14, 30, 0xa, "□");
	PrintEmptyFrame(WALL_WIDE, 32, 14, 14, 0xa, "□");



	while (true)
	{
		//服务端接受客户数据
		if (LinkType == TCP_LINK_SERVER)
		{
			int nDataSize = MySever.RecvData(RecvMyData);
			for (int i = 0; i < nDataSize; i++)
			{
				//刷新客户端玩家的坦克移动信息
				if (RecvMyData[i].m_State == DATA_MOVE && RecvMyData[i].m_Id == TankBId)
				{
					MyTank2.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
				}
				//刷新客户端玩家的3种炮弹的信息
				else if (RecvMyData[i].m_State == DATA_BULLET && RecvMyData[i].m_Id == TankBId)
				{
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(1);
					MyBullet.SetBullet(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, "◎", RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);
				}
				else if (RecvMyData[i].m_State == DATA_BULLETGROUP && RecvMyData[i].m_Id == TankBId)
				{

					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(5);
					MyBullet.SetBulletGroup(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, "◎", RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);

				}
				else if (RecvMyData[i].m_State == DATA_BULLETSUPER && RecvMyData[i].m_Id == TankBId)
				{
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(20);
					MyBullet.SetBulletSpuer(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, "◎", RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);
				}
				//刷新客户端游戏结束信息
				else if (RecvMyData[i].m_State == DATA_GAMEOVER)
				{
					goto PrintGameOver;
				}
			}
			if (nDataSize)delete[]RecvMyData;

		}
		//客户接受服务端数据
		if (LinkType == TCP_LINK_CLIENT)
		{
			int nDataSize = MyClient.RecvData(RecvMyData);
			for (int i = 0; i < nDataSize; i++)
			{
				//刷新服务端玩家的坦克移动信息
				if (RecvMyData[i].m_State == DATA_MOVE && RecvMyData[i].m_Id == TankAId)
				{
					MyTank.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
				}
				//刷新服务端的PC坦克移动信息
				else if (RecvMyData[i].m_State == DATA_MOVE)
				{
					for (auto &val : pPcTank)//遍历所有PC坦克
					{
						if (val.GetTankId() == RecvMyData[i].m_Id)//如果坦克ID匹配相同
						{	//调用该坦克的移动函数，进行移动
							val.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
							break;
						}
					}
				}
				//刷新服务端玩家的3种炮弹信息
				else if (RecvMyData[i].m_State == DATA_BULLET && RecvMyData[i].m_Id != TankBId)
				{
					int nColor = 0xc;
					char *nData = "●";

					if (RecvMyData[i].m_Id == TankAId)
					{
						nColor = 11;
						nData = "◎";
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(1);
					}

					MyBullet.SetBullet(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, nData, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);
				}
				else if (RecvMyData[i].m_State == DATA_BULLETGROUP && RecvMyData[i].m_Id != TankBId)
				{
					char *nData = "●";
					if (RecvMyData[i].m_Id == TankAId)
					{
						nData = "◎";
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(5);
					}

					MyBullet.SetBulletGroup(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, nData, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);

				}
				else if (RecvMyData[i].m_State == DATA_BULLETSUPER && RecvMyData[i].m_Id != TankBId)
				{
					char *nData = "●";
					if (RecvMyData[i].m_Id == TankAId)
					{
						nData = "◎";
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(20);
					}

					MyBullet.SetBulletSpuer(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, nData, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);

				}
				//刷新服务端刷新的加成道具信息
				else if (RecvMyData[i].m_State == DATA_BUFFER)
				{
					MyBuffer.AddBuffer(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Type);
				}
				//刷新服务端创建的随机PC坦克信息
				else if (RecvMyData[i].m_State == DATA_CREATTANK)
				{
					MyAutoTank.CreatRandTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);
				}
				//刷新服务端游戏结束信息
				else if (RecvMyData[i].m_State == DATA_GAMEOVER)
				{
					goto PrintGameOver;
				}

				/*******************强制同步刷新，以服务端数据为准***********************/
				//刷新玩家A的坦克基本信息
				else if (RecvMyData[i].m_State == DATA_MOVE_REFRESH && RecvMyData[i].m_Id == TankAId)
				{
					MyTank.CleanTank();
					MyTank.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
					MyTank.SetHp(RecvMyData[i].m_Hp);
					MyTank.SetMp(RecvMyData[i].m_Mp);
					MyTank.SetRank(RecvMyData[i].m_Rank);
				}
				//刷新玩家B的坦克基本信息
				else if (RecvMyData[i].m_State == DATA_MOVE_REFRESH && RecvMyData[i].m_Id == TankBId)
				{
					MyTank2.CleanTank();
					MyTank2.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
					MyTank2.SetHp(RecvMyData[i].m_Hp);
					MyTank2.SetMp(RecvMyData[i].m_Mp);
					MyTank2.SetRank(RecvMyData[i].m_Rank);
				}
				//清理所有PC坦克
				else if (RecvMyData[i].m_State == DATA_CREATTANK_REFRESH_CLEAR)
				{
					MyAutoTank.ClearAllTankLink();
				}
				//刷新所有PC坦克的基本信息
				else if (RecvMyData[i].m_State == DATA_CREATTANK_REFRESH)
				{
					MyAutoTank.RefreshTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id, RecvMyData[i].m_Hp, RecvMyData[i].m_MaxHp);
				}
				//清理所有的加成道具
				else if (RecvMyData[i].m_State == DATA_BUFFER_REFRESH_CLEAR)
				{
					MyBuffer.ClearBuffer();
				}
				//刷新所有的加成道具
				else if (RecvMyData[i].m_State == DATA_BUFFER_REFRESH)
				{
					MyBuffer.AddBuffer(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Type);
				}
				//刷新草丛信息
				else if (RecvMyData[i].m_State == DATA_OBSTACLE_GRASS_REFRESH)
				{
					MyObstacle.PrintGrassPoint();
				}

			}
			if (nDataSize)delete[]RecvMyData;

		}


		//取按键状态，使用GetAsyncKeyState函数
		if (LinkType == TCP_LINK_SERVER)
			{
				if (GetKey(KEY_PA_UP))
				{
					if (MyTank.GetDir() == DIR_UP)
					{
							//判断我的坦克是否与PC坦克和PlayerB坦克重叠
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId())
							&& !MyTank.IsTouchMyTank(MyTank2, MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir()))
							MyTank.MoveTank();
					}
					else MyTank.SetDir(DIR_UP);
				}
				if (GetKey(KEY_PA_DOWN))
				{
					if (MyTank.GetDir() == DIR_DOWN)
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId())
							&& !MyTank.IsTouchMyTank(MyTank2, MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir()))
							MyTank.MoveTank();
					}
					else MyTank.SetDir(DIR_DOWN);
				}
				if (GetKey(KEY_PA_LEFT))
				{
					if (MyTank.GetDir() == DIR_LEFT)
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId())
							&& !MyTank.IsTouchMyTank(MyTank2, MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir()))
							MyTank.MoveTank();
					}
					else MyTank.SetDir(DIR_LEFT);
				}
				if (GetKey(KEY_PA_RIGHT))
				{
					if (MyTank.GetDir() == DIR_RIGHT)
					{
						if (!MyTank.IsTouchPcTank(MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir(), MyTank.GetTankId())
							&& !MyTank.IsTouchMyTank(MyTank2, MyTank.GetTankX(), MyTank.GetTankY(), MyTank.GetDir()))
							MyTank.MoveTank();
					}
					else MyTank.SetDir(DIR_RIGHT);
				}
				if (GetKey(KEY_PA_BULLET))
				{
					if (MyTank.IsLaunchBullet(1))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBullet(MyTank.GetTankX(), MyTank.GetTankY(), 11, "◎", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(1);
					}
				}
				if (GetKey(KEY_PA_BULLETGROUP))
				{
					if (MyTank.IsLaunchBullet(5))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBulletGroup(MyTank.GetTankX(), MyTank.GetTankY(), 11, "◎", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(5);
					}
				}
				if (GetKey(KEY_PA_BULLETSPUER))
				{
					if (MyTank.IsLaunchBullet(20))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBulletSpuer(MyTank.GetTankX(), MyTank.GetTankY(), 11, "◎", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(20);
					}
				}
				if (GetKey(KEY_SPACE)) {}
			}
		else if (LinkType == TCP_LINK_CLIENT)
			{
				if (GetKey(KEY_PB_UP))
				{
					if (MyTank2.GetDir() == DIR_UP)
					{
						if (!MyTank2.IsTouchPcTank(MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir(), MyTank2.GetTankId())
							&& !MyTank2.IsTouchMyTank(MyTank, MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir()))
							MyTank2.MoveTank();
					}
					else MyTank2.SetDir(DIR_UP);
				}
				if (GetKey(KEY_PB_DOWN))
				{
					if (MyTank2.GetDir() == DIR_DOWN)
					{
						if (!MyTank2.IsTouchPcTank(MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir(), MyTank2.GetTankId())
							&& !MyTank2.IsTouchMyTank(MyTank, MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir()))
							MyTank2.MoveTank();
					}
					else MyTank2.SetDir(DIR_DOWN);
				}
				if (GetKey(KEY_PB_LEFT))
				{
					if (MyTank2.GetDir() == DIR_LEFT)
					{
						if (!MyTank2.IsTouchPcTank(MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir(), MyTank2.GetTankId())
							&& !MyTank2.IsTouchMyTank(MyTank, MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir()))
							MyTank2.MoveTank();
					}
					else MyTank2.SetDir(DIR_LEFT);
				}
				if (GetKey(KEY_PB_RIGHT))
				{
					if (MyTank2.GetDir() == DIR_RIGHT)
					{
						if (!MyTank2.IsTouchPcTank(MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir(), MyTank2.GetTankId())
							&& !MyTank2.IsTouchMyTank(MyTank, MyTank2.GetTankX(), MyTank2.GetTankY(), MyTank2.GetDir()))
							MyTank2.MoveTank();
					}
					else MyTank2.SetDir(DIR_RIGHT);
				}
				if (GetKey(KEY_PB_BULLET))
				{
					if (MyTank2.IsLaunchBullet(1))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBullet(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "◎", MyTank2.GetDir(), BULLET_MY2, MyTank2.GetTankId());
						if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(1);
					}
				}
				if (GetKey(KEY_PB_BULLETGROUP))
				{
					if (MyTank2.IsLaunchBullet(5))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBulletGroup(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "◎", MyTank2.GetDir(), BULLET_MY2, MyTank2.GetTankId());
						if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(5);
					}
				}
				if (GetKey(KEY_PB_BULLETSPUER))
				{
					if (MyTank2.IsLaunchBullet(20))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBulletSpuer(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "◎", MyTank2.GetDir(), BULLET_MY, MyTank2.GetTankId());
						if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(20);
					}
				}
			}


		//判断PlayerA，B是否碰到电脑的炮弹
		if (MyTank.IsTouchBullet(MyTank.GetTankX(), MyTank.GetTankY(), BULLET_PC))
		{
			if (!MyTank.IsNoHurtBuffer())MyTank.SubHp(1);//减去hp
			if (MyTank.IsDie())
			{
				SendMyData.m_State = DATA_GAMEOVER;
				if (LinkType == TCP_LINK_SERVER)
				{
					MySever.ClearData();
					MySever.AddData(SendMyData);
					MySever.SendData();
				}
				else if (LinkType == TCP_LINK_CLIENT)
				{
					MyClient.ClearData();
					MyClient.AddData(SendMyData);
					MyClient.SendData();
				}
				goto PrintGameOver;
			}
		}
		if (MyTank2.IsTouchBullet(MyTank2.GetTankX(), MyTank2.GetTankY(), BULLET_PC))//是否碰到电脑的炮弹
		{
			if (!MyTank2.IsNoHurtBuffer())MyTank2.SubHp(1);
			if (MyTank2.IsDie())
			{
				SendMyData.m_State = DATA_GAMEOVER;
				if (LinkType == TCP_LINK_SERVER)
				{
					MySever.ClearData();
					MySever.AddData(SendMyData);
					MySever.SendData();
				}
				else if (LinkType == TCP_LINK_CLIENT)
				{
					MyClient.ClearData();
					MyClient.AddData(SendMyData);
					MyClient.SendData();
				}

				goto PrintGameOver;
			}
		}

		//调用AutoTank类，负责所有PC坦克的行为
		if (LinkType == TCP_LINK_SERVER)MyAutoTank.MoveTank();
		else if (LinkType == TCP_LINK_CLIENT) MyAutoTank.MoveTankLink();

		//恢复HP，MP，打印玩家坦克
		{
			MyTank.PrintTank();//打印坦克
			MyTank.RevHp();//自动恢复HP
			MyTank.RevMp();//自动恢复MP

			MyTank2.PrintTank();//打印坦克2
			MyTank2.RevHp();//自动恢复HP
			MyTank2.RevMp();//自动恢复MP
		}

		//炮弹打到的障碍物，炮弹移动速度
		int nOType = MyBullet.MoveBullet(15);

		//判断是否被打到基地了
		if (nOType == OBSTACLE_CAMP_A)
		{
			SendMyData.m_State = DATA_GAMEOVER;
			if (LinkType == TCP_LINK_SERVER) MySever.AddData(SendMyData);
			else if (LinkType == TCP_LINK_CLIENT) MyClient.AddData(SendMyData);
		PrintGameOver:
			int nButton = BUTTON_RESTGAME;
			int nPage = MENU_LINKGAMEOVERPAGE;
			PrintLinkGameOverMenu(22, 20, nButton);//打印游戏结束文字与菜单
			nButton = MessageLoop(nPage);//进入鼠标消息循环
			if (nButton == BUTTON_RESTGAME) { return nButton; }//点击 重新开始 按钮返回消息
			else if (nButton == BUTTON_BACKHOMEPAGE) { return nButton; }//点击 回到主页 按钮返回消息
		}

		//判断PlayerA，B吃到食物设置状态
		{
			int nBufferType = MyBuffer.IsTouchBuffer(MyTank.GetTankX(), MyTank.GetTankY());
			if (nBufferType == BUFFER_ATTACK)MyTank.SetAttackBuffer();//加攻击
			if (nBufferType == BUFFER_BULLECTCD)MyTank.SetBulletCdBuffer();//无限子弹
			if (nBufferType == BUFFER_NOHURT)MyTank.SetNoHurtBuffer();//无敌
			if (nBufferType == BUFFER_STATICALL)MyTank.SetStaticAll();//冰冻敌人
			if (nBufferType == BUFFER_KILLALL)MyTank.SetKillAll();//全屏秒杀

			nBufferType = MyBuffer.IsTouchBuffer(MyTank2.GetTankX(), MyTank2.GetTankY());
			if (nBufferType == BUFFER_ATTACK)MyTank2.SetAttackBuffer();
			if (nBufferType == BUFFER_BULLECTCD)MyTank2.SetBulletCdBuffer();
			if (nBufferType == BUFFER_NOHURT)MyTank2.SetNoHurtBuffer();
			if (nBufferType == BUFFER_STATICALL)MyTank2.SetStaticAll();
			if (nBufferType == BUFFER_KILLALL)MyTank2.SetKillAll();
		}

		//刷新食物，打印食物，打印炮弹
		{
			MyBuffer.RefreshBuffer();//固定时间随机地点刷新食物
			MyBuffer.PrintBuffer();//打印食物
			MyBullet.PrintBullet();//打印炮弹
		}

		//打印玩家游戏信息
		{
			PrintGameTips(WALL_WIDE + 4, 33);//打印游戏操作提示，道具说明

			PrintPoint(WALL_WIDE + 3, 2, "玩家1", 0xb);
			PrintPoint(WALL_WIDE + 4, 4, "当前得分：", MyTank.GetRank(), 10);
			PrintPoint(WALL_WIDE + 4, 6, "当前HP：", MyTank.GetHp(), 12);
			PrintPoint(WALL_WIDE + 4, 7, "HP回复率：1HP/秒", 12);
			PrintPoint(WALL_WIDE + 4, 9, "当前MP：", MyTank.GetMp(), 9);
			PrintPoint(WALL_WIDE + 4, 10, "MP回复率：3MP/秒", 9);
			PrintPoint(WALL_WIDE + 4, 12, "当前攻击力：", MyTank.GetAttack(), 0xc);

			PrintPoint(WALL_WIDE + 3, 15, "玩家2", 0xb);
			PrintPoint(WALL_WIDE + 4, 17, "当前得分：", MyTank2.GetRank(), 10);
			PrintPoint(WALL_WIDE + 4, 19, "当前HP：", MyTank2.GetHp(), 12);
			PrintPoint(WALL_WIDE + 4, 20, "HP回复率：1HP/秒", 12);
			PrintPoint(WALL_WIDE + 4, 22, "当前MP：", MyTank2.GetMp(), 9);
			PrintPoint(WALL_WIDE + 4, 23, "MP回复率：3MP/秒", 9);
			PrintPoint(WALL_WIDE + 4, 25, "当前攻击力：", MyTank2.GetAttack(), 0xc);
		}

		//强制同步刷新，以服务端为准，2秒一次
		if (clock() - nNowTime > 2000)
		{
			nNowTime = clock();

			MySever.ClearData();//清除所有待发包数据

			//同步所有电脑坦克
			SendMyData.m_State = DATA_CREATTANK_REFRESH_CLEAR;
			MySever.AddData(SendMyData);
			for (auto &val : pPcTank)
			{
				SendMyData.m_State = DATA_CREATTANK_REFRESH;
				SendMyData.m_X = val.GetTankX();
				SendMyData.m_Y = val.GetTankY();
				SendMyData.m_Dir = val.GetDir();
				SendMyData.m_Type = val.GetTankType();
				SendMyData.m_Id = val.GetTankId();
				SendMyData.m_Hp = val.GetHp();
				SendMyData.m_MaxHp = val.GetMaxHp();
				MySever.AddData(SendMyData);
			}

			//同步所有加成道具
			SendMyData.m_State = DATA_BUFFER_REFRESH_CLEAR;
			MySever.AddData(SendMyData);
			for (auto &val : pAllBuffer)
			{
				SendMyData.m_State = DATA_BUFFER_REFRESH;
				SendMyData.m_X = val.m_x;
				SendMyData.m_Y = val.m_y;
				SendMyData.m_Type = val.m_type;
				MySever.AddData(SendMyData);
			}

			//同步刷新草丛
			SendMyData.m_State = DATA_OBSTACLE_GRASS_REFRESH;
			MySever.AddData(SendMyData);
			MyObstacle.PrintGrassPoint();//服务端自身也刷新草丛

			//同步PlayerA坦克信息
			SendMyData.m_State = DATA_MOVE_REFRESH;
			SendMyData.m_X = MyTank.GetTankX();
			SendMyData.m_Y = MyTank.GetTankY();
			SendMyData.m_Dir = MyTank.GetDir();
			SendMyData.m_Type = MyTank.GetTankType();
			SendMyData.m_Id = MyTank.GetTankId();
			SendMyData.m_Hp = MyTank.GetHp();
			SendMyData.m_Mp = MyTank.GetMp();
			SendMyData.m_Rank = MyTank.GetRank();
			SendMyData.m_MaxHp = MyTank.GetMaxHp();
			MySever.AddData(SendMyData);

			//同步PlayerB坦克信息
			SendMyData.m_State = DATA_MOVE_REFRESH;
			SendMyData.m_X = MyTank2.GetTankX();
			SendMyData.m_Y = MyTank2.GetTankY();
			SendMyData.m_Dir = MyTank2.GetDir();
			SendMyData.m_Type = MyTank2.GetTankType();
			SendMyData.m_Id = MyTank2.GetTankId();
			SendMyData.m_Hp = MyTank2.GetHp();
			SendMyData.m_Mp = MyTank2.GetMp();
			SendMyData.m_Rank = MyTank2.GetRank();
			SendMyData.m_MaxHp = MyTank2.GetMaxHp();
			MySever.AddData(SendMyData);


		}

		//根据连接类型进行发包
		if (LinkType == TCP_LINK_SERVER)
		{
			bool nRet = MySever.SendData();
			if (!nRet)return BUTTON_BACKHOMEPAGE;
		}
		else if (LinkType == TCP_LINK_CLIENT)
		{
			bool nRet = MyClient.SendData();
			if (!nRet)return BUTTON_BACKHOMEPAGE;
		}

		Sleep(3);
	}

}

int main()
{
	system("title My Tank");
	system("mode con cols=130 lines=60");
	keybd_event(VK_SHIFT, 0, 0, 0);//取消输入法状态
	Sleep(100);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	srand((unsigned int)time(NULL));//置随机种子


	while (true)
	{
		int nButton = BUTTON_STARTGAME;//默认按钮 开始游戏
		int nPage = MENU_HOMEPAGE;//当前菜单页面 主页
		system("cls");
		PrintTilteText(13, 15);//打印主页文字
		PrintEmptyFrameEx(5, 5, 53, 25, 0xe, "□");//打印菜单框架
		PrintEmptyFrame(23, 41, 18, 12, 0xb);//打印菜单框架
		PrintTilteMenu(nButton, 30, 43);//打印主页菜单

		nButton =MessageLoop(nPage);//处理鼠标消息循环

		if (nButton == BUTTON_STARTGAME)//点击 开始游戏 按钮
		{
			nButton = BUTTON_SINGLE;//默认按钮 单人游戏
			ClsTilteMenu(30, 43);//清除菜单文字与菜单框架
			PrintEmptyFrame(23, 41, 18, 12, 0xb);//打印菜单框架
			PrintStartGameMenu(nButton, 30, 43);//打印 开始游戏 内的菜单
			nButton = MessageLoop(MENU_STARTGAMEPAGE);//处理鼠标消息循环

			if (nButton == BUTTON_SINGLE)//点击 单人游戏 按钮
			{
				Map MyMap(false);
				int nStageNum = 0;//关卡变量
				char nStage[5][255] = {
					"stage\\第一关.map",
					"stage\\第二关.map",
					"stage\\第三关.map",
					"stage\\第四关.map",
					"stage\\第五关.map"};
				for (nStageNum=0; nStageNum<_countof(nStage);nStageNum++)
				{
					bool nRet = MyMap.OpenMap(nStage[nStageNum]);//打开指定关卡地图
					if(!nRet)break;//打开失败退出循环
				ReStart_Single:
					PrintStageText(18,20,nStageNum + 1);//打印开始游戏前文字
					nButton = StartGame(nStageNum + 1);//开始游戏
					//游戏中按下 重新开始 后goto到ReStart_Single标签重新载入关卡地图
					if (nButton == BUTTON_RESTGAME) { MyMap.OpenMap(nStage[nStageNum]); goto ReStart_Single; }
					//游戏过关后返回下一关消息，不操作，直接回到循环头部进行下一次读取关卡
					else if(nButton == BUTTON_NEXTGAME){}
					//点击回到主页后 break出循环回到主页
					else if (nButton == BUTTON_BACKHOMEPAGE)break;;
				}
			}
			else if (nButton == BUTTON_PARTNER) //点击 双人游戏 按钮
			{
				Map MyMap(false);
				int nStageNum = 0;
				char nStage[5][255] = {
					"stage\\第一关双人.map",
					"stage\\第二关双人.map",
					"stage\\第三关双人.map",
					"stage\\第四关双人.map",
					"stage\\第五关双人.map" };
				for (nStageNum = 0; nStageNum < _countof(nStage); nStageNum++)
				{
					bool nRet = MyMap.OpenMap(nStage[nStageNum]);
					if (!nRet)break;
				ReStart_Parnter:
					PrintStageText(18, 20, nStageNum + 1);
					nButton = StartGame(nStageNum + 1);
					if (nButton == BUTTON_RESTGAME) { MyMap.OpenMap(nStage[nStageNum]); goto ReStart_Parnter; }
					else if (nButton == BUTTON_NEXTGAME) {}
					else if (nButton == BUTTON_BACKHOMEPAGE)break;
				}
			}
			else if (nButton == BUTTON_CUSTOMGAME)//点击 自定义游戏 按钮
			{
				Map MyMap(false);
				char *nRoute = GetRoute(ROUTE_OPENMAP);//获取指定地图文件路径
				if (nRoute != NULL)
				{
					bool bRet = MyMap.OpenMap(nRoute);//分析地图文件并载入
					if (bRet)
					{
					ReStart_OpenMap:
						PrintStageText(18, 20, NULL);//打印开始游戏前文字
						nButton = StartGame();//开始游戏
						//游戏中按下 重新开始 后goto到ReStart_OpenMap标签重新载入自定义地图
						if (nButton == BUTTON_RESTGAME) { MyMap.OpenMap(nRoute); goto ReStart_OpenMap; }
					}
				}
			}
			else if (nButton == BUTTON_LINKGAME)//点击 联机游戏 按钮
			{
				nButton = BUTTON_CREATROOM;
				ClsTilteMenu(30, 43);//清除菜单文字与菜单框架
				PrintEmptyFrame(23, 41, 18, 12, 0xb);//打印菜单框架
				PrintLinkGameMenu(nButton, 30, 43);//打印 联机游戏 内的菜单
				nButton = MessageLoop(MENU_LINKGAMEPAGE);//处理鼠标消息循环
				
				if (nButton == BUTTON_CREATROOM)//点击 创建房间 按钮
				{
					system("cls");
					bool nRet = false;
					int nPort = 6666;//声明端口号
					ReadServerIni(nPort);//从TankLink.ini配置文件中读取端口号

					nRet = MySever.IntiSever(nPort);//初始化绑定端口
					if (!nRet)continue;

					PrintPoint(1, 1, "初始化服务端成功！端口号：", nPort, 0xb);
					PrintPoint(1, 2, "等待玩家加入...", 0xb);

					while (!MySever.WaitClient())Sleep(10);//等待玩家B连接

					PrintPoint(1, 3, "玩家已加入，准备开始游戏！", 0xb);

					TankAId = GetRandTankId();//服务端生成玩家A，B坦克ID，用于识别玩家坦克操作
					TankBId = GetRandTankId();

					SendMyData.m_State = DATA_INTIDATA;//数据包类型为：初始化数据
					SendMyData.m_X = TankAId;//将坦克ID传入结构体内
					SendMyData.m_Y = TankBId;
					MySever.ClearData();
					MySever.AddData(SendMyData);//将数据包压入待发缓冲区
					MySever.SendData();//发送初始化数据给玩家B

					Sleep(1000);

					Map MyMap(false);
					nRet = MyMap.OpenMap("linkmap\\双人联机.map");//打开指定关卡地图
					if (!nRet)continue;

					LinkType = TCP_LINK_SERVER;//连接类型为 服务端
					StartLinkGame();//调用函数开始游戏
					MySever.~TCP_SERVER();//游戏结束后调用析构函数
				}
				if (nButton == BUTTON_ADDROOM)//点击 加入房间 按钮
				{
					bool nRet = false;
					char nIp[256]{};
					int nPort = 6666;
					ReadClientIni(nIp, nPort);

					system("cls");

					nRet = MyClient.IntiClient();
					if (!nRet)continue;
					PrintPoint(1, 1, "初始化客户端成功！", 0xb);

					nRet = MyClient.LineServer(nIp, nPort);
					if (!nRet)continue;
					PrintPoint(1, 2, "连接服务端成功！",  0xb);
					PrintPoint(1, 3, "等待初始化数据...", 0xb);

					while (true)
					{
						int nDataSize = MyClient.RecvData(RecvMyData);
						if (nDataSize)
						{
							if (RecvMyData->m_State == DATA_INTIDATA)//如果为初始化数据包头
							{
								TankAId = RecvMyData->m_X;
								TankBId = RecvMyData->m_Y;

								PrintPoint(1, 5, "客户端收到初始化TankAId：", TankAId, 0xa);
								PrintPoint(1, 6, "客户端收到初始化TankBId：", TankBId, 0xa);
								PrintPoint(1, 7, "初始化数据成功，准备开始游戏！", 0xa);

								delete[]RecvMyData;
								break;
							}
						}
						Sleep(10);
					}

					Sleep(1000);

					Map MyMap(false);
					nRet = MyMap.OpenMap("linkmap\\双人联机.map");//打开指定关卡地图
					if (!nRet)continue;

					LinkType = TCP_LINK_CLIENT;
					StartLinkGame();
					MyClient.~TCP_CLIENT();

				}
				else if (nButton == BUTTON_BACKHOMEPAGE)continue;//点击 回到主页 按钮

			}

			else if (nButton == BUTTON_BACKHOMEPAGE)continue;//点击 回到主页 按钮，跳出循环
		}
		else if (nButton == BUTTON_LOADGAME)//点击 载入存档 按钮
		{
			Map MyMap(false);
			char *nRoute = GetRoute(ROUTE_LOADGAME);//打开指定存档文件
			if(nRoute!=NULL)
			{
				bool bRet = MyMap.LoadGame(nRoute);//调用载入游戏方法
				if (bRet)
				{
				ReStart_LoadGame:
					nButton = StartGame();//开始游戏
					if (nButton == BUTTON_RESTGAME) { MyMap.LoadGame(nRoute); goto ReStart_LoadGame; }//游戏中按下 重新开始 后goto到ReStart_LoadGame标签重新载入自定义地图
				}
			}
		}
		else if (nButton == BUTTON_CUSTOMMAP)//点击 编辑地图 按钮
		{
			nButton = BUTTON_NEWMAP;
			ClsTilteMenu(30, 43);//清除菜单文字与菜单框架
			PrintEmptyFrame(23, 41, 18, 12, 0xb);//打印菜单框架
			PrintCustomMenu(nButton, 30, 43);//打印 编辑地图 内的菜单
			nButton = MessageLoop(MENU_CUSTOMPAGE);//处理鼠标消息循环

			if (nButton == BUTTON_NEWMAP)//点击 新建地图 按钮
			{
				system("cls");
				Map MyMap(true);
				MessageLoop(MENU_DRAWMAPPAGE);
			}
			if (nButton == BUTTON_OPENMAP)//点击 打开地图 按钮
			{
				char *nRoute = GetRoute(ROUTE_OPENMAP);
				if(nRoute!=NULL)
				{
					Map MyMap(nRoute);
					MessageLoop(MENU_EDITMAPPAGE);
				}
			}
			else if (nButton == BUTTON_BACKHOMEPAGE)continue;//点击 回到主页 按钮

		}
		else if (nButton == BUTTON_ENDGAME)break;//点击 结束游戏 按钮，break循环结束程序

	}
    return 0;
}

int MouseEvenProc(MOUSE_EVENT_RECORD mer,int nPage,Map &MyMap)
{
	static int nMouseFlags = 0;//使用静态局部变量，储存鼠标按下状态
	nMouseFlags = mer.dwButtonState;//将鼠标按下状态赋值nMouseFlags

	switch (mer.dwEventFlags)
	{
	case 0://鼠标被单击
		if (nMouseFlags == FROM_LEFT_1ST_BUTTON_PRESSED)//左键按下
		{
			int nMouseX = mer.dwMousePosition.X / 2;
			int nMouseY = mer.dwMousePosition.Y;
			int nFrame=0;
			int nButton = 0;

			switch (nPage)
			{
			case MENU_HOMEPAGE://游戏主页
				nButton = GetTilteButton(30, 43, nMouseX, nMouseY);
				return nButton;
			case MENU_STARTGAMEPAGE://开始游戏页面
				nButton = GetStartGameButton(30, 43, nMouseX, nMouseY);
				return nButton;
			case MENU_LINKGAMEPAGE://联机游戏页面
				nButton = GetLinkGameButton(30, 43, nMouseX, nMouseY);
				return nButton;
			case MENU_CUSTOMPAGE://自定义地图页面
				nButton = GetCustomButton(30, 43, nMouseX, nMouseY);
				return nButton;
			case MENU_DRAWMAPPAGE:case MENU_EDITMAPPAGE://新建地图或者编辑地图页面
				nFrame = MyMap.GetClickFrame(nMouseX, nMouseY);//取出当前绘图页面区域
				//在绘图区域点击，调用画图方法
				if (nFrame == FRAME_DRAW )MyMap.PaintMap(nMouseX, nMouseY);
				//在工具区域点击，调用打印工具方法，刷新当前选择的工具
				else if (nFrame == FRAME_TOOLS)MyMap.PrintTools(nMouseX, nMouseY);
				//在保存地图区域点击
				else if (nFrame == FRAME_SAVEINFO)
				{
					nButton = MyMap.GetButton(nMouseX, nMouseY);//取出点击了什么按钮
					MyMap.PrintButton(nButton);
					if (nButton== BUTTON_SAVEMAP)MyMap.SaveMap();//点击 保存地图 
					else return BUTTON_ENDEDITMAP;//点击退出编辑地图
				}
				break;
			case MENU_GAMEPAGE://在游戏页面
				nButton = GetPauseButton(WALL_WIDE + 4, 50, nMouseX, nMouseY);
				return nButton;
			case MENU_GAMEOVERPAGE:case MENU_LINKGAMEOVERPAGE://在游戏结束页面
				nButton = GetGameOverButton(22, 20, nMouseX, nMouseY);
				return nButton;
			}


		}
		else if (nMouseFlags == RIGHTMOST_BUTTON_PRESSED)//右键按下
		{
			int nMouseX = mer.dwMousePosition.X / 2;
			int nMouseY = mer.dwMousePosition.Y;
			int nFrame = 0;

			switch (nPage)
			{
			case MENU_DRAWMAPPAGE:case MENU_EDITMAPPAGE://新建地图或者编辑地图页面
				nFrame = MyMap.GetClickFrame(nMouseX, nMouseY);
				if (nFrame == 0)MyMap.EraseWall(nMouseX, nMouseY);//在绘图区域点击，调用擦除方法
				break;
			}
		}
		break;
	case DOUBLE_CLICK://鼠标被双击
		break;
	case MOUSE_MOVED://鼠标被移动
		int nMouseX = mer.dwMousePosition.X / 2;
		int nMouseY = mer.dwMousePosition.Y;
		int nFrame=0 ;
		int nButton = 0;

		switch (nPage)
		{
		case MENU_HOMEPAGE:
			nButton = GetTilteButton(30, 43, nMouseX, nMouseY);
			PrintTilteMenu(nButton, 30, 43);
			break;
		case MENU_STARTGAMEPAGE:
			nButton = GetStartGameButton(30, 43, nMouseX, nMouseY);
			PrintStartGameMenu(nButton, 30, 43);
			break;
		case MENU_LINKGAMEPAGE://联机游戏页面
			nButton = GetLinkGameButton(30, 43, nMouseX, nMouseY);
			PrintLinkGameMenu(nButton, 30, 43);
			break;
		case MENU_CUSTOMPAGE:
			nButton = GetCustomButton(30, 43, nMouseX, nMouseY);
			PrintCustomMenu(nButton, 30, 43);
			break;
		case MENU_DRAWMAPPAGE:case MENU_EDITMAPPAGE:
			nFrame = MyMap.GetClickFrame(nMouseX, nMouseY);
			if (nFrame== FRAME_DRAW)
			{
				if (nMouseFlags == MOUSE_LEFT)MyMap.PaintMap(nMouseX, nMouseY);
				else if (nMouseFlags == MOUSE_RIGHT)MyMap.EraseWall(nMouseX, nMouseY);
			}
			else if(nFrame == FRAME_SAVEINFO)
			{
				nButton = MyMap.GetButton(nMouseX, nMouseY);
				MyMap.PrintButton(nButton);
			}
			break;
		case MENU_GAMEPAGE:
			nButton = GetPauseButton(WALL_WIDE+4, 50, nMouseX, nMouseY);
			PrintPauseMenu(WALL_WIDE + 4, 50, nButton);
			break;
		case MENU_GAMEOVERPAGE:
			nButton = GetGameOverButton(22, 20, nMouseX, nMouseY);
			PrintGameOverMenu(22, 20, nButton);
			break;
		case MENU_LINKGAMEOVERPAGE:
			nButton = GetGameOverButton(22, 20, nMouseX, nMouseY);
			PrintLinkGameOverMenu(22, 20, nButton);
			break;
		}
	}
	return -1;
}
int MessageLoop(int nPage)//鼠标消息循环
{


	Map MyMap(nPage==MENU_DRAWMAPPAGE?true:false);
	HANDLE HND = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRcord = { 0 };
	DWORD dwRead;
	int nRet;
	SetConsoleMode(HND, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	while (1)
	{
		DWORD lpNumberOfEvents = 0;//获取输入缓冲中未读取的输入记录的个数。
		GetNumberOfConsoleInputEvents(HND, &lpNumberOfEvents);
		
		if (lpNumberOfEvents >= 1)//如果缓冲区数量大于1则进入消息判断
		{
			ReadConsoleInput(HND, &stcRcord, 1, &dwRead);//会阻塞线程
			if (stcRcord.EventType == KEY_EVENT)//如果是键盘消息
			{
				if (nPage == MENU_GAMEPAGE)	//如果当前是在 游戏页面
					if (GetKey(KEY_SPACE) || GetKey(KEY_ESC)) { Sleep(300); return BUTTON_CONTIUNEGAME; }//按下空格或者ESC就继续游戏
			}
			else if (stcRcord.EventType == MOUSE_EVENT)//如果是鼠标消息
			{
				nRet = MouseEvenProc(stcRcord.Event.MouseEvent, nPage, MyMap);
				if (nRet != -1)break;//如果返回不等于-1，就是有效的返回，跳出循环结束鼠标消息
			}
		}

		//主页动画
		if (nPage == MENU_HOMEPAGE || nPage == MENU_STARTGAMEPAGE || nPage == MENU_CUSTOMPAGE || nPage == MENU_LINKGAMEPAGE)
		{
			static int nTime = clock();
			static int nTankX[6] = { 8,32,55,55,32,8 };
			static int nTankY[6] = { 8,8,8,27,27,27 };
			static int nTankDir[6] = { DIR_RIGHT,DIR_RIGHT,DIR_DOWN,DIR_LEFT,DIR_LEFT,DIR_UP };
			static int nTankColor[6] = { 0xa,0xb,0xd,0xe,0xf,0x9 };
			if (clock() - nTime > 100)
			{
				nTime = clock();
				PrintTilteTextRandColor(13, 15);//打印主页文字

				for (int i = 0; i<_countof(nTankX); i++)PrintTank(nTankX[i], nTankY[i], "  ", 0xa, nTankDir[i]);

				for (int i = 0; i<_countof(nTankX); i++)
				{
					if (nTankX[i] >= 55 && (nTankDir[i] == DIR_RIGHT || nTankDir[i] == DIR_DOWN))
					{
						nTankY[i]++;
						nTankDir[i] = DIR_DOWN;
					}
					if (nTankY[i] >= 27 && (nTankDir[i] == DIR_LEFT || nTankDir[i] == DIR_DOWN))
					{
						nTankX[i]--;
						nTankDir[i] = DIR_LEFT;
					}
					if (nTankX[i] <= 8 && (nTankDir[i] == DIR_LEFT || nTankDir[i] == DIR_UP))
					{
						nTankY[i]--;
						nTankDir[i] = DIR_UP;
					}
					if (nTankY[i] <= 8 && (nTankDir[i] == DIR_UP || nTankDir[i] == DIR_RIGHT))
					{
						nTankX[i]++;
						nTankDir[i] = DIR_RIGHT;
					}
					PrintTank(nTankX[i], nTankY[i], "■", nTankColor[i], nTankDir[i]);
				}
			}
		}
	}
	return nRet;

};