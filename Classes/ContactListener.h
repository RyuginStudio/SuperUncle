/*
*ʱ�䣺2017��9��12��15:56:11
*���ߣ�vszed
*���ܣ���ײ����
*˵����������ײ���һϵ�д���
*�Ľ����պ���ܸ�����Ϣ����=>�۲���
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