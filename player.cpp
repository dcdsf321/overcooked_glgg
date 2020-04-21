
#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
#include<windows.h>
#include<queue>
#include<map>
#define PX PlayerInfo.position.x
#define PY PlayerInfo.position.y
using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::Cook;//指定人物天赋。选手代码必须定义此变量，否则报错

/*
{int(Protobuf::Flour), {0,10000,0} },
{int(Protobuf::Noodle), {0,10000,0} },
{int(Protobuf::Bread), {0,10000,0} },
{int(Protobuf::CookedRice), {0,10000,0} },
{int(Protobuf::Ketchup), {0,10000,0} },
{int(Protobuf::Cream), {0,10000,0} },
{int(Protobuf::TomatoFriedEgg), {50,10000,60000} },
{int(Protobuf::TomatoFriedEggNoodle), {100,15000,90000} },
{int(Protobuf::BeefNoodle), {80,20000,90000} },
{int(Protobuf::OverRice), {90,20000,90000} },
{int(Protobuf::YellowPheasant), {100,20000,90000} },
{int(Protobuf::Barbecue), {55,20000,90000} },
{int(Protobuf::FrenchFries), {60,15000,90000} },
{int(Protobuf::PlumJuice), {50,10000,90000} },
{int(Protobuf::Hamburger), {110,20000,100000} },
{int(Protobuf::StrawberryIcecream), {60,10000,90000} },
{int(Protobuf::PopcornChicken), {60,15000,90000} },
{int(Protobuf::AgaricFriedEgg), {50,15000,90000} },
{int(Protobuf::Cake), {160,30000,120000} },
{int(Protobuf::SugarCoatedHaws), {20,10000,60000} },
{int(Protobuf::FruitSalad), {100,20000,120000} },
{int(Protobuf::SpicedPot), {0,60000,300000} },
{int(Protobuf::DarkDish), {-10,60000,30000} },
{int(Protobuf::OverCookedDish), {-10,60000,300000} },
 第二个参数为烹饪时间
 */
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

int checkdish[50][50];
DishType TASK;
bool checkbegin, tryfault;
int vis_pre_x[51][51], vis_pre_y[51][51];
string INFO;

void Begin() {
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
    cout << "Task: " << TASK << endl;
    cout << "INFO: " << INFO << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;
}

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

/*void info_clear() {
    for (int i = 0; i <= 15; i++) {
        PlayerInfo.recieveText[i] = '1';
        infonow[i] = '1';
    }
    for (int i = 0; i < dishsize(TASK); i++) {
        PlayerInfo.recieveText[i] = '0';
        infonow[i] = '0';
    }
}*/

bool checktask(DishType dd) {
    for (list<DishType>::iterator iter = task_list.begin(); iter != task_list.end(); ++iter)
        if (*iter == dd) return 1;
    return 0;
}

void info_add(DishType cui) { //比如玩家1取到需要食材cui，把信息添加到传递信息再传递
    string str1 = PlayerInfo.recieveText;
    str1[checkdish[TASK][cui] - 1] = '1';
    PlayerInfo.recieveText = str1;
    speakToFriend(str1);
}

bool info_decide(DishType dd,string infonow) {
    for (int i = 0; i <= dishsize(dd); i++)
        if (infonow[i] == '0')
            return 0;
    return 1;
}

void player_wait(int Tim) {
    for (int i = 1; i <= Tim / 100; ++i) {
        move(Left, 50);
        move(Right, 50);
    }move(Right, 0);
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
        //cout << "(" << vis_pre_x[nowx][nowy] << "," << vis_pre_y[nowx][nowy] << ")-->(" << nowx << "," << nowy << ")   " << next[vis_pre_x[nowx][nowy]][vis_pre_y[nowx][nowy]]<< endl;
        int tnowx = vis_pre_x[nowx][nowy], tnowy = vis_pre_y[nowx][nowy];
        nowx = tnowx; nowy = tnowy;
    }//Sleep(10000);
    //真正的行走操作
    while (!(nowx == int(ex) && nowy == int(ey))) {
        Print_player();                 //查看目前状态，用于调试
        //cout <<next[nowx][nowy] << endl;
        THUAI3::move(next[nowx][nowy], 200);  //每次只移动一个单位 //速度为5的情况下
        Sleep(400);                      //挂起以等待操作完成
        nowx = PlayerInfo.position.x; //迭代
        nowy = PlayerInfo.position.y;
    }
}




