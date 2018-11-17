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

class Map;//�Զ����ͼ��
class Tank;//̹����
class Obstacle;//�ϰ�����
class Bullet;//�ڵ���
class AutoTank;//����PC����̹����Ϊ��
class Buffer;//ʳ��ӳ���

int MessageLoop(int nPage);
int MouseEvenProc(MOUSE_EVENT_RECORD mer, int nPage,Map &MyMap);

vector <POINTDATA> pObstacle;//���������ϰ��������������
vector <POINTDATA> pBullet;//���������ڵ�������������
vector <Tank> pPcTank;//��������PC̹�˵�����
vector <POINTDATA> pAllBuffer;//�������мӳɵ�����

vector <POINTDATA> pMap;//�����Զ����ͼʱ�滭ͼ��������������
vector <POINTDATA> pPcTankConfig;//�����ȡ��ͼʱ����PC̹�˵�����������
vector <POINTDATA> pPlayerTankConfig;//�����ȡ��ͼʱ���̹�˵�����������
vector <TankInfo> pPlayerTankConfigLoad;//�����ȡ�浵ʱ���̹�˵ľ�����Ϣ
vector <POINTDATA> pBufferConfig;//���浱ǰʳ��ӳɵķֲ�������Ϣ

TCP_SERVER MySever;//����TCP�������
TCP_CLIENT MyClient;//����TCP�ͻ�����
MYDATA SendMyData{};//��ʼ���������ݽṹ��
PMYDATA RecvMyData = nullptr;//��ʼ���������ݰ��ṹ��ָ��
int LinkType = TCP_LINK_NULL;//��������Ĭ�ϵ���
int TankAId = 0;//����̹��A���ɵ�ID
int TankBId = 0;//����̹��B���ɵ�ID

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
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 8, "��", 0, OBSTACLE_WALL));
				else
				{
					if (h == 0 || h == higt)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 8, "��", 0, OBSTACLE_WALL));

					}
					else if (w == 0 || w == wide)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 8, "��", 0, OBSTACLE_WALL));
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
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 6, "��", 0, OBSTACLE_EARTHWALL));
				else
				{
					if (h == 0 || h == higt)
					{
					pObstacle.push_back(POINTDATA(w + x, h + y, 6, "��", 0, OBSTACLE_EARTHWALL));

					}
					else if (w == 0 || w == wide)
					{
					pObstacle.push_back(POINTDATA(w + x, h + y, 6, "��", 0, OBSTACLE_EARTHWALL));
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
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 9, "��", 0, OBSTACLE_WATER));
				else
				{
					if (h == 0 || h == higt)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 9, "��", 0, OBSTACLE_WATER));

					}
					else if (w == 0 || w == wide)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 9, "��", 0, OBSTACLE_WATER));
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
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 2, "��", 0, OBSTACLE_GRASS));
				else
				{
					if (h == 0 || h == higt)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 2, "��", 0, OBSTACLE_GRASS));

					}
					else if (w == 0 || w == wide)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 2, "��", 0, OBSTACLE_GRASS));
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
				if (fill) pObstacle.push_back(POINTDATA(w + x, h + y, 14, "��", 0, type));
				else
				{
					if (h == 0 || h == higt)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 14, "��", 0, type));

					}
					else if (w == 0 || w == wide)
					{
						pObstacle.push_back(POINTDATA(w + x, h + y, 14, "��", 0, type));
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
	int m_bulletmovtime = clock();//�ڵ��ƶ�ʱ���¼
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
		//������Ϸ����
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
			dir = DIR_UP;//���Ϸ�3���ڵ�
			//�ж��ڵ�������������û���ϰ����������
			nOType[0] = GetObstacleType(x - 1, y - 2, dir);
			nOType[1] = GetObstacleType(x, y - 2, dir);
			nOType[2] = GetObstacleType(x + 1, y - 2, dir);
			//�ж��Ƿ��������λ�÷����ڵ�
			if (IsSetBullet(nOType[0]))
				pBullet.push_back(POINTDATA(x - 1, y - 2, color, data, dir, type));
			if (IsSetBullet(nOType[1]))
				pBullet.push_back(POINTDATA(x, y - 2, color, data, dir, type));
			if (IsSetBullet(nOType[2]))
				pBullet.push_back(POINTDATA(x + 1, y - 2, color, data, dir, type));

			dir = DIR_DOWN;//���·�3���ڵ�
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

		for (auto &val: pBullet)//�������л���ڵ�
		{
				if (val.m_dir == DIR_UP)//�������ж�
				{
					val.m_y--;//�ƶ��ڵ�������
					//ȡ���ڵ�����λ����ʲô�ϰ���
					nOType = GetObstacleType(val.m_x, val.m_y, val.m_dir);
					if (nOType==OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)
					{	//����Ǹ�ǽ����ǽ������
						PrintPoint(val.m_x, val.m_y + 1, "  ", val.m_color);//����ǰһ���ӵ�����ͼ��
						RefreshObstacle(val.m_x, val.m_y + 1);//ˢ�±������ĲݴԺͺ���
						val.m_type = BULLET_CLEAN;//���ӵ�ɾ��
						//����򵽻��ؾ�ֱ�ӷ��أ���������Ϸ��
						if (nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return nOType;
					}
					else if(nOType==OBSTACLE_WATER || nOType == OBSTACLE_GRASS)
					{	//�����ˮ���߲ݴ�
						PrintPoint(val.m_x, val.m_y+1, "  ", val.m_color);//����ǰһ���ӵ�����ͼ��
						RefreshObstacle(val.m_x, val.m_y + 1);//ˢ�±������Ĳݴ������
					}
					else if (nOType == OBSTACLE_NULL)
					{	//���ǰ��û���ϰ���
						PrintPoint(val.m_x, val.m_y + 1, "  ", val.m_color);//����ǰһ���ӵ�����ͼ��
						RefreshObstacle(val.m_x, val.m_y + 2);//ˢ�±������Ĳݴ������
					}
				}

				else if (val.m_dir == DIR_DOWN)
				{
					val.m_y++;
					nOType = GetObstacleType(val.m_x, val.m_y, val.m_dir);
					if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)
					{
						PrintPoint(val.m_x, val.m_y-1, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x, val.m_y - 1);//ˢ�±�������Obstacle
						val.m_type = BULLET_CLEAN;

						if (nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return nOType;

					}
					else if (nOType == OBSTACLE_WATER || nOType == OBSTACLE_GRASS)
					{
						PrintPoint(val.m_x, val.m_y - 1, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x, val.m_y - 1);//ˢ�±�������Obstacle
					}
					else if (nOType == OBSTACLE_NULL)
					{
						PrintPoint(val.m_x, val.m_y - 1, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x, val.m_y - 2);//ˢ�±�������Obstacle
					}
				}

				else if (val.m_dir == DIR_LEFT)
				{
					val.m_x--;
					nOType = GetObstacleType(val.m_x, val.m_y, val.m_dir);
					if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)
					{
						PrintPoint(val.m_x+1, val.m_y, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x + 1, val.m_y);//ˢ�±�������Obstacle
						val.m_type = BULLET_CLEAN;

						if (nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return nOType;

					}
					else if (nOType == OBSTACLE_WATER || nOType == OBSTACLE_GRASS)
					{
						PrintPoint(val.m_x + 1, val.m_y, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x + 1, val.m_y);//ˢ�±�������Obstacle
					}
					else if (nOType == OBSTACLE_NULL)
					{
						PrintPoint(val.m_x + 1, val.m_y, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x + 2, val.m_y);//ˢ�±�������Obstacle
					}
				}

				else if (val.m_dir == DIR_RIGHT)
				{
					val.m_x++;
					nOType = GetObstacleType(val.m_x, val.m_y, val.m_dir);
					if (nOType == OBSTACLE_WALL || nOType == OBSTACLE_EARTHWALL || nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)
					{
						PrintPoint(val.m_x-1, val.m_y, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x - 1, val.m_y);//ˢ�±�������Obstacle
						val.m_type = BULLET_CLEAN;

						if (nOType >= OBSTACLE_CAMP_A && nOType <= OBSTACLE_CAMP_PC)return nOType;

					}
					else if (nOType == OBSTACLE_WATER || nOType == OBSTACLE_GRASS)
					{
						PrintPoint(val.m_x - 1, val.m_y, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x - 1, val.m_y);//ˢ�±�������Obstacle
					}
					else if (nOType == OBSTACLE_NULL)
					{
						PrintPoint(val.m_x - 1, val.m_y, "  ", val.m_color);//����ǰһ������ͼ��
						RefreshObstacle(val.m_x - 2, val.m_y);//ˢ�±�������Obstacle
					}
				}

				//valΪ��ǰ�ӵ���val2����ȫ���ӵ�
				for (auto &val2 : pBullet)
				{	//�жϵ�ǰ�ӵ�������������ӵ����꣬�ų�����������ʱ���ж�time
					if (val.m_x == val2.m_x && val.m_y == val2.m_y && val.m_time != val2.m_time)
					{
						val.m_type = BULLET_CLEAN;//���Ϊɾ������
						val2.m_type = BULLET_CLEAN;//���Ϊɾ������
					}
				}
		}

		//Ҫ�õ���ѭ������Ȼɾ����ʱ���Ӱ������жϵ�����Ԫ��λ��
		for (int i = pBullet.size() - 1; i >= 0; i--)
		{
			if (pBullet[i].m_type == BULLET_CLEAN) pBullet.erase(pBullet.begin() + i);
		}


		return nOType;

	}
	int GetObstacleType(int x, int y,int dir)//�Ƿ���ϰ�����
	{
		for (int i= pObstacle.size()-1;i>=0;i--)
		{
			if (pObstacle[i].m_x == x && pObstacle[i].m_y == y)
			{
				if (pObstacle[i].m_type == OBSTACLE_EARTHWALL)//����ǽ������
				{
					pObstacle.erase(pObstacle.begin() + i);
					PrintPoint(x, y, "  ", 6);
					EraserEarthWall(x,y, dir);//�����ڵ����ڵ�������ǽ
					return OBSTACLE_EARTHWALL;
				}
				else if (pObstacle[i].m_type == OBSTACLE_WATER)//��ˮ
				{
					return OBSTACLE_WATER;
				}
				else if (pObstacle[i].m_type == OBSTACLE_WALL)//��ǽ
				{
					return OBSTACLE_WALL;
				}
				else if (pObstacle[i].m_type == OBSTACLE_GRASS)//�򵽲�
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
			if (pObstacle[i].m_type == OBSTACLE_EARTHWALL)//����ǽ������
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
	int m_foodcd = 5000;//ms��λ
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
			data = "��";
			ncolor = 0xc;
			break;
		case BUFFER_BULLECTCD:
			data = "��";
			ncolor = 0x9;
			break;
		case BUFFER_NOHURT:
			data = "��";
			ncolor = 0xc;
			break;
		case BUFFER_STATICALL:
			data = "��";
			ncolor = 0x9;
			break;
		case BUFFER_KILLALL:
			data = "�d";
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
	int m_x=2;//̹������
	int m_y=2;

	int m_maxhp = 10;//̹�����hp
	int m_maxmp = 100;//̹�����mp

	int m_hp = 10;//̹��hp
	int m_mp = 100;//̹��mp

	int m_revhp = 1;//ÿ�λָ�Hp
	int m_revmp = 3;//ÿ�λָ�Mp

	int m_hp_time = 1;//�ָ�ʱ��1��
	int m_mp_time = 1;//�ָ�ʱ��1��

	int m_dir = 0;//̹�˷���
	int m_color = 0;//̹����ɫ
	int m_tankid = 0;//̹��id
	int m_rank = 0;//̹�˼Ʒ�

	int m_speed = 1;//̹���ٶ�
	int m_speed_time = 1;

	int m_bulletcd = 100;//ms �����ӳ�
	int m_bullet_time = 0;

	int m_attack = 1;//������
	int m_attack_buff = 10000;//ms
	int m_attack_time = 0;

	int m_bullercd_buff = 10000;//ms
	int m_bullercd_time = 0;

	int m_nohurt_buff = 10000;//ms
	int m_nohurt_time = 0;
	char *m_tank_data="��";
	char m_tank_hp_data[3] = "��";

	int m_staticall_buff = 5000;//ms
	int m_staticall_time = 0;

	int m_killall_buff = 1000;//ms
	int m_killall_time = 0;

	int m_Ai = 10000;//ms
	int m_Ai_time = 0;
	int m_Ai_type = TANK_PA;

	vector <POINTXY> pPointXY;//����A���㷨·������

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
	{	//��¼��ǰʱ��
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
		m_tank_data = "��";//̹����ͨ״̬��ʵ��������
		//�������ʱ��С������ʱ���򷵻�false
		if (clock() <= m_nohurt_buff)return false;
		//�������ʱ���ȥ�����޵е�ʱ�䣬����10���ӣ�����false
		else if (clock() - m_nohurt_time > m_nohurt_buff)return false;
		m_tank_data = "��";//�����޸�Ϊ���������α�ʾ�޵�
		return true;//����true
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

		SendMyData.m_State = DATA_MOVE;//���ݰ�����Ϊ���ƶ�
		SendMyData.m_X = m_x;//��������
		SendMyData.m_Y = m_y;
		SendMyData.m_Dir = m_dir;//���뷽��
		SendMyData.m_Id = m_tankid;//�����ƶ���̹��ID
		//���ṹ���ڵ����ݼ����������
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
			PrintPoint(m_x - 1, m_y - 1, ncolor[0] == 2 ? "��" : "  ", ncolor[0]);
			PrintPoint(m_x, m_y - 1, ncolor[1] == 2 ? "��" : m_tank_data, ncolor[1]);
			PrintPoint(m_x + 1, m_y - 1, ncolor[2] == 2 ? "��" : "  ", ncolor[2] );


			PrintPoint(m_x - 1, m_y, ncolor[3] == 2 ? "��" : m_tank_data, ncolor[3]);
			PrintPoint(m_x, m_y, ncolor[4] == 2 ? "��" : m_tank_hp_data, ncolor[4]);
			PrintPoint(m_x + 1, m_y, ncolor[5] == 2 ? "��" : m_tank_data, ncolor[5]);


			PrintPoint(m_x - 1, m_y + 1, ncolor[6] == 2 ? "��" : m_tank_data, ncolor[6]);
			PrintPoint(m_x, m_y + 1, ncolor[7] == 2 ? "��" : "  ", ncolor[7]);
			PrintPoint(m_x + 1, m_y + 1, ncolor[8] == 2 ? "��" : m_tank_data, ncolor[8]);

			nX[0] = m_x - 1;
			nX[1] = m_x ;
			nX[2] = m_x + 1;
			nY[0] = m_y + 2;
			nY[1] = m_y + 2;
			nY[2] = m_y + 2;
			RefreshObstacle(nX, nY,_countof(nX));//ˢ�±�������Obstacle


		}
		else if (m_dir == DIR_DOWN)
		{
			HideTank( ncolor);

			PrintPoint(m_x - 1, m_y - 1, ncolor[0] == 2 ? "��" : m_tank_data, ncolor[0]);
			PrintPoint(m_x, m_y - 1, ncolor[1] == 2 ? "��" : "  ", ncolor[1]);
			PrintPoint(m_x + 1, m_y - 1, ncolor[2] == 2 ? "��" : m_tank_data, ncolor[2]);

			PrintPoint(m_x - 1, m_y, ncolor[3] == 2 ? "��" : m_tank_data, ncolor[3]);
			PrintPoint(m_x, m_y, ncolor[4] == 2 ? "��" : m_tank_hp_data, ncolor[4]);
			PrintPoint(m_x + 1, m_y, ncolor[5] == 2 ? "��" : m_tank_data, ncolor[5]);

			PrintPoint(m_x - 1, m_y + 1, ncolor[6] == 2 ? "��" : "  ", ncolor[6]);
			PrintPoint(m_x, m_y + 1, ncolor[7] == 2 ? "��" : m_tank_data, ncolor[7]);
			PrintPoint(m_x + 1, m_y + 1, ncolor[8] == 2 ? "��" : "  ", ncolor[8]);

			nX[0] = m_x - 1;
			nX[1] = m_x;
			nX[2] = m_x + 1;
			nY[0] = m_y - 2;
			nY[1] = m_y - 2;
			nY[2] = m_y - 2; 
			RefreshObstacle(nX, nY, _countof(nX));//ˢ�±�������Obstacle

		}
		else if (m_dir == DIR_LEFT)
		{
			HideTank( ncolor);
			PrintPoint(m_x - 1, m_y - 1, ncolor[0] == 2 ? "��" : "  ", ncolor[0]);
			PrintPoint(m_x, m_y - 1, ncolor[1] == 2 ? "��" : m_tank_data, ncolor[1]);
			PrintPoint(m_x + 1, m_y - 1, ncolor[2] == 2 ? "��" : m_tank_data, ncolor[2]);

			PrintPoint(m_x - 1, m_y, ncolor[3] == 2 ? "��" : m_tank_data, ncolor[3]);
			PrintPoint(m_x, m_y, ncolor[4] == 2 ? "��" : m_tank_hp_data, ncolor[4]);
			PrintPoint(m_x + 1, m_y, ncolor[5] == 2 ? "��" : "  ", ncolor[5]);

			PrintPoint(m_x - 1, m_y + 1, ncolor[6] == 2 ? "��" : "  ", ncolor[6]);
			PrintPoint(m_x, m_y + 1, ncolor[7] == 2 ? "��" : m_tank_data, ncolor[7]);
			PrintPoint(m_x + 1, m_y + 1, ncolor[8] == 2 ? "��" : m_tank_data, ncolor[8]);

			nX[0] = m_x +2;
			nX[1] = m_x + 2;
			nX[2] = m_x + 2;
			nY[0] = m_y - 1;
			nY[1] = m_y ;
			nY[2] = m_y +1;
			RefreshObstacle(nX, nY, _countof(nX));//ˢ�±�������Obstacle

		}
		else if (m_dir == DIR_RIGHT)
		{
			HideTank( ncolor);
			PrintPoint(m_x - 1, m_y - 1, ncolor[0] == 2 ? "��" : m_tank_data, ncolor[0]);
			PrintPoint(m_x, m_y - 1, ncolor[1] == 2 ? "��" : m_tank_data, ncolor[1]);
			PrintPoint(m_x + 1, m_y - 1, ncolor[2] == 2 ? "��" : "  ", ncolor[2]);

			PrintPoint(m_x - 1, m_y, ncolor[3] == 2 ? "��" : "  ", ncolor[3]);
			PrintPoint(m_x, m_y, ncolor[4] == 2 ? "��" : m_tank_hp_data, ncolor[4]);
			PrintPoint(m_x + 1, m_y, ncolor[5] == 2 ? "��" : m_tank_data, ncolor[5]);

			PrintPoint(m_x - 1, m_y + 1, ncolor[6] == 2 ? "��" : m_tank_data, ncolor[6]);
			PrintPoint(m_x, m_y + 1, ncolor[7] == 2 ? "��" : m_tank_data, ncolor[7]);
			PrintPoint(m_x + 1, m_y + 1, ncolor[8] == 2 ? "��" : "  ", ncolor[8]);

			nX[0] = m_x - 2;
			nX[1] = m_x - 2;
			nX[2] = m_x - 2;
			nY[0] = m_y - 1;
			nY[1] = m_y;
			nY[2] = m_y + 1;
			RefreshObstacle(nX, nY, _countof(nX));//ˢ�±�������Obstacle

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

			if (pPointXY.size() <= 6) //����С��6�㣬���ƶ�
			{
				pPointXY.clear();
				vector <POINTXY>().swap(pPointXY);
				return MOVE_NO;
			}

			pPointXY.pop_back();//ȥ�������������
				
			if (pPointXY.size()>=6)//ȥ������Ŀ��6�����꣬���־���
			{
				pPointXY.erase(pPointXY.begin());
				pPointXY.erase(pPointXY.begin());
				pPointXY.erase(pPointXY.begin());
				pPointXY.erase(pPointXY.begin());
				pPointXY.erase(pPointXY.begin());
			}
		}

		if (pPointXY.size() > 0)//�����·����ı䷽��
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


		CleanTank();//����̹�˿�ʼ�ƶ�
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
		vector <POINTSSS> pOpenPoint;//���б�
		vector <POINTSSS> pClosePoint;//�ر��б�
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

	int m_tankcd_time = 3000;//������̹�˵�cd
	int m_addtank_time;
	vector <POINTDATA> pPcTankTeam;//��ǰ��Ҫ���ɵ�̹����Ϣ

public:
	AutoTank(Tank &tank, Tank &tank2, Bullet &bullet,bool loading) : MyTank(tank), MyTank2(tank2), MyBullet(bullet), m_addtank_time(clock())
	{ 
		if (LinkType == TCP_LINK_CLIENT)return;//���Ϊ������Ϸ�ͻ��ˣ�����ʼ������̹���б�

		pPcTankTeam.assign(pPcTankConfig.begin(), pPcTankConfig.end());//vector��ֵ
		
		if (IsHaveBossTank() != -1)m_tanknum = pPcTankTeam.size() - 1;//�����boos�ͼ�ȥ1����boosԤ��1��λ��
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
	{//�����ض�����̹��
		pPcTank.push_back(Tank(x, y, color, dir, m_speed, bulletcd, hp, mp, revhp, revmp,tankid, maxhp, maxmp));
	}
	void CreatRandTank()
	{
		if (clock() - m_addtank_time <= m_tankcd_time)return;
		m_addtank_time = clock();


		//�������̹��
		int nTankNum = m_creatboss ? pPcTank.size() - 1 : pPcTank.size();
		if (nTankNum < m_tanknum)//̹�������Ƿ�ﵽ
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
				
				switch (pPcTankTeam[nRandTank].m_type)//�ж�Ҫ���ɵ�̹������
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
				//���ɵĵ��Ƿ����ϰ������̹���赲���еĻ��ͷ��أ��ȴ�3������ж�����
				if (!IsObstacleNull(pPcTankTeam[nRandTank].m_x, pPcTankTeam[nRandTank].m_y))return;

			} while (!IsObstacleNull(pPcTankTeam[nRandTank].m_x, pPcTankTeam[nRandTank].m_y));

			//����һ���µ�̹��
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
		if (m_creatboss)return;//�Ѿ�������boss�ͷ���
		int nBossTankWeak = IsHaveBossTank();//�Ƿ�����Ҫ����boss��Ϣ
		if (IsHaveBossTank() == -1) { m_creatboss = true; return; }//���û����Ҫ����boss��Ϣ���ͷ���

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
			if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)continue;//ȫ����ɱ��boss��Ч

			pPcTank[i].CleanTank();

			int tX = pPcTank[i].GetTankX();
			int tY = pPcTank[i].GetTankY();

			int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
			int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
			pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));//ˢ�²ݴ�

			pPcTank.erase(pPcTank.begin() + i);
			if (tanktype == 0)MyTank.AddRank();
			else MyTank2.AddRank();
		}
		m_addtank_time = clock() - 5000;
	}
	
	//�ͻ��˽��շ���������½�̹��
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
	//�ͻ��˽��շ��������ǿ��ͬ��ˢ��̹��
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
	//�ͻ��˽��շ���������������̹��
	void ClearAllTankLink()
	{
		for (int i = pPcTank.size() - 1; i >= 0; i--)
		{
			pPcTank[i].CleanTank();

			int tX = pPcTank[i].GetTankX();
			int tY = pPcTank[i].GetTankY();

			int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
			int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
			pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));//ˢ�²ݴ�

			pPcTank.erase(pPcTank.begin() + i);
		}
		m_addtank_time = clock() - 5000;
	}

	//��������PC̹�˵���Ϊ
	void MoveTank()
	{//��������PC̹�˵���Ϊ

		if (MyTank.IsKillAllBuffer()) { ClearAllTank(0); return; }//���PlayerA����ȫ����ɱbuff�����ȫ��̹��
		if (MyTank2.IsKillAllBuffer()) { ClearAllTank(1); return; }//���PlayerB����ȫ����ɱbuff�����ȫ��̹��

		for (int i = pPcTank.size() - 1; i >= 0; i--)
		{	//������˱���buff�������ƶ�����
			if (MyTank.IsStaticAllBuffer()|| MyTank2.IsStaticAllBuffer())goto DontMove;
			//����PC̹�������Ϊ
			int nGet = GetRandNum(ACTION_DIR, ACTION_MOV + 3);
			//�ı䷽��
			if (nGet == ACTION_DIR)
			{
				pPcTank[i].SetDir(GetRandNum(DIR_UP + 1, DIR_RIGHT + 1) - 1);//�����������
			}
			//�����ڵ�
			else if (nGet == ACTION_ATTACK)
			{
				if (pPcTank[i].IsLaunchBullet(1))
					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)//�����boss
					{
						int nBulletType = GetRandNum(1,5);//��������ķ�������
						if (nBulletType==1) MyBullet.SetBulletGroup(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), 0x9, "��", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
						else MyBullet.SetBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), 0x9, "��", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
					}
					else MyBullet.SetBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), 12, "��", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());

					
			}
			//�ƶ�
			else
			{
			//ReStart://�Ƿ�̹���ص�

				if (!pPcTank[i].IsTouchPcTank(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), pPcTank[i].GetDir(), pPcTank[i].GetTankId())
					&& !pPcTank[i].IsTouchMyTank(MyTank, pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), pPcTank[i].GetDir())
					&& !pPcTank[i].IsTouchMyTank(MyTank2, pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), pPcTank[i].GetDir()))
				{
					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP && pPcTank[i].IsAi())//�����boss���ж��Ƿ�Ai״̬
					{
						int nEndX = pPcTank[i].GetAiType()==TANK_PA? MyTank.GetTankX(): MyTank2.GetTankX();//�Ƿ�ΪAi״̬���Զ�Ѱ�������ΪĿ�꣩
						int nEndY = pPcTank[i].GetAiType() == TANK_PA ? MyTank.GetTankY() : MyTank2.GetTankY();//�Ƿ�ΪAi״̬���Զ�Ѱ�������ΪĿ�꣩
						int nMov = pPcTank[i].MoveTankAi(nEndX, nEndY);//�ƶ��󷵻��ƶ����
						if (nMov == MOVE_NO)//����ڷ�Χ�ڲ�ʹ��A��Ѱ·����ԭ�ط���
						{
							int nButtle = pPcTank[i].IsBulletCdBuffer();
							if(!nButtle)MyBullet.SetBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), 0x9, "��", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
						}

					}
					else pPcTank[i].MoveTank();//��̹ͨ������ƶ�
					
				}
				else//�ص���ı䷽�������ж��Ƿ�̹���ص�
				{
					pPcTank[i].SetDir(GetRandNum(DIR_UP + 1, DIR_RIGHT + 1) - 1);//�����������
					//goto ReStart;
				}
			}

		DontMove:
			//�Ƿ��������A���ڵ�
			if (pPcTank[i].IsTouchBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), BULLET_MY))
			{
				if (pPcTank[i].GetMaxHp() == TANK_PC5_HP || pPcTank[i].GetMaxHp() == TANK_PC4_HP)//�����boss������������
				{
					int nBulletColor = pPcTank[i].GetMaxHp() == TANK_PC5_HP ? 0x9 : 0xc;//�ж��ڵ���ɫ
					MyBullet.SetBulletSpuer(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), nBulletColor, "��", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());//�����ڵ�

					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)pPcTank[i].SetAi(TANK_PA);//�����boss������10��Ai״̬��Ѱ�����
				}

				pPcTank[i].SubHp(MyTank.GetAttack());//��ȥhp
				if (pPcTank[i].IsDie())//�ж��Ƿ�����
				{
					MyTank.AddRank();//�ӷ�
					pPcTank[i].CleanTank();//����PC̹��

					int tX = pPcTank[i].GetTankX();
					int tY = pPcTank[i].GetTankY();

					int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
					int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
					pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));//����ڲݴԣ���ˢ�²ݴ�ͼ��

					pPcTank.erase(pPcTank.begin() + i);//����̹�˳�Ա
					continue;
				}
			}
			//�Ƿ��������B���ڵ�
			if (pPcTank[i].IsTouchBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), BULLET_MY2))
			{
				if (pPcTank[i].GetMaxHp() == TANK_PC5_HP || pPcTank[i].GetMaxHp() == TANK_PC4_HP)
				{
					int nBulletColor = pPcTank[i].GetMaxHp() == TANK_PC5_HP ? 0x9 : 0xc;
					MyBullet.SetBulletSpuer(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), nBulletColor, "��", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)pPcTank[i].SetAi(TANK_PB);

				}

				pPcTank[i].SubHp(MyTank2.GetAttack());//��ȥhp
				if (pPcTank[i].IsDie())//�ж��Ƿ�����
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
			pPcTank[i].PrintTank();//��ӡ̹��
		}

		if (!MyTank.IsStaticAllBuffer()||!MyTank2.IsStaticAllBuffer())CreatRandTank();//������Ƕ���״̬�����ú��������µ�̹��
		
	}
	void MoveTankLink()
	{//��������PC̹�˵���Ϊ

		if (MyTank.IsKillAllBuffer()) { ClearAllTank(0); return; }//���PlayerA����ȫ����ɱbuff�����ȫ��̹��
		if (MyTank2.IsKillAllBuffer()) { ClearAllTank(1); return; }//���PlayerB����ȫ����ɱbuff�����ȫ��̹��

		for (int i = pPcTank.size() - 1; i >= 0; i--)
		{
			if (pPcTank[i].IsTouchBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), BULLET_MY))//�Ƿ������ҵ��ڵ�
			{
				if (pPcTank[i].GetMaxHp() == TANK_PC5_HP || pPcTank[i].GetMaxHp() == TANK_PC4_HP)//�����boss������������
				{
					int nBulletColor = pPcTank[i].GetMaxHp() == TANK_PC5_HP ? 0x9 : 0xc;//�ж��ڵ���ɫ
					MyBullet.SetBulletSpuer(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), nBulletColor, "��", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());//�����ڵ�

					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)pPcTank[i].SetAi(TANK_PA);//�����boss������10��Ai״̬��Ѱ�����
				}

				pPcTank[i].SubHp(MyTank.GetAttack());//��ȥhp
				if (pPcTank[i].IsDie())//�ж��Ƿ�����
				{
					MyTank.AddRank();//�ӷ�
					pPcTank[i].CleanTank();//����PC̹��

					int tX = pPcTank[i].GetTankX();
					int tY = pPcTank[i].GetTankY();

					int nX[9] = { tX - 1,tX,tX + 1,tX - 1,tX,tX + 1 ,tX - 1,tX,tX + 1 };
					int nY[9] = { tY - 1,tY - 1 ,tY - 1 ,tY,tY,tY,tY + 1,tY + 1 ,tY + 1 };
					pPcTank[i].RefreshObstacle(nX, nY, _countof(nX));//����ڲݴԣ���ˢ�²ݴ�ͼ��

					pPcTank.erase(pPcTank.begin() + i);//����̹�˳�Ա
					continue;
				}
			}
			if (pPcTank[i].IsTouchBullet(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), BULLET_MY2))//�Ƿ������ҵ��ڵ�
			{
				if (pPcTank[i].GetMaxHp() == TANK_PC5_HP || pPcTank[i].GetMaxHp() == TANK_PC4_HP)
				{
					int nBulletColor = pPcTank[i].GetMaxHp() == TANK_PC5_HP ? 0x9 : 0xc;
					MyBullet.SetBulletSpuer(pPcTank[i].GetTankX(), pPcTank[i].GetTankY(), nBulletColor, "��", pPcTank[i].GetDir(), BULLET_PC, pPcTank[i].GetTankId());
					if (pPcTank[i].GetMaxHp() == TANK_PC5_HP)pPcTank[i].SetAi(TANK_PB);

				}

				pPcTank[i].SubHp(MyTank2.GetAttack());//��ȥhp
				if (pPcTank[i].IsDie())//�ж��Ƿ�����
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
			pPcTank[i].PrintTank();//��ӡ̹��
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
			CreatBasicWall(WALL_WIDE, WALL_HIGH, false);//��ӡ��ͼ�����ܻ���ǽ��
			PrintEmptyFrame(WALL_WIDE, 0, 14, 50, 0xa, "��");//��ӡ���߿�
			PrintEmptyFrame(WALL_WIDE, 52, 14, 6, 0xa, "��");//��ӡ�����

			PrintTools();//��ӡ�ϰ���ѡ��
			PrintPaintTips();//��ӡ������ʾ
			PrintButton();//��ӡ������˳���ť
			PrintBasicWallPoint();
		}
	}
	Map(bool creatbasic)
	{
		if (!creatbasic)return;

		CreatBasicWall(WALL_WIDE, WALL_HIGH, true);//��ӡ��ͼ�����ܻ���ǽ��
		PrintEmptyFrame(WALL_WIDE, 0, 14, 50, 0xa, "��");//��ӡ���߿�
		PrintEmptyFrame(WALL_WIDE, 52, 14, 6, 0xa, "��");//��ӡ�����

		PrintTools();//��ӡ�ϰ���ѡ��
		PrintPaintTips();//��ӡ������ʾ
		PrintButton();//��ӡ������˳���ť
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
		PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

		PrintPoint(px, py + 2, "��   �� ����ǽ��  ", 0x8);
		PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
		PrintPoint(px, py + 6, "   �� ����  ", 0x9);
		PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
		PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
		PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
		PrintPoint(px, py + 14, "������", 0xc);
		PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
		PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
		PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
		PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
		PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);

		PrintPoint(px, py + 26, "�����", 0xc);
		PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
		PrintPoint(px, py + 30, "   �� ���2  ", 0xa);

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
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "��   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa); 
			break;

		case 1:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "��   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa); 
			break;

		case 2:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "��   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 3:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "��   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 4:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "��   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 5:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "��   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 6:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "��   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 7:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "��   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 8:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "��   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 9:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "��   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 10:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "��   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "   �� ���2  ", 0xa);
			break;
		case 11:
			PrintPoint(px, py, "��ͼ�ϰ���", 0xc);

			PrintPoint(px, py + 2, "   �� ����ǽ��  ", 0x8);
			PrintPoint(px, py + 4, "   �� ����ǽ��  ", 0x6);
			PrintPoint(px, py + 6, "   �� ����  ", 0x9);
			PrintPoint(px, py + 8, "   �� �ݴ�  ", 0x2);
			PrintPoint(px, py + 10, "��Ʒ�ӳ���", 0xc);
			PrintPoint(px, py + 12, "   �d �����Ʒ  ", 0xc);
			PrintPoint(px, py + 14, "������", 0xc);
			PrintPoint(px, py + 16, "   �� ��������  ", 0xf);
			PrintPoint(px, py + 18, "   �� �м�����  ", 0xe);
			PrintPoint(px, py + 20, "   �� �߼�����  ", 0xd);
			PrintPoint(px, py + 22, "   �� ��������  ", 0xc);
			PrintPoint(px, py + 24, "   �� �ռ�BOSS  ", 0x9);
			PrintPoint(px, py + 26, "�����", 0xc);
			PrintPoint(px, py + 28, "   �� ���1  ", 0xb);
			PrintPoint(px, py + 30, "��   �� ���2  ", 0xa);
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
		PrintPoint(px, py, "�滭����", 0xc);

		PrintPoint(px, py + 2, "�� �Ҳ�ѡ�����", 0xa);
		PrintPoint(px, py + 4, "�� ����������ͼ", 0xa);
		PrintPoint(px, py + 6, "�� ������Ҽ�����", 0xa);

	}
	void PrintButton()
	{
		int px = WALL_WIDE+3;
		int py = 54;
		PrintPoint(px, py, "���浽��ͼ    ", 0xa);
		PrintPoint(px, py + 2, "�˳��Զ���༭", 0x2);

	}
	void PrintButton(int nButton)
	{
		int px = WALL_WIDE+3;
		int py = 54;

		switch (nButton)
		{
		case BUTTON_SAVEMAP:
			PrintPoint(px, py, "���浽��ͼ    ", 0xa);
			PrintPoint(px, py + 2, "�˳��Զ���༭", 0x2);
			break;
		case BUTTON_ENDEDITMAP:
			PrintPoint(px, py, "���浽��ͼ    ", 0x2);
			PrintPoint(px, py + 2, "�˳��Զ���༭", 0xa);
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
					pMap.push_back(POINTDATA(w, h, 8, "��",0, 88888));
				}
				else if (w == 0 || w == wide - 1)
				{
					pMap.push_back(POINTDATA(w, h, 8, "��", 0, 88888));
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
						pMap.push_back(POINTDATA(w + x, h + y, 6, "��", 0, OBSTACLE_EARTHWALL));

					}
					else if (w == 0 || w == wide)
					{
						pMap.push_back(POINTDATA(w + x, h + y, 6, "��", 0, OBSTACLE_EARTHWALL));
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
				pMap.push_back(POINTDATA(w + x, h + y, 14, "��", 0, OBSTACLE_CAMP_A));
			}
		}
	}

	void PaintMap(int x, int y)
	{
		if (IsMapPoint(x, y, nType))return;

		if (nTypeArry[nType] == OBSTACLE_WALL)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "��", 0, nTypeArry[nType]));
			PrintPoint(x, y, "��", nColor[nType]);
		}
		else if (nTypeArry[nType]==OBSTACLE_EARTHWALL)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "��", 0, nTypeArry[nType]));
			PrintPoint(x, y, "��", nColor[nType]);
		}
		else if (nTypeArry[nType] == OBSTACLE_WATER)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "��", 0, nTypeArry[nType]));
			PrintPoint(x, y, "��", nColor[nType]);
		}
		else if (nTypeArry[nType] == OBSTACLE_GRASS)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "��", 0, nTypeArry[nType]));
			PrintPoint(x, y, "��", nColor[nType]);
		}
		else if (nTypeArry[nType] == BUFFER_RANDBUFFER)
		{
			pMap.push_back(POINTDATA(x, y, nColor[nType], "�d", 0, nTypeArry[nType]));
			PrintPoint(x, y, "�d", nColor[nType]);
		}
		else if (nTypeArry[nType] >= TANK_PC1 && nTypeArry[nType] <= TANK_PB)
		{

			if (nTypeArry[nType] == TANK_PA&&IsHavePlayerTank(1))return;
			if (nTypeArry[nType] == TANK_PB&&IsHavePlayerTank(2))return;
			if (nTypeArry[nType] == TANK_PC5&&IsHaveBossTank())return;

			


			pMap.push_back(POINTDATA(x, y, nColor[nType], "��", 0, nTypeArry[nType]));
			PrintPoint(x-1, y-1, "  ", nColor[nType]);
			PrintPoint(x, y - 1, "��", nColor[nType]);
			PrintPoint(x+1, y - 1, "  ", nColor[nType]);

			PrintPoint(x - 1, y, "��", nColor[nType]);
			PrintPoint(x, y, "��", nColor[nType]);
			PrintPoint(x + 1, y, "��", nColor[nType]);

			PrintPoint(x - 1, y + 1, "��", nColor[nType]);
			PrintPoint(x, y + 1, "  ", nColor[nType]);
			PrintPoint(x + 1, y + 1, "��", nColor[nType]);
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
	bool IsMapPoint(int x, int y,int type)//�Ƿ��ѻ�ͼ
	{
		int nDrawType = 0;
		if (nTypeArry[type]>=TANK_PC1 && nTypeArry[type] <= TANK_PB)nDrawType = 1;//Ҫ��̹��
		else nDrawType = 0;//Ҫ����ͨ�ϰ���

		for (auto &val : pMap)
		{
			if (val.m_type>=TANK_PC1 && val.m_type <= TANK_PB)//����̹�˵ĵ�
			{
				if (nDrawType==0)//���Ҫ����ͨ�ϰ���
				{
					if (x >= val.m_x - 1 && x <= val.m_x + 1 && y >= val.m_y - 1 && y <= val.m_y + 1)return true;
				}
				else if(nDrawType == 1)//���Ҫ��̹��
				{
					if (x >= val.m_x - 2 && x <= val.m_x + 2 && y >= val.m_y - 2 && y <= val.m_y + 2)return true;
				}
			}
			else //����һ����ͨ���ϰ����
			{ 
				if (nDrawType == 0)//���Ҫ���ϰ���
				{
					if (val.m_x == x && val.m_y == y)return true;
				}
				else if (nDrawType == 1)//���Ҫ��̹��
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
	int GetClickFrame(int x, int y)//����0���滭���� 1�����߽��� 2��������� -1����Ч����
	{
		if (x > 0 && x < WALL_WIDE && y >0 && y < WALL_HIGH)return FRAME_DRAW;
		else if (x > WALL_WIDE && x < WALL_WIDE + 14 && y >0 && y < 48)return FRAME_TOOLS;
		else if (x > WALL_WIDE && x < WALL_WIDE + 14 && y >48 && y < WALL_HIGH)return FRAME_SAVEINFO;
		return -1;
	}
	void PrintBasicWallPoint()//��ӡȫ���滭�����ĵ�
	{
		for (auto &val : pMap) {
			if (val.m_type>=TANK_PC1&&val.m_type <=TANK_PB)//��ԭ̹������ͼ��
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
	void ClsAll(int wide, int higt)//������е�
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
		//��̬�������ͽ��б��棬�ȴ�������Ԫ�ص�������Ȼ���ٽ��Ŵ��������ڵ����ݡ�
		FILE *pfile;
		char *szFileName = GetRoute(ROUTE_SAVEGAME);
		if (szFileName == NULL)return false;
		int nsize = 0;

		fopen_s(&pfile, szFileName, "wb");//����д����ʽ�򿪣�����ԭ�ļ�
		if (pfile == NULL) { MessageBoxA(NULL, "д���ļ�ʧ�ܣ�", "����ʧ��", MB_OK + MB_ICONERROR); return false; }
		if (MyTank2.GetColor() == 0)nsize = 1;//�ж��Ƿ���̹��B���
		else nsize = 2;
		fwrite(&nsize, 1, sizeof(int), pfile);
		fclose(pfile);


		//����MyTank����
		vector <TankInfo> pMyTankInfo;

		pMyTankInfo.push_back(TankInfo{ MyTank.GetTankX(),MyTank.GetTankY() ,MyTank.GetColor() ,MyTank.GetDir() ,MyTank.GetSpeed() ,MyTank.GetBulletCd(),MyTank.GetHp(),MyTank.GetMp(),MyTank.GetRank() ,MyTank.GetMaxHp() ,MyTank.GetMaxMp() });
		if(nsize==2)pMyTankInfo.push_back(TankInfo{ MyTank2.GetTankX(),MyTank2.GetTankY() ,MyTank2.GetColor() ,MyTank2.GetDir() ,MyTank2.GetSpeed() ,MyTank2.GetBulletCd(),MyTank2.GetHp(),MyTank2.GetMp(),MyTank2.GetRank(),MyTank2.GetMaxHp() ,MyTank2.GetMaxMp() });
		for (auto &val: pMyTankInfo)
		{
			fopen_s(&pfile, szFileName, "ab");//����д����ʽ�򿪣�����ԭ�ļ�
			fwrite(&val, 1, sizeof(TankInfo), pfile);
			fclose(pfile);
		}

		//����pObstacle����
		nsize = pObstacle.size();//ȡ���ϰ�������
		fopen_s(&pfile, szFileName, "ab");
		fwrite(&nsize, 1, sizeof(int), pfile);//����д��ǰ4�ֽ�
		fclose(pfile);
		for (auto &val : pObstacle)
		{
			fopen_s(&pfile, szFileName, "ab");
			//����ѭ��д��ÿ���ϰ������������
			fwrite(&val, 1, sizeof(POINTDATA), pfile);
			fclose(pfile);
		}

		//����pBullet����
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

		//����pPcTankConfig����
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


		//����pPcTank����
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


		//����pBuffer����
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

		MessageBoxA(NULL, "�浵�ɹ���", "��ʾ", MB_OK + MB_ICONINFORMATION);
		return true;
	}
	bool LoadGame(char *filename = NULL)
	{
		if (filename == NULL) filename = GetRoute(ROUTE_LOADGAME);

		char *szFileName = filename;
		if (szFileName == NULL)return false;

		//��ʼ�����б������ݵĶ�̬����
		pPlayerTankConfig.clear();//�浵�����̹�˵�������Ϣ
		pPlayerTankConfigLoad.clear();//�浵�����̹�˵�ʣ��HP����Ϣ
		pPcTankConfig.clear();//�浵������̹�˵Ĺ�����Ϣ
		pBufferConfig.clear();//�ӳ�ʳ����ߵ�����������Ϣ
		vector <POINTDATA>().swap(pPlayerTankConfig);
		vector <TankInfo>().swap(pPlayerTankConfigLoad);
		vector <POINTDATA>().swap(pPcTankConfig);
		vector <POINTDATA>().swap(pBufferConfig);
		//��ʼ�����п�ʼ��Ϸ����Ҫ��ȡ�Ķ�̬��������
		pObstacle.clear();//�ϰ�����
		pBullet.clear();//�ڵ���
		pPcTank.clear();//PC̹����
		vector <POINTDATA>().swap(pObstacle);
		vector <POINTDATA>().swap(pBullet);
		vector <Tank>().swap(pPcTank);


		FILE *pfile;
		int nsize = 0;

		//��ȡMyTank����
		fopen_s(&pfile, szFileName, "rb");
		if (pfile == NULL) { MessageBoxA(NULL, "���ļ�ʧ�ܣ�", "����ʧ��", MB_OK + MB_ICONERROR); return false; }
		fread(&nsize, 1, sizeof(int), pfile);

		TankInfo *mytank = new TankInfo;
		for (int i = 0; i < nsize; i++)
		{
			fread(mytank, 1, sizeof(TankInfo), pfile);
			pPlayerTankConfigLoad.push_back(TankInfo{ mytank->x, mytank->y, mytank->color,mytank->dir,mytank->speed,mytank->bulletcd, mytank->hp, mytank->mp, mytank->rank, mytank->maxhp, mytank->maxmp });
		}
		delete mytank;


		POINTDATA *temp = new POINTDATA;//����һ����ʱ�ѿռ�
		//ǰ4���ֽ��Ƕ�ȡcObstacle�ڵ�����
		fread(&nsize, 1, sizeof(int), pfile);
		//ѭ����ȡcObstacle����������
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//��ȡ�����ݷ�����ڴ�ռ�
			switch (temp->m_type)//������ѡ��
			{
			case OBSTACLE_WALL:case OBSTACLE_EARTHWALL:
				temp->m_data = "��";//���¸�ֵͼ��
				break;
			case OBSTACLE_GRASS:
				temp->m_data = "��";
				break;
			case OBSTACLE_WATER:
				temp->m_data = "��";
				break;
			case OBSTACLE_CAMP_A:
				temp->m_data = "��";
				break;
			}
			pObstacle.push_back(*temp);//����pObstacle��̬������
		}

		//��ȡpBullet����
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//��ȡ�����ݷ�����ڴ�ռ�
			if(temp->m_type==BULLET_MY)temp->m_data = "��";
			else temp->m_data = "��";
			pBullet.push_back(*temp);
		}

		//��ȡpPcTankConfig����
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//��ȡ�����ݷ�����ڴ�ռ�
			temp->m_data = "��";
			pPcTankConfig.push_back(*temp);
		}

		vector <TankInfo*> pPcTankInfo;
		TankInfo *tank = new TankInfo;
		
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(tank, 1, sizeof(TankInfo), pfile);//��ȡ�����ݷ�����ڴ�ռ�

			pPcTank.push_back(Tank(tank->x, tank->y, tank->color, tank->dir, tank->speed, tank->bulletcd, tank->hp, tank->mp, 1, 3,GetRandTankId(), tank->hp, tank->mp));
		}
		delete tank;


		//��ȡpBufferConfig����
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//��ȡ�����ݷ�����ڴ�ռ�
			temp->m_data = "�d";
			pBufferConfig.push_back(*temp);
		}
		delete temp;
		fclose(pfile);
		return true;
	}
	bool SaveMap()
	{
		if (IsHavePlayerTank(1) == false) { MessageBoxA(NULL,"���ٳ�ʼ��PlayerA(���1)λ�ã�","����ʧ��", MB_OK + MB_ICONERROR);return false; }

		vector <POINTDATA> cObstacle;
		vector <POINTDATA> cBuffer;
		vector <POINTDATA> cTank;

		//����ǰ�Ȱ�������࣬ѹ�벻ͬ���͵Ķ�̬������
		for (auto &val : pMap)//�������ĵ�ͼ���������������
		{
			if (val.m_type ==OBSTACLE_WALL)//��ǽ��
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "��",0, val.m_type));
			}
			else if (val.m_type == OBSTACLE_EARTHWALL)//��ǽ
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "��",0, val.m_type));
			}
			else if (val.m_type == OBSTACLE_WATER)//����
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "��", 0, val.m_type));
			}
			else if (val.m_type == OBSTACLE_GRASS)//�ݴ�
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "��", 0, val.m_type));
			}
			else if (val.m_type == BUFFER_RANDBUFFER)//ʳ��
			{
				cBuffer.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "�d", 0, val.m_type));
			}
			else if (val.m_type == OBSTACLE_CAMP_A)//����
			{
				cObstacle.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "��", 0, val.m_type));
			}
			else if (val.m_type >= TANK_PC1&& val.m_type <= TANK_PB)//̹��
			{
				cTank.push_back(POINTDATA(val.m_x, val.m_y, val.m_color, "��", 0, val.m_type));
			}

		}

		//Ȼ���ٰ���̬�������ͽ��б��棬�ȴ�������Ԫ�ص�������Ȼ���ٽ��Ŵ��������ڵ����ݡ�
		FILE *pfile;
		char *szFileName = GetRoute(ROUTE_SAVEMAP);
		if (szFileName == NULL)return false;
		int nsize = 0;

		//����cObstacle����
		nsize = cObstacle.size();
		fopen_s(&pfile, szFileName, "wb");//����д����ʽ�򿪣�����ԭ�ļ�
		if (pfile == NULL) { MessageBoxA(NULL, "д���ļ�ʧ�ܣ�", "����ʧ��", MB_OK + MB_ICONERROR); return false; }
		fwrite(&nsize, 1, sizeof(int), pfile);//ǰ4���ֽ���д�����������
		fclose(pfile);
		for (auto &val : cObstacle)
		{
			fopen_s(&pfile, szFileName, "ab");//����׷�ӷ�ʽ�򿪣����ļ��������׷����������
			fwrite(&val, 1, sizeof(POINTDATA), pfile);//��ʼд������
			fclose(pfile);
		}

		//����cBuffer����
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


		//����cTank����
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

		MessageBoxA(NULL, "����ɹ���", "��ʾ", MB_OK + MB_ICONINFORMATION);
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

		//��ȡcObstacle����
		fopen_s(&pfile, szFileName, "rb");
		if (pfile == NULL) //ʹ���ļ��Ի������Ҫ����·�����ܴ򿪹ؿ���ͼ�ļ�
		{ 
			char *tRoute = GetProgramDir();
			strcat_s(tRoute,strlen(szFileName)+1, szFileName);
			fopen_s(&pfile, tRoute, "rb");
			if (pfile == NULL) { MessageBoxA(NULL, "���ļ�ʧ�ܣ�", "����ʧ��", MB_OK + MB_ICONERROR); return false; }
		}
	

		fread(&nsize, 1, sizeof(int), pfile);
		//�ȶ�ȡǰ4�ֽ�,�����������Ȼ��forѭ�����ζ���ָ�����������ݣ�������ʱ�ڴ���

		POINTDATA *temp = new POINTDATA;//����һ����ʱ�ڴ�ռ�
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);//��ȡ�����ݷ�����ڴ�ռ�

			switch (temp->m_type)
			{
			case OBSTACLE_WALL:case OBSTACLE_EARTHWALL:
				temp->m_data = "��";
				break;
			case OBSTACLE_GRASS:
				temp->m_data = "��";
				break;
			case OBSTACLE_WATER:
				temp->m_data = "��";
				break;
			case OBSTACLE_CAMP_A:
				temp->m_data = "��";
				break;
			}
			pObstacle.push_back(*temp);
		}

		//��ȡpBufferConfig
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);
			temp->m_data = "�d";
			pBufferConfig.push_back(*temp);
			
		}

		//��ȡpPlayerTankConfig,pPcTankConfig
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);
			temp->m_data = "��";

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
		//��ȡcObstacle���ݵ�����
		fopen_s(&pfile, szFileName, "rb");
		if (pfile == NULL) { MessageBoxA(NULL, "���ļ�ʧ�ܣ�", "����ʧ��", MB_OK + MB_ICONERROR); return false; }
		fread(&nsize, 1, sizeof(int), pfile);
		//�ȶ�ȡǰ4�ֽ�,�����������Ȼ��forѭ�����ζ���ָ�����������ݣ�������ʱ�ڴ���

		POINTDATA *temp = new POINTDATA;//����һ����ʱ�ڴ�ռ�
		for (int i = 0; i < nsize; i++)
		{
			//��ȡ�����ݷ�����ڴ�ռ�
			fread(temp, 1, sizeof(POINTDATA), pfile);
			switch (temp->m_type)//������ѡ��
			{
			case OBSTACLE_WALL:case OBSTACLE_EARTHWALL:
				temp->m_data = "��";//���¸�ֵͼ��
				break;
			case OBSTACLE_GRASS:
				temp->m_data = "��";
				break;
			case OBSTACLE_WATER:
				temp->m_data = "��";
				break;
			case OBSTACLE_CAMP_A:
				temp->m_data = "��";
				break;
			}
			pMap.push_back(POINTDATA(*temp));//�����ݼ��붯̬����Map��
		}

		//��ȡpBufferConfig
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);
			temp->m_data = "�d";
			pMap.push_back(POINTDATA(*temp));
		}


		//��ȡpPlayerTankConfig,pPcTankConfig
		fread(&nsize, 1, sizeof(int), pfile);
		for (int i = 0; i < nsize; i++)
		{
			fread(temp, 1, sizeof(POINTDATA), pfile);
			temp->m_data = "��";
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
	bool PlayerB = false;//�Ƿ�˫��ģʽ
	bool Suspend = false;//�Ƿ���ͣ��Ϸ
	bool Loading = false;//�Ƿ�Ϊ����浵

	Obstacle MyObstacle;
	Bullet MyBullet;
	Buffer MyBuffer;

	//PlayerA��B��ʼ��ʱ�Ĳ�������
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

	//���Load̹�˴�������0�����Ƕ�ȡ�浵
	if (pPlayerTankConfigLoad.size()>0)Loading = true;
	else Loading = false;
	
	//��ʼ����PlayerA̹�˵ĳ�ʼ����Ϣ
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

	//��ʼ����PlayerB̹�˵ĳ�ʼ����Ϣ
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


	//��ӡ��Ϸ���Ͽ��п�
	PrintEmptyFrame(WALL_WIDE, 0, 14, 30, 0xa, "��");
	PrintEmptyFrame(WALL_WIDE, 32, 14, 14, 0xa, "��");

	while (true)
	{
		//ȡ����״̬��ʹ��GetAsyncKeyState����
		{
			if (GetKey(KEY_PA_UP))
			{
				if (MyTank.GetDir() == DIR_UP)
				{
					if (PlayerB)
					{
						//�ж��ҵ�̹���Ƿ���PC̹�˺�PlayerB̹���ص�
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
					MyBullet.SetBullet(MyTank.GetTankX(), MyTank.GetTankY(), 11, "��", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
					if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(1);
				}
			}
			if (GetKey(KEY_PA_BULLETGROUP))
			{
				if (MyTank.IsLaunchBullet(5))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBulletGroup(MyTank.GetTankX(), MyTank.GetTankY(), 11, "��", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
					if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(5);
				}
			}
			if (GetKey(KEY_PA_BULLETSPUER))
			{
				if (MyTank.IsLaunchBullet(20))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBulletSpuer(MyTank.GetTankX(), MyTank.GetTankY(), 11, "��", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
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
					MyBullet.SetBullet(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "��", MyTank2.GetDir(), BULLET_MY2, MyTank2.GetTankId());
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(1);
				}
			}
			if (GetKey(KEY_PB_BULLETGROUP))
			{
				if (MyTank2.IsLaunchBullet(5))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBulletGroup(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "��", MyTank2.GetDir(), BULLET_MY2, MyTank2.GetTankId());
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(5);
				}
			}
			if (GetKey(KEY_PB_BULLETSPUER))
			{
				if (MyTank2.IsLaunchBullet(20))
				{
					PlayMusic(SOUND_ATTACK);
					MyBullet.SetBulletSpuer(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "��", MyTank2.GetDir(), BULLET_MY, MyTank2.GetTankId());
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(20);
				}
			}

			if (GetKey(KEY_SPACE) || GetKey(KEY_ESC))Suspend = true;//�ո����ESC����ͣ��Ϸ
		}

		//�ж�PlayerA��B�Ƿ��������Ե��ڵ�
		if (MyTank.IsTouchBullet(MyTank.GetTankX(), MyTank.GetTankY(), BULLET_PC))
		{
			if (!MyTank.IsNoHurtBuffer())MyTank.SubHp(1);//��ȥhp
			if (MyTank.IsDie())goto PrintGameOver;
		}
		if (PlayerB && MyTank2.IsTouchBullet(MyTank2.GetTankX(), MyTank2.GetTankY(), BULLET_PC))//�Ƿ��������Ե��ڵ�
		{
			if (!MyTank2.IsNoHurtBuffer())MyTank2.SubHp(1);
			if (MyTank2.IsDie())goto PrintGameOver;
		}

		//����AutoTank�࣬��������PC̹�˵���Ϊ
		MyAutoTank.MoveTank();

		MyTank.PrintTank();//��ӡ̹��
		MyTank.RevHp();//�Զ��ָ�HP
		MyTank.RevMp();//�Զ��ָ�MP
		if (PlayerB)
		{
			MyTank2.PrintTank();//��ӡ̹��2
			MyTank2.RevHp();//�Զ��ָ�HP
			MyTank2.RevMp();//�Զ��ָ�MP
		}

		//�ڵ��򵽵��ϰ���ڵ��ƶ��ٶ�
		int nOType = MyBullet.MoveBullet(10);

		//�ж��Ƿ񱻴򵽻�����
		if (nOType == OBSTACLE_CAMP_A)
		{
			PrintGameOver:
			int nButton = BUTTON_RESTGAME;
			int nPage = MENU_GAMEOVERPAGE;
			PrintGameOverMenu(22,20, nButton);//��ӡ��Ϸ����������˵�
			nButton = MessageLoop(nPage);//���������Ϣѭ��
			if (nButton == BUTTON_RESTGAME) { return nButton; }//��� ���¿�ʼ ��ť������Ϣ
			else if (nButton == BUTTON_BACKHOMEPAGE) { return nButton; }//��� �ص���ҳ ��ť������Ϣ
		}

		//�ж�PlayerA��B�Ե�ʳ������״̬
		{
		int nBufferType = MyBuffer.IsTouchBuffer(MyTank.GetTankX(), MyTank.GetTankY());
		if (nBufferType ==BUFFER_ATTACK)MyTank.SetAttackBuffer();//�ӹ���
		if (nBufferType == BUFFER_BULLECTCD)MyTank.SetBulletCdBuffer();//�����ӵ�
		if (nBufferType == BUFFER_NOHURT)MyTank.SetNoHurtBuffer();//�޵�
		if (nBufferType == BUFFER_STATICALL)MyTank.SetStaticAll();//��������
		if (nBufferType == BUFFER_KILLALL)MyTank.SetKillAll();//ȫ����ɱ
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


		MyBuffer.RefreshBuffer();//�̶�ʱ������ص�ˢ��ʳ��
		MyBuffer.PrintBuffer();//��ӡʳ��
		MyBullet.PrintBullet();//��ӡ�ڵ�


		PrintGameTips(WALL_WIDE + 4, 33);//��ӡ��Ϸ������ʾ������˵��

		//��ӡ�����Ϸ��Ϣ
		{
		PrintPoint(WALL_WIDE + 3, 2, "���1", 0xb);
		PrintPoint(WALL_WIDE + 4, 4, "��ǰ�÷֣�", MyTank.GetRank(), 10);
		PrintPoint(WALL_WIDE + 4, 6, "��ǰHP��", MyTank.GetHp(), 12);
		PrintPoint(WALL_WIDE + 4, 7, "HP�ظ��ʣ�1HP/��", 12);
		PrintPoint(WALL_WIDE + 4, 9, "��ǰMP��", MyTank.GetMp(), 9);
		PrintPoint(WALL_WIDE + 4, 10, "MP�ظ��ʣ�3MP/��", 9);
		PrintPoint(WALL_WIDE + 4, 12, "��ǰ��������", MyTank.GetAttack(), 0xc);
		if (PlayerB)
		{
			PrintPoint(WALL_WIDE + 3, 15, "���2", 0xb);
			PrintPoint(WALL_WIDE + 4, 17, "��ǰ�÷֣�", MyTank2.GetRank(), 10);
			PrintPoint(WALL_WIDE + 4, 19, "��ǰHP��", MyTank2.GetHp(), 12);
			PrintPoint(WALL_WIDE + 4, 20, "HP�ظ��ʣ�1HP/��", 12);
			PrintPoint(WALL_WIDE + 4, 22, "��ǰMP��", MyTank2.GetMp(), 9);
			PrintPoint(WALL_WIDE + 4, 23, "MP�ظ��ʣ�3MP/��", 9);
			PrintPoint(WALL_WIDE + 4, 25, "��ǰ��������", MyTank2.GetAttack(), 0xc);
		}
		}
		
		//�ж��Ƿ�ؿ�ģʽ����һ��
		if (nStage!=NULL)
		{
			char nText[255] = {0};
			int nPlayerARank = IsNextStage(nStage, MyTank.GetRank());
			int nPlayerBRank = IsNextStage(nStage, MyTank2.GetRank());

			PrintPoint(WALL_WIDE + 3, 27, "��������", 0xb);
			sprintf_s(nText,_countof(nText), "����%d���о�", nPlayerARank > nPlayerBRank ? nPlayerBRank : nPlayerARank);

			if (MyAutoTank.IsHaveBossTank() != -1)//�����boss
			{
				strcpy_s(nText,strlen("�����ռ�BOSS")+1, "�����ռ�BOSS") ;
				if (MyAutoTank.IsBossTankDie())goto NextStage;//���boss����
			}
			else if (nPlayerARank <=0 || nPlayerBRank <=0)//û��boss���жϷ���
			{
			NextStage:
				PrintNextStageText(18, 20, nStage);//��ӡ��ϲ��������
				return BUTTON_NEXTGAME;
			}
			
			PrintPoint(WALL_WIDE + 4, 28, nText, 0xa);

		}

		//�Ƿ���ͣ����Ϸ
		while (Suspend)
		{
			int nButton = BUTTON_CONTIUNEGAME;
			int nPage = MENU_GAMEPAGE;
			PrintEmptyFrame(WALL_WIDE, 48, 14, 10, 0xa, "��");//��ӡ�˵����
			PrintPauseMenu(WALL_WIDE + 4, 50, nButton);//��ӡ�˵�����
			nButton = MessageLoop(nPage);//���������Ϣѭ��

			if (nButton == BUTTON_CONTIUNEGAME) { ClsPuaseMenu(WALL_WIDE + 4, 50); Suspend = false; }//��� ������Ϸ ��ť������˵���������
			else if (nButton == BUTTON_SAVEGAME) //��� ������Ϸ ��ť
			{
				Map MyMap(false);
				MyMap.SaveGame(MyTank,MyTank2);//���ñ�����Ϸ����
			}
			else if (nButton == BUTTON_RESTGAME) { return nButton; }//��� ���¿�ʼ ��ť�����ذ�ť��Ϣ
			else if (nButton == BUTTON_BACKHOMEPAGE) { return nButton;}//��� �ص���ҳ ��ť�����ذ�ť��Ϣ
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
	int nNowTime = clock();//ǿ��ͬ��ʱ��

	//PlayerA��B��ʼ��ʱ�Ĳ�������
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

	//��ʼ����PlayerA̹�˵ĳ�ʼ����Ϣ
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

	//��ʼ����PlayerB̹�˵ĳ�ʼ����Ϣ
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


	//��ӡ��Ϸ���Ͽ��п�
	PrintEmptyFrame(WALL_WIDE, 0, 14, 30, 0xa, "��");
	PrintEmptyFrame(WALL_WIDE, 32, 14, 14, 0xa, "��");



	while (true)
	{
		//����˽��ܿͻ�����
		if (LinkType == TCP_LINK_SERVER)
		{
			int nDataSize = MySever.RecvData(RecvMyData);
			for (int i = 0; i < nDataSize; i++)
			{
				//ˢ�¿ͻ�����ҵ�̹���ƶ���Ϣ
				if (RecvMyData[i].m_State == DATA_MOVE && RecvMyData[i].m_Id == TankBId)
				{
					MyTank2.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
				}
				//ˢ�¿ͻ�����ҵ�3���ڵ�����Ϣ
				else if (RecvMyData[i].m_State == DATA_BULLET && RecvMyData[i].m_Id == TankBId)
				{
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(1);
					MyBullet.SetBullet(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, "��", RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);
				}
				else if (RecvMyData[i].m_State == DATA_BULLETGROUP && RecvMyData[i].m_Id == TankBId)
				{

					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(5);
					MyBullet.SetBulletGroup(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, "��", RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);

				}
				else if (RecvMyData[i].m_State == DATA_BULLETSUPER && RecvMyData[i].m_Id == TankBId)
				{
					if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(20);
					MyBullet.SetBulletSpuer(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, "��", RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);
				}
				//ˢ�¿ͻ�����Ϸ������Ϣ
				else if (RecvMyData[i].m_State == DATA_GAMEOVER)
				{
					goto PrintGameOver;
				}
			}
			if (nDataSize)delete[]RecvMyData;

		}
		//�ͻ����ܷ��������
		if (LinkType == TCP_LINK_CLIENT)
		{
			int nDataSize = MyClient.RecvData(RecvMyData);
			for (int i = 0; i < nDataSize; i++)
			{
				//ˢ�·������ҵ�̹���ƶ���Ϣ
				if (RecvMyData[i].m_State == DATA_MOVE && RecvMyData[i].m_Id == TankAId)
				{
					MyTank.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
				}
				//ˢ�·���˵�PC̹���ƶ���Ϣ
				else if (RecvMyData[i].m_State == DATA_MOVE)
				{
					for (auto &val : pPcTank)//��������PC̹��
					{
						if (val.GetTankId() == RecvMyData[i].m_Id)//���̹��IDƥ����ͬ
						{	//���ø�̹�˵��ƶ������������ƶ�
							val.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
							break;
						}
					}
				}
				//ˢ�·������ҵ�3���ڵ���Ϣ
				else if (RecvMyData[i].m_State == DATA_BULLET && RecvMyData[i].m_Id != TankBId)
				{
					int nColor = 0xc;
					char *nData = "��";

					if (RecvMyData[i].m_Id == TankAId)
					{
						nColor = 11;
						nData = "��";
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(1);
					}

					MyBullet.SetBullet(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, nData, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);
				}
				else if (RecvMyData[i].m_State == DATA_BULLETGROUP && RecvMyData[i].m_Id != TankBId)
				{
					char *nData = "��";
					if (RecvMyData[i].m_Id == TankAId)
					{
						nData = "��";
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(5);
					}

					MyBullet.SetBulletGroup(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, nData, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);

				}
				else if (RecvMyData[i].m_State == DATA_BULLETSUPER && RecvMyData[i].m_Id != TankBId)
				{
					char *nData = "��";
					if (RecvMyData[i].m_Id == TankAId)
					{
						nData = "��";
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(20);
					}

					MyBullet.SetBulletSpuer(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Color, nData, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);

				}
				//ˢ�·����ˢ�µļӳɵ�����Ϣ
				else if (RecvMyData[i].m_State == DATA_BUFFER)
				{
					MyBuffer.AddBuffer(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Type);
				}
				//ˢ�·���˴��������PC̹����Ϣ
				else if (RecvMyData[i].m_State == DATA_CREATTANK)
				{
					MyAutoTank.CreatRandTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id);
				}
				//ˢ�·������Ϸ������Ϣ
				else if (RecvMyData[i].m_State == DATA_GAMEOVER)
				{
					goto PrintGameOver;
				}

				/*******************ǿ��ͬ��ˢ�£��Է��������Ϊ׼***********************/
				//ˢ�����A��̹�˻�����Ϣ
				else if (RecvMyData[i].m_State == DATA_MOVE_REFRESH && RecvMyData[i].m_Id == TankAId)
				{
					MyTank.CleanTank();
					MyTank.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
					MyTank.SetHp(RecvMyData[i].m_Hp);
					MyTank.SetMp(RecvMyData[i].m_Mp);
					MyTank.SetRank(RecvMyData[i].m_Rank);
				}
				//ˢ�����B��̹�˻�����Ϣ
				else if (RecvMyData[i].m_State == DATA_MOVE_REFRESH && RecvMyData[i].m_Id == TankBId)
				{
					MyTank2.CleanTank();
					MyTank2.MoveTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir);
					MyTank2.SetHp(RecvMyData[i].m_Hp);
					MyTank2.SetMp(RecvMyData[i].m_Mp);
					MyTank2.SetRank(RecvMyData[i].m_Rank);
				}
				//��������PC̹��
				else if (RecvMyData[i].m_State == DATA_CREATTANK_REFRESH_CLEAR)
				{
					MyAutoTank.ClearAllTankLink();
				}
				//ˢ������PC̹�˵Ļ�����Ϣ
				else if (RecvMyData[i].m_State == DATA_CREATTANK_REFRESH)
				{
					MyAutoTank.RefreshTankLink(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Dir, RecvMyData[i].m_Type, RecvMyData[i].m_Id, RecvMyData[i].m_Hp, RecvMyData[i].m_MaxHp);
				}
				//�������еļӳɵ���
				else if (RecvMyData[i].m_State == DATA_BUFFER_REFRESH_CLEAR)
				{
					MyBuffer.ClearBuffer();
				}
				//ˢ�����еļӳɵ���
				else if (RecvMyData[i].m_State == DATA_BUFFER_REFRESH)
				{
					MyBuffer.AddBuffer(RecvMyData[i].m_X, RecvMyData[i].m_Y, RecvMyData[i].m_Type);
				}
				//ˢ�²ݴ���Ϣ
				else if (RecvMyData[i].m_State == DATA_OBSTACLE_GRASS_REFRESH)
				{
					MyObstacle.PrintGrassPoint();
				}

			}
			if (nDataSize)delete[]RecvMyData;

		}


		//ȡ����״̬��ʹ��GetAsyncKeyState����
		if (LinkType == TCP_LINK_SERVER)
			{
				if (GetKey(KEY_PA_UP))
				{
					if (MyTank.GetDir() == DIR_UP)
					{
							//�ж��ҵ�̹���Ƿ���PC̹�˺�PlayerB̹���ص�
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
						MyBullet.SetBullet(MyTank.GetTankX(), MyTank.GetTankY(), 11, "��", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(1);
					}
				}
				if (GetKey(KEY_PA_BULLETGROUP))
				{
					if (MyTank.IsLaunchBullet(5))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBulletGroup(MyTank.GetTankX(), MyTank.GetTankY(), 11, "��", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
						if (!MyTank.IsBulletCdBuffer())MyTank.SubMp(5);
					}
				}
				if (GetKey(KEY_PA_BULLETSPUER))
				{
					if (MyTank.IsLaunchBullet(20))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBulletSpuer(MyTank.GetTankX(), MyTank.GetTankY(), 11, "��", MyTank.GetDir(), BULLET_MY, MyTank.GetTankId());
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
						MyBullet.SetBullet(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "��", MyTank2.GetDir(), BULLET_MY2, MyTank2.GetTankId());
						if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(1);
					}
				}
				if (GetKey(KEY_PB_BULLETGROUP))
				{
					if (MyTank2.IsLaunchBullet(5))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBulletGroup(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "��", MyTank2.GetDir(), BULLET_MY2, MyTank2.GetTankId());
						if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(5);
					}
				}
				if (GetKey(KEY_PB_BULLETSPUER))
				{
					if (MyTank2.IsLaunchBullet(20))
					{
						PlayMusic(SOUND_ATTACK);
						MyBullet.SetBulletSpuer(MyTank2.GetTankX(), MyTank2.GetTankY(), 0xa, "��", MyTank2.GetDir(), BULLET_MY, MyTank2.GetTankId());
						if (!MyTank2.IsBulletCdBuffer())MyTank2.SubMp(20);
					}
				}
			}


		//�ж�PlayerA��B�Ƿ��������Ե��ڵ�
		if (MyTank.IsTouchBullet(MyTank.GetTankX(), MyTank.GetTankY(), BULLET_PC))
		{
			if (!MyTank.IsNoHurtBuffer())MyTank.SubHp(1);//��ȥhp
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
		if (MyTank2.IsTouchBullet(MyTank2.GetTankX(), MyTank2.GetTankY(), BULLET_PC))//�Ƿ��������Ե��ڵ�
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

		//����AutoTank�࣬��������PC̹�˵���Ϊ
		if (LinkType == TCP_LINK_SERVER)MyAutoTank.MoveTank();
		else if (LinkType == TCP_LINK_CLIENT) MyAutoTank.MoveTankLink();

		//�ָ�HP��MP����ӡ���̹��
		{
			MyTank.PrintTank();//��ӡ̹��
			MyTank.RevHp();//�Զ��ָ�HP
			MyTank.RevMp();//�Զ��ָ�MP

			MyTank2.PrintTank();//��ӡ̹��2
			MyTank2.RevHp();//�Զ��ָ�HP
			MyTank2.RevMp();//�Զ��ָ�MP
		}

		//�ڵ��򵽵��ϰ���ڵ��ƶ��ٶ�
		int nOType = MyBullet.MoveBullet(15);

		//�ж��Ƿ񱻴򵽻�����
		if (nOType == OBSTACLE_CAMP_A)
		{
			SendMyData.m_State = DATA_GAMEOVER;
			if (LinkType == TCP_LINK_SERVER) MySever.AddData(SendMyData);
			else if (LinkType == TCP_LINK_CLIENT) MyClient.AddData(SendMyData);
		PrintGameOver:
			int nButton = BUTTON_RESTGAME;
			int nPage = MENU_LINKGAMEOVERPAGE;
			PrintLinkGameOverMenu(22, 20, nButton);//��ӡ��Ϸ����������˵�
			nButton = MessageLoop(nPage);//���������Ϣѭ��
			if (nButton == BUTTON_RESTGAME) { return nButton; }//��� ���¿�ʼ ��ť������Ϣ
			else if (nButton == BUTTON_BACKHOMEPAGE) { return nButton; }//��� �ص���ҳ ��ť������Ϣ
		}

		//�ж�PlayerA��B�Ե�ʳ������״̬
		{
			int nBufferType = MyBuffer.IsTouchBuffer(MyTank.GetTankX(), MyTank.GetTankY());
			if (nBufferType == BUFFER_ATTACK)MyTank.SetAttackBuffer();//�ӹ���
			if (nBufferType == BUFFER_BULLECTCD)MyTank.SetBulletCdBuffer();//�����ӵ�
			if (nBufferType == BUFFER_NOHURT)MyTank.SetNoHurtBuffer();//�޵�
			if (nBufferType == BUFFER_STATICALL)MyTank.SetStaticAll();//��������
			if (nBufferType == BUFFER_KILLALL)MyTank.SetKillAll();//ȫ����ɱ

			nBufferType = MyBuffer.IsTouchBuffer(MyTank2.GetTankX(), MyTank2.GetTankY());
			if (nBufferType == BUFFER_ATTACK)MyTank2.SetAttackBuffer();
			if (nBufferType == BUFFER_BULLECTCD)MyTank2.SetBulletCdBuffer();
			if (nBufferType == BUFFER_NOHURT)MyTank2.SetNoHurtBuffer();
			if (nBufferType == BUFFER_STATICALL)MyTank2.SetStaticAll();
			if (nBufferType == BUFFER_KILLALL)MyTank2.SetKillAll();
		}

		//ˢ��ʳ���ӡʳ���ӡ�ڵ�
		{
			MyBuffer.RefreshBuffer();//�̶�ʱ������ص�ˢ��ʳ��
			MyBuffer.PrintBuffer();//��ӡʳ��
			MyBullet.PrintBullet();//��ӡ�ڵ�
		}

		//��ӡ�����Ϸ��Ϣ
		{
			PrintGameTips(WALL_WIDE + 4, 33);//��ӡ��Ϸ������ʾ������˵��

			PrintPoint(WALL_WIDE + 3, 2, "���1", 0xb);
			PrintPoint(WALL_WIDE + 4, 4, "��ǰ�÷֣�", MyTank.GetRank(), 10);
			PrintPoint(WALL_WIDE + 4, 6, "��ǰHP��", MyTank.GetHp(), 12);
			PrintPoint(WALL_WIDE + 4, 7, "HP�ظ��ʣ�1HP/��", 12);
			PrintPoint(WALL_WIDE + 4, 9, "��ǰMP��", MyTank.GetMp(), 9);
			PrintPoint(WALL_WIDE + 4, 10, "MP�ظ��ʣ�3MP/��", 9);
			PrintPoint(WALL_WIDE + 4, 12, "��ǰ��������", MyTank.GetAttack(), 0xc);

			PrintPoint(WALL_WIDE + 3, 15, "���2", 0xb);
			PrintPoint(WALL_WIDE + 4, 17, "��ǰ�÷֣�", MyTank2.GetRank(), 10);
			PrintPoint(WALL_WIDE + 4, 19, "��ǰHP��", MyTank2.GetHp(), 12);
			PrintPoint(WALL_WIDE + 4, 20, "HP�ظ��ʣ�1HP/��", 12);
			PrintPoint(WALL_WIDE + 4, 22, "��ǰMP��", MyTank2.GetMp(), 9);
			PrintPoint(WALL_WIDE + 4, 23, "MP�ظ��ʣ�3MP/��", 9);
			PrintPoint(WALL_WIDE + 4, 25, "��ǰ��������", MyTank2.GetAttack(), 0xc);
		}

		//ǿ��ͬ��ˢ�£��Է����Ϊ׼��2��һ��
		if (clock() - nNowTime > 2000)
		{
			nNowTime = clock();

			MySever.ClearData();//������д���������

			//ͬ�����е���̹��
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

			//ͬ�����мӳɵ���
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

			//ͬ��ˢ�²ݴ�
			SendMyData.m_State = DATA_OBSTACLE_GRASS_REFRESH;
			MySever.AddData(SendMyData);
			MyObstacle.PrintGrassPoint();//���������Ҳˢ�²ݴ�

			//ͬ��PlayerA̹����Ϣ
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

			//ͬ��PlayerB̹����Ϣ
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

		//�����������ͽ��з���
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
	keybd_event(VK_SHIFT, 0, 0, 0);//ȡ�����뷨״̬
	Sleep(100);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	srand((unsigned int)time(NULL));//���������


	while (true)
	{
		int nButton = BUTTON_STARTGAME;//Ĭ�ϰ�ť ��ʼ��Ϸ
		int nPage = MENU_HOMEPAGE;//��ǰ�˵�ҳ�� ��ҳ
		system("cls");
		PrintTilteText(13, 15);//��ӡ��ҳ����
		PrintEmptyFrameEx(5, 5, 53, 25, 0xe, "��");//��ӡ�˵����
		PrintEmptyFrame(23, 41, 18, 12, 0xb);//��ӡ�˵����
		PrintTilteMenu(nButton, 30, 43);//��ӡ��ҳ�˵�

		nButton =MessageLoop(nPage);//���������Ϣѭ��

		if (nButton == BUTTON_STARTGAME)//��� ��ʼ��Ϸ ��ť
		{
			nButton = BUTTON_SINGLE;//Ĭ�ϰ�ť ������Ϸ
			ClsTilteMenu(30, 43);//����˵�������˵����
			PrintEmptyFrame(23, 41, 18, 12, 0xb);//��ӡ�˵����
			PrintStartGameMenu(nButton, 30, 43);//��ӡ ��ʼ��Ϸ �ڵĲ˵�
			nButton = MessageLoop(MENU_STARTGAMEPAGE);//���������Ϣѭ��

			if (nButton == BUTTON_SINGLE)//��� ������Ϸ ��ť
			{
				Map MyMap(false);
				int nStageNum = 0;//�ؿ�����
				char nStage[5][255] = {
					"stage\\��һ��.map",
					"stage\\�ڶ���.map",
					"stage\\������.map",
					"stage\\���Ĺ�.map",
					"stage\\�����.map"};
				for (nStageNum=0; nStageNum<_countof(nStage);nStageNum++)
				{
					bool nRet = MyMap.OpenMap(nStage[nStageNum]);//��ָ���ؿ���ͼ
					if(!nRet)break;//��ʧ���˳�ѭ��
				ReStart_Single:
					PrintStageText(18,20,nStageNum + 1);//��ӡ��ʼ��Ϸǰ����
					nButton = StartGame(nStageNum + 1);//��ʼ��Ϸ
					//��Ϸ�а��� ���¿�ʼ ��goto��ReStart_Single��ǩ��������ؿ���ͼ
					if (nButton == BUTTON_RESTGAME) { MyMap.OpenMap(nStage[nStageNum]); goto ReStart_Single; }
					//��Ϸ���غ󷵻���һ����Ϣ����������ֱ�ӻص�ѭ��ͷ��������һ�ζ�ȡ�ؿ�
					else if(nButton == BUTTON_NEXTGAME){}
					//����ص���ҳ�� break��ѭ���ص���ҳ
					else if (nButton == BUTTON_BACKHOMEPAGE)break;;
				}
			}
			else if (nButton == BUTTON_PARTNER) //��� ˫����Ϸ ��ť
			{
				Map MyMap(false);
				int nStageNum = 0;
				char nStage[5][255] = {
					"stage\\��һ��˫��.map",
					"stage\\�ڶ���˫��.map",
					"stage\\������˫��.map",
					"stage\\���Ĺ�˫��.map",
					"stage\\�����˫��.map" };
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
			else if (nButton == BUTTON_CUSTOMGAME)//��� �Զ�����Ϸ ��ť
			{
				Map MyMap(false);
				char *nRoute = GetRoute(ROUTE_OPENMAP);//��ȡָ����ͼ�ļ�·��
				if (nRoute != NULL)
				{
					bool bRet = MyMap.OpenMap(nRoute);//������ͼ�ļ�������
					if (bRet)
					{
					ReStart_OpenMap:
						PrintStageText(18, 20, NULL);//��ӡ��ʼ��Ϸǰ����
						nButton = StartGame();//��ʼ��Ϸ
						//��Ϸ�а��� ���¿�ʼ ��goto��ReStart_OpenMap��ǩ���������Զ����ͼ
						if (nButton == BUTTON_RESTGAME) { MyMap.OpenMap(nRoute); goto ReStart_OpenMap; }
					}
				}
			}
			else if (nButton == BUTTON_LINKGAME)//��� ������Ϸ ��ť
			{
				nButton = BUTTON_CREATROOM;
				ClsTilteMenu(30, 43);//����˵�������˵����
				PrintEmptyFrame(23, 41, 18, 12, 0xb);//��ӡ�˵����
				PrintLinkGameMenu(nButton, 30, 43);//��ӡ ������Ϸ �ڵĲ˵�
				nButton = MessageLoop(MENU_LINKGAMEPAGE);//���������Ϣѭ��
				
				if (nButton == BUTTON_CREATROOM)//��� �������� ��ť
				{
					system("cls");
					bool nRet = false;
					int nPort = 6666;//�����˿ں�
					ReadServerIni(nPort);//��TankLink.ini�����ļ��ж�ȡ�˿ں�

					nRet = MySever.IntiSever(nPort);//��ʼ���󶨶˿�
					if (!nRet)continue;

					PrintPoint(1, 1, "��ʼ������˳ɹ����˿ںţ�", nPort, 0xb);
					PrintPoint(1, 2, "�ȴ���Ҽ���...", 0xb);

					while (!MySever.WaitClient())Sleep(10);//�ȴ����B����

					PrintPoint(1, 3, "����Ѽ��룬׼����ʼ��Ϸ��", 0xb);

					TankAId = GetRandTankId();//������������A��B̹��ID������ʶ�����̹�˲���
					TankBId = GetRandTankId();

					SendMyData.m_State = DATA_INTIDATA;//���ݰ�����Ϊ����ʼ������
					SendMyData.m_X = TankAId;//��̹��ID����ṹ����
					SendMyData.m_Y = TankBId;
					MySever.ClearData();
					MySever.AddData(SendMyData);//�����ݰ�ѹ�����������
					MySever.SendData();//���ͳ�ʼ�����ݸ����B

					Sleep(1000);

					Map MyMap(false);
					nRet = MyMap.OpenMap("linkmap\\˫������.map");//��ָ���ؿ���ͼ
					if (!nRet)continue;

					LinkType = TCP_LINK_SERVER;//��������Ϊ �����
					StartLinkGame();//���ú�����ʼ��Ϸ
					MySever.~TCP_SERVER();//��Ϸ�����������������
				}
				if (nButton == BUTTON_ADDROOM)//��� ���뷿�� ��ť
				{
					bool nRet = false;
					char nIp[256]{};
					int nPort = 6666;
					ReadClientIni(nIp, nPort);

					system("cls");

					nRet = MyClient.IntiClient();
					if (!nRet)continue;
					PrintPoint(1, 1, "��ʼ���ͻ��˳ɹ���", 0xb);

					nRet = MyClient.LineServer(nIp, nPort);
					if (!nRet)continue;
					PrintPoint(1, 2, "���ӷ���˳ɹ���",  0xb);
					PrintPoint(1, 3, "�ȴ���ʼ������...", 0xb);

					while (true)
					{
						int nDataSize = MyClient.RecvData(RecvMyData);
						if (nDataSize)
						{
							if (RecvMyData->m_State == DATA_INTIDATA)//���Ϊ��ʼ�����ݰ�ͷ
							{
								TankAId = RecvMyData->m_X;
								TankBId = RecvMyData->m_Y;

								PrintPoint(1, 5, "�ͻ����յ���ʼ��TankAId��", TankAId, 0xa);
								PrintPoint(1, 6, "�ͻ����յ���ʼ��TankBId��", TankBId, 0xa);
								PrintPoint(1, 7, "��ʼ�����ݳɹ���׼����ʼ��Ϸ��", 0xa);

								delete[]RecvMyData;
								break;
							}
						}
						Sleep(10);
					}

					Sleep(1000);

					Map MyMap(false);
					nRet = MyMap.OpenMap("linkmap\\˫������.map");//��ָ���ؿ���ͼ
					if (!nRet)continue;

					LinkType = TCP_LINK_CLIENT;
					StartLinkGame();
					MyClient.~TCP_CLIENT();

				}
				else if (nButton == BUTTON_BACKHOMEPAGE)continue;//��� �ص���ҳ ��ť

			}

			else if (nButton == BUTTON_BACKHOMEPAGE)continue;//��� �ص���ҳ ��ť������ѭ��
		}
		else if (nButton == BUTTON_LOADGAME)//��� ����浵 ��ť
		{
			Map MyMap(false);
			char *nRoute = GetRoute(ROUTE_LOADGAME);//��ָ���浵�ļ�
			if(nRoute!=NULL)
			{
				bool bRet = MyMap.LoadGame(nRoute);//����������Ϸ����
				if (bRet)
				{
				ReStart_LoadGame:
					nButton = StartGame();//��ʼ��Ϸ
					if (nButton == BUTTON_RESTGAME) { MyMap.LoadGame(nRoute); goto ReStart_LoadGame; }//��Ϸ�а��� ���¿�ʼ ��goto��ReStart_LoadGame��ǩ���������Զ����ͼ
				}
			}
		}
		else if (nButton == BUTTON_CUSTOMMAP)//��� �༭��ͼ ��ť
		{
			nButton = BUTTON_NEWMAP;
			ClsTilteMenu(30, 43);//����˵�������˵����
			PrintEmptyFrame(23, 41, 18, 12, 0xb);//��ӡ�˵����
			PrintCustomMenu(nButton, 30, 43);//��ӡ �༭��ͼ �ڵĲ˵�
			nButton = MessageLoop(MENU_CUSTOMPAGE);//���������Ϣѭ��

			if (nButton == BUTTON_NEWMAP)//��� �½���ͼ ��ť
			{
				system("cls");
				Map MyMap(true);
				MessageLoop(MENU_DRAWMAPPAGE);
			}
			if (nButton == BUTTON_OPENMAP)//��� �򿪵�ͼ ��ť
			{
				char *nRoute = GetRoute(ROUTE_OPENMAP);
				if(nRoute!=NULL)
				{
					Map MyMap(nRoute);
					MessageLoop(MENU_EDITMAPPAGE);
				}
			}
			else if (nButton == BUTTON_BACKHOMEPAGE)continue;//��� �ص���ҳ ��ť

		}
		else if (nButton == BUTTON_ENDGAME)break;//��� ������Ϸ ��ť��breakѭ����������

	}
    return 0;
}

int MouseEvenProc(MOUSE_EVENT_RECORD mer,int nPage,Map &MyMap)
{
	static int nMouseFlags = 0;//ʹ�þ�̬�ֲ�������������갴��״̬
	nMouseFlags = mer.dwButtonState;//����갴��״̬��ֵnMouseFlags

	switch (mer.dwEventFlags)
	{
	case 0://��걻����
		if (nMouseFlags == FROM_LEFT_1ST_BUTTON_PRESSED)//�������
		{
			int nMouseX = mer.dwMousePosition.X / 2;
			int nMouseY = mer.dwMousePosition.Y;
			int nFrame=0;
			int nButton = 0;

			switch (nPage)
			{
			case MENU_HOMEPAGE://��Ϸ��ҳ
				nButton = GetTilteButton(30, 43, nMouseX, nMouseY);
				return nButton;
			case MENU_STARTGAMEPAGE://��ʼ��Ϸҳ��
				nButton = GetStartGameButton(30, 43, nMouseX, nMouseY);
				return nButton;
			case MENU_LINKGAMEPAGE://������Ϸҳ��
				nButton = GetLinkGameButton(30, 43, nMouseX, nMouseY);
				return nButton;
			case MENU_CUSTOMPAGE://�Զ����ͼҳ��
				nButton = GetCustomButton(30, 43, nMouseX, nMouseY);
				return nButton;
			case MENU_DRAWMAPPAGE:case MENU_EDITMAPPAGE://�½���ͼ���߱༭��ͼҳ��
				nFrame = MyMap.GetClickFrame(nMouseX, nMouseY);//ȡ����ǰ��ͼҳ������
				//�ڻ�ͼ�����������û�ͼ����
				if (nFrame == FRAME_DRAW )MyMap.PaintMap(nMouseX, nMouseY);
				//�ڹ��������������ô�ӡ���߷�����ˢ�µ�ǰѡ��Ĺ���
				else if (nFrame == FRAME_TOOLS)MyMap.PrintTools(nMouseX, nMouseY);
				//�ڱ����ͼ������
				else if (nFrame == FRAME_SAVEINFO)
				{
					nButton = MyMap.GetButton(nMouseX, nMouseY);//ȡ�������ʲô��ť
					MyMap.PrintButton(nButton);
					if (nButton== BUTTON_SAVEMAP)MyMap.SaveMap();//��� �����ͼ 
					else return BUTTON_ENDEDITMAP;//����˳��༭��ͼ
				}
				break;
			case MENU_GAMEPAGE://����Ϸҳ��
				nButton = GetPauseButton(WALL_WIDE + 4, 50, nMouseX, nMouseY);
				return nButton;
			case MENU_GAMEOVERPAGE:case MENU_LINKGAMEOVERPAGE://����Ϸ����ҳ��
				nButton = GetGameOverButton(22, 20, nMouseX, nMouseY);
				return nButton;
			}


		}
		else if (nMouseFlags == RIGHTMOST_BUTTON_PRESSED)//�Ҽ�����
		{
			int nMouseX = mer.dwMousePosition.X / 2;
			int nMouseY = mer.dwMousePosition.Y;
			int nFrame = 0;

			switch (nPage)
			{
			case MENU_DRAWMAPPAGE:case MENU_EDITMAPPAGE://�½���ͼ���߱༭��ͼҳ��
				nFrame = MyMap.GetClickFrame(nMouseX, nMouseY);
				if (nFrame == 0)MyMap.EraseWall(nMouseX, nMouseY);//�ڻ�ͼ�����������ò�������
				break;
			}
		}
		break;
	case DOUBLE_CLICK://��걻˫��
		break;
	case MOUSE_MOVED://��걻�ƶ�
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
		case MENU_LINKGAMEPAGE://������Ϸҳ��
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
int MessageLoop(int nPage)//�����Ϣѭ��
{


	Map MyMap(nPage==MENU_DRAWMAPPAGE?true:false);
	HANDLE HND = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRcord = { 0 };
	DWORD dwRead;
	int nRet;
	SetConsoleMode(HND, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	while (1)
	{
		DWORD lpNumberOfEvents = 0;//��ȡ���뻺����δ��ȡ�������¼�ĸ�����
		GetNumberOfConsoleInputEvents(HND, &lpNumberOfEvents);
		
		if (lpNumberOfEvents >= 1)//�����������������1�������Ϣ�ж�
		{
			ReadConsoleInput(HND, &stcRcord, 1, &dwRead);//�������߳�
			if (stcRcord.EventType == KEY_EVENT)//����Ǽ�����Ϣ
			{
				if (nPage == MENU_GAMEPAGE)	//�����ǰ���� ��Ϸҳ��
					if (GetKey(KEY_SPACE) || GetKey(KEY_ESC)) { Sleep(300); return BUTTON_CONTIUNEGAME; }//���¿ո����ESC�ͼ�����Ϸ
			}
			else if (stcRcord.EventType == MOUSE_EVENT)//����������Ϣ
			{
				nRet = MouseEvenProc(stcRcord.Event.MouseEvent, nPage, MyMap);
				if (nRet != -1)break;//������ز�����-1��������Ч�ķ��أ�����ѭ�����������Ϣ
			}
		}

		//��ҳ����
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
				PrintTilteTextRandColor(13, 15);//��ӡ��ҳ����

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
					PrintTank(nTankX[i], nTankY[i], "��", nTankColor[i], nTankDir[i]);
				}
			}
		}
	}
	return nRet;

};