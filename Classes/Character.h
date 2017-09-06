/*
*时间：2017-8-29 13:54:08
*作者：vszed
*功能：游戏角色类
*说明：单例设计模式
*/

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h" 
#include "Box2D\Box2D.h"
USING_NS_CC;

class Character : public Node
{
public:

	//============单例模式============//
	static Character * getInstance();
	~Character();
	//==============单例模式==========//

	void move(int speedPlus_minus, float delta);     //角色横向移动及动画(向左走传入-1，向右走传入1，停止动作传入0)

	void jump();    //角色跳动函数

	void initBeginPos(TMXTiledMap *map);   //初始化角色位置（通过瓦片地图）

	double get_speed();

	void set_speed(double newSpeed);

	Sprite3D * sp_man;  //3D精灵

	b2Body * body_man;  //3D精灵刚体


	struct CharacterStatus       //角色状态
	{
		bool MoveLeft = false;
		bool MoveRight = false;
		bool InSky = false;

	}characterStatus;



private:

	static Character * character;

	Character();

	double speed; //移速

	bool big_small; //大、小状态    初始为false => 体型：小

	int jump_height_small; //小型角色弹跳高度

	int jump_height_big;   //变大角色弹跳高度
};


#endif // !__CHARACTER_H__