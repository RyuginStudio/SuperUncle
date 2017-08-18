#include <string>
#include <iostream>
#include "LivesScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameData.h"
#include "TestScene.h"

USING_NS_CC;

#define TEST 9999

using namespace std;
using namespace CocosDenshion;
using namespace cocostudio::timeline;

extern int mario_lives;
extern int mission;

Scene* LivesScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LivesScene::create();
	scene->addChild(layer);
	return scene;
}

bool LivesScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	auto visSize = Director::getInstance()->getVisibleSize();

	string string_mario_lives = to_string(mario_lives);

	auto pic_lives = Sprite::create("res/PICTURE/lives.png");                              //马里奥生命头像
	auto label_lives = Label::create("x"+string_mario_lives,"NewSuperMarioFontU.ttf",80);  //马里奥还有几条命
	label_lives->setAnchorPoint(Vec2(0, 0.5));
	
	pic_lives->setPosition(Point(visSize.width / 2 - 50, visSize.height / 2));
	label_lives->setPosition(Point(visSize.width * 0.5, visSize.height / 2));

	this->addChild(label_lives);
	this->addChild(pic_lives);

	//设定计时器等待若干秒确保玩家看到还有多少条命
	this->scheduleOnce(CC_CALLBACK_0(LivesScene::missionJump, this), 3.f, "jumpToMissionScene");  //3秒后跳转至关卡

	return true;
}

void LivesScene::missionJump()
{
	//接下来跳转至XX场景关卡
	switch (mission)//根据mission的数值跳转关卡
	{
	case 1://第一关

		break;
	case 2:

		break;
	case 3:

		break;
	case 4:

		break;
	case 5:

		break;
	case 6:

		break;
	case 7:

		break;
	case 8:

		break;
	case 9:

		break;
	case 10://第十关

		break;

	case TEST://测试关
	{		
		Director::getInstance()->replaceScene(TransitionFade::create(2.f, TestScene::createScene()));
		break;
	}
		
	default://考虑达成某些条件，设置为隐藏关卡
		break;
	}

}