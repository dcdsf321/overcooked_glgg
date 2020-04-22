
#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
#include<windows.h>
#include<queue>
#include<cmath>
using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::Cook;//指定人物天赋。选手代码必须定义此变量，否则报错

 //保存初始时的地图，即只有各类墙体的位置信息
int map_start[50][50] = {
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
{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 } };
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
//上，左，右，下
int cookbook[51][4] =
{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1,0,0,0},
    {20,0,0,0},
    {20,4,0,0},
    {2,0,0,0},
    {3,0,0,0},
    {5,0,0,0},
    {3,4,0,0},
    {26,21,0,0},
    {11,21,0,0},
    {23,12,18,0},
    {23,13,14,0},
    {12,15,0,0},
    {14,24,0,0},
    {8,9,0,0},
    {22,11,15,0},
    {10,25,0,0},
    {13,24,0,0},
    {16,4,0,0},
    {4,20,25,10},
    {9,0,0,0},
    {6,7,3,10}
};
double COOK_x[4] = {8.5,8.5,33.5,41.5};
double COOK_y[4] = {24.5,24.5,18.5,28.5};
double SPAWN_x[4] = {7.5,7.5,25.5,42.5};
double SPAWN_y[4] = {41.5,41.5,5.5,40.5};
double commit_x[4] = {23.5,23.5,26.5,26.5};
double commit_y[4] = {25.5,25.5,24.5,25.5};
Direction DIRE_cook[4][2] = { {Right,Up},{Right,Up},{Up,Right},{Right,Up} };
Direction DIRE_commit[4] = { Right,Right,Left,Left };

int POSI;
MapInfo mapp;
int checkdish[50][50];
int checkdish_cook[51];
//DishType TASK;
bool checkbegin;
int vis_pre_x[51][51], vis_pre_y[51][51];

void Begin() {
    if (PlayerInfo.position.x < 5 && PlayerInfo.position.y < 5) POSI = 0;//左下
    if (PlayerInfo.position.x < 5 && PlayerInfo.position.y > 40) POSI = 1;//左上
    if (PlayerInfo.position.x > 40 && PlayerInfo.position.y < 5) POSI = 2;//右下
    if (PlayerInfo.position.x > 40 && PlayerInfo.position.y > 40) POSI = 3;//右上
    checkbegin = 1;
    checkdish[20][1] = 1;
    checkdish[21][20] = 1;
    checkdish[22][20] = 1; checkdish[22][4] = 2;
    checkdish[23][2] = 1;
    checkdish[24][3] = 1;
    checkdish[25][5] = 1;
    checkdish[26][3] = 1; checkdish[26][4] = 2;
    checkdish[27][26] = 1; checkdish[27][21] = 2;
    checkdish[28][11] = 1; checkdish[28][21] = 2;
    checkdish[29][23] = 1; checkdish[29][12] = 2; checkdish[29][18] = 3;
    checkdish[30][23] = 1; checkdish[30][13] = 2; checkdish[30][14] = 3;
    checkdish[31][12] = 1; checkdish[31][15] = 2;
    checkdish[32][14] = 1; checkdish[32][24] = 2;
    checkdish[33][8] = 1; checkdish[33][9] = 2;
    checkdish[34][22] = 1; checkdish[34][11] = 2; checkdish[34][15] = 3;
    checkdish[35][10] = 1; checkdish[35][25] = 2;
    checkdish[36][13] = 1; checkdish[36][24] = 2;
    checkdish[37][16] = 1; checkdish[37][4] = 2;
    checkdish[38][4] = 1; checkdish[38][20] = 2; checkdish[38][25] = 3; checkdish[38][10] = 4;
    checkdish[39][9] = 1;
    checkdish[40][6] = 1; checkdish[40][7] = 2; checkdish[40][3] = 3; checkdish[40][10] = 4;
}

//输出人物当前信息
/*void Print_player() {
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
    cout << "recieve_Text: " << PlayerInfo.recieveText << endl;
    cout << "Task: " << TASK << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;
}*/

int dishsize(DishType dd) {
    if (dd == Flour || dd == Noodle || dd == CookedRice || dd == Ketchup || dd == Cream || dd == SugarCoatedHaws) return 1;
    if (dd == Bread || dd == TomatoFriedEgg || dd == TomatoFriedEggNoodle || dd == BeefNoodle || dd == Barbecue || dd == FrenchFries || dd == PlumJuice || dd == StrawberryIcecream || dd == PopcornChicken || dd == AgaricFriedEgg) return 2;
    if (dd == OverRice || dd == YellowPheasant || dd == Hamburger) return 3;
    if (dd == Cake || dd == FruitSalad) return 4;
}

