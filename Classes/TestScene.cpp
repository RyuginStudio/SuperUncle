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
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	world = new b2World(gravity);

	// Do we want to let bodies sleep?
	world->SetAllowSleeping(false);  //����͸���Ϊ��̬����

	world->SetContinuousPhysics(true);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner

									  // Call the body factory which allocates memory for the ground body
									  // from a pool and creates the ground box shape (also from a pool).
									  // The body is also added to the world.
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2EdgeShape groundBox;

	// bottom
	groundBox.Set(b2Vec2(0 / PTM_RATIO, 0 / PTM_RATIO), b2Vec2(visSize.width / PTM_RATIO, 0 / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// top
	groundBox.Set(b2Vec2(0 / PTM_RATIO, visSize.height / PTM_RATIO), b2Vec2(visSize.width / PTM_RATIO, visSize.height / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// left
	groundBox.Set(b2Vec2(0 / PTM_RATIO, 0 / PTM_RATIO), b2Vec2(0 / PTM_RATIO, visSize.height / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// right
	groundBox.Set(b2Vec2(visSize.width / PTM_RATIO, 0), b2Vec2(visSize.width / PTM_RATIO, visSize.height / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);


	_debugDraw = new GLESDebugDraw(32);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;

	_debugDraw->SetFlags(flags);
	world->SetDebugDraw(_debugDraw);


	Layer_BG->setVisible(false);
	Layer_Control->setVisible(false);
	//Layer_TitledMap->setVisible(false);
}

void TestScene::createPhysicalUnCross()  //������ͨ�в�
{
	//����һ�����physics�������ж����Group
	TMXObjectGroup* objGroup = tiledMap->getObjectGroup("physics");

	auto vec = objGroup->getObjects();

	for (auto i : vec)
	{
		float obj_X = i.asValueMap().at("x").asFloat();   //�õ������x��y����
		float obj_Y = i.asValueMap().at("y").asFloat();


		float obj_width = i.asValueMap().at("width").asFloat();
		float obj_height = i.asValueMap().at("height").asFloat();


		CCLOG("Run to createPhysicalUnCross x:%f, y:%f, width: %f, height: %f", obj_X, obj_Y, obj_width, obj_height);


		b2BodyDef body_def;
		body_def.type = b2_staticBody;
		body_def.position.Set((obj_X) / PTM_RATIO + 53/PTM_RATIO, (obj_Y) / PTM_RATIO + 53 / PTM_RATIO);
		auto _pyhsicalBody = world->CreateBody(&body_def);



		b2PolygonShape polygon;
		polygon.SetAsBox(obj_width / PTM_RATIO / 2, obj_height / PTM_RATIO / 2);
		b2FixtureDef fix_def;
		fix_def.shape = &polygon;

		_pyhsicalBody->CreateFixture(&fix_def);
	}
}


void TestScene::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)  //�������ֵ���
{
	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);

	world->DrawDebugData();

	CHECK_GL_ERROR_DEBUG();
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

	initPysics();
	createPhysicalUnCross(); //������Ƭ��ͼ��������ͨ�в�

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

	Character::getInstance()->sp_man->setAnchorPoint(Vec2(0, 1.1));  //��ɫ������Ǻϵ���
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(Character::getInstance()->sp_man->getPosition().x / PTM_RATIO, Character::getInstance()->sp_man->getPosition().y / PTM_RATIO);

	auto body_man = world->CreateBody(&bodydef);
	body_man->SetUserData(Character::getInstance()->sp_man);

	//PhysicsEditor:�������������
	b2Vec2 points[27] = {
		b2Vec2(12.00000 / PTM_RATIO,30.00000 / PTM_RATIO), b2Vec2(12.00000 / PTM_RATIO,36.00000 / PTM_RATIO),b2Vec2(20.00000 / PTM_RATIO,34.00000 / PTM_RATIO),
		b2Vec2(12.00000 / PTM_RATIO,36.00000 / PTM_RATIO),b2Vec2(12.00000 / PTM_RATIO,22.00000 / PTM_RATIO), b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO),
		b2Vec2(-12.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(-20.00000 / PTM_RATIO,30.00000 / PTM_RATIO),b2Vec2(-4.00000 / PTM_RATIO,43.00000 / PTM_RATIO),
		b2Vec2(9.00000 / PTM_RATIO,46.00000 / PTM_RATIO),b2Vec2(21.00000 / PTM_RATIO,17.00000 / PTM_RATIO), b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),
		b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(12.00000 / PTM_RATIO,22.00000 / PTM_RATIO),b2Vec2(20.00000 / PTM_RATIO,23.00000 / PTM_RATIO),
		b2Vec2(11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO),b2Vec2(-11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO), b2Vec2(4.00000 / PTM_RATIO,-35.00000 / PTM_RATIO),
		b2Vec2(11.00000 / PTM_RATIO,-38.00000 / PTM_RATIO), b2Vec2(-11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO),b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO),
		b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),b2Vec2(6.00000 / PTM_RATIO,-17.00000 / PTM_RATIO), b2Vec2(4.00000 / PTM_RATIO,-35.00000 / PTM_RATIO),
		b2Vec2(6.00000 / PTM_RATIO,-17.00000 / PTM_RATIO), b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),b2Vec2(12.00000 / PTM_RATIO,-12.00000 / PTM_RATIO) };
	

	b2PolygonShape shape_body_man;
	shape_body_man.Set(points, 27);

	b2FixtureDef fixturedef;
	fixturedef.density = 1.0f;   //�ܶ�
	fixturedef.friction = 0.2f;   //Ħ��
	fixturedef.shape = &shape_body_man;
	body_man->CreateFixture(&fixturedef);

	Character::getInstance()->body_man = body_man;  //���崫��Character



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
			//sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle())); //����ת
		}
	}

	Controler::tiledMapScroll(delta, world);    //��ͼ����
	//Controler::cloudPosControl();             //�ƶ�λ�ÿ���
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
