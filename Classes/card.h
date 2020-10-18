#pragma once
#include<cocos2d.h>
#include"ui/CocosGUI.h"
using namespace cocos2d;

class card :public cocos2d::Sprite
{
public:
	bool Minit(cocos2d::Sprite* now);
	bool creatNewCard(std::string des, int i, int j, int num, cocos2d::Sprite* now);
	virtual bool init();
	CREATE_FUNC(card);
	cocos2d::ui::Button* des[4][4];
	cocos2d::Label* dataShow[4][4];//60
	float dataSize[5] = { 0.0,1.0,50.0 / 60.0,40.0 / 60.0,0.5 };
	int data[4][4];
	cocos2d::Color3B colorRec[13] = { 
		Color3B::BLACK,
		Color3B::BLACK,
		Color3B::BLUE,
		Color3B::Color3B(255,0,255),
		Color3B::Color3B(0,255,85),
		Color3B::Color3B(255,255,0),
		Color3B::Color3B(255,0,255),
		Color3B::Color3B(255,0,140),
		Color3B::Color3B(44,197,5),
		Color3B::Color3B(20,17,38),
		Color3B::Color3B(17,38,24),
		Color3B::Color3B(236,15,12),
		Color3B::RED };
	static card* getInstance();
private:
	static card* _instance;
};

