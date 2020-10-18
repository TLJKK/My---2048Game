#include "card.h"
#include<stdlib.h>
#include"cocos2d.h"
#include"ui/CocosGUI.h"

card* card::_instance = nullptr;

card* card::getInstance()
{
	if (_instance)
	{
		return _instance;
	}
	else
	{
		_instance = new(std::nothrow)card;
		return _instance;
	}
}


bool card::creatNewCard(std::string des, int i, int j, int num, cocos2d::Sprite* now)// num the real num digit
{
	/*dataShow[i][j] =
		cocos2d::Label::createWithTTF(des, "fonts/Marker Felt.ttf", dataSize[num]);
	dataShow[i][j]->setColor(colorRec[num]);
	now->addChild(dataShow[i][j]);*/
	return true;
}

bool card::Minit(cocos2d::Sprite* now)
{

	auto ele = cocos2d::Sprite::create("element.png");
	auto pos = Vec2(65.1, now->getBoundingBox().size.height - 45);
	
	//auto test = cocos2d::Sprite

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			auto tmp = cocos2d::ui::Button::create("element.png");
			tmp->setScale(0.8);
			tmp->setAnchorPoint(Vec2(0, 1));
			tmp->setPosition(pos - Vec2(-tmp->getBoundingBox().size.width * j,
				tmp->getBoundingBox().size.height * i));
			int a = i, b = j;
			tmp->addClickEventListener([a, b](Ref* ref)
				{
					auto ali = card::getInstance();
					card::getInstance()->data[a][b] = 0;
					for (int i = 0; i < 4; i++)
						for (int j = 0; j < 4; j++)
							if (ali->data[i][j] > 0)
							{
								ali->dataShow[i][j]->setVisible(true);
								ali->dataShow[i][j]->setString(std::to_string(ali->data[i][j]));
								int digit = 0, love = ali->data[i][j], color = ali->data[i][j], coloridx = 0;
								while (color > 1)color /= 2, coloridx++;
								while (love)digit++, love /= 10;
								ali->dataShow[i][j]->setColor(ali->colorRec[coloridx]);
								ali->dataShow[i][j]->setScale(ali->dataSize[digit]);
							}
							else ali->dataShow[i][j]->setVisible(false);
				});


			auto dispatcher = Director::getInstance()->getEventDispatcher();

			now->addChild(tmp);
			des[i][j] = tmp;

			dataShow[i][j] = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 60);
			dataShow[i][j]->setPosition(pos - Vec2(-tmp->getBoundingBox().size.width * j,
				tmp->getBoundingBox().size.height * i) + Vec2(50, -60));
			dataShow[i][j]->setColor(colorRec[1]);
			now->addChild(dataShow[i][j]);
			dataShow[i][j]->setVisible(false);

		}
	}

	srand(time(NULL));
	int i = rand() % 4, j = rand() % 4;
	dataShow[i][j]->setString("2048");
	dataShow[i][j]->setScale(dataSize[1]);
	dataShow[i][j]->setVisible(true);
	data[i][j] = 2048;

	return true;
}

bool card::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	memset(data, 0, sizeof(data));
	return true;
}