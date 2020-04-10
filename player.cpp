#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::Cook;//指定人物天赋。选手代码必须定义此变量，否则报错

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

void Move_player(double sx, double sy, double ex, double ey) {

}

void play()
{
	
	move(Up, 50);
	
	Print_player();
	PauseCommunication();
	cout << "test" << endl;
	char pauseee;
	cin >> pauseee;
}