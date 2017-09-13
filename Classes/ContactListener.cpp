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

extern Layer* Layer_UI;  //���ڻ�����ײ��Ĳ���


void ContactListener::BeginContact(b2Contact * contact)
{
	CCLOG("BeginContact");
	
	if (contact->GetFixtureA()->GetBody() == Character::getInstance()->body_man)
	{
		auto worldManifold = new b2WorldManifold();
		//������ײ��ȫ�����꣬�浽worldManifold����
		contact->GetWorldManifold(worldManifold);
		//��ȡ��ײ��
		auto contactPoint0 = worldManifold->points[0];
		auto contactPoint1 = worldManifold->points[1];
		/*CCLOG("x: %f  y: %f", contactPoint0.x * PTM_RATIO, contactPoint0.y * PTM_RATIO);
		CCLOG("x: %f  y: %f", contactPoint1.x * PTM_RATIO, contactPoint1.y * PTM_RATIO);*/

		auto sp1 = Sprite::create("PICTURE/whiteBlock.png");
		auto sp2 = Sprite::create("PICTURE/whiteBlock.png");

		sp1->setPosition(contactPoint0.x * PTM_RATIO, contactPoint0.y * PTM_RATIO);
		sp2->setPosition(contactPoint1.x * PTM_RATIO, contactPoint1.y * PTM_RATIO);

		Layer_UI->addChild(sp1);
		Layer_UI->addChild(sp2);

		if (contactPoint0.y == contactPoint1.y) //�����
		{
			if (contactPoint0.y < Character::getInstance()->sp_man->getPosition().y)
			{
				CCLOG("BOTTOM");
			}
			else
			{
				CCLOG("TOP");
			}
		}

		//auto charPos = Character::getInstance()->sp_man->getPosition();

	}
	else if (contact->GetFixtureB()->GetBody() == Character::getInstance()->body_man)
	{
		auto worldManifold = new b2WorldManifold();
		contact->GetWorldManifold(worldManifold);
		auto contactPoint0 = worldManifold->points[0];
		auto contactPoint1 = worldManifold->points[1];

		/*CCLOG("x: %f  y: %f", contactPoint0.x * PTM_RATIO, contactPoint0.y * PTM_RATIO);
		CCLOG("x: %f  y: %f", contactPoint1.x * PTM_RATIO, contactPoint1.y * PTM_RATIO);*/

		auto sp1 = Sprite::create("PICTURE/whiteBlock.png");
		auto sp2 = Sprite::create("PICTURE/whiteBlock.png");

		sp1->setPosition(contactPoint0.x * PTM_RATIO, contactPoint0.y * PTM_RATIO);
		sp2->setPosition(contactPoint1.x * PTM_RATIO, contactPoint1.y * PTM_RATIO);

		Layer_UI->addChild(sp1);
		Layer_UI->addChild(sp2);

		if (contactPoint0.y == contactPoint1.y) //�����
		{
			if (contactPoint0.y < Character::getInstance()->sp_man->getPosition().y)
			{
				CCLOG("BOTTOM");
			}
			else
			{
				CCLOG("TOP");
			}
		}
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
