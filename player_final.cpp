#include "API.h"
#include "Constant.h"
#include "player.h"
#include <iostream>
#include "OS_related.h"
#include<cmath>
#define PREX 25.5
#define PREY 31.5
#define PI 3.1415926
using namespace THUAI3;
Protobuf::Talent initTalent = Protobuf::Talent::Runner;//指定人物天赋。选手代码必须定义此变量，否则报错

int task_pro[31][31];
double COOK_x[4] = { 25.5,41.5,33.5,8.5 };
double COOK_y[4] = { 38.5,28.5,18.5,24.5 };
double SPAWN_x[30];
double SPAWN_y[30];
double commit_x[4] = { 24.5,24.5,25.5,25.5 };
double commit_y[4] = { 24.5,25.5,24.5,25.5 };

MapInfo mapp;
int checkdish_cook[51];
bool checkbegin,checkmember;
int taskWait[51],DD;

unsigned long long timetmp1,timetmp2;
unsigned long long average_task_time[8] = { 23000, 43000, 87000, 60000, 57000, 37000, 55000, 87000 };
//1CookedRice, 2TomatoFriedEgg, 3TomatoFriedEggNoodle, 4BeefNoodle, 5OverRice, 6Barbecue, 7FrenchFries, 8Hamburger
double xingjiabi[8] = { 20,40,85,60,60,40,45,80 };

void Begin() {
    if (abs(PlayerInfo.position.x - 1.5) < 1e-4 || abs(PlayerInfo.position.x - 48.5) < 1e-4) checkmember = 1;
    else checkmember = 2;
    checkbegin = 1;
    SPAWN_x[int(Wheat)] = 4.5; SPAWN_y[int(Wheat)] = 24.5;
    SPAWN_x[int(Rice)] = 5.5; SPAWN_y[int(Rice)] = 5.5;
    SPAWN_x[int(Tomato)] = 7.5; SPAWN_y[int(Tomato)] = 41.5;
    SPAWN_x[int(Egg)] = 25.5; SPAWN_y[int(Egg)] = 5.5;
    SPAWN_x[int(Beef)] = 31.5; SPAWN_y[int(Beef)] = 41.5;
    SPAWN_x[int(Pork)] = 42.5; SPAWN_y[int(Pork)] = 40.5;
    SPAWN_x[int(Potato)] = 43.5; SPAWN_y[int(Potato)] = 6.5;
    SPAWN_x[int(Lettuce)] = 43.5; SPAWN_y[int(Lettuce)] = 25.5;
}
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
    cout << "-----------------------------------------------------------------------------------" << endl;
}
int dish_cooktime(DishType dd) {
    if (dd == Flour || dd == Noodle || dd == Bread || dd == CookedRice || dd == Ketchup) return 10000;
    if (dd == FrenchFries) return 15000;
    if (dd == TomatoFriedEggNoodle || dd == BeefNoodle || dd == OverRice || dd == Barbecue || dd == Hamburger || dd == TomatoFriedEgg) return 20000;
    if (dd == SpicedPot) return 60000;
}
bool checktask(DishType dd) {
    for (list<DishType>::iterator iter = task_list.begin(); iter != task_list.end(); ++iter)
        if (*iter == dd) return 1;
    return 0;
}
Direction calcdirection(double x1, double y1, double x2, double y2) {
    if (x1 <= x2 - 0.5 && abs(y1 - y2) < 0.5) return Right;
    if (x1 <= x2 - 0.5 && y1 <= y2 - 0.5) return RightUp;
    if (abs(x1 - x2) < 0.5 && y1 <= y2 - 0.5) return Up;
    if (x1 - 0.5 >= x2 && y1 <= y2 - 0.5) return LeftUp;
    if (x1 - 0.5 >= x2 && abs(y1 - y2) < 0.5) return Left;
    if (x1 - 0.5 >= x2 && y1 - 0.5 >= y2) return LeftDown;
    if (abs(x1 - x2) < 0.5 && y1 - 0.5 >= y2) return Down;
    if (x1 <= x2 - 0.5 && y1 - 0.5 >= y2) return RightDown;
}
double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
double ang(double x1, double y1, double x2, double y2) {
    if (x1 <= x2) return atan((y2 - y1) / (x2 - x1));
    else return atan((y2 - y1) / (x2 - x1)) + PI;
}