int dish_cooktime(DishType dd) {
    if (dd == Flour || dd == Noodle || dd == Bread || dd == CookedRice || dd == Ketchup || dd == Cream || dd == TomatoFriedEgg) return 10000;
    if (dd == PlumJuice || dd == StrawberryIcecream || dd == SugarCoatedHaws) return 10000;
    if (dd == TomatoFriedEggNoodle || dd == FrenchFries || dd == PopcornChicken || dd == AgaricFriedEgg) return 15000;
    if (dd == BeefNoodle || dd == OverRice || dd == YellowPheasant || dd == Barbecue || dd == Hamburger || dd == FruitSalad) return 20000;
    if (dd == Cake) return 30000;
    if (dd == SpicedPot) return 60000;
}

bool checktask(DishType dd) {
    for (list<DishType>::iterator iter = task_list.begin(); iter != task_list.end(); ++iter)
        if (*iter == dd) return 1;
    return 0;
}

Direction calcdirection(int x1, int y1, int x2, int y2) {
    if (x1 < x2 && y1 == y2) return Right;
    if (x1 < x2 && y1 < y2) return RightUp;
    if (x1 == x2 && y1 < y2) return Up;
    if (x1 > x2 && y1 < y2) return LeftUp;
    if (x1 > x2 && y1 == y2) return Left;
    if (x1 > x2 && y1 > y2) return LeftDown;
    if (x1 == x2 && y1 > y2) return Down;
    if (x1 > x2 && y1 < y2) return RightDown;
}

void Move_player(double sx, double sy, double ex, double ey) {   //sx=start_xposition, ex=end_position
    for (int i = 1; i <= 50; ++i)
        for (int j = 1; j <= 50; ++j) {
            vis_pre_x[i][j] = 0;                //保存进入队列时前置节点的x坐标
            vis_pre_y[i][j] = 0;                //保存进入队列时前置节点的y坐标
        }
    queue<int> qx, qy; bool flag = 0;            //qx,qy为两分量坐标的队列，flag 判断bfs时是否到达目标节点
    qx.push(int(sx)); qy.push(int(sy));
    vis_pre_x[int(sx)][int(sy)] = sx; vis_pre_y[int(sx)][int(sy)] = sy;
    while (!qx.empty() && flag == 0) {
        int nowx = qx.front(), nowy = qy.front();
        qx.pop(); qy.pop();
        for (int i = 0; i < 4; ++i) { //目前只考虑上下左右
            int newx = nowx + stepx[i], newy = nowy + stepy[i];
            if (newx >= 1 && newx <= 49 && newy >= 1 && newy <= 49 && map_start[newx][newy] == 0 && vis_pre_x[newx][newy] == 0) {
                vis_pre_x[newx][newy] = nowx; vis_pre_y[newx][newy] = nowy;
                if (newx == int(ex) && newy == int(ey)) {    //已到达目标节点
                    flag = 1;
                    break;
                }
                qx.push(newx); qy.push(newy);
            }
        }
    }
    int nowx = int(ex), nowy = int(ey);
    Direction next[51][51];            //保存行走方向
    while (!(nowx == int(sx) && nowy == int(sy))) { //反向遍历来保存行走路径
        next[vis_pre_x[nowx][nowy]][vis_pre_y[nowx][nowy]] = calcdirection(vis_pre_x[nowx][nowy], vis_pre_y[nowx][nowy], nowx, nowy); //计算前后节点相对位置即得到所需行走方向
        int tnowx = vis_pre_x[nowx][nowy], tnowy = vis_pre_y[nowx][nowy];
        nowx = tnowx; nowy = tnowy;
    }
    //真正的行走操作
    //Print_player();
    while (!(nowx == int(ex) && nowy == int(ey))) {
        THUAI3::move(next[nowx][nowy], 200);  //每次只移动一个单位 //速度为5的情况下
        Sleep(200);                      //挂起以等待操作完成
        nowx = PlayerInfo.position.x; //迭代
        nowy = PlayerInfo.position.y;
    }
    //Print_player(); Sleep(1000);
}

void Move_player_near(double sx, double sy, double ex, double ey) {
    double Ex = 233, Ey = 233;
    for (int i = 0; i < 4; ++i)
        if (abs(sx - Ex) + abs(sy - Ey) > abs(sx - ex - stepx[i]) + abs(sy - ey - stepy[i])) {
            Ex = ex + stepx[i];
            Ey = ey + stepy[i];
        }
    Move_player(sx, sy, Ex, Ey);
    THUAI3::move(calcdirection(Ex, Ey, ex, ey), 0); Sleep(500);
}

