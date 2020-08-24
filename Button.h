/*By.Wheat*/
/*�������ڣ�2017��07��07��*/

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
	virtual void Normal() = 0;						//��״̬����������
	virtual void Move() = 0;						//��״̬��������ͣ
	virtual void Down() = 0;						//��״̬����������

	void Trigge_Down() { if (trigge_down) trigge_down(message); }		//����������������
	void Trigge_Up() { if (trigge_up) trigge_up(message); }				//����������������
	void Trigge_Click() { if (trigge_click) trigge_click(message); }	//���������������

	friend class ButtonVector;
};

class ButtonVector {
	Button* buttonActiveted = nullptr;							//�������ť����������Ϊ����ڸð�ť������
	vector<Button*> buttonVec;

public:
	void AddButton(Button* button) { button->Normal(); buttonVec.push_back(button); }
	void ButtonTrigger(const MOUSEMSG& msg);
}buttonVec;

class Button_Base :public Button {
#define BUTTON_STATUS_NORMAL 0
#define BUTTON_STATUS_MOVE 1
#define BUTTON_STATUS_DOWN 2
	int status= -1;									//��ť��ǰ״̬
	myImage ButtonRes[3];							//��ť���Ӿ���Դ������Normal,Move,Down��˳��


public:
	Button_Base(char NormalPath[],
		int left, int top, int right, int bottom,
		void* message,
		void(*trigge_click)(void*),
		void(*trigge_down)(void*),
		void(*trigge_up)(void*));

	virtual void Normal();							//��״̬����������
	virtual void Move();							//��״̬��������ͣ
	virtual void Down();							//��״̬����������

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
	if (msg.uMsg == WM_LBUTTONDOWN) oldmousexy = mousexy;				//��¼��갴��ʱ������

	if (!msg.mkLButton && buttonActiveted && !PtInRect(&buttonActiveted->rect, mousexy)) {//����뿪�����ť
		buttonActiveted->Normal();										//����״̬
		buttonActiveted = nullptr;
	}

	for (auto i : buttonVec) {
		if (PtInRect(&i->rect, mousexy)) {								//����ڸð�ť����

			if (msg.mkLButton && PtInRect(&i->rect, oldmousexy)) {		//����״̬��������ʱ����������ڣ����ǰ���״̬
				buttonActiveted = i;									//��ť����
				i->Down();
				if (msg.uMsg == WM_LBUTTONDOWN)	
					i->Trigge_Down();									//������������
			}
			else if (!msg.mkLButton){									//��ͣ״̬
				buttonActiveted = i;									//��ť����
				i->Move();
				if (msg.uMsg == WM_LBUTTONUP) {
					i->Trigge_Up();										//������������
					if (PtInRect(&i->rect, oldmousexy))					//��������ʱ����������ڣ��������
						i->Trigge_Click();
				}
			}
			break;
		}
	}

	/*ע�����������������÷ֱ�Ϊ��1.�жϰ�ťʧ����2.�жϰ�ť���˳�򲻿ɵߵ�����Ϊ��
			����������������ʱ��A�Ǽ���ģ���ťA������2��Ϊ����״̬�������������
			  �ڣ�A������ʧ����1����2���������壻
			��������������������ʱ��A��ʧ���ģ���2��1ǰ�棬����2�����а�ťB����
			  ���ôbuttonActiveted���������ǣ�A�޷�������Ϊ����״̬��
		������������߼�Ӧ���ǣ����������2�ٿ�����1��
	*/
}

