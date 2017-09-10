/*
*ʱ�䣺2017��8��12��00:31:21
*���ߣ�vszed
*���ܣ���Ϸ�ܿ�
*˵����������
*/

#ifndef __CONTROLER_H__
#define __CONTROLER_H__
#include "cocos2d.h"

class Controler
{
public:

	//==============����ģʽ==============//
	static Controler* getInstance();
	~Controler();
	//==============����ģʽ==============//


	static void createCloud();

	static void cloudPosControl();

	static void CreateUpdateUI();            //����|ˢ��UI�����ʱ���

	static void tiledMapScroll(float delta, b2World *world); //��ͼ����

	static void createBackGround();          //����ͼƬ

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);   //���̰��»ص�

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);  //����̧��ص�

	static void keyBoardControler(float delta);  //���̿�������һ������ => ����֡��ʱ��

	void onAcceleration(Acceleration * acc, Event * unused_event);  //�����ƣ���ҡһҡ����ͣ��Ϸ

	static void GamePauseAndSettings();      //��Ϸ��ͣ������

	static void blurControl();     //SpriteBlur�� => ��˹ģ����ͣ��ı���

private:

	static Controler *controler;

	Controler();
};

#endif // !__CONTROLER_H__