int findway(DishType start) {
    int flag = 1;
    if (!checkdish_cook[start]) {
        if (dishsize(start) == 0) flag = 0;
        for (int i = 0; i < dishsize(start); ++i) {
            int x; x = findway(DishType(cookbook[start][i]));
            if (!x) {
                flag = 0;
                break;
            }
            else { flag = min(flag, x); }
        }
    }
    else flag = checkdish_cook[start];
    if (flag) checkdish_cook[start] =flag;
    return flag;
}

void put_in_pot_and_cook(DishType task_tmp, DishType task_cur)
{
    double cook_x = COOK_x[POSI], cook_y = COOK_y[POSI];
    double spawn_x = SPAWN_x[POSI], spawn_y = SPAWN_y[POSI];
    //把锅里的东西都拿出来
    list<Obj> Objlist_pot = mapp.get_mapcell(cook_x, cook_y);
    DishType tmpp[100]; int tot = 0;
    for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
    {
        if (iter->dish!=0) tmpp[++tot] = iter->dish;
    }
    for (int i = 1; i <= tot; ++i) {
        THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
        THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
        if (POSI != 2) THUAI3::put(1, 1.57, true);
        else THUAI3::put(1, -1.57, true);
        Sleep(50);
    }
    //放锅里
    for (int i = 0; i < dishsize(task_tmp); ++i)
    {
        if (cookbook[task_tmp][i] >= 20 && cookbook[task_tmp][i] <= 25) put_in_pot_and_cook(DishType(cookbook[task_tmp][i]), task_cur);
        --checkdish_cook[cookbook[task_tmp][i]];
        THUAI3::move(DIRE_cook[POSI][1], 0); Sleep(50); 
        THUAI3::pick(false, Dish, cookbook[task_tmp][i]); Sleep(50); 
        THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50); //Print_player();
    }
    //制作
    //cout << "task_tmp= "<<task_tmp << endl;
    bool flagg = 0;
    if (!flagg) {
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(task_tmp));
        //提交
        THUAI3::pick(false, Block, task_tmp); Sleep(50); //Print_player();
        if (PlayerInfo.dish >= 49)//如果是黑暗料理就扔掉
        {
            THUAI3::put(1, 3.14, true); Sleep(50);
        }
        else if (PlayerInfo.dish >= 20 && PlayerInfo.dish <= 25) {
            if (POSI != 2) THUAI3::put(1, 1.57, true);
            else THUAI3::put(1, -1.57, true);
            Sleep(50);
        }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[POSI], commit_y[POSI]);
            THUAI3::move(DIRE_commit[POSI], 50); Sleep(50);
            if (checktask(task_tmp)) THUAI3::use(0, 0, 0);
            else THUAI3::put(1, 1.57, true);
            Sleep(50);
            list<Obj> Objlist_pot = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y+1);
            THUAI3::move(Up, 0); Sleep(50);
            DishType tmppp1[100]; int tott1=0;
            for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
            {
                if (checktask(iter->dish)) {
                    tmppp1[++tott1] = iter->dish;
                }
            }
            for (int i = 1; i <= tott1; ++i) {
                THUAI3::pick(false, Dish, tmppp1[i]); Sleep(50);
                THUAI3::move(DIRE_commit[POSI], 0); Sleep(50);
                if (checktask(task_tmp)) THUAI3::use(0, 0, 0);
                else {
                    if (POSI < 2) THUAI3::put(1, 1.57, true);
                    else THUAI3::put(1, -1.57, true);
                }
                THUAI3::move(Up, 0); Sleep(50);
            }
            Objlist_pot = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y);
            //THUAI3::move(Up, 0); Sleep(50);
            DishType tmppp2[100]; int tott2 = 0;
            for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
            {
                if (checktask(iter->dish)) {
                    tmppp2[++tott2] = iter->dish;
                }
            }
            for (int i = 1; i <= tott2; ++i) {
                THUAI3::pick(true, Dish, tmppp2[i]); Sleep(50);
                THUAI3::move(DIRE_commit[POSI], 0); Sleep(50);
                if (checktask(task_tmp)) THUAI3::use(0, 0, 0);
                else {
                    if (POSI < 2) THUAI3::put(1, 1.57, true);
                    else THUAI3::put(1, -1.57, true);
                }
                //THUAI3::move(Up, 0); Sleep(50);
            }
            Objlist_pot = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y - 1);
            THUAI3::move(Down, 0); Sleep(50);
            DishType tmppp3[100]; int tott3 = 0;
            for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
            {
                if (checktask(iter->dish)) {
                    tmppp3[++tott3] = iter->dish;
                }
            }
            for (int i = 1; i <= tott3; ++i) {
                THUAI3::pick(false, Dish, tmppp3[i]); Sleep(50);
                THUAI3::move(DIRE_commit[POSI], 0); Sleep(50);
                if (checktask(task_tmp)) THUAI3::use(0, 0, 0);
                else {
                    if (POSI < 2) THUAI3::put(1, 1.57, true);
                    else THUAI3::put(1, -1.57, true);
                }
                THUAI3::move(Down, 0); Sleep(50);
            }
        }
        if (POSI != 2) Move_player(PlayerInfo.position.x, PlayerInfo.position.y, cook_x - 1, cook_y);
        else Move_player(PlayerInfo.position.x, PlayerInfo.position.y, cook_x, cook_y - 1);
        THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
    }
}

