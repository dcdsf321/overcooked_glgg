#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
#include<queue>
#define PI 3.1415926
using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::Cook;//指定人物天赋。选手代码必须定义此变量，否则报错

 //保存初始时的地图，即只有各类墙体的位置信息
int map_start[50][50] = {
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 5, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 6, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5},
    {5, 0, 0, 0, 0, 2, 0, 0, 0, 5, 5, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}
};
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
int cookbook[51][4];
double COOK_x[4] = { 25.5,41.5,33.5,8.5 };
double COOK_y[4] = { 38.5,28.5,18.5,24.5 };
double SPAWN_x[30];
double SPAWN_y[30];
double commit_x[4] = { 23.5,23.5,26.5,26.5 };
double commit_y[4] = { 24.5,25.5,24.5,25.5 };
Direction DIRE_cook[4][2] = { {Right,Up},{Right,Up},{Up,Right},{Right,Up} };
Direction DIRE_commit[4] = { Right,Right,Left,Left };

int POSI;
MapInfo mapp;
int checkdish[50][50];
int checkdish_cook[51];
bool checkbegin;
int checkmember;
int vis_pre_x[51][51], vis_pre_y[51][51];

void Begin() {
    if (PlayerInfo.position.x < 5 && PlayerInfo.position.y < 5) POSI = 0;//左下
    if (PlayerInfo.position.x < 5 && PlayerInfo.position.y > 40) POSI = 1;//左上
    if (PlayerInfo.position.x > 40 && PlayerInfo.position.y < 5) POSI = 3;//右下
    if (PlayerInfo.position.x > 40 && PlayerInfo.position.y > 40) POSI = 3;//右上
    if (abs(PlayerInfo.position.x - 1.5) < 1e-4 || abs(PlayerInfo.position.x - 49.5) < 1e-4) checkmember = 1;
    else {
        checkmember = 2;
        POSI = 3 - POSI;
    }
    checkbegin = 1;
    checkdish[int(Flour)][int(Wheat)] = 1;
    checkdish[int(Noodle)][int(Flour)] = 1;
    checkdish[int(Bread)][int(Egg)] = 1; checkdish[int(Bread)][int(Flour)] = 2;
    checkdish[int(CookedRice)][int(Rice)] = 1;
    checkdish[int(Ketchup)][int(Tomato)] = 1;
    checkdish[int(TomatoFriedEgg)][int(Tomato)] = 1; checkdish[int(TomatoFriedEgg)][int(Egg)] = 2;
    checkdish[int(TomatoFriedEggNoodle)][int(Noodle)] = 1; checkdish[int(TomatoFriedEggNoodle)][int(TomatoFriedEgg)] = 2;
    checkdish[int(BeefNoodle)][int(Beef)] = 1; checkdish[int(BeefNoodle)][int(Noodle)] = 2;
    checkdish[int(OverRice)][int(Rice)] = 1; checkdish[int(OverRice)][int(Pork)] = 2; checkdish[int(OverRice)][int(Potato)] = 3;
    checkdish[int(Barbecue)][int(Pork)] = 1; checkdish[int(Barbecue)][int(Lettuce)] = 2;
    checkdish[int(FrenchFries)][int(Potato)] = 1; checkdish[int(FrenchFries)][int(Ketchup)] = 2;
    checkdish[int(Hamburger)][int(Beef)] = 1; checkdish[int(Hamburger)][int(Lettuce)] = 2; checkdish[int(Hamburger)][int(Bread)] = 3; 
    cookbook[int(Flour)][0] = int(Wheat);
    cookbook[int(Noodle)][0] = int(Flour);
    cookbook[int(Bread)][0] = int(Egg); cookbook[int(Bread)][1] = int(Flour);
    cookbook[int(CookedRice)][0] = int(Rice);
    cookbook[int(Ketchup)][0] = int(Tomato);
    cookbook[int(TomatoFriedEgg)][0] = int(Tomato); cookbook[int(TomatoFriedEgg)][1] = int(Egg);
    cookbook[int(TomatoFriedEggNoodle)][0] = int(Noodle); cookbook[int(TomatoFriedEggNoodle)][1] = int(TomatoFriedEgg);
    cookbook[int(BeefNoodle)][0] = int(Beef); cookbook[int(BeefNoodle)][1] = int(Noodle);
    cookbook[int(OverRice)][0] = int(Rice); cookbook[int(OverRice)][1] = int(Pork); cookbook[int(OverRice)][2] = int(Potato);
    cookbook[int(Barbecue)][0] = int(Pork); cookbook[int(Barbecue)][1] = int(Lettuce);
    cookbook[int(FrenchFries)][0] = int(Potato); cookbook[int(FrenchFries)][1] = int(Ketchup);
    cookbook[int(Hamburger)][0] = int(Beef); cookbook[int(Hamburger)][1] = int(Lettuce); cookbook[int(Hamburger)][2] = int(Bread);
    SPAWN_x[int(Wheat)] = 4.5; SPAWN_y[int(Wheat)] = 24.5;
    SPAWN_x[int(Rice)] = 5.5; SPAWN_y[int(Rice)] = 5.5;
    SPAWN_x[int(Tomato)] = 7.5; SPAWN_y[int(Tomato)] = 41.5;
    SPAWN_x[int(Egg)] = 25.5; SPAWN_y[int(Egg)] = 5.5;
    SPAWN_x[int(Beef)] = 31.5; SPAWN_y[int(Beef)] = 41.5;
    SPAWN_x[int(Pork)] = 42.5; SPAWN_y[int(Pork)] = 40.5;
    SPAWN_x[int(Potato)] = 43.5; SPAWN_y[int(Potato)] = 6.5;
    SPAWN_x[int(Lettuce)] = 43.5; SPAWN_y[int(Lettuce)] = 25.5;
}

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
    cout << "recieve_Text: " << PlayerInfo.recieveText << endl;
    //cout << "Task: " << TASK << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;
}