void Move_player(double sx, double sy, double ex, double ey) {   //sx=start_xposition, ex=end_position
    list<Obj> Objlist_p = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y);
    for (list<Obj>::iterator iter = Objlist_p.begin(); iter != Objlist_p.end(); ++iter)
    if (iter->objType == Tool) {
        if (iter->tool == WaveGlueBottle || iter->tool == LandMine || iter->tool == TrapTool || iter->tool == FlashBomb) {
            THUAI3::pick(true, Tool, iter->tool); Sleep(50);
            THUAI3::use(1, 0, 0); Sleep(50);
        }
        else if (iter->tool == BreastPlate) {
            THUAI3::pick(true, Tool, iter->tool); Sleep(50);
        }
    }
    if (getGameTime() - timetmp1 > 13 * 1000) return;
    if (abs(ex - sx) <=1 && abs(ey - sy) <= 1&&!(abs(ex-sx)==1&&abs(ey-sy)==1)) return;
    if (abs(sx - 30.5) < 1e-5 && (9.5 <= sy && sy <= 15.5)) {
        if (ey < sy) {
            THUAI3::move(Down, 1000); Sleep(1000);
            THUAI3::move(Left, 250); Sleep(250);
            if (abs(sx - 30.5) < 1e-5 && (9.5 <= sy && sy <= 15.5)) {
                THUAI3::move(Up, 1000); Sleep(1000);
                THUAI3::move(Left, 250); Sleep(250);
            }
        }
        else {
            THUAI3::move(Up, 1000); Sleep(1000);
            THUAI3::move(Left, 250); Sleep(250);
            if (abs(sx - 30.5) < 1e-5 && (9.5 <= sy && sy <= 15.5)) {
                THUAI3::move(Down, 1000); Sleep(1000);
                THUAI3::move(Left, 250); Sleep(250);
            }
        }
    }else if (abs(sx - 28.5) < 1e-5 && (9.5 <= sy && sy <= 15.5)) {
        if (ey < sy) {
            THUAI3::move(Down, 1000); Sleep(1000);
            THUAI3::move(Right, 250); Sleep(250);
            if (abs(sx - 30.5) < 1e-5 && (9.5 <= sy && sy <= 15.5)) {
                THUAI3::move(Up, 1000); Sleep(1000);
                THUAI3::move(Right, 250); Sleep(250);
            }
        }
        else {
            THUAI3::move(Up, 1000); Sleep(1000);
            THUAI3::move(Right, 250); Sleep(250);
            if (abs(sx - 30.5) < 1e-5 && (9.5 <= sy && sy <= 15.5)) {
                THUAI3::move(Down, 1000); Sleep(1000);
                THUAI3::move(Right, 250); Sleep(250);
            }
        }
    }
    else if (abs(sy - 13.5) < 1e-5 && (sx >= 40.5 && sx <= 45.5)) {
        THUAI3::move(Left, 1000); Sleep(1000);
        THUAI3::move(Down, 250); Sleep(250);
        if (abs(sy - 13.5) < 1e-5 && (sx >= 40.5 && sx <= 45.5)) {
            THUAI3::move(Right, 1000); Sleep(1000);
            THUAI3::move(Down, 250); Sleep(250);
        }
    }
    else if (abs(sy - 11.5) < 1e-5 && (sx >= 40.5 && sx <= 45.5)) {
        THUAI3::move(Left, 1000); Sleep(1000);
        THUAI3::move(Up, 250); Sleep(250);
        if (abs(sy - 11.5) < 1e-5 && (sx >= 40.5 && sx <= 45.5)) {
            THUAI3::move(Right, 1000); Sleep(1000);
            THUAI3::move(Up, 250); Sleep(250);
        }
    }
    else {
        if (abs(ey - sy) < 0.5) {
            if (ex - sx > 0) {
                THUAI3::move(Right, 50); Sleep(50);
                if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                    Direction dire_now = Right;
                    for (int i = 1; i <= 7; ++i) {
                        dire_now = Direction((dire_now + DD) % 8);
                        if (dire_now % 2) {
                            THUAI3::move(dire_now, 200); Sleep(200);
                        }
                        else {
                            THUAI3::move(dire_now, 150); Sleep(150);
                        }
                        if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                    }
                }
            }
            else {
                THUAI3::move(Left, 50); Sleep(50);
                if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                    Direction dire_now = Left;
                    for (int i = 1; i <= 7; ++i) {
                        dire_now = Direction((dire_now + DD) % 8);
                        if (dire_now % 2) {
                            THUAI3::move(dire_now, 200); Sleep(200);
                        }
                        else {
                            THUAI3::move(dire_now, 150); Sleep(150);
                        }
                        if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                    }
                }
            }
        }
        else if (abs(ex - sx) < 0.5) {
            if (ey - sy > 0) {
                THUAI3::move(Up, 50); Sleep(50);
                if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                    Direction dire_now = Up;
                    for (int i = 1; i <= 7; ++i) {
                        dire_now = Direction((dire_now + DD) % 8);
                        if (dire_now % 2) {
                            THUAI3::move(dire_now, 200); Sleep(200);
                        }
                        else {
                            THUAI3::move(dire_now, 150); Sleep(150);
                        }
                        if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                    }
                }
            }
            else {
                THUAI3::move(Down, 50); Sleep(50);
                if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                    Direction dire_now = Down;
                    for (int i = 1; i <= 7; ++i) {
                        dire_now = Direction((dire_now + DD) % 8);
                        if (dire_now % 2) {
                            THUAI3::move(dire_now, 200); Sleep(200);
                        }
                        else {
                            THUAI3::move(dire_now, 150); Sleep(150);
                        }
                        if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                    }
                }
            }
        }
        else {
            THUAI3::move(calcdirection(sx, sy, ex, ey), 50); Sleep(50);
            Direction dire_now = calcdirection(sx, sy, ex, ey);
            if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                for (int i = 1; i <= 7; ++i) {
                    dire_now = Direction((dire_now + DD) % 8);
                    if (dire_now % 2) {
                        THUAI3::move(dire_now, 200); Sleep(200);
                    }
                    else {
                        THUAI3::move(dire_now, 150); Sleep(150);
                    }
                    if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                }
            }
        }
    }
    Move_player(PlayerInfo.position.x, PlayerInfo.position.y, ex, ey);
}
void MovePlayer(double sx, double sy, double ex, double ey) {
    //if (sx >= ex && sy >= ey) 
        DD = 1;
    //else DD = 7;
    timetmp1 = getGameTime();
    Move_player(sx, sy, ex, ey);
    timetmp2 = getGameTime();
}
void Move_player_near(double sx, double sy, double ex, double ey) {
    MovePlayer(sx, sy, ex, ey);
    THUAI3::move(calcdirection(PlayerInfo.position.x, PlayerInfo.position.y, ex, ey), 0); Sleep(50);
}

