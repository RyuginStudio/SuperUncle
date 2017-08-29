#include <string>
#include <vector>
#include <iostream>
#include "TestScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Character.h"
#include "Controler.h"
#include "VirtualRockerAndButton.h"

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

Layer* Layer_BG;            //背景+云
Layer* Layer_UI;            //金币+得分+时间
Layer* Layer_Control;       //摇杆+按钮
Layer* Layer_GameSettings;  //游戏相关设置
Layer* Layer_GameScene;     //游戏场景

extern map<string, bool> map_keyPressed;
extern Size visSize;


Scene* TestScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TestScene::create();
	scene->addChild(layer);
	return scene;
}

bool TestScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visSize = Director::getInstance()->getVisibleSize();

	Layer_BG = Layer::create();
	Layer_BG->setName("Layer_BG");
	this->addChild(Layer_BG, 0);

	Layer_UI = Layer::create();
	Layer_UI->setName("Layer_UI");
	this->addChild(Layer_UI, 10);

	Layer_Control = Layer::create();
	Layer_Control->setName("Layer_Control");
	this->addChild(Layer_Control, 11);

	Layer_GameSettings = Layer::create();
	Layer_GameSettings->setName("Layer_GameSettings");
	this->addChild(Layer_GameSettings, 12);

	Layer_GameScene = this;


	//背景图片
	Controler::createBackGround(Layer_BG, visSize);

	//虚拟摇杆及事件
	VirtualRockerAndButton::getInstance(Layer_Control, visSize);

	//抗锯齿
	auto pChildrenArray = tiledMap->getChildren();

	SpriteBatchNode* child = NULL;
	Ref* pObject = NULL;

	for (auto it = pChildrenArray.begin(); it != pChildrenArray.end(); it++) {
		pObject = *it;
		child = (SpriteBatchNode*)pObject;
		child->getTexture()->setAliasTexParameters();
	}

	//正交视图
	Director::getInstance()->setProjection(Director::Projection::_2D);

	//角色初始化相关
	Character::getInstance()->initBeginPos(tiledMap);
	Character::getInstance()->sp_man->setGlobalZOrder(1);
	this->addChild(Character::getInstance()->sp_man);

	//加载瓦片地图
	this->addChild(tiledMap, 9);

	//BGM
	SimpleAudioEngine::getInstance()->playBackgroundMusic("res/BGM/Mission1BGM.mp3", true);

	//定时器
	this->schedule(CC_CALLBACK_1(TestScene::update_per_second, this), 1.0f, "oneSecond");    //游戏时间衰减，每1.0秒后调用
	this->scheduleUpdate();

	//重力计监听事件 => 摇一摇暂停游戏
	auto eventListenerAcceleration = EventListenerAcceleration::create(CC_CALLBACK_2(TestScene::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerAcceleration, this);
	Device::setAccelerometerEnabled(true); //开启重力计

	//键盘监听事件
	auto eventListenerKeyboard = EventListenerKeyboard::create();
	eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(Controler::onKeyPressed, Controler::getInstance());
	eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(Controler::onKeyReleased, Controler::getInstance());
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);

	//监听遥感按钮等触摸事件
	auto eventListenerTouch = EventListenerTouchOneByOne::create();
	eventListenerTouch->onTouchBegan = CC_CALLBACK_2(VirtualRockerAndButton::onTouchBegan, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	eventListenerTouch->onTouchMoved = CC_CALLBACK_2(VirtualRockerAndButton::onTouchesMoved, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	eventListenerTouch->onTouchEnded = CC_CALLBACK_2(VirtualRockerAndButton::onTouchEnded, VirtualRockerAndButton::getInstance(Layer_UI, visSize));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerTouch, this);

	//初始化即将操控的map容器
	map_keyPressed.insert(pair<string, bool>("up", false));
	map_keyPressed.insert(pair<string, bool>("down", false));
	map_keyPressed.insert(pair<string, bool>("left", false));
	map_keyPressed.insert(pair<string, bool>("right", false));

	return true;
}

void TestScene::update_per_second(float delta) //1s定时器
{
	//Controler::CreateUpdateUI(Layer_UI, visSize, gameTime, coin, score); //创建|刷新时间等UI
	Controler::createCloud(Layer_BG, Character::getInstance()->sp_man);
}

void TestScene::update(float delta)  //帧定时器
{
	Controler::tiledMapScroll(this, Layer_BG, Layer_UI, Layer_Control, tiledMap, Character::getInstance()->sp_man, delta);  //地图滚动
	Controler::cloudPosControl();       //云朵位置控制
	Controler::keyBoardControler(delta);        //键盘控制器进一步处理  =>  触控也调用
	VirtualRockerAndButton::touchMoveControl(); //触摸行走控制
}

void TestScene::onAcceleration(Acceleration * acc, Event * unused_event)  //“摇一摇”暂停游戏
{
	static Acceleration lastAcc = *acc;

	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	static long lLastTime = 0;

	//x,y,z方向的加速的平方加起来超过一个阈值视为“摇一摇”
	if ((acc->x - lastAcc.x)*(acc->x - lastAcc.x) + (acc->y - lastAcc.y)*(acc->y - lastAcc.y) + (acc->z - lastAcc.z)*(acc->z - lastAcc.z) >= 10)
	{
		if (nowTimeval.tv_sec - lLastTime > 1.0)
		{
			if (!Director::getInstance()->isPaused())
				Controler::GamePauseAndSettings(this, Layer_GameSettings, visSize);
			else
			{
				Layer_GameSettings->setVisible(false);
				Director::getInstance()->resume();
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
				SimpleAudioEngine::getInstance()->resumeAllEffects();
			}
			lLastTime = nowTimeval.tv_sec;
		}
	}
	lastAcc = *acc;
}
