/*
时间：2017年7月17日10:38:48
作者：张天翼
功能：测试场景实现用于游戏测试
*/

#include "b2TestScene.h"
#include <vector>

USING_NS_CC;
using namespace std;

vector <b2Body*> vec_body_all;  //所有刚体
vector <b2Body*> vec_body_delete;  //一个用于存放待删除刚体的容器

bool changeSpriteFrame = false;

enum BOX2DMASK   //碰撞类别 
{
 BOX2D_MASK_WORLD_BORDER = 0X0001,
 BOX2D_MASK_BOX = 0X0002,
};


cocos2d::Scene * b2TestScene::createScene()
{
	auto scene = Scene::create();

	auto layer = b2TestScene::create();

	scene->addChild(layer);

	return scene;
}

bool b2TestScene::init()
{
	auto eventListenerKeyboard = EventListenerKeyboard::create();
	eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(b2TestScene::onKeyPressed, this);
	eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(b2TestScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);

	if (!Layer::init())
	{
		return false;
	}

	//初始化物理引擎
	this->iniPhysics();

	setTouchEnabled(true);   //设置单点触控
	setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);

	this->scheduleUpdate();  //加载帧定时器

	return true;
}

void b2TestScene::update(float dt) //定时器回调函数
{
	float timeStep = 0.03f;
	int32 velocityinterations = 8;
	int32 positionInterations = 1;

	world->Step(timeStep, velocityinterations, positionInterations);

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			Sprite *sprite = (Sprite*)b->GetUserData();
			sprite->setPosition(Vec2(b->GetPosition().x * PTM_RATIO, b->GetPosition().y* PTM_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

	this->delete_contact_body();
}


bool b2TestScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	this->addNewSpriteAtPosition(touch->getLocation());
	return true;
}

void b2TestScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event * event)
{
	switch (keycode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	{
		for (auto i : vec_body_all)
		{
			auto pos = i->GetPosition();
			auto sprite = (CCSprite*)i->GetFixtureList()->GetUserData();
			sprite->setPosition(--pos.x, pos.y);
		}
		break;
	}
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		break;
	}
	default:
		break;
	}
}

void b2TestScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *)
{
	CCLOG("Released!");
}

void b2TestScene::iniPhysics()
{
	Size s = Director::getInstance()->getVisibleSize();  //获取尺寸

	//重力参数
	b2Vec2 gravity;             //b2vec2 是一个二维矢量
	gravity.Set(0.0f, -40.0f);  //设置在x轴、y轴方向上的矢量值

	//创建世界
	world = new b2World(gravity);

	world->SetContactListener(this);

	//允许物体是否休眠
	world->SetAllowSleeping(true); 

	//开启连续物理测试
	world->SetContinuousPhysics(true);

	//定义地面物体
	b2BodyDef groundBodyDef;

	//左下角
	groundBodyDef.position.Set(0, 0);

	//创建地面物体
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	//定义一个有边的形状
	b2EdgeShape groundBox;

	//底部
	groundBox.Set(b2Vec2(0, 0), b2Vec2(s.width / PTM_RATIO, 0));
	//使用夹具固定形状到物体上
	groundBody->CreateFixture(&groundBox, 0);

	//顶部
	groundBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(s.width / PTM_RATIO, s.height / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	//左部
	groundBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(0, 0));
	groundBody->CreateFixture(&groundBox, 0);
	
	//右部
	groundBox.Set(b2Vec2(s.width/PTM_RATIO, s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO, 0));
	groundBody->CreateFixture(&groundBox, 0);


	b2Fixture* fixture = groundBody->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();

	filter.categoryBits = BOX2D_MASK_WORLD_BORDER; //类别标志位
	filter.maskBits = BOX2D_MASK_BOX; //遮罩标志位(致命碰撞) => 可以跟谁碰撞

	fixture->SetFilterData(filter);
}

void b2TestScene::addNewSpriteAtPosition(cocos2d::Point p)
{
	CCLOG("Add sprite x: %d, y: %d", p.x, p.y);

	//创建物理引擎精灵对象

	Sprite *sprite = Sprite::create("res/Picture/box.png");

	if (changeSpriteFrame == false)
	{
		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("res/Picture/box.png");
		sprite->setTexture(texture);
		sprite->setPosition(p);
		this->addChild(sprite);
		changeSpriteFrame = true;
	}
	else
	{
		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("res/Picture/square.png");
		sprite->setTexture(texture);
		sprite->setPosition(p);
		this->addChild(sprite);
		changeSpriteFrame = false;
	}

	//物体定义
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	
	bodyDef.position.Set(p.x / PTM_RATIO, p.y / PTM_RATIO);
	b2Body *body = world->CreateBody(&bodyDef);
	body->SetUserData(sprite);


	//定义两米见方的盒子形状
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.5, 0.48);

	//夹具定义
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.f;
	body->CreateFixture(&fixtureDef);

	b2Fixture* fixture = body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();

	filter.categoryBits = BOX2D_MASK_BOX; //类别标志位
	filter.maskBits = BOX2D_MASK_BOX | BOX2D_MASK_WORLD_BORDER; //遮罩标志位(致命碰撞) => 可以跟谁碰撞

	fixture->SetFilterData(filter);

	vec_body_all.push_back(body);
	
}

void b2TestScene::BeginContact(b2Contact * contact)  //不能在碰撞检测里销毁刚体
{
	auto bodyA = contact->GetFixtureA()->GetBody();
	auto bodyB = contact->GetFixtureB()->GetBody();
	
	contact->GetManifold();

	/*if (spriteB == nullptr)
		exit(0);*/

	//spriteB->setColor(Color3B::YELLOW);


	if (bodyB->GetFixtureList()->GetFilterData().categoryBits == BOX2D_MASK_BOX 
		&&bodyA->GetFixtureList()->GetFilterData().categoryBits == BOX2D_MASK_BOX)   //=>检测碰撞的物体为两个盒子时
	{	

		bodyB->ApplyForce(b2Vec2::b2Vec2(0, 6*PTM_RATIO), bodyB->GetWorldCenter(), false);
		
		CCLOG("delete box!");
		vec_body_delete.push_back(bodyA);
	}
}

void b2TestScene::EndContact(b2Contact * contact)
{
}

void b2TestScene::delete_contact_body()  //删除待删除容器中的所有刚体
{
	for (size_t i = 0; i < vec_body_delete.size(); i++)
	{
		this->removeChild((Sprite*)vec_body_delete[i]->GetUserData());
		vec_body_delete[i]->SetUserData(nullptr);
		world->DestroyBody(vec_body_delete[i]);
	}
	vec_body_delete.clear();
}

b2TestScene::b2TestScene()
{

}

b2TestScene::~b2TestScene()
{
	CC_SAFE_DELETE(world); //new world => 在C++中，为new关键字分配内存空间，需要在析构中用delete释放内存 => cocos中有相应的delete宏定义
}