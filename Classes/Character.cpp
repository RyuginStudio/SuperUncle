/*
*时间：2017-8-29 13:54:08
*作者：vszed
*功能：游戏角色类
*说明：单例设计模式
*/

#include "Character.h"
#include "TestScene.h"
#include "Box2D\Box2D.h"

bool moveAnimtionFinished = true;       //移动动画播放完成
bool isSporting = false;                //角色是否处于运动中


//=================单例模式=================//
Character *Character::character = nullptr;

Character* Character::getInstance()
{
	if (character == nullptr)
		character = new Character();

	return character;
}

Character::Character()
{
	this->speed = 5;

	this->jump_height_small = 10;
	this->jump_height_big = 10;


	sp_man = Sprite3D::create("3DspriteAndAction/sprite_stand.c3b");  //精灵素材，配合素材文件使用

	sp_man->setScale(1.5f);     //设置放大比例

	auto ac_RotateTo = RotateTo::create(0.001f, Vec3(0, 90, 0));//设定旋转动作，操纵Sprite => 不能设置为0.f部分机型不执行

	sp_man->runAction(ac_RotateTo);

}

Character::~Character()
{
	CC_SAFE_DELETE(character);
}
//=================单例模式=================//


void Character::move(int speedPlus_minus, float delta) //角色横向移动及动画
{
	auto animation_bone = Animation3D::create("3DspriteAndAction/run_to_stand_final.c3b");    //获取骨骼信息
	auto animation_run = Animate3D::create(animation_bone, 0.f, 0.5f);                        //3D跑动动画
	auto ac_repeatForever = RepeatForever::create(animation_run);                             //封装跑动动画永动
	auto animation_recover = Animate3D::create(animation_bone, 0.5f, 0.68f);                  //3D恢复站立动画


	if (speedPlus_minus == 0) //执行跑步恢复至站立的动画
	{
		if (isSporting == false)  //角色是否处于运动中
			return;

		sp_man->stopAllActions();

		isSporting = false;            //角色不在运动中
		moveAnimtionFinished = true;   //角色移动动画结束

		animation_recover->setSpeed(100.0f);
		sp_man->runAction(animation_recover);

		return;
	}

	//防止动画附加
	if (moveAnimtionFinished == true)
	{
		moveAnimtionFinished = false;
		sp_man->runAction(ac_repeatForever);
	}

	auto pos = sp_man->getPosition();
	sp_man->setRotation3D(Vec3(0, speedPlus_minus * 90, 0));  //转向

	body_man->SetTransform(b2Vec2(body_man->GetPosition().x + speedPlus_minus * speed / PTM_RATIO, body_man->GetPosition().y), delta);  //移动

}

double Character::get_speed()
{
	return this->speed;
}
void Character::set_speed(double speed)
{
	this->speed = speed;
}

void Character::jump()
{
	//if (!characterStatus.InSky)  //判断角色状态
	//{
	//	characterStatus.InSky = true;

	//	auto ac_jump = JumpBy::create(0.5f, Point(0, 0), 40, 1);
	//	sp_man->runAction(ac_jump);
	//}
	characterStatus.InSky = true;

	body_man->SetTransform(b2Vec2(body_man->GetPosition().x, body_man->GetPosition().y + 180 / PTM_RATIO), 0);
}

void Character::initBeginPos(TMXTiledMap *map)  //初始化角色位置（通过瓦片地图对象）
{
	//创建一个存放对象的Group
	TMXObjectGroup* objGroup = map->getObjectGroup("character");  //character对象层
	//加载PlayerPoint对象
	ValueMap PlayerPoint = objGroup->getObject("PlayerPoint"); //获取对象组中的PlayerPoint对象

	float posBegin_X = PlayerPoint.at("x").asFloat();  //得到对象的x，y坐标
	float posBegin_Y = PlayerPoint.at("y").asFloat();

	sp_man->setPosition(Point(posBegin_X, posBegin_Y));   //设定角色初始化的位置
}