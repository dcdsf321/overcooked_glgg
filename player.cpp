#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
#include<windows.h>
#include<queue>
#define PX PlayerInfo.position.x
#define PY PlayerInfo.position.y
using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::Cook;//指定人物天赋。选手代码必须定义此变量，否则报错

int map_start[50][50]= {
{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 5, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 6, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 2, 0, 0, 0, 5, 5, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }};
// 0 : 空地
// 1 : 提交点，位于中心2x2区域
// 2 : 食材产生点，共3个
// 3 : 工作台，用于合成食物，共4个
// 4 : 垃圾桶，共5个
// 5 : 障碍，不可穿过，不透明
// 6 : 桌子，不可穿过，可放置物品

//double stepx[8] = { -1,0,1,-1,1,-1,1 };
//double stepy[8] = { 1,1,1,0,0,-1,-1,-1 };
//左上，上，右上，左，右，左下，下，右下
double stepx[4] = { 0,-1,1,0 };
double stepy[4] = { 1,0,0,-1 };

int vis_pre_x[51][51], vis_pre_y[51][51];
/*
class XY_position {
public:
	double x;
	double y;
	XY_position() { x = 0; y = 0; }
	XY_position(double xx, double yy) { x = xx; y = yy; }
	~XY_position() { x = 0; y = 0; }
};

class Heap_Point {
private:
	//XY_position position;
	double value;
public:
	XY_position position;
	Heap_Point() { value = 0; }
	Heap_Point(XY_position node, double V) { position = node;value=V; }
	friend bool operator < (const Heap_Point &x,const Heap_Point& y) {
		return x.value < y.value;
	}
	~Heap_Point(){ value = 0; }
};
class PQ {
private:
	Heap_Point heap[2550];
	int Len;
public:
	PQ() {Len = 0;}
	void push(XY_position node, double V) {
		Heap_Point Now(node,V);
		heap[++Len] = Now;
		int point = Len;
		while (point != 1) {
			if (heap[point] < heap[point / 2]) {
				Heap_Point c = heap[point];
				heap[point] = heap[point / 2];
				heap[point / 2] = c;
				point /= 2;
			}else break;
		}
	}
	Heap_Point pop() {
		Heap_Point ans = heap[1];
		heap[1] = heap[Len--];
		int point = 1;
		while (point * 2 <= Len) {
			int now;
			if ((point * 2 + 1 <= Len) && (heap[point * 2+1] < heap[point * 2 ])) now = point * 2 + 1;
			else now = point * 2;
			if (heap[now] < heap[point]) {
				Heap_Point temp = heap[now];
				heap[now] = heap[point];
				heap[point] = heap[now];
				point = now;
			}else break;
		}
		return ans;
	}
}open_list;
double F_value[51][51], G_value[51][51], H_value[51][51];
bool viso[51][51], visc[51][51];
*/
//输出人物当前信息
void Print_player() {
	cout << "-----------------------------------------------------------------------------------" << endl;
	cout << "ID: " << PlayerInfo.id << endl;
	cout << "xy_position: " << PlayerInfo.position.x << " " << PlayerInfo.position.y << endl;
	cout << "facing_direction: " << PlayerInfo.facingDirection << endl;
	cout << "move_speed: " << PlayerInfo.moveSpeed << endl;
	cout << "sight_range: " << PlayerInfo.sightRange << endl;
	cout << "talent: " << PlayerInfo.talent << endl;
	cout << "score: " << PlayerInfo.score << endl;
	cout << "dish: " << PlayerInfo.dish << endl;
	cout << "tool: " << PlayerInfo.tool << endl;
	cout << "recieve_Text: " << PlayerInfo.recieveText << endl ;
	cout << "-----------------------------------------------------------------------------------" << endl;
}

Direction calcdirection(int x1, int y1, int x2, int y2) {
	if (x1 + 1 == x2 && y1 == y2) return Right;
	if (x1 + 1 == x2 && y1 + 1 == y2) return RightUp;
	if (x1 == x2 && y1 + 1 == y2) return Up;
	if (x1 - 1 == x2 && y1 + 1 == y2) return LeftUp;
	if (x1 - 1 == x2 && y1 == y2) return Left;
	if (x1 - 1 == x2 && y1 - 1 == y2) return LeftDown;
	if (x1 - 1 == x2 && y1 == y2) return Down;
	if (x1 - 1 == x2 && y1 + 1 == y2) return RightDown;
}

