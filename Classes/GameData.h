/*============以下内容为游戏数据相关，可考虑移动至txt文档，进行i/o操作，方便用户改写游戏数据==========

*出于安全考虑目前未实现上述功能，防止恶意改写导致游戏崩溃或者丧失可玩性

*游戏本身未连接至数据库，本头文件做数据库之用，以后或有补充数据库的功能

*未完待续。。。

*/


#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;


int mario_lives = 6; //设定初始马里奥"生命数"    0-99条命不影响显示效果，初衷为给你无限命也过不去

int mission = 9999;     //设定初始游戏关卡：第一关    9999 => 游戏测试

int specialSceneScore = 0; //设定达成特殊游戏场景所需的点数（隐藏结局方向）

int latest_draw = 99;   //最后绘制

#endif 