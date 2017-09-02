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
	this->speed = 300;

	this->jump_height_small = 10;
	this->jump_height_big = 10;


	sp_man = Sprite3D::create("3DspriteAndAction/sprite_stand.c3b");  //精灵素材，配合素材文件使用

	sp_man->setScale(1.5f);     //设置放大比例

	auto ac_RotateTo = RotateTo::create(0.001f, Vec3(0, 90, 0));//设定旋转动作，操纵Sprite => 不能设置为0.f部分机型不执行

	sp_man->runAction(ac_RotateTo);


	////精灵刚体初始化
	//b2BodyDef bodydef;
	//bodydef.type = b2_dynamicBody;
	//bodydef.position.Set(Character::getInstance()->sp_man->getPosition().x / PTM_RATIO, Character::getInstance()->sp_man->getPosition().y / PTM_RATIO);

	//body_man = TestScene::getInstance()->world->CreateBody(&bodydef);
	//body_man->SetUserData(Character::getInstance()->sp_man);

	////PhysicsEditor:不规则刚体数据
	///*b2Vec2 points[27] = {
	//b2Vec2(12.00000 / PTM_RATIO,30.00000 / PTM_RATIO), b2Vec2(12.00000 / PTM_RATIO,36.00000 / PTM_RATIO),b2Vec2(20.00000 / PTM_RATIO,34.00000 / PTM_RATIO),
	//b2Vec2(12.00000 / PTM_RATIO,36.00000 / PTM_RATIO),b2Vec2(12.00000 / PTM_RATIO,22.00000 / PTM_RATIO), b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO),
	//b2Vec2(-12.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(-20.00000 / PTM_RATIO,30.00000 / PTM_RATIO),b2Vec2(-4.00000 / PTM_RATIO,43.00000 / PTM_RATIO),
	//b2Vec2(9.00000 / PTM_RATIO,46.00000 / PTM_RATIO),b2Vec2(21.00000 / PTM_RATIO,17.00000 / PTM_RATIO), b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),
	//b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(12.00000 / PTM_RATIO,22.00000 / PTM_RATIO),b2Vec2(20.00000 / PTM_RATIO,23.00000 / PTM_RATIO),
	//b2Vec2(11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO),b2Vec2(-11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO), b2Vec2(4.00000 / PTM_RATIO,-35.00000 / PTM_RATIO),
	//b2Vec2(11.00000 / PTM_RATIO,-38.00000 / PTM_RATIO), b2Vec2(-11.00000 / PTM_RATIO,-46.00000 / PTM_RATIO),b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO),
	//b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),b2Vec2(6.00000 / PTM_RATIO,-17.00000 / PTM_RATIO), b2Vec2(4.00000 / PTM_RATIO,-35.00000 / PTM_RATIO),
	//b2Vec2(6.00000 / PTM_RATIO,-17.00000 / PTM_RATIO), b2Vec2(5.00000 / PTM_RATIO,7.00000 / PTM_RATIO),b2Vec2(12.00000 / PTM_RATIO,-12.00000 / PTM_RATIO) };*/
	//b2Vec2 points[27] = {
	//	b2Vec2(20.000 / PTM_RATIO,34.000 / PTM_RATIO), b2Vec2(12.000 / PTM_RATIO,36.000 / PTM_RATIO),b2Vec2(12.000 / PTM_RATIO,30.000 / PTM_RATIO),
	//	b2Vec2(9.000 / PTM_RATIO,46.000 / PTM_RATIO),b2Vec2(-4.000 / PTM_RATIO,43.000 / PTM_RATIO), b2Vec2(-20.000 / PTM_RATIO,30.000 / PTM_RATIO),
	//	b2Vec2(-12.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(-8.000 / PTM_RATIO,9.000 / PTM_RATIO),b2Vec2(12.000 / PTM_RATIO,22.000 / PTM_RATIO),
	//	b2Vec2(12.000 / PTM_RATIO,36.000 / PTM_RATIO),

	//	b2Vec2(20.000 / PTM_RATIO,23.000 / PTM_RATIO), b2Vec2(12.000 / PTM_RATIO,22.000 / PTM_RATIO),
	//	b2Vec2(-8.00000 / PTM_RATIO,9.00000 / PTM_RATIO), b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO),b2Vec2(21.000 / PTM_RATIO,17.000 / PTM_RATIO),




	//	b2Vec2(11.00000 / PTM_RATIO,-38.000 / PTM_RATIO),b2Vec2(4.000 / PTM_RATIO,-35.000 / PTM_RATIO), b2Vec2(-11.000 / PTM_RATIO,-46.000 / PTM_RATIO),
	//	b2Vec2(11.000 / PTM_RATIO,-46.000 / PTM_RATIO),b2Vec2(4.000 / PTM_RATIO,-35.000 / PTM_RATIO), b2Vec2(6.000 / PTM_RATIO,-17.000 / PTM_RATIO),
	//	b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO),b2Vec2(-8.000 / PTM_RATIO,9.000 / PTM_RATIO),b2Vec2(-11.000 / PTM_RATIO,-46.000 / PTM_RATIO),
	//	b2Vec2(12.000 / PTM_RATIO,-12.000 / PTM_RATIO),b2Vec2(5.000 / PTM_RATIO,7.000 / PTM_RATIO), b2Vec2(6.000 / PTM_RATIO,-17.000 / PTM_RATIO) };


	//b2PolygonShape shape_sp_man;
	//shape_sp_man.Set(points, 27);

	//b2FixtureDef fixturedef;
	//fixturedef.density = 1.0f;   //密度
	//fixturedef.friction = 0.3f;  //摩擦
	//fixturedef.shape = &shape_sp_man;
	//body_man->CreateFixture(&fixturedef);








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
	sp_man->setRotation3D(Vec3(0, speedPlus_minus * 90, 0));                     //转向
	//sp_man->setPosition(Point(pos.x + speedPlus_minus * delta * speed, pos.y));  //移动
}

int Character::get_speed()
{
	return this->speed;
}
void Character::set_speed(int speed)
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

	auto ac_jump = JumpBy::create(0.5f, Point(0, 0), 40, 1);
	b2Vec2(0, 40);
}

void Character::initBeginPos(TMXTiledMap *map)  //初始化角色位置（通过瓦片地图对象）
{
	//创建一个存放对象的Group
	TMXObjectGroup* objGroup = map->getObjectGroup("objects");  //objects表示瓦片地图对象层的名字
	//加载PlayerPoint对象
	ValueMap PlayerPoint = objGroup->getObject("PlayerPoint"); //获取对象组中的PlayerPoint对象

	float posBegin_X = PlayerPoint.at("x").asFloat();  //得到对象的x，y坐标
	float posBegin_Y = PlayerPoint.at("y").asFloat();

	sp_man->setPosition(Point(posBegin_X, posBegin_Y));   //设定角色初始化的位置
}