void Move_player(double sx, double sy, double ex, double ey) {
	/*for (int i=0;i<=50;++i)
		for (int j = 0; j <= 50; ++j) {
			F_value[i][j] = abs(ex-i)+abs(ey-j);
			G_value[i][j] = 0;
			H_value[i][j] = abs(ex-i)+abs(ey-j);
			viso[i][j] = 0;
			visc[i][j] = 0;
		}
	viso[int(sx)][int(sy)] = 1;
	open_list.push(XY_position(sx, sy),F_value[int(sx)][int(sy)]);
	while (1) {
		Heap_Point now = open_list.pop();
		for (int i = 0; i < 8; ++i) {
			double xp = now.position.x + stepx[i], yp = now.position.y + stepy[i];
			if (xp>=1&&xp<=)
		}
	}*/
	for (int i=1;i<=50;++i)
		for (int j = 1; j <= 50; ++j) {
			vis_pre_x[i][j] = 0;
			vis_pre_y[i][j] = 0;
		}
	queue<int> qx, qy; bool flag = 0;
	qx.push(int(sx)); qy.push(int(sy)); 
	vis_pre_x[int(sx)][int(sy)] = sx; vis_pre_y[int(sx)][int(sy)] = sy;
	while (!qx.empty()&&flag==0) {
		int nowx = qx.front(),nowy=qy.front();
		qx.pop(); qy.pop();
		for (int i = 0; i < 4; ++i) {
			int xx = nowx + stepx[i], yy = nowy + stepy[i];
			if (xx >= 1 && xx <= 49 && yy >= 1 && yy <= 49 && map_start[xx][yy] == 0&&vis_pre_x[xx][yy]==0) {
				vis_pre_x[xx][yy] = nowx; vis_pre_y[xx][yy] = nowy;
				if (xx == int(ex) && yy == int(ey)) {
					flag = 1;
					break;
				}
				qx.push(xx); qy.push(yy);
			}
		}
	}
	int xx = int(ex), yy = int(ey);
	Direction next[51][51];
	while (!(xx == int(sx) && yy == int(sy))) {
		int xxx = vis_pre_x[xx][yy], yyy = vis_pre_y[xx][yy];
		next[xxx][yyy] = calcdirection(xxx, yyy, xx, yy);
		//cout << "(" << xxx << "," << yyy << ")-->(" << xx << "," << yy << ")   " << endl;
		/*switch (next[xxx][yyy]) {
			case Right: cout << "Right" << endl << endl; break;
			case RightUp: cout << "RightUp" << endl << endl; break;
			case Up: cout << "Up" << endl << endl; break;
			case LeftUp: cout << "LeftUp" << endl << endl; break;
			case Left: cout << "Left" << endl << endl; break;
			case LeftDown: cout << "LeftDown" << endl << endl; break;
			case Down: cout << "Down" << endl << endl; break;
			case RightDown: cout << "RightDown" << endl << endl;
		}*/
		xx = xxx; yy = yyy;
	}
	
	while (!(xx == int(ex) && yy == int(ey))) {
		/*switch (next[xx][yy]) {
			case Right: move(Right, 200); Sleep(400); break;
			case RightUp: move(RightUp, 200); Sleep(400); break;
			case Up: move(Up, 200); Sleep(400); break;
			case LeftUp: move(LeftUp, 200); Sleep(400); break;
			case Left: move(Left, 200); Sleep(400); break;
			case LeftDown: move(LeftDown, 200); Sleep(400); break;
			case Down: move(Down, 200); Sleep(400); break;
			case RightDown: move(RightDown, 200); Sleep(400);
		}*/
		move(next[xx][yy], 200);
		Sleep(400);
		xx = PlayerInfo.position.x;
		yy = PlayerInfo.position.y;
		/*switch (next[xx][yy]) {
		case Right: xx = xx + 1; break;
		case RightUp: xx = xx + 1; yy = yy + 1; break;
		case Up: yy = yy + 1; break;
		case LeftUp: xx = xx - 1; yy = yy + 1; break;
		case Left: xx = xx - 1; break;
		case LeftDown: xx = xx - 1; yy = yy - 1; break;
		case Down: yy = yy - 1; break;
		case RightDown: xx = xx + 1; yy = yy - 1;
		}*/
		Print_player();
	}
}

void play()
{
	Print_player();
	Move_player(2.5, 1.5, 40, 40);
	PauseCommunication();
	Sleep(10000);
}