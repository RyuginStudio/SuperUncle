#include <string>
#include "LivesScene.h"
#include "GameData.h"
#include "TestScene.h"

using namespace cocos2d;


#define TEST 9999

static Size visSize;

extern int mario_lives;
extern int mission;
extern int specialSceneScore; //隐藏结局点数

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

	auto string_mario_lives = __String::createWithFormat("%d", mario_lives)->_string;

	auto pic_lives = Sprite::create("res/PICTURE/lives.png");
	auto label_lives = Label::createWithTTF("x" + string_mario_lives, "NewSuperMarioFontU.ttf", 80);
	label_lives->setAnchorPoint(Vec2(0, 0.5));

	pic_lives->setPosition(Point(visSize.width / 2 - 50, visSize.height / 2));
	label_lives->setPosition(Point(visSize.width * 0.5, visSize.height / 2));

	this->addChild(label_lives);
	this->addChild(pic_lives);

	this->scheduleOnce(CC_CALLBACK_0(LivesScene::missionJump, this), 3.f, "missionJump");

	return true;
}

void LivesScene::missionJump()
{
	switch (mission)//根据数值跳转关卡
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
	case 10:

		break;

	case TEST://测试
	{
		Director::getInstance()->replaceScene(TransitionFade::create(2.f, TestScene::createScene()));
		break;
	}

	default://隐藏关卡
		break;
	}
}