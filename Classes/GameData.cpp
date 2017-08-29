/*
*游戏数据相关
*考虑日后使用xml或者sqlite数据库
*在该类中完成读表处理
*/


#include "GameData.h"

int mario_lives = 6;         //初始"生命"
int mission = 9999;          //初始关卡    9999 => 游戏测试
int specialSceneScore = 0;   //隐藏结局点数  
int coin = 0;                //金币数
int gameTime = 500;          //游戏时间
int score = 0;               //计分板

GameData::GameData()
{
}

GameData::~GameData()
{
}