int dishsize(DishType dd) {
    if (dd == Flour || dd == Noodle || dd == CookedRice || dd == Ketchup ) return 1;
    if (dd == Bread || dd == TomatoFriedEgg || dd == TomatoFriedEggNoodle || dd == BeefNoodle || dd == Barbecue || dd == FrenchFries ) return 2;
    if (dd == OverRice || dd == Hamburger) return 3;
}

int dish_cooktime(DishType dd) {
    if (dd == Flour || dd == Noodle || dd == Bread || dd == CookedRice || dd == Ketchup ) return 10000;
    if (dd == FrenchFries ) return 15000;
    if (dd == TomatoFriedEggNoodle || dd == BeefNoodle || dd == OverRice  || dd == Barbecue || dd == Hamburger || dd == TomatoFriedEgg) return 20000;
    if (dd == SpicedPot) return 60000;
}

bool checkDishDirection(DishType d) {
    if (d == TomatoFriedEggNoodle || d == BeefNoodle || d == FrenchFries || d == Hamburger) return 0;
    return 1;
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
    while (!(nowx == int(ex) && nowy == int(ey))) {
        THUAI3::move(next[nowx][nowy], 250);  //每次只移动一个单位 //速度为5的情况下
        Sleep(500);                      //挂起以等待操作完成
        Print_player();
        nowx = PlayerInfo.position.x; //迭代
        nowy = PlayerInfo.position.y;
    }
}

