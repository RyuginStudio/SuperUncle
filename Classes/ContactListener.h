/*
*时间：2017年9月12日15:56:11
*作者：vszed
*功能：碰撞处理
*说明：发生碰撞后的一系列处理
*改进：日后可能改用消息机制=>观察者
*/

#ifndef __CONTACT_LISTENER_H__
#define __CONTACT_LISTENER_H__

#include "Box2D/Box2D.h"

class ContactListener : public b2ContactListener
{
public:

	virtual void BeginContact(b2Contact* contact);

	virtual void EndContact(b2Contact* contact);

	ContactListener();
	~ContactListener();
};


#endif // !__CONTACT_LISTENER_H__