void Move_player_1(double sx, double sy, double ex, double ey) {   //sx=start_xposition, ex=end_position
    list<Obj> Objlist_p = mapp.get_mapcell(PlayerInfo.position.x, PlayerInfo.position.y);
    for (list<Obj>::iterator iter = Objlist_p.begin(); iter != Objlist_p.end(); ++iter)
        if (iter->objType == Tool) {
            if (iter->tool == WaveGlueBottle || iter->tool == LandMine || iter->tool == TrapTool || iter->tool == FlashBomb) {
                while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
                THUAI3::pick(true, Tool, iter->tool); Sleep(50);
                while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
                THUAI3::use(1, 0, 0); Sleep(50);
            }
            else if (iter->tool == BreastPlate) {
                while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
                THUAI3::pick(true, Tool, iter->tool); Sleep(50);
            }
        }
    if (abs(ex - sx) <= 1 && abs(ey - sy) <= 1 && !(abs(ex - sx) == 1 && abs(ey - sy) == 1)) return;
    if (abs(sy - 13.5) < 1e-5 && (sx >= 40.5 && sx <= 45.5)) {
        while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
        THUAI3::move(Right, 1000); Sleep(1000);
        while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
        THUAI3::move(Down, 250); Sleep(250);
        if (abs(sy - 13.5) < 1e-5 && (sx >= 40.5 && sx <= 45.5)) {
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
            THUAI3::move(Left, 1000); Sleep(1000);
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
            THUAI3::move(Down, 250); Sleep(250);
        }
    }
    else if (abs(sy - 11.5) < 1e-5 && (sx >= 40.5 && sx <= 45.5)) {
        while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
        THUAI3::move(Left, 1000); Sleep(1000);
        while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
        THUAI3::move(Up, 250); Sleep(250);
        if (abs(sy - 11.5) < 1e-5 && (sx >= 40.5 && sx <= 45.5)) {
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
            THUAI3::move(Right, 1000); Sleep(1000);
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
            THUAI3::move(Up, 250); Sleep(250);
        }
    }else{
    if (abs(ey - sy) < 0.5) {
        if (ex - sx > 0) {
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
            THUAI3::move(Right, 50); Sleep(50);
            if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                Direction dire_now = Right;
                for (int i = 1; i <= 7; ++i) {
                    dire_now = Direction((dire_now + DD) % 8);
                    if (dire_now % 2) {
                        while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
                        THUAI3::move(dire_now, 200); Sleep(200);
                    }
                    else {
                        while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
                        THUAI3::move(dire_now, 150); Sleep(150);
                    }
                    if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                }
            }
        }
        else {
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
            THUAI3::move(Left, 50); Sleep(50);
            if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                Direction dire_now = Left;
                for (int i = 1; i <= 7; ++i) {
                    dire_now = Direction((dire_now + DD) % 8);
                    if (dire_now % 2) {
                        while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
                        THUAI3::move(dire_now, 200); Sleep(200);
                    }
                    else {
                        while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
                        THUAI3::move(dire_now, 150); Sleep(150);
                    }
                    if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                }
            }
        }
    }
    else if (abs(ex - sx) < 0.5) {
        if (ey - sy > 0) {
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
            THUAI3::move(Up, 50); Sleep(50);
            if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                Direction dire_now = Up;
                for (int i = 1; i <= 7; ++i) {
                    dire_now = Direction((dire_now + DD) % 8);
                    if (dire_now % 2) {
                        while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
                        THUAI3::move(dire_now, 200); Sleep(200);
                    }
                    else {
                        while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
                        THUAI3::move(dire_now, 150); Sleep(150);
                    }
                    if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                }
            }
        }
        else {
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
            THUAI3::move(Down, 50); Sleep(50);
            if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
                Direction dire_now = Down;
                for (int i = 1; i <= 7; ++i) {
                    dire_now = Direction((dire_now + DD) % 8);
                    if (dire_now % 2) {
                        while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
                        THUAI3::move(dire_now, 200); Sleep(200);
                    }
                    else {
                        while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
                        THUAI3::move(dire_now, 150); Sleep(150);
                    }
                    if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
                }
            }
        }
    }
    else {
        while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
        THUAI3::move(calcdirection(sx, sy, ex, ey), 50); Sleep(50);
        Direction dire_now = calcdirection(sx, sy, ex, ey);
        if (abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5) {
            for (int i = 1; i <= 7; ++i) {
                dire_now = Direction((dire_now + DD) % 8);
                if (dire_now % 2) {
                    while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
                    THUAI3::move(dire_now, 200); Sleep(200);
                }
                else {
                    while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
                    THUAI3::move(dire_now, 150); Sleep(150);
                }
                if (!(abs(sx - PlayerInfo.position.x) < 1e-5 && abs(sy - PlayerInfo.position.y) < 1e-5)) break;
            }
        }
    }
    }
    while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
    Move_player_1(PlayerInfo.position.x, PlayerInfo.position.y, ex, ey);
}
void MovePlayer_1(double sx, double sy, double ex, double ey) {
    //if (sx >= ex && sy >= ey) 
    DD = 1;
    //else DD = 7;
    timetmp1 = getGameTime();
    while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
    Move_player_1(sx, sy, ex, ey);
    timetmp2 = getGameTime();
}
void Move_player_near_1(double sx, double sy, double ex, double ey) {
    while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
    MovePlayer_1(sx, sy, ex, ey);
    while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
    THUAI3::move(calcdirection(PlayerInfo.position.x, PlayerInfo.position.y, ex, ey), 0); Sleep(50);
}

