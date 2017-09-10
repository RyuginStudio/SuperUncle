/*
ʱ�䣺2017��9��11��00:47:10
���ߣ�nono_o
�޸ģ�������
���ܣ��Ѿ����ͼƬ����ģ������
˼·��
1.Ϊ��Ҫ����ģ������ľ����ͼƬ��ͼ
2.���ùٷ��Դ���ģ��shader(��ɫ��)�Խ�ͼ����ģ������
3.��ģ������ľ����ͼƬ����һ����ʱ������Ϊ���ϲ�(�ٱ���)
CSDN��http://blog.csdn.net/nono_o/article/details/45891757
*/

#ifndef __SPRITE_BLUR_H__
#define __SPRITE_BLUR_H__

#include "cocos2d.h"

class SpriteBlur : public cocos2d::Sprite
{
public:

	SpriteBlur();
	~SpriteBlur();

	static SpriteBlur* create(cocos2d::Sprite* pSprite, const float fRadius = 8.0f, const float fSampleNum = 8.0f);
	bool initWithTexture(cocos2d::Texture2D* pTexture, const cocos2d::Rect&  rRect);
	void initGLProgram();

	void setBlurRadius(float fRadius);
	void setBlurSampleNum(float fSampleNum);

	static cocos2d::RenderTexture* ScreenShot(const bool bIsSave, std::function<void(cocos2d::RenderTexture*, const std::string&)> pFuncCallback);
	static cocos2d::RenderTexture* SpriteBlurer(cocos2d::Sprite* pSprite, const float fRadius = 8.0f, const float fSampleNum = 8.0f);

protected:
	float fBlurRadius_;
	float fBlurSampleNum_;
};

#endif // !__SPRITE_BLUR_H__