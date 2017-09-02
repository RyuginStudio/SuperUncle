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

Layer* Layer_BG;             //����+��
Layer* Layer_UI;             //���+�÷�+ʱ��
Layer* Layer_Control;        //ҡ��+��ť
Layer* Layer_GameSettings;   //��Ϸ�������
Layer* Layer_TitledMap;      //��Ƭ��ͼ


TMXTiledMap *tiledMap;       //��Ƭ��ͼ

extern Size visSize;


void TestScene::initPysics() //��ʼ��������
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


	//����ͼƬ
	Controler::createBackGround();

	//����ҡ�˼��¼�
	VirtualRockerAndButton::getInstance();

	//�����
	auto pChildrenArray = tiledMap->getChildren();

	SpriteBatchNode* child = NULL;
	Ref* pObject = NULL;

	for (auto it = pChildrenArray.begin(); it != pChildrenArray.end(); it++) {
		pObject = *it;
		child = (SpriteBatchNode*)pObject;
		child->getTexture()->setAliasTexParameters();
	}

	//������ͼ
	Director::getInstance()->setProjection(Director::Projection::_2D);

	//��ɫ��ʼ�����
	Character::getInstance()->initBeginPos(tiledMap);
	Character::getInstance()->sp_man->setGlobalZOrder(1);
	this->addChild(Character::getInstance()->sp_man);

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(Character::getInstance()->sp_man->getPosition().x / PTM_RATIO, Character::getInstance()->sp_man->getPosition().y / PTM_RATIO);

	auto body_man = world->CreateBody(&bodydef);
	body_man->SetUserData(Character::getInstance()->sp_man);

	//PhysicsEditor:�������������
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
	fixturedef.density = 1.0f;   //�ܶ�
	fixturedef.friction = 0.3f;  //Ħ��
	fixturedef.shape = &shape_body_man;
	body_man->CreateFixture(&fixturedef);

	Character::getInstance()->body_man = body_man;  //���崫��Character


	for (auto &object : tiledMap->getChildren())
	{
		auto layer = static_cast<TMXLayer*>(object);
		if (layer)
		{
			for (int x = 0;x < layer->getLayerSize().width;++x)
			{
				for (int y = 0; y < layer->getLayerSize().height; ++y)
				{
					//��������tiles  
					auto tile = layer->getTileAt(Point(x, y));
					if (tile)
					{
						tile->setAnchorPoint(Vec2(0, 0));
						int gid = layer->getTileGIDAt(Point(x, y));
						auto properties = tiledMap->getPropertiesForGID(gid);
						//�����unCross���ԣ������Fixture�����ں�����ײ����  
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










	//������Ƭ��ͼ
	Layer_TitledMap->addChild(tiledMap);

	//BGM
	SimpleAudioEngine::getInstance()->playBackgroundMusic("res/BGM/Mission1BGM.mp3", true);

	//��ʱ��
	this->schedule(CC_CALLBACK_1(TestScene::update_per_second, this), 1.0f, "oneSecond");    //��Ϸʱ��˥����ÿ1.0������
	this->scheduleUpdate();

	//�����Ƽ����¼� => ҡһҡ��ͣ��Ϸ
	auto eventListenerAcceleration = EventListenerAcceleration::create(CC_CALLBACK_2(Controler::onAcceleration, Controler::getInstance()));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerAcceleration, this);
	Device::setAccelerometerEnabled(true); //�豸����������

	//���̼����¼�
	auto eventListenerKeyboard = EventListenerKeyboard::create();
	eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(Controler::onKeyPressed, Controler::getInstance());
	eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(Controler::onKeyReleased, Controler::getInstance());
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);

	//����ң�а�ť�ȴ����¼�
	auto eventListenerTouch = EventListenerTouchOneByOne::create();
	eventListenerTouch->onTouchBegan = CC_CALLBACK_2(VirtualRockerAndButton::onTouchBegan, VirtualRockerAndButton::getInstance());
	eventListenerTouch->onTouchMoved = CC_CALLBACK_2(VirtualRockerAndButton::onTouchesMoved, VirtualRockerAndButton::getInstance());
	eventListenerTouch->onTouchEnded = CC_CALLBACK_2(VirtualRockerAndButton::onTouchEnded, VirtualRockerAndButton::getInstance());
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerTouch, this);

	return true;
}

void TestScene::update_per_second(float delta)
{
	//Controler::CreateUpdateUI(); //����|ˢ��ʱ���UI  =>   ���ڵ÷ַ�ʽ��Ϊ���۲���ģʽ
	//Controler::createCloud();
}

void TestScene::update(float delta)
{
	//����ģ����ڷ���Controler��
	world->Step(delta, 10, 10);
	for (auto body = world->GetBodyList(); body != nullptr; body = body->GetNext())
	{
		if (body->GetUserData() != nullptr)
		{
			auto sprite = (Sprite *)body->GetUserData();
			sprite->setPosition(Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle())); //��ת
		}
	}

	Controler::tiledMapScroll(delta);           //��ͼ����
	//Controler::cloudPosControl();               //�ƶ�λ�ÿ���
	Controler::keyBoardControler(delta);        //���̿�������һ������  =>  ����Ҳ����
	VirtualRockerAndButton::touchMoveControl(); //�������߿���
}

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
	CC_SAFE_DELETE(world);
}
