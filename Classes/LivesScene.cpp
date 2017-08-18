#include <string>
#include <iostream>
#include "LivesScene.h"
#include "GameData.h"
#include "TestScene.h"

USING_NS_CC;

#define TEST 9999

using namespace std;

static Size visSize;

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

	visSize = Director::getInstance()->getVisibleSize();

	string string_mario_lives = to_string(mario_lives);

	auto pic_lives = Sprite::create("res/PICTURE/lives.png");
	auto label_lives = Label::create("x"+string_mario_lives,"NewSuperMarioFontU.ttf",80);
	label_lives->setAnchorPoint(Vec2(0, 0.5));
	
	pic_lives->setPosition(Point(visSize.width / 2 - 50, visSize.height / 2));
	label_lives->setPosition(Point(visSize.width * 0.5, visSize.height / 2));


	this->addChild(label_lives);
	this->addChild(pic_lives);


	this->scheduleOnce(CC_CALLBACK_1(LivesScene::missionJump, this), 3.f, "jumpToMissionScene");

	return true;
}

void LivesScene::missionJump(float dt)
{
	switch (mission)//������ֵ��ת�ؿ�
	{
	case 1://��һ��

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
	case 10://��ʮ��

		break;

	case TEST://���Թ�
	{		
		Director::getInstance()->replaceScene(TransitionFade::create(2.f, TestScene::createScene()));
		break;
	}
		
	default://���Ǵ��ĳЩ����������Ϊ���عؿ�
		break;
	}

}