void Move_player_near(double sx, double sy, double ex, double ey) {
    double Ex = 233, Ey = 233;
    for (int i = 0; i < 4; ++i)
        if (abs(sx - Ex) + abs(sy - Ey) > abs(sx - ex - stepx[i]) + abs(sy - ey - stepy[i])) {
            Ex = ex + stepx[i];
            Ey = ey + stepy[i];
        }
    Move_player(sx, sy, Ex, Ey);
    THUAI3::move(calcdirection(Ex, Ey, ex, ey), 0); Sleep(100);
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
    if (flag) checkdish_cook[start] = flag;
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
        if (iter->dish != 0) tmpp[++tot] = iter->dish;
    }
    for (int i = 1; i <= tot; ++i) {
        THUAI3::move(DIRE_cook[POSI][0], 0); Sleep(50);
        THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
        if (POSI != 2) THUAI3::put(1, 1.57, true);
        else THUAI3::put(1, 0, true);
        Sleep(50);
    }
    //放锅里
    THUAI3::move(DIRE_cook[POSI][1], 0); Sleep(50);
    for (int i = 0; i < dishsize(task_tmp); ++i)
    {
        if (cookbook[task_tmp][i] >= 20 && cookbook[task_tmp][i] <= 26) {
            THUAI3::pick(false, Dish, cookbook[task_tmp][i]); Sleep(50);
            if (PlayerInfo.dish != 0) {
                if (POSI != 2) THUAI3::put(1, 0, true);
                else put(1, 1.57, true);
                Sleep(50);
                continue;
            }
        }
        if (cookbook[task_tmp][i] >= 20 && cookbook[task_tmp][i] <= 26) put_in_pot_and_cook(DishType(cookbook[task_tmp][i]), task_cur);
    }
    for (int i = 0; i < dishsize(task_tmp); ++i)
    {
        --checkdish_cook[cookbook[task_tmp][i]];
        THUAI3::move(DIRE_cook[POSI][1], 0); Sleep(50);
        THUAI3::pick(false, Dish, cookbook[task_tmp][i]); Sleep(50);
        THUAI3::move(DIRE_cook[POSI][0], 0); Sleep(50);
        if (POSI != 2) THUAI3::put(1, 0, true);  //Print_player();
        else put(1, 1.57, true);
        Sleep(50);
    }
    //制作
    bool flagg = 0;
    if (!flagg) {
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(task_tmp));
        //提交
        THUAI3::pick(false, Block, task_tmp); Sleep(50); //Print_player();
        if (PlayerInfo.dish ==DarkDish || PlayerInfo.dish==OverCookedDish)//如果是黑暗料理就扔掉
        {
            THUAI3::put(1, 3.14, true); Sleep(50);
        }
        else if (PlayerInfo.dish >= 20 && PlayerInfo.dish <= 25) {
            if (POSI != 2) THUAI3::put(1, 1.57, true);
            else THUAI3::put(1, 0, true);
            Sleep(50);
        }
        else if (PlayerInfo.dish == 26) {
            if (!checktask(PlayerInfo.dish)) {
                if (POSI != 2) THUAI3::put(1, 1.57, true);
                else THUAI3::put(1, 0, true);
                Sleep(50);
            }
            else {
                Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[POSI], commit_y[POSI]);
                THUAI3::move(DIRE_commit[POSI], 50); Sleep(50);
                if (checktask(task_tmp)) THUAI3::use(0, 0, 0);
                else THUAI3::put(1, 1.57, true);
                Sleep(50);
                list<Obj> Objlist_pot = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y + 1);
                THUAI3::move(Up, 0); Sleep(50);
                DishType tmppp1[100]; int tott1 = 0;
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
                        THUAI3::put(1, 1.57, true);
                        //else THUAI3::put(1, 1.57, true);
                    }
                    THUAI3::move(Up, 0); Sleep(50);
                }
                Objlist_pot = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y);
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
                        THUAI3::put(1, 1.57, true);
                        //else THUAI3::put(1, 1.57, true);
                    }
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
                        THUAI3::put(1, 1.57, true);
                        //else THUAI3::put(1, 1.57, true);
                    }
                    THUAI3::move(Down, 0); Sleep(50);
                }
            }
        }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[POSI], commit_y[POSI]);
            THUAI3::move(DIRE_commit[POSI], 50); Sleep(50);
            if (checktask(task_tmp)) THUAI3::use(0, 0, 0);
            else THUAI3::put(1, 1.57, true);
            Sleep(50);
            list<Obj> Objlist_pot = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y + 1);
            THUAI3::move(Up, 0); Sleep(50);
            DishType tmppp1[100]; int tott1 = 0;
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
                    THUAI3::put(1, 1.57, true);
                }
                THUAI3::move(Up, 0); Sleep(50);
            }
            Objlist_pot = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y);
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
                    THUAI3::put(1, 1.57, true);
                }
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
                    THUAI3::put(1, 1.57, true);
                }
                THUAI3::move(Down, 0); Sleep(50);
            }
        }
    }
}
/*
void task_finish(DishType task, DishType task_root) {
    double cook_x = COOK_x[POSI], cook_y = COOK_y[POSI];
    double spawn_x = SPAWN_x[POSI], spawn_y = SPAWN_y[POSI];
    while (checktask(task_root)) {
        //工作台旁如果有当前任务的原料就放锅里（改成，工作台旁如果有足够完成一个任务的食材，就把锅里的东西取出来，食材全部放锅里
        if ((((PlayerInfo.position.x - (cook_x - 1)) < 1e-4) && ((PlayerInfo.position.y - cook_y) < 1e-4) && POSI != 2) || (POSI == 2 && (PlayerInfo.position.x - cook_x < 1e-4) && (PlayerInfo.position.y - cook_y + 1 < 1e-4))) {
            //把锅里的东西都拿出来
            list<Obj> Objlist_pot = mapp.get_mapcell(cook_x, cook_y);
            THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
            DishType tmpp[100]; int tot = 0;
            for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
            {
                if (iter->dish != 0) tmpp[++tot] = iter->dish;
            }
            for (int i = 1; i <= tot; ++i) {
                THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
                THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
                if (POSI != 2) THUAI3::put(1, 1.57, true);
                else THUAI3::put(1, 0, true);
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
                    break;
                }
            }
        }
        //去食物产生点取食材
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, spawn_x, spawn_y);
        THUAI3::pick(false, Block, mapp.get_mapcell(spawn_x, spawn_y).back().dish); Sleep(500);
        while (PlayerInfo.dish == 0) {
            THUAI3::pick(false, Block, mapp.get_mapcell(spawn_x, spawn_y).back().dish); Sleep(50);
        }
        while ( PlayerInfo.dish == 0) {
            THUAI3::put(2, 0, true); Sleep(1000);
            THUAI3::pick(false, Block, mapp.get_mapcell(spawn_x, spawn_y).back().dish); Sleep(50);
        }
        //去工作台放食材
        if (POSI != 2) Move_player(PlayerInfo.position.x, PlayerInfo.position.y, cook_x - 1, cook_y);
        else Move_player(PlayerInfo.position.x, PlayerInfo.position.y, cook_x, cook_y - 1);
        THUAI3::move(DIRE_cook[POSI][0], 50); Sleep(50);
        if (POSI != 2) THUAI3::put(1, 1.57, true);
        else THUAI3::put(1, 0, true);
        Sleep(50); //Print_player();
    }
}*/

