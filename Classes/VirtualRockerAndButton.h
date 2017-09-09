/*
*���ܣ�����Ļ�ϴ�������ҡ�˺Ͱ���
*���ߣ�vszed
*ʱ�䣺2017��8��5��01:28:43
*�ص㣺������
*/

#ifndef __VIRTUAL_ROCKER_AND_BUTTON_H__
#define __VIRTUAL_ROCKER_AND_BUTTON_H__
#include "cocos2d.h"

USING_NS_CC;


class VirtualRockerAndButton
{
private:

	//==============����ģʽ==============//
	static VirtualRockerAndButton *virtualRockerAndButton;

	//���캯��Ϊ˽�У�ֻ������
	VirtualRockerAndButton();
	//==============����ģʽ==============//

public:

	~VirtualRockerAndButton();

	static VirtualRockerAndButton* getInstance(); //��ȡ��������

	//�����¼��ص�
	void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);

	static void touchMoveControl();

	//�Ƿ�����ҡ��move����
	bool admitRockerMove;

	//����id
	int btnAtouchID;
	int btnBtouchID;
	int rockerTouchID;
	

	//ҡ�˺�����·����Ϣ
	std::string circlePath = "PICTURE/VirtualCircle.png";
	std::string pointPath = "PICTURE/VirtualPoint.png";

	//��ťA��B��·����Ϣ
	std::string btnApath = "PICTURE/VirtualButtonA.png";
	std::string btnBpath = "PICTURE/VirtualButtonB.png";

};

#endif // !__VIRTUAL_ROCKER_AND_BUTTON_H__