#include "QuitScene.h"

USING_NS_CC;

Scene* QuitScene::createScene()
{
	auto scene = Scene::create();
	auto layer = QuitScene::create();
	scene->addChild(layer);
	return scene;
}

bool QuitScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	static Size win_size = Director::getInstance()->getVisibleSize();

	switch (random(0, 2))
	{
	case 0:
	{
		auto sprite_mario_quit = Sprite::create("res/PICTURE/MarioForQuit.jpg"); //斯大林
		sprite_mario_quit->setPosition(Point(win_size.width / 2, win_size.height / 1.65 - 20));

		auto label_Stalin = Label::createWithTTF("Mario Stalin", "NewSuperMarioFontU.ttf", 80);
		label_Stalin->setPosition(Point(win_size.width / 2, win_size.height / 5 - 25));

		this->addChild(sprite_mario_quit);
		this->addChild(label_Stalin);
		break;
	}

	case 1:
	{
		auto sprite_mario_quit2 = Sprite::create("res/PICTURE/MarioForQuit2.jpg"); //马里奥
		sprite_mario_quit2->setPosition(Point(win_size.width / 2, win_size.height / 1.65 - 20));

		auto label_Mario = Label::createWithTTF("Mario", "NewSuperMarioFontU.ttf", 80);
		label_Mario->setPosition(Point(win_size.width / 2, win_size.height / 5 - 25));

		this->addChild(sprite_mario_quit2);
		this->addChild(label_Mario);
		break;
	}

	case 2:
	{
		auto sprite_mario_quit3 = Sprite::create("res/PICTURE/MarioForQuit3.jpg"); //路易基
		sprite_mario_quit3->setPosition(Point(win_size.width / 2, win_size.height / 1.65 - 20));

		auto label_Luigi = Label::createWithTTF("Luigi Mario", "NewSuperMarioFontU.ttf", 80);
		label_Luigi->setPosition(Point(win_size.width / 2, win_size.height / 5 - 25));

		this->addChild(sprite_mario_quit3);
		this->addChild(label_Luigi);
		break;
	}
	}

	scheduleOnce(CC_CALLBACK_0(QuitScene::quitgame, this), 5.f, "quitgame");   //5秒后退出游戏

	return true;
}

void QuitScene::quitgame()
{
	exit(0); //退出游戏
}
