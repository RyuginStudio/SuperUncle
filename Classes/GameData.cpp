/*
*游戏数据相关
*考虑日后使用xml或者sqlite数据库
*/


#include "GameData.h"

int mario_lives = 6; //初始"生命"

int mission = 9999;  //初始关卡    9999 => 游戏测试

int specialSceneScore = 0; //隐藏结局点数  


GameData::GameData()
{
}

GameData::~GameData()
{
}