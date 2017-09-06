/*
*ʱ�䣺2017-8-29 13:54:08
*���ߣ�vszed
*���ܣ���Ϸ��ɫ��
*˵�����������ģʽ
*/

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h" 
#include "Box2D\Box2D.h"
USING_NS_CC;

class Character : public Node
{
public:

	//============����ģʽ============//
	static Character * getInstance();
	~Character();
	//==============����ģʽ==========//

	void move(int speedPlus_minus, float delta);     //��ɫ�����ƶ�������(�����ߴ���-1�������ߴ���1��ֹͣ��������0)

	void jump();    //��ɫ��������

	void initBeginPos(TMXTiledMap *map);   //��ʼ����ɫλ�ã�ͨ����Ƭ��ͼ��

	double get_speed();

	void set_speed(double newSpeed);

	Sprite3D * sp_man;  //3D����

	b2Body * body_man;  //3D�������


	struct CharacterStatus       //��ɫ״̬
	{
		bool MoveLeft = false;
		bool MoveRight = false;
		bool InSky = false;

	}characterStatus;



private:

	static Character * character;

	Character();

	double speed; //����

	bool big_small; //��С״̬    ��ʼΪfalse => ���ͣ�С

	int jump_height_small; //С�ͽ�ɫ�����߶�

	int jump_height_big;   //����ɫ�����߶�
};


#endif // !__CHARACTER_H__