void task_finish(DishType task,DishType task_root) {
    TASK = task; string infonow;
    for (int i = 0; i <= 15; i++) {
        PlayerInfo.recieveText[i] = '1';
        infonow[i] = '1';
    }
    for (int i = 0; i < dishsize(TASK); i++) {
        PlayerInfo.recieveText[i] = '0';
        infonow[i] = '0';
    }
    for (int i = 0; i < dishsize(task); i++)
    {
        if (20 <= cookbook[task][i] && cookbook[task][i] <= 25)
        {
            task_finish(DishType(cookbook[task][i]), task_root);
        }
    }TASK = task; INFO = infonow;
    Print_player(); cout << "Now Task is" << task << endl;
    MapInfo mapp;
    double cook_x = 8.5, cook_y = 24.5;
    double spawn_x = 7.5, spawn_y = 41.5;
    while (!info_decide(task,infonow) && checktask(task_root)) {
        if (((PlayerInfo.position.x - (cook_x - 1)) < 1e-4) && ((PlayerInfo.position.y - cook_y) < 1e-4)) {
            cout << "!!!!!!!!!!!!!!!!" << endl;
            THUAI3::move(Up, 0);
            for (int i = 0; i < dishsize(task); ++i)
                if (infonow[checkdish[task][cookbook[task][i]] - 1] == '0') {
                    THUAI3::pick(false, Dish, cookbook[task][i]); Sleep(100);
                    if (PlayerInfo.dish != 0) {
                        THUAI3::move(Right, 50); Sleep(100);
                        infonow[checkdish[task][PlayerInfo.dish] - 1] = '1';
                        THUAI3::put(1, 0, true);
                    }
                }
        }
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, spawn_x - 1, spawn_y);
        THUAI3::move(Right, 0); Sleep(1000);
        pick(false, Block, mapp.get_mapcell(spawn_x, spawn_y).back().dish);
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, cook_x - 1, cook_y);
        THUAI3::move(Right, 50); Sleep(1000);
        if (checkdish[task][PlayerInfo.dish] && infonow[checkdish[task][PlayerInfo.dish] - 1] == '0') {
            infonow[checkdish[task][PlayerInfo.dish] - 1] = '1';
            THUAI3::put(1, 0, true);
            cout << infonow << endl;
        }
        else put(1, 1.57, true);
        Sleep(1000); Print_player(); cout << task << endl;
    }
    if (!checktask(task_root)) return;//{ tryfault = 1; return; }
    THUAI3::use(0, 0, 0);
    Sleep(dish_cooktime(task) * 1.2);
    if (task >= 26)
    {
        THUAI3::pick(false, Block, task); Sleep(1000); Print_player();
        Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 23.5, 24.5); move(Right, 50);
        if (checktask(task_root)) THUAI3::use(0, 0, 0);
        else put(1, 1.57, true);
        return;
    }
    else
    {
        THUAI3::pick(false, Block, task); Sleep(1000); Print_player();
        put(1, 1.57, true);
        return;
    }
}

void play()
{
    if (!checkbegin) Begin();
    else {
        Print_player();
        if (PlayerInfo.id & 1) {
            Move_player(PlayerInfo.position.x, PlayerInfo.position.y, 6.5, 41.5);
            THUAI3::move(Right, 0);
            task_finish(task_list.back(),task_list.back());//task_finish(SugarCoatedHaws);//Move_player(2.5, 1.5, 40, 40); //从（2.5，1.5）走到（40，40） 目前只能完成与坐标轴平行的操作即只能上下左右 //把小数截尾整数化处理坐标
            //PauseCommunication();
            //Sleep(10000);
        }
    }
}
