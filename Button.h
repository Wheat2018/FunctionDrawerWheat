/*By.Wheat*/
/*最新日期：2017年07月07日*/

#pragma once
#include <graphics.h>
#include <vector>
using namespace std;

class myImage {
	IMAGE image;
public:
	RECT rect;

	myImage() { ; }
	myImage(char path[], RECT& loc) { LoadmyImage(path, loc); }

	void LoadmyImage(char path[], RECT& loc);
	void PutmyImage();
};


class Button {
protected:
	RECT rect;
	void* message = nullptr;

	void (*trigge_down)(void*) = nullptr;
	void (*trigge_up)(void*) = nullptr;
	void (*trigge_click)(void*) = nullptr;

public:
	virtual void Normal() = 0;						//【状态函数】常规
	virtual void Move() = 0;						//【状态函数】悬停
	virtual void Down() = 0;						//【状态函数】按下

	void Trigge_Down() { if (trigge_down) trigge_down(message); }		//【触发函数】按下
	void Trigge_Up() { if (trigge_up) trigge_up(message); }				//【触发函数】弹起
	void Trigge_Click() { if (trigge_click) trigge_click(message); }	//【触发函数】点击

	friend class ButtonVector;
};

class ButtonVector {
	Button* buttonActiveted = nullptr;							//【被激活按钮】激活条件为鼠标在该按钮区域内
	vector<Button*> buttonVec;

public:
	void AddButton(Button* button) { button->Normal(); buttonVec.push_back(button); }
	void ButtonTrigger(const MOUSEMSG& msg);
}buttonVec;

class Button_Base :public Button {
#define BUTTON_STATUS_NORMAL 0
#define BUTTON_STATUS_MOVE 1
#define BUTTON_STATUS_DOWN 2
	int status= -1;									//按钮当前状态
	myImage ButtonRes[3];							//按钮的视觉资源，按照Normal,Move,Down的顺序


public:
	Button_Base(char NormalPath[],
		int left, int top, int right, int bottom,
		void* message,
		void(*trigge_click)(void*),
		void(*trigge_down)(void*),
		void(*trigge_up)(void*));

	virtual void Normal();							//【状态函数】常规
	virtual void Move();							//【状态函数】悬停
	virtual void Down();							//【状态函数】按下

};






void myImage::LoadmyImage(char path[], RECT& loc ) {
	rect = loc;
	loadimage(&image, path, rect.right - rect.left, rect.bottom - rect.top, 0);
}

inline void myImage::PutmyImage() {
	putimage(rect.left, rect.top, &image);
}



inline Button_Base::Button_Base(char NormalPath[], 
	int left, int top, int right, int bottom, 
	void * message = nullptr, 
	void(*trigge_click)(void *) = nullptr,
	void(*trigge_down)(void *) = nullptr,
	void(*trigge_up)(void *) = nullptr
	)
{
	this->message = message;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;
	this->trigge_down = trigge_down;
	this->trigge_up = trigge_up;
	this->trigge_click = trigge_click;

	char path[100];
	strcpy(path, NormalPath);

	char *p;
	if (!(p = strstr(path, "Nor"))) return;

	ButtonRes[0].LoadmyImage(path, rect);
	*p = 'M'; *(p + 1) = 'o'; *(p + 2) = 'v';
	ButtonRes[1].LoadmyImage(path, rect);
	*p = 'P'; *(p + 1) = 'u'; *(p + 2) = 's';
	ButtonRes[2].LoadmyImage(path, rect);
}

inline void Button_Base::Normal() {
	if (status == BUTTON_STATUS_NORMAL)
		return;

	status = BUTTON_STATUS_NORMAL;
	ButtonRes[status].PutmyImage();
	FlushBatchDraw();
}

inline void Button_Base::Move() {
	if (status == BUTTON_STATUS_MOVE)
		return;

	status = BUTTON_STATUS_MOVE;
	ButtonRes[status].PutmyImage();
	FlushBatchDraw();
}

inline void Button_Base::Down() {
	if (status == BUTTON_STATUS_DOWN)
		return;

	status = BUTTON_STATUS_DOWN;
	ButtonRes[status].PutmyImage();
	FlushBatchDraw();
}

void ButtonVector::ButtonTrigger(const MOUSEMSG& msg) {
	static POINT oldmousexy = { -1,-1 };
	POINT mousexy = { msg.x,msg.y };
	if (msg.uMsg == WM_LBUTTONDOWN) oldmousexy = mousexy;				//记录鼠标按下时的坐标

	if (!msg.mkLButton && buttonActiveted && !PtInRect(&buttonActiveted->rect, mousexy)) {//鼠标离开被激活按钮
		buttonActiveted->Normal();										//常规状态
		buttonActiveted = nullptr;
	}

	for (auto i : buttonVec) {
		if (PtInRect(&i->rect, mousexy)) {								//鼠标在该按钮区域

			if (msg.mkLButton && PtInRect(&i->rect, oldmousexy)) {		//按下状态——按下时鼠标在区域内，才是按下状态
				buttonActiveted = i;									//按钮激活
				i->Down();
				if (msg.uMsg == WM_LBUTTONDOWN)	
					i->Trigge_Down();									//——触发按下
			}
			else if (!msg.mkLButton){									//悬停状态
				buttonActiveted = i;									//按钮激活
				i->Move();
				if (msg.uMsg == WM_LBUTTONUP) {
					i->Trigge_Up();										//——触发弹起
					if (PtInRect(&i->rect, oldmousexy))					//——按下时鼠标在区域内，触发点击
						i->Trigge_Click();
				}
			}
			break;
		}
	}

	/*注：以上两段语句块作用分别为：1.判断按钮失焦、2.判断按钮激活。顺序不可颠倒，因为：
			·如果进入这个函数时，A是激活的，按钮A被语句块2改为激活状态，这个函数周期
			  内，A不可能失焦，1放在2后面无意义；
			·但是如果进入这个函数时，A是失焦的，若2在1前面，语句块2当中有按钮B被激
			  活，那么buttonActiveted变量被覆盖，A无法被更改为常规状态。
		但是理解代码的逻辑应该是，先清楚语句块2再看语句块1。
	*/
}

