#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h" 
USING_NS_CC;

class Character : public Node                       //继承Node：Node类中有些关于精灵的属性和方法已经写好，可以直接继承过来
{
public:
	Character();
	~Character();

	void move(int speedPlus_minus);     //角色横向移动及动画(向左走传入-1，向右走传入1，停止动作传入0)
	void moveAnimtionCallback();        //移动动画播放完的回调

	void jump();     //角色跳动函数
	void initBeginPos(TMXTiledMap *map);   //初始化角色位置（通过瓦片地图）

	
	int get_speed();	
	
	Sprite3D * sp_man;  //3D精灵


private:
	int hp;  //血量
	int sp;  //蓝量
	int speed; //移速
	
	bool big_small; //大、小状态    初始为false => 体型：小

	int jump_height_small; //小型角色弹跳高度
	int jump_height_big;   //变大角色弹跳高度
};


#endif // !__CHARACTER_H__