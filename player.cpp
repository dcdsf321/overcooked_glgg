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
int taskWait[51];

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

int dishsize(DishType dd) {
    if (dd == Flour || dd == Noodle || dd == CookedRice || dd == Ketchup) return 1;
    if (dd == Bread || dd == TomatoFriedEgg || dd == TomatoFriedEggNoodle || dd == BeefNoodle || dd == Barbecue || dd == FrenchFries) return 2;
    if (dd == OverRice || dd == Hamburger) return 3;
}

int dish_cooktime(DishType dd) {
    if (dd == Flour || dd == Noodle || dd == Bread || dd == CookedRice || dd == Ketchup) return 10000;
    if (dd == FrenchFries) return 15000;
    if (dd == TomatoFriedEggNoodle || dd == BeefNoodle || dd == OverRice || dd == Barbecue || dd == Hamburger || dd == TomatoFriedEgg) return 20000;
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
        Sleep(300);                      //挂起以等待操作完成
        //Print_player();
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
    THUAI3::move(calcdirection(Ex, Ey, ex, ey), 0); Sleep(50);
}

void task_finish(DishType tasknow) {
    if (tasknow == TomatoFriedEgg) {
        //拿鸡蛋+放鸡蛋
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Egg)], SPAWN_y[int(Egg)]);
        THUAI3::pick(false, Block, Egg); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Egg); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 2);
        THUAI3::put(1, PI / 2, true); Sleep(50);
        //拿番茄+拿鸡蛋+做番茄炒蛋
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Tomato)], SPAWN_y[int(Tomato)]);
        THUAI3::pick(false, Block, Tomato); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Tomato); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]); Sleep(50);
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
            THUAI3::put(1, PI * 1.5, true);
            Sleep(50);
        }
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Tomato); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        THUAI3::pick(false, Dish, Egg); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(TomatoFriedEgg));
        //拿番茄炒蛋+交番茄炒蛋
        THUAI3::pick(false, Block, TomatoFriedEgg); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
        move(Right, 0); Sleep(50);
        if (checktask(tasknow)) {THUAI3::use(0, 0, 0); Sleep(50);}
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
            THUAI3::put(1, PI / 2, true); Sleep(50);
            ++taskWait[tasknow];
        }
    }
    if (tasknow == TomatoFriedEggNoodle) {
        //拿鸡蛋+放鸡蛋
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Egg)], SPAWN_y[int(Egg)]);
        THUAI3::pick(false, Block, Egg); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Egg); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 2);
        THUAI3::put(1, PI / 2, true); Sleep(50);
        //拿番茄+拿鸡蛋+做番茄炒蛋
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Tomato)], SPAWN_y[int(Tomato)]);
        THUAI3::pick(false, Block, Tomato); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Tomato); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]); Sleep(50);
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
            THUAI3::put(1, PI * 1.5, true);
            Sleep(50);
        }
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Tomato); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        THUAI3::pick(false, Dish, Egg); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(TomatoFriedEgg));
        //拿番茄炒蛋+放番茄炒蛋
        THUAI3::pick(false, Block, TomatoFriedEgg); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        //拿小麦+做面粉
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
        THUAI3::pick(false, Block, Wheat); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Wheat); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        for (int i = 0; i < 100;++i) tmpp[i]=DishType(0); tot = 0;
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
        THUAI3::pick(false, Dish, Wheat); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(1000);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Flour));
        //拿面粉+做面条
        THUAI3::pick(false, Block, Flour); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Noodle));
        //拿面条+放面条+拿番茄炒蛋+拿面+做番茄炒蛋面
        THUAI3::pick(false, Block, Noodle); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        THUAI3::move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, TomatoFriedEgg); Sleep(50);
        THUAI3::move(Right, 0); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(TomatoFriedEggNoodle));
        //拿番茄炒蛋面+交番茄炒蛋面
        THUAI3::pick(false, Block, TomatoFriedEggNoodle); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
        move(Right, 0); Sleep(50);
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
            THUAI3::put(1, PI / 2, true); Sleep(50);
            ++taskWait[tasknow];
        }
    }
    if (tasknow == BeefNoodle) {
        //拿小麦+做面粉
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
        THUAI3::pick(false, Block, Wheat); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Wheat); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]); Sleep(50);
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
            THUAI3::put(1, PI * 1.5, true);
            Sleep(50);
        }
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Wheat); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(1000);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Flour));
        //拿面粉+做面条
        THUAI3::pick(false, Block, Flour); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Noodle));
        //拿面条+放面条+拿牛肉+做牛肉面
        THUAI3::pick(false, Block, Noodle); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0] + 1); Sleep(50);
        THUAI3::put(1, PI, true); Sleep(50);
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Beef)], SPAWN_y[int(Beef)]);
        THUAI3::pick(false, Block, Beef); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0] + 1); Sleep(50);
        THUAI3::put(1, PI, true); Sleep(50);
        Objlist_pot = mapp.get_mapcell(COOK_x[0], COOK_y[0]);
        for (int i = 0; i < 100; ++i) tmpp[i] = DishType(0); tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
        {
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::move(Down, 0); Sleep(50);
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(1, PI, true);
            Sleep(50);
        }
        move(Left, 0); Sleep(50);
        THUAI3::pick(false, Dish, Noodle); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        THUAI3::pick(false, Dish, Beef); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        move(Down, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(BeefNoodle));
        //拿牛肉面+交牛肉面
        THUAI3::pick(false, Block, BeefNoodle); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        move(Left, 0); Sleep(50);
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
            THUAI3::put(1, PI / 2, true); Sleep(50);
            ++taskWait[tasknow];
        }
    }
    if (tasknow == Barbecue)
    {
        //拿猪肉+放猪肉
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Pork)], SPAWN_y[int(Pork)]);
        THUAI3::pick(false, Block, Pork); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Pork); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1] - 1);
        THUAI3::put(1, PI, true); Sleep(1000);
        //拿生菜+做烤肉
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Lettuce)], SPAWN_y[int(Lettuce)]);
        THUAI3::pick(false, Block, Lettuce); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Lettuce); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1] - 1);
        THUAI3::put(1, PI, true); Sleep(50);
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[1], COOK_y[1]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
        {
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::move(Up, 0); Sleep(50);
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(1, PI * 1.5, true);
            Sleep(50);
        }
        move(Left, 0); Sleep(50);
        THUAI3::pick(false, Dish, Pork); Sleep(50);
        THUAI3::put(1, PI / 2, true); Sleep(50);
        THUAI3::pick(false, Dish, Lettuce); Sleep(50);
        THUAI3::put(1, PI / 2, true); Sleep(50);
        move(Up, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Barbecue));
        //拿烤肉+交烤肉
        THUAI3::pick(false, Block, Barbecue); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        move(Left, 0); Sleep(50);
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
            THUAI3::put(1, PI / 2, true); Sleep(50);
            ++taskWait[tasknow];
        }
    }
    if (tasknow == FrenchFries)
    {
        //拿土豆+放土豆
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Potato)], SPAWN_y[int(Potato)]);
        THUAI3::pick(false, Block, Potato); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Potato); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 2);
        THUAI3::put(1, PI / 2, true); Sleep(50);
        //拿番茄+做番茄酱+放番茄酱
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Tomato)], SPAWN_y[int(Tomato)]);
        THUAI3::pick(false, Block, Tomato); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Tomato); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]); Sleep(50);
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
            THUAI3::put(1, PI * 1.5, true);
            Sleep(50);
        }
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Tomato); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(Ketchup));
        THUAI3::pick(false, Block, Ketchup); Sleep(50);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
        //拿番茄酱+拿土豆+做薯条
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Ketchup); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        move(Down, 0); Sleep(50);
        THUAI3::pick(false, Dish, Potato); Sleep(50);
        THUAI3::put(1, 0, true); Sleep(50);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(FrenchFries));
        //交薯条
        THUAI3::pick(false, Block, FrenchFries); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
        move(Right, 0); Sleep(50);
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
            THUAI3::put(1, PI / 2, true); Sleep(50);
            ++taskWait[tasknow];
        }
    }
    if (tasknow == Hamburger) {
        //拿鸡蛋+放鸡蛋
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Egg)], SPAWN_y[int(Egg)]);
        THUAI3::pick(false, Block, Egg); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Egg); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 2);
        THUAI3::put(1, PI / 2, true); Sleep(50);
        //拿小麦+做面粉
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
        THUAI3::pick(false, Block, Wheat); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Wheat); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]); move(Right, 0); Sleep(50);
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
            THUAI3::put(1, PI * 1.5, true);
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
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Lettuce); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0] + 1); Sleep(50);
        THUAI3::put(1, PI, true); Sleep(50);
        //拿牛肉+做汉堡
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Beef)], SPAWN_y[int(Beef)]);
        THUAI3::pick(false, Block, Beef); Sleep(50);
        while (PlayerInfo.dish == 0) { Sleep(1000); THUAI3::pick(false, Block, Beef); }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0] + 1); Sleep(50);
        THUAI3::put(1, PI, true); Sleep(50);
        Objlist_pot = mapp.get_mapcell(COOK_x[0], COOK_y[0]);
        for (int i = 0; i < 100; ++i) tmpp[i] = DishType(0); tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
        {
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::move(Down, 0); Sleep(50);
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(1, PI, true);
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
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
            THUAI3::put(1, PI / 2, true); Sleep(50);
            ++taskWait[tasknow];
        }
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
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
            THUAI3::put(1, PI / 2, true); Sleep(50);
            ++taskWait[tasknow];
        }
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
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1] + 2);
        THUAI3::put(1, PI * 1.5, true); Sleep(50);
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
            THUAI3::move(Down, 250); Sleep(300);
            THUAI3::move(Right, 0); Sleep(50);
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::move(Up, 250); Sleep(300);
            THUAI3::put(1, 0, true); Sleep(50);
        }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] - 1, COOK_y[1] + 1);
        move(Right, 0); Sleep(50);
        THUAI3::pick(false, Dish, Rice); Sleep(50);
        move(Down, 250); Sleep(300);
        THUAI3::put(1, 0, true); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] - 1, COOK_y[1] + 1);
        move(Right, 0); Sleep(50);
        THUAI3::pick(false, Dish, Pork); Sleep(50);
        move(Down, 250); Sleep(300);
        THUAI3::put(1, 0, true); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] - 1, COOK_y[1] + 1);
        move(Right, 0); Sleep(50);
        THUAI3::pick(false, Dish, Potato); Sleep(50);
        move(Down, 250); Sleep(300);
        THUAI3::put(1, 0, true); Sleep(50);
        move(Right, 0); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        Sleep(dish_cooktime(OverRice));
        //拿盖浇饭+交盖浇饭
        THUAI3::pick(false, Block, OverRice); Sleep(50);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        move(Left, 0); Sleep(50);
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
            THUAI3::put(1, PI / 2, true); Sleep(50);
            ++taskWait[tasknow];
        }
    }
}
void play()
{
    if (!checkbegin) Begin();
    else {
        for (list<DishType>::iterator iter = task_list.begin(); iter != task_list.end(); ++iter)
            if (taskWait[*iter]) {
                Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
                move(Up, 0); Sleep(50);
                THUAI3::pick(false, Dish, *iter); Sleep(50);
                Move_player(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
                move(Right, 0); Sleep(50);
                if (checktask(*iter)) { THUAI3::use(0, 0, 0); Sleep(50); --taskWait[*iter]; }
                else {
                    Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 24.5, 30.5);
                    THUAI3::put(1, PI / 2, true); Sleep(50);
                }
                break;
            }
        task_finish(task_list.back());
    }
}