void task_finish(DishType task, DishType task_root) {
    //TASK = task;
    //Print_player();
    double cook_x = COOK_x[POSI], cook_y = COOK_y[POSI];
    double spawn_x = SPAWN_x[POSI], spawn_y = SPAWN_y[POSI];
    while (checktask(task_root)) {
        //工作台旁如果有当前任务的原料就放锅里（改成，工作台旁如果有足够完成一个任务的食材，就把锅里的东西取出来，食材全部放锅里
        if ((((PlayerInfo.position.x - (cook_x - 1)) < 1e-4) && ((PlayerInfo.position.y - cook_y) < 1e-4)&&POSI!=2)||(POSI==2&&(PlayerInfo.position.x-cook_x<1e-4)&&(PlayerInfo.position.y-cook_y+1<1e-4))) {
            //把锅里的东西都拿出来
            list<Obj> Objlist_pot = mapp.get_mapcell(cook_x, cook_y);
            THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
            DishType tmpp[100]; int tot = 0;
            for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
            {
                if (iter->dish!=0) tmpp[++tot] = iter->dish;
            }
            for (int i = 1; i <= tot; ++i) {
                THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
                THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
                if (POSI != 2) THUAI3::put(1, 1.57, true);
                else THUAI3::put(1, -1.57, true);
                Sleep(50);
            }
            //遍历所有当前格子中的食材
            THUAI3::move(DIRE_cook[POSI][1], 0);
            list<Obj> Objlist_cook;
            if (POSI != 2) Objlist_cook = mapp.get_mapcell(cook_x - 1, cook_y + 1);
            else Objlist_cook = mapp.get_mapcell(cook_x + 1, cook_y - 1);
            for (int i = 1; i <= 50; ++i) checkdish_cook[i] = 0;
            for (list<Obj>::iterator iter = Objlist_cook.begin(); iter != Objlist_cook.end(); ++iter) ++checkdish_cook[iter->dish];
            //遍历所有任务，看有没有可以做的
            for (list<DishType>::iterator iter = task_list.begin(); iter != task_list.end(); ++iter)
            {
                if (findway(*iter))
                {
                    put_in_pot_and_cook(*iter, task);
                }
            }
        }
        //去食物产生点取食材
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, spawn_x, spawn_y);
        THUAI3::pick(false, Block, mapp.get_mapcell(spawn_x, spawn_y).back().dish); Sleep(500);
        while (PlayerInfo.dish == 0) {
            THUAI3::pick(false, Block, mapp.get_mapcell(spawn_x, spawn_y).back().dish); Sleep(50);
        }
        while (PlayerInfo.dish == NeedleMushroom) {
            THUAI3::put(2, 0, true); Sleep(10000);
            THUAI3::pick(false, Block, mapp.get_mapcell(spawn_x, spawn_y).back().dish); Sleep(500);
        }
        //去工作台放食材
        if (POSI!=2) Move_player(PlayerInfo.position.x, PlayerInfo.position.y, cook_x - 1, cook_y);
        else Move_player(PlayerInfo.position.x, PlayerInfo.position.y, cook_x, cook_y-1);
        THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
        if (POSI != 2) THUAI3::put(1, 1.57, true);
        else THUAI3::put(1, -1.57, true);
        Sleep(50); //Print_player();
    }
}

void play()
{
    if (!checkbegin) Begin();
    else {
        //Print_player();
        if (PlayerInfo.id & 1) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[POSI], SPAWN_y[POSI]);
            task_finish(task_list.back(), task_list.back());
        }
    }
}