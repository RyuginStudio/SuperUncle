/*
ʱ�䣺2017��7��17��10:38:23
���ߣ�������
���ܣ����Գ���ͷ�ļ�������Ϸ����
*/

#ifndef __B2TEST_SCENE_H__
#define __B2TEST_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"  //����box2d�����ͷ�ļ�

#define PTM_RATIO 62     //�궨�� => ��Ļ�ϵ�32����Ϊ1��

class b2TestScene : public cocos2d::Layer, public b2ContactListener
{

	b2World* world;  //b2World����ָ��world => ���ݳ�Ա  => ����һ��box2d������

public:

	static cocos2d::Scene * createScene();

	virtual bool init();

	virtual void update(float dt);  //֡��ʱ���ص�����

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);  //��д����layer�е�onTouchBegan���� => �����������
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);   //���̰�����̧��Ļص�
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);

	CREATE_FUNC(b2TestScene);

	void iniPhysics();    //��ʼ���������溯��
	void addNewSpriteAtPosition(cocos2d::Point p);  //��ָ����������Ӿ������

	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);

	void delete_contact_body();

	b2TestScene();
	~b2TestScene();
};

#endif // !__B2TEST_SCENE_H__