void task_finish(DishType tasknow) {
    if (checkDishDirection(tasknow)) {
        for (int i = 1; i <= dishsize(tasknow); ++i)
        { }
    }
    if (tasknow == Hamburger) {
        //拿鸡蛋+放鸡蛋
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Egg)], SPAWN_y[int(Egg)]);
        THUAI3::pick(false, Block, Egg); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Egg);  }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 2);
        THUAI3::put(1, PI/2, true); Sleep(1000);
        //拿小麦+做面粉
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
        THUAI3::pick(false, Block, Wheat); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Wheat); }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]); move(Right, 0); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
        {
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::move(Right, 0); Sleep(50);
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(1, PI*1.5, true);
            Sleep(50);
        }
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Wheat); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(1000);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Flour));
        //拿面粉+拿鸡蛋+做面包
        THUAI3::pick(false, Block, Flour); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Egg); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50); move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Bread));
        //拿面包+放面包
        THUAI3::pick(false, Block, Bread); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]); Sleep(50);
        THUAI3::put(1, PI / 2, true); Sleep(50);
        //拿生菜+放生菜
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Lettuce)], SPAWN_y[int(Lettuce)]);
        THUAI3::pick(false, Block, Lettuce); Sleep(50);
        while (PlayerInfo.dish == 0) {Sleep(1000); THUAI3::pick(false, Block, Lettuce);}
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0] + 1); Sleep(50);
        THUAI3::put(1, PI, true); Sleep(50);
        //拿牛肉+做汉堡
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Beef)], SPAWN_y[int(Beef)]);
        THUAI3::pick(false, Block, Beef); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Beef); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0] + 1); Sleep(50);
        THUAI3::put(1, PI, true); Sleep(50);
        Objlist_pot = mapp.get_mapcell(COOK_x[0], COOK_y[0]);
        for (int i = 0; i < 100;++i) tmpp[i]=DishType(0); tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
        {
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::move(Down, 0); Sleep(50);
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(1, PI , true);
            Sleep(50);
        }
        move(Left, 0); Sleep(50);
        THUAI3::pick(false, Dish, Bread); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        THUAI3::pick(false, Dish, Beef); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        THUAI3::pick(false, Dish, Lettuce); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        move(Down, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Hamburger));
        //拿汉堡+交汉堡
        THUAI3::pick(false, Block, Hamburger); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        move(Left, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
    }
    if (tasknow == CookedRice) {
        //拿米饭+放米饭
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Rice)], SPAWN_y[int(Rice)]);
        THUAI3::pick(false, Block, Rice); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Rice); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        //做米饭
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
        {
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::move(Right, 0); Sleep(50);
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(1, PI * 1.5, true);
            Sleep(50);
        }
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Rice); Sleep(50);
        move(Right, 0); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(CookedRice));
        //拿米饭+交米饭
        THUAI3::pick(false, Block, CookedRice); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
    }
    if (tasknow == OverRice) {
        //拿小麦+放小麦
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Rice)], SPAWN_y[int(Rice)]);
        THUAI3::pick(false, Block, Rice); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Rice); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] - 1, COOK_y[1] + 1);
        THUAI3::put(1, 0, true); Sleep(50);
        //拿猪肉+放猪肉
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Pork)], SPAWN_y[int(Pork)]);
        THUAI3::pick(false, Block, Pork); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Pork); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] , COOK_y[1] + 2);
        THUAI3::put(1, PI*1.5, true); Sleep(50);
        //拿土豆+放土豆
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Potato)], SPAWN_y[int(Potato)]);
        THUAI3::pick(false, Block, Potato); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Potato); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] - 1, COOK_y[1] + 1);
        THUAI3::put(1, 0, true); Sleep(50);
        //清空锅+做盖浇饭
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[1], COOK_y[1]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
        {
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::move(Down, 250); Sleep(500);
            THUAI3::move(Right, 0); Sleep(50);
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::move(Up, 250); Sleep(500);
            THUAI3::put(1, 0, true);Sleep(50);
        }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] - 1, COOK_y[1] + 1);
        move(Right, 0); Sleep(50);
        THUAI3::pick(false, Dish, Rice); Sleep(50);
        move(Down, 250); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] - 1, COOK_y[1] + 1);
        move(Right, 0); Sleep(50);
        THUAI3::pick(false, Dish, Pork); Sleep(50);
        move(Down, 250); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] - 1, COOK_y[1] + 1);
        move(Right, 0); Sleep(50);
        THUAI3::pick(false, Dish, Potato); Sleep(50);
        move(Down, 250); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(OverRice));
        //拿盖浇饭+交盖浇饭
        THUAI3::pick(false, Block, OverRice); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        move(Left, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
    }
}

void play()
{
    if (!checkbegin) Begin();
    else {
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
        Sleep(1000);
        THUAI3::pick(false, Block, mapp.get_mapcell(SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]).back().dish); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Potato)], SPAWN_y[int(Potato)]);
        //if (checkmember == 1) {
        //task_finish(task_list.back());// , task_list.back());
        //}
        //else {
        //    Sleep(1000);
        //    Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
       // }
    }
}