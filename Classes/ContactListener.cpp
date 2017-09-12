/*
*时间：2017年9月12日15:56:11
*作者：vszed
*功能：碰撞处理
*说明：发生碰撞后的一系列处理
*改进：日后可能改用消息机制=>观察者
*/

#include "ContactListener.h"
#include "cocos2d.h"
#include "Character.h"

#define PTM_RATIO 32 

USING_NS_CC;


void ContactListener::BeginContact(b2Contact * contact)
{
	CCLOG("BeginContact");
	
	if (contact->GetFixtureA()->GetBody() == Character::getInstance()->body_man)
	{
		auto worldManifold = new b2WorldManifold();
		//计算碰撞点全局坐标，存到worldManifold变量
		contact->GetWorldManifold(worldManifold);
		//获取碰撞点
		auto contactPoint = worldManifold->points[0];
		CCLOG("x: %f  y: %f", contactPoint.x, contactPoint.y);
	}
	else if (contact->GetFixtureB()->GetBody() == Character::getInstance()->body_man)
	{
		auto worldManifold = new b2WorldManifold();
		contact->GetWorldManifold(worldManifold);
		auto contactPoint = worldManifold->points[0];
		CCLOG("x: %f  y: %f", contactPoint.x, contactPoint.y);
	}

}

void ContactListener::EndContact(b2Contact * contact)
{
	CCLOG("EndContact");
}

ContactListener::ContactListener()
{
}


ContactListener::~ContactListener()
{
}