void task_finish(DishType tasknow) {
    if (tasknow == TomatoFriedEgg) {
        //拿鸡蛋+放鸡蛋
        if (task_pro[int(tasknow)][int(Egg)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Egg)], SPAWN_y[int(Egg)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return;
            THUAI3::pick(false, Block, Egg); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Egg); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
            if (dist(PlayerInfo.position.x,PlayerInfo.position.y,COOK_x[3]-1,COOK_y[3]-1)>=9) return;
            THUAI3::put(dist(PlayerInfo.position.x,PlayerInfo.position.y,COOK_x[3]-1,COOK_y[3]-1),ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) , true); Sleep(50);
            ++task_pro[int(tasknow)][int(Egg)];
        }
        //拿番茄+拿鸡蛋+做番茄炒蛋
        if (task_pro[int(tasknow)][int(Tomato)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Tomato)], SPAWN_y[int(Tomato)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return;
            THUAI3::pick(false, Block, Tomato); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Tomato); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3]-1, COOK_y[3]-1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Tomato)];
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3]-1, COOK_y[3]-1);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        THUAI3::pick(false, Dish, Tomato); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Tomato)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Tomato)];
        THUAI3::pick(false, Dish, Egg); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Egg)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Egg)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("ml"); Sleep(50);
        Sleep(dish_cooktime(TomatoFriedEgg) - 11100);
        speakToFriend("m"); Sleep(11000);
        speakToFriend("p"); Sleep(100);
        //拿番茄炒蛋+交番茄炒蛋
        THUAI3::pick(false, Block, TomatoFriedEgg); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
            ++taskWait[tasknow];
        }
    }
    if (tasknow == TomatoFriedEggNoodle) {
        list<Obj> Objlist_pot;
        DishType tmpp[100]; int tot = 0;
        if (taskWait[TomatoFriedEgg]) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (timetmp2 - timetmp1 > 13 * 1000) return;
            THUAI3::pick(false, Dish, TomatoFriedEgg); Sleep(50);
            --taskWait[TomatoFriedEgg];
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
        }
        else {
            //拿鸡蛋+放鸡蛋
            if (task_pro[int(tasknow)][int(Egg)] == 0) {
                Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Egg)], SPAWN_y[int(Egg)]);
                if (timetmp2 - timetmp1 > 13 * 1000) return;
                THUAI3::pick(false, Block, Egg); Sleep(50);
                while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Egg); }
                Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
                if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return; 
                THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
                ++task_pro[int(tasknow)][int(Egg)];
            }
            //拿番茄+拿鸡蛋+做番茄炒蛋
            if (task_pro[int(tasknow)][int(Tomato)] == 0) {
                Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Tomato)], SPAWN_y[int(Tomato)]);
                if (timetmp2 - timetmp1 > 13 * 1000) return;
                THUAI3::pick(false, Block, Tomato); Sleep(50);
                while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Tomato); }
                Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]-1);
                if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
                THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
                ++task_pro[int(tasknow)][int(Tomato)];
            }Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
            if (timetmp2 - timetmp1 > 13 * 1000) return;
            Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
            for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
                if (iter->dish == CookingDish) return;
                if (iter->dish != 0) tmpp[++tot] = iter->dish;
            }
            for (int i = 1; i <= tot; ++i) {
                THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
                THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true);
                Sleep(50);
            }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
            if (timetmp2 - timetmp1 > 13 * 1000) return;
            THUAI3::pick(false, Dish, Tomato); Sleep(50);
            if (PlayerInfo.dish == 0) {
                task_pro[int(tasknow)][int(Tomato)] = 0;
                return;
            }
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3] ), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3] ), true); Sleep(50);
            --task_pro[int(tasknow)][int(Tomato)];
            THUAI3::pick(false, Dish, Egg); Sleep(50);
            if (PlayerInfo.dish == 0) {
                task_pro[int(tasknow)][int(Egg)] = 0;
                return;
            }
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
            --task_pro[int(tasknow)][int(Egg)];
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
            if (timetmp2 - timetmp1 > 13 * 1000) return;
            THUAI3::use(0, 0, 0); Sleep(50);
            speakToFriend("ml"); Sleep(50);
            Sleep(dish_cooktime(TomatoFriedEgg) - 11100);
            speakToFriend("m"); Sleep(11000);
            speakToFriend("p"); Sleep(100);
            //拿番茄炒蛋+放番茄炒蛋
            THUAI3::pick(false, Block, TomatoFriedEgg); Sleep(50);
            if (PlayerInfo.dish >= 28) {
                THUAI3::put(2, PI, true);
                return;
            }
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
        ++task_pro[int(tasknow)][int(TomatoFriedEgg)];
        //拿小麦+做面粉
        if (task_pro[int(tasknow)][int(Wheat)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return;
            THUAI3::pick(false, Block, Wheat); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Wheat); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3]-1, COOK_y[3]-1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Wheat)];
        }Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        for (int i = 0; i < 100; ++i) tmpp[i] = DishType(0); tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        THUAI3::pick(false, Dish, Wheat); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Wheat)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3] ), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Wheat)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(Flour) - 100);
        speakToFriend("p"); Sleep(100);
        //拿面粉+做面条
        THUAI3::pick(false, Block, Flour); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3] ), true); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(Noodle) - 100);
        speakToFriend("p"); Sleep(100);
        //拿面条+放面条+拿番茄炒蛋+拿面+做番茄炒蛋面
        THUAI3::pick(false, Block, Noodle); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        THUAI3::pick(false, Dish, TomatoFriedEgg); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(TomatoFriedEgg)] = 0;
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] , COOK_y[3] ), true); Sleep(50);
        --task_pro[int(tasknow)][int(TomatoFriedEgg)];
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("ml"); Sleep(50);
        Sleep(dish_cooktime(TomatoFriedEggNoodle) - 11100);
        speakToFriend("m"); Sleep(11000);
        speakToFriend("p"); Sleep(100);
        //拿番茄炒蛋面+交番茄炒蛋面
        THUAI3::pick(false, Block, TomatoFriedEggNoodle); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
        }
    }
    if (tasknow == BeefNoodle) {
        //拿小麦+做面粉
        if (task_pro[int(tasknow)][int(Wheat)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return;
            THUAI3::pick(false, Block, Wheat); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Wheat); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]-1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Wheat)];
        }Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        THUAI3::pick(false, Dish, Wheat); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Wheat)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Wheat)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(Flour) - 100);
        speakToFriend("p"); Sleep(100);
        //拿面粉+做面条
        THUAI3::pick(false, Block, Flour); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3] ), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(Noodle) - 100);
        speakToFriend("p"); Sleep(100);
        //拿面条+放面条+拿牛肉+做牛肉面
        THUAI3::pick(false, Block, Noodle); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0]-1, COOK_y[0]+1); 
        if (!(abs(PlayerInfo.position.x - COOK_x[0]+1) <= 1 && abs(PlayerInfo.position.y - COOK_y[0]-1) <= 1)) return;
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), true); Sleep(50);
        ++task_pro[int(tasknow)][int(Noodle)];
        if (task_pro[int(tasknow)][int(Beef)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Beef)], SPAWN_y[int(Beef)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Beef); Sleep(50);
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0]-1, COOK_y[0]+1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Beef)];
        }Move_player(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0] );
        Objlist_pot = mapp.get_mapcell(COOK_x[0], COOK_y[0]);
        for (int i = 0; i < 100; ++i) tmpp[i] = DishType(0); tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::pick(false, Dish, Noodle); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Noodle)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] , COOK_y[0]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Noodle)];
        THUAI3::pick(false, Dish, Beef); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Noodle)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Beef)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y,COOK_x[0], COOK_y[0]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("ml"); Sleep(50);
        Sleep(dish_cooktime(BeefNoodle) - 11100);
        speakToFriend("m"); Sleep(11000);
        speakToFriend("p"); Sleep(100);
        //拿牛肉面+交牛肉面
        THUAI3::pick(false, Block, BeefNoodle); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
        }
    }
    if (tasknow == Barbecue)
    {
        //拿猪肉+放猪肉
        if (task_pro[int(tasknow)][int(Pork)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Pork)], SPAWN_y[int(Pork)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Pork); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Pork); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] +1, COOK_y[1]) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] + 1, COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] + 1, COOK_y[1]), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Pork)];
        }
        //拿生菜+做烤肉
        if (task_pro[int(tasknow)][int(Lettuce)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Lettuce)], SPAWN_y[int(Lettuce)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Lettuce); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Lettuce); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] + 1, COOK_y[1]) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] + 1, COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] + 1, COOK_y[1]), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Lettuce)];
        }Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[1], COOK_y[1]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] + 1, COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] + 1, COOK_y[1]), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1] + 1, COOK_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::pick(false, Dish, Pork); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Pork)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Pork)];
        THUAI3::pick(false, Dish, Lettuce); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Lettuce)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Lettuce)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("ml"); Sleep(50);
        Sleep(dish_cooktime(Barbecue) - 11100);
        speakToFriend("m"); Sleep(11000);
        speakToFriend("p"); Sleep(100);
        //拿烤肉+交烤肉
        THUAI3::pick(false, Block, Barbecue); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
        }
    }
    if (tasknow == FrenchFries)
    {
        //拿土豆+放土豆
        if (task_pro[int(tasknow)][int(Potato)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Potato)], SPAWN_y[int(Potato)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Potato); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Potato); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Potato)];
        }
        //拿番茄+做番茄酱+放番茄酱
        if (task_pro[int(tasknow)][int(Tomato)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Tomato)], SPAWN_y[int(Tomato)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Tomato); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Tomato); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3]-1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Tomato)];
        }Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::pick(false, Dish, Tomato); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Tomato)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Tomato)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(Ketchup) - 100);
        speakToFriend("p"); Sleep(100);
        THUAI3::pick(false, Block, Ketchup); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
       //拿番茄酱+拿土豆+做薯条
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::pick(false, Dish, Potato); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Potato)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Potato)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(FrenchFries) - 100);
        speakToFriend("p"); Sleep(100);
        //交薯条
        THUAI3::pick(false, Block, FrenchFries); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
        }
    }
    if (tasknow == Hamburger) {
        //拿鸡蛋+放鸡蛋
        if (task_pro[int(tasknow)][int(Egg)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Egg)], SPAWN_y[int(Egg)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Egg); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Egg); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Egg)];
        }
        //拿小麦+做面粉
        if (task_pro[int(tasknow)][int(Wheat)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Wheat)], SPAWN_y[int(Wheat)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Wheat); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Wheat); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Wheat)];
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::pick(false, Dish, Wheat); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Wheat)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Wheat)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(Flour) - 100);
        speakToFriend("p"); Sleep(100);
        //拿面粉+拿鸡蛋+做面包
        THUAI3::pick(false, Block, Flour); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::pick(false, Dish, Egg); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Egg)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Egg)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(Bread) - 100);
        speakToFriend("p"); Sleep(100);
        //拿面包+放面包
        THUAI3::pick(false, Block, Bread); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1);
        if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1 ) >= 9) return;
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1 ), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1 ), true); Sleep(50);
        ++task_pro[int(tasknow)][int(Bread)];
        //拿生菜+放生菜
        if (task_pro[int(tasknow)][int(Lettuce)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Lettuce)], SPAWN_y[int(Lettuce)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Lettuce); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Lettuce); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1 );
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1 ) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1 ), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Lettuce)];
        }
        //拿牛肉+做汉堡
        if (task_pro[int(tasknow)][int(Beef)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Beef)], SPAWN_y[int(Beef)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Beef); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Beef); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Beef)];
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        Objlist_pot = mapp.get_mapcell(COOK_x[0], COOK_y[0]);
        for (int i = 0; i < 100; ++i) tmpp[i] = DishType(0); tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1), true); Sleep(50);
            Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0] - 1, COOK_y[0]+1 );
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::pick(false, Dish, Bread); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Bread)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Bread)];
        THUAI3::pick(false, Dish, Beef); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Beef)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Beef)];
        THUAI3::pick(false, Dish, Lettuce); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Lettuce)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Lettuce)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[0], COOK_y[0]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("ml"); Sleep(50);
        Sleep(dish_cooktime(Hamburger) - 11100);
        speakToFriend("m"); Sleep(11000);
        speakToFriend("p"); Sleep(100);
        //拿汉堡+交汉堡
        THUAI3::pick(false, Block, Hamburger); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
        }
    }
    if (tasknow == CookedRice) {
        //拿米饭+放米饭
        if (task_pro[int(tasknow)][int(Rice)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Rice)], SPAWN_y[int(Rice)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Rice); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Rice); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Rice)];
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        //做米饭
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[3], COOK_y[3]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3] - 1, COOK_y[3] - 1);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        THUAI3::pick(false, Dish, Rice); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Rice)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Rice)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[3], COOK_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("m"); Sleep(50);
        Sleep(dish_cooktime(CookedRice) - 100);
        speakToFriend("p"); Sleep(100);
        //拿米饭+交米饭
        THUAI3::pick(false, Block, CookedRice); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
        }
    }
    if (tasknow == OverRice) {
        //拿小麦+放小麦
        if (task_pro[int(tasknow)][int(Wheat)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Rice)], SPAWN_y[int(Rice)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Rice); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Rice); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1] );
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1] ) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1] ), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Rice)];
        }
        //拿猪肉+放猪肉
        if (task_pro[int(tasknow)][int(Pork)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Pork)], SPAWN_y[int(Pork)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Pork); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Pork); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1] ) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Pork)];
        }
        //拿土豆+放土豆
        if (task_pro[int(tasknow)][int(Potato)] == 0) {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Potato)], SPAWN_y[int(Potato)]);
            if (timetmp2 - timetmp1 > 13 * 1000) return; 
            THUAI3::pick(false, Block, Potato); Sleep(50);
            while (PlayerInfo.dish == 0) { Sleep(50); THUAI3::pick(false, Block, Potato); }
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]), true); Sleep(50);
            ++task_pro[int(tasknow)][int(Potato)];
        }Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return;
        //清空锅+做盖浇饭
        list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[1], COOK_y[1]);
        DishType tmpp[100]; int tot = 0;
        for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter) {
            if (iter->dish == CookingDish) return;
            if (iter->dish != 0) tmpp[++tot] = iter->dish;
        }
        for (int i = 1; i <= tot; ++i) {
            THUAI3::pick(false, Dish, tmpp[i]); Sleep(50);
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]), true); Sleep(50);
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1]+1, COOK_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::pick(false, Dish, Rice); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Rice)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Rice)];
        THUAI3::pick(false, Dish, Pork); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Pork)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Pork)];
        THUAI3::pick(false, Dish, Potato); Sleep(50);
        if (PlayerInfo.dish == 0) {
            task_pro[int(tasknow)][int(Potato)] = 0;
            return;
        }
        THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]), true); Sleep(50);
        --task_pro[int(tasknow)][int(Potato)];
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[1], COOK_y[1]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        THUAI3::use(0, 0, 0); Sleep(50);
        speakToFriend("ml"); Sleep(50);
        Sleep(dish_cooktime(OverRice) - 11100);
        speakToFriend("m"); Sleep(11000);
        speakToFriend("p"); Sleep(100);
        //拿盖浇饭+交盖浇饭
        THUAI3::pick(false, Block, OverRice); Sleep(50);
        if (PlayerInfo.dish >= 28) {
            THUAI3::put(2, PI, true);
            return;
        }
        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[3], commit_y[3]);
        if (timetmp2 - timetmp1 > 13 * 1000) return; 
        if (checktask(tasknow)) { THUAI3::use(0, 0, 0); Sleep(50); }
        else {
            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
            if (dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY) >= 9) return;
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
        }
    }
}

