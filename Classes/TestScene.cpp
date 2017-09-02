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

Layer* Layer_BG;             //背景+云
Layer* Layer_UI;             //金币+得分+时间
Layer* Layer_Control;        //摇杆+按钮
Layer* Layer_GameSettings;   //游戏相关设置
Layer* Layer_TitledMap;      //瓦片地图


TMXTiledMap *tiledMap;       //瓦片地图

extern Size visSize;


void TestScene::initPysics() //初始物理引擎
{
	auto gravity = b2Vec2(0.f, -10.f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
}

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

	initPysics();

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

	Layer_TitledMap = Layer::create();
	Layer_TitledMap->setName("Layer_TitledMap");
	this->addChild(Layer_TitledMap, 1);

	tiledMap = cocos2d::TMXTiledMap::create("MAP/Mission1.tmx");


	//背景图片
	Controler::createBackGround();

	//虚拟摇杆及事件
	VirtualRockerAndButton::getInstance();

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

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(Character::getInstance()->sp_man->getPosition().x / PTM_RATIO, Character::getInstance()->sp_man->getPosition().y / PTM_RATIO);

	auto body_man = world->CreateBody(&bodydef);
	body_man->SetUserData(Character::getInstance()->sp_man);

	//PhysicsEditor:不规则刚体数据
	/*b2Vec2 points[27] = {
		b2Vec2(12.00000 / PTM_RATIO,30.00000 / PTM_RATIO), b2Vec2(12.00000 / PTM_RATIO,36.00000 / PTM_RATIO),b2Vec2(20.00000 / PTM_RATIO,34.00000 / PTM_RATIO),
		b2Vec2(12.00000 / PTM_RATIO,36.00000 / PTM_RATIO),b2Vec2(12.00000 / PTM_RATIO,22.00000 / PTM_RATIO), b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO),
		b2Vec2(-12.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(-20.00000 / PTM_RATIO,30.00000 / PTM_RATIO),b2Vec2(-4.00000 / PTM_RATIO,43.00000 / PTM_RATIO),
		b2Vec2(9.00000 / PTM_RATIO,46.00000 / PTM_RATIO),b2Vec2(21.00000 / PTM_RATIO,17.00000 / PTM_RATIO), b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),
		b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(12.00000 / PTM_RATIO,22.00000 / PTM_RATIO),b2Vec2(20.00000 / PTM_RATIO,23.00000 / PTM_RATIO),
		b2Vec2(11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO),b2Vec2(-11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO), b2Vec2(4.00000 / PTM_RATIO,-35.00000 / PTM_RATIO),
		b2Vec2(11.00000 / PTM_RATIO,-38.00000 / PTM_RATIO), b2Vec2(-11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO),b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO),
		b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),b2Vec2(6.00000 / PTM_RATIO,-17.00000 / PTM_RATIO), b2Vec2(4.00000 / PTM_RATIO,-35.00000 / PTM_RATIO),
		b2Vec2(6.00000 / PTM_RATIO,-17.00000 / PTM_RATIO), b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),b2Vec2(12.00000 / PTM_RATIO,-12.00000 / PTM_RATIO) };*/
	b2Vec2 points[27] = {
		b2Vec2(20.000 / PTM_RATIO,34.000 / PTM_RATIO), b2Vec2(12.000 / PTM_RATIO,36.000 / PTM_RATIO),b2Vec2(12.000 / PTM_RATIO,30.000 / PTM_RATIO),
		b2Vec2(9.000 / PTM_RATIO,46.000 / PTM_RATIO),b2Vec2(-4.000 / PTM_RATIO,43.000 / PTM_RATIO), b2Vec2(-20.000 / PTM_RATIO,30.000 / PTM_RATIO),
		b2Vec2(-12.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(-8.000 / PTM_RATIO,9.000 / PTM_RATIO),b2Vec2(12.000 / PTM_RATIO,22.000 / PTM_RATIO),
		b2Vec2(12.000 / PTM_RATIO,36.000 / PTM_RATIO), b2Vec2(20.000 / PTM_RATIO,23.000 / PTM_RATIO), b2Vec2(12.000 / PTM_RATIO,22.000 / PTM_RATIO),
		b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO),b2Vec2(21.000 / PTM_RATIO,17.000 / PTM_RATIO),
		b2Vec2(11.00000 / PTM_RATIO,-38.000 / PTM_RATIO),b2Vec2(4.000 / PTM_RATIO,-35.000 / PTM_RATIO), b2Vec2(-11.000 / PTM_RATIO,-46.000 / PTM_RATIO),
		b2Vec2(11.000 / PTM_RATIO,-46.000 / PTM_RATIO),b2Vec2(4.000 / PTM_RATIO,-35.000 / PTM_RATIO), b2Vec2(6.000 / PTM_RATIO,-17.000 / PTM_RATIO),
		b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO),b2Vec2(-8.000 / PTM_RATIO,9.000 / PTM_RATIO),b2Vec2(-11.000 / PTM_RATIO,-46.000 / PTM_RATIO),
		b2Vec2(12.000 / PTM_RATIO,-12.000 / PTM_RATIO),b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO), b2Vec2(6.000 / PTM_RATIO,-17.000 / PTM_RATIO) };


	b2PolygonShape shape_body_man;
	shape_body_man.Set(points, 27);

	b2FixtureDef fixturedef;
	fixturedef.density = 1.0f;   //密度
	fixturedef.friction = 0.3f;  //摩擦
	fixturedef.shape = &shape_body_man;
	body_man->CreateFixture(&fixturedef);

	Character::getInstance()->body_man = body_man;  //刚体传入Character


	for (auto &object : tiledMap->getChildren())
	{
		auto layer = static_cast<TMXLayer*>(object);
		if (layer)
		{
			for (int x = 0;x < layer->getLayerSize().width;++x)
			{
				for (int y = 0; y < layer->getLayerSize().height; ++y)
				{
					//遍历所有tiles  
					auto tile = layer->getTileAt(Point(x, y));
					if (tile)
					{
						tile->setAnchorPoint(Vec2(0, 0));
						int gid = layer->getTileGIDAt(Point(x, y));
						auto properties = tiledMap->getPropertiesForGID(gid);
						//如果是unCross属性，则添加Fixture，便于后续碰撞处理  
						if (!properties.isNull()
							&& properties.asValueMap().find("UnCross") != properties.asValueMap().end())
						{
							b2BodyDef bdef;
							bdef.type = b2_staticBody;
							bdef.position.Set(tile->getPosition().x / PTM_RATIO, tile->getPosition().y / PTM_RATIO);

							auto body_tile = world->CreateBody(&bdef);
							body_tile->SetUserData(tile);

							b2PolygonShape b2square;
							b2square.SetAsBox(53 / PTM_RATIO, 53 / PTM_RATIO);

							b2FixtureDef b2fix;
							b2fix.density = 1.f;
							b2fix.friction = 0.f;
							b2fix.shape = &b2square;

							body_tile->CreateFixture(&b2fix);

						}
					}
				}
			}
		}
	}










	//加载瓦片地图
	Layer_TitledMap->addChild(tiledMap);

	//BGM
	SimpleAudioEngine::getInstance()->playBackgroundMusic("res/BGM/Mission1BGM.mp3", true);

	//定时器
	this->schedule(CC_CALLBACK_1(TestScene::update_per_second, this), 1.0f, "oneSecond");    //游戏时间衰减，每1.0秒后调用
	this->scheduleUpdate();

	//重力计监听事件 => 摇一摇暂停游戏
	auto eventListenerAcceleration = EventListenerAcceleration::create(CC_CALLBACK_2(Controler::onAcceleration, Controler::getInstance()));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerAcceleration, this);
	Device::setAccelerometerEnabled(true); //设备开启重力计

	//键盘监听事件
	auto eventListenerKeyboard = EventListenerKeyboard::create();
	eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(Controler::onKeyPressed, Controler::getInstance());
	eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(Controler::onKeyReleased, Controler::getInstance());
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);

	//监听遥感按钮等触摸事件
	auto eventListenerTouch = EventListenerTouchOneByOne::create();
	eventListenerTouch->onTouchBegan = CC_CALLBACK_2(VirtualRockerAndButton::onTouchBegan, VirtualRockerAndButton::getInstance());
	eventListenerTouch->onTouchMoved = CC_CALLBACK_2(VirtualRockerAndButton::onTouchesMoved, VirtualRockerAndButton::getInstance());
	eventListenerTouch->onTouchEnded = CC_CALLBACK_2(VirtualRockerAndButton::onTouchEnded, VirtualRockerAndButton::getInstance());
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerTouch, this);

	return true;
}

void TestScene::update_per_second(float delta)
{
	//Controler::CreateUpdateUI(); //创建|刷新时间等UI  =>   后期得分方式改为：观察者模式
	//Controler::createCloud();
}

void TestScene::update(float delta)
{
	//物理模拟后期放入Controler类
	world->Step(delta, 10, 10);
	for (auto body = world->GetBodyList(); body != nullptr; body = body->GetNext())
	{
		if (body->GetUserData() != nullptr)
		{
			auto sprite = (Sprite *)body->GetUserData();
			sprite->setPosition(Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle())); //旋转
		}
	}

	Controler::tiledMapScroll(delta);           //地图滚动
	//Controler::cloudPosControl();               //云朵位置控制
	Controler::keyBoardControler(delta);        //键盘控制器进一步处理  =>  触控也调用
	VirtualRockerAndButton::touchMoveControl(); //触摸行走控制
}

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
	CC_SAFE_DELETE(world);
}
