/*
*ʱ�䣺2017��9��12��15:56:11
*���ߣ�vszed
*���ܣ���ײ����
*˵����������ײ���һϵ�д���
*�Ľ����պ���ܸ�����Ϣ����=>�۲���
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
		//������ײ��ȫ�����꣬�浽worldManifold����
		contact->GetWorldManifold(worldManifold);
		//��ȡ��ײ��
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