void play()
{
    if (!checkbegin) Begin();
    else if (checkmember == 1) {
        if ((10 * 60 * 1000 - getGameTime()) <= 90000) {
            double maxxjb = 0.0;
            int maxid = 0;
            for (list<DishType>::iterator iter = task_list.begin(); iter != task_list.end(); ++iter) {
                int id = (int)(*iter - CookedRice);
                if (average_task_time[id] < (600000 - getGameTime()) && xingjiabi[id]>maxxjb) {
                    maxxjb = xingjiabi[id];
                    maxid = id + (int)CookedRice;
                }
            }
            if (maxid)
            {
                bool checkwait;
                Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
                list<Obj> Objlist_pot = mapp.get_mapcell(PREX, PREY);
                DishType tmpp[100]; int tot = 0;
                for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
                    if (iter->dish != 0) tmpp[++tot] = iter->dish;
                for (int i = 1; i <= tot; ++i)
                    if (checktask(tmpp[i])) {
                        THUAI3::pick(false, Dish, tmpp[i]);
                        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
                        if (checktask(tmpp[i])) { THUAI3::use(0, 0, 0); Sleep(50); }
                        else {
                            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
                            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
                        }
                        checkwait = 1;
                        break;
                    }
                if (!checkwait) task_finish((DishType)maxid);
            }
        }
        else
        {
            bool checkwait = 0;
            if (PlayerInfo.position.x >= 23 && PlayerInfo.position.x <= 27) {
                Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY);
                list<Obj> Objlist_pot = mapp.get_mapcell(PREX, PREY);
                DishType tmpp[100]; int tot = 0;
                for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter)
                    if (iter->dish != 0) tmpp[++tot] = iter->dish;
                for (int i = 1; i <= tot; ++i)
                    if (checktask(tmpp[i])) {
                        THUAI3::pick(false, Dish, tmpp[i]);
                        Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, commit_x[1], commit_y[1]);
                        if (checktask(PlayerInfo.dish)) { THUAI3::use(0, 0, 0); Sleep(50); if (PlayerInfo.dish == TomatoFriedEgg) --taskWait[PlayerInfo.dish]; }
                        else {
                            Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, PREX,PREY);
                            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), ang(PlayerInfo.position.x, PlayerInfo.position.y, PREX, PREY), true); Sleep(50);
                        }
                        checkwait = 1;
                        break;
                    }
            }
            if (!checkwait) task_finish(task_list.back());
        }
    }
    else {
        bool checkwait = 0;
        if (!checkwait) {
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
            Move_player_near_1(PlayerInfo.position.x, PlayerInfo.position.y, SPAWN_x[int(Potato)], SPAWN_y[int(Potato)]);
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5);
            THUAI3::pick(false, Block, Potato); Sleep(50);
            while (PlayerInfo.dish == 0) { while (PlayerInfo.recieveText[0] != 'm') Sleep(5); Sleep(50); THUAI3::pick(false, Block, Potato); }
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5); Move_player_near(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[2], COOK_y[2]);
            //做米饭
            list<Obj> Objlist_pot = mapp.get_mapcell(COOK_x[2], COOK_y[2]);
            DishType tmpp[100]; int tot = 0;
            for (list<Obj>::iterator iter = Objlist_pot.begin(); iter != Objlist_pot.end(); ++iter){
                while (iter->dish == CookingDish) Sleep(1);
                break;
            }
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5); 
            THUAI3::put(dist(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[2], COOK_y[2]), ang(PlayerInfo.position.x, PlayerInfo.position.y, COOK_x[2], COOK_y[2]), true);Sleep(50);
            while (PlayerInfo.recieveText[0] != 'm') Sleep(5); THUAI3::use(0, 0, 0); Sleep(50);
        }
    }
}
