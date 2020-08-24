/*By.Wheat*/
/*�������ڣ�2017��07��07��*/

#pragma once
#include "CoorSys.h"
#include "ExpCal.h"


typedef struct {
	IMAGE image;
	RECT rect;
}myIMAGE;
//************************************************************************
//**																	**
//**							��Ҫ��ȫ�ֱ���							**
//**																	**
//************************************************************************
bool RePaintFunction = false;					//��RePaintFunctionΪtrueʱ���ػ�һ��ͼ��
bool IfPaintFunction = false;					//��IfPaintFunctionΪfalseʱ���������ͼ�񣨿����������ѻ��Ƶ�ͼ��
bool RePaintExpBox = true;

//************************************************************************
//**																	**
//**							�Զ��庯��								**
//**																	**
//************************************************************************

//����ͼ����
void LoadmyIMAGE(myIMAGE* image, LPCTSTR path)
{
	loadimage(&image->image, path, image->rect.right - image->rect.left, image->rect.bottom - image->rect.top, 0);
}
//��ӡͼ����
void PutmyIMAGE(myIMAGE* image)
{
	putimage(image->rect.left, image->rect.top, &image->image);
}

//*********************************�������¼���*********************************

void CleanEvent(void* coorSys) {
	((CoorSys*)coorSys)->Clean();
	FlushBatchDraw();
}

//NewExp��ť�¼�
void NewExpEvent(void* coorSys)
{
	CoorSys* cs = ((CoorSys*)coorSys);
	char a[ExpMAXCHAR] = "f(";
	if (cs->GetBackExpression())
		strcat(a, cs->GetBackExpression()->GetVariableFormat());
	else
		strcat(a, "x,y");
	strcat(a, ")=");
	if (cs->GetBackExpression())
		strcat(a, cs->GetBackExpression()->GetExpression());
	if (InputBox(a, ExpMAXCHAR, "������ʽf(x)=", "������ʽ", a, 0, 0, 0)) {
		char* flag1 = strstr(a, "(");
		char* flag2 = strstr(a, ")=");
		if (flag2) {
			*flag2 = '\0';
			cs->NewExpression(flag2 + 2, a + 2);
		}
		else{
			cs->NewExpression(a, "x,y");
		}
		RePaintFunction = true;
		IfPaintFunction = true;
		RePaintExpBox = true;
	}
}
//Reset��ť�¼�
void ResetCoorEvent(void* coorSys)
{
	CoorSys* cs = ((CoorSys*)coorSys);
	cs->ResetCoor();
	RePaintFunction = true;
}
//Savebmp��ť�¼�
void SavebmpEvent(void* coorSys)
{
	IMAGE save;
	RECT FunctionRect = ((CoorSys*)coorSys)->GetPaintRect();
	getimage(&save, FunctionRect.left, 
		FunctionRect.top,
		FunctionRect.right - FunctionRect.left,
		FunctionRect.bottom - FunctionRect.top);
	char savename[ExpMAXCHAR + 7] = "";
	//if (angle) strcpy(savename, "(R)");
	//else strcpy(savename, "(P)");
	Expression* exp = ((CoorSys*)coorSys)->GetBackExpression();
	if (!exp) return;
	strcat(savename, exp->GetExpression());
	strcat(savename, ".bmp");
	FILE *stream = fopen(savename, "w");
	if (!stream) {
		itoa(clock(), savename, 10);
		strcat(savename, ".bmp");
	}
	else fclose(stream);
	saveimage(savename, &save);
}

//Load�¼�
void LoadEvent(myIMAGE load[], int frame) {
	static int i = 0;
	static clock_t t = 0, tlast = clock();
	if (t == 0) {
		PutmyIMAGE(&load[i++]);						//�״ε���������ִ��
		FlushBatchDraw();
	}
	if (((t = clock()) - tlast) >= 500) {			//500����һ֡
		tlast = t;
		PutmyIMAGE(&load[i++]);
		FlushBatchDraw();
	}
	if (i >= frame) i = 0;
}

//*********************************�������¼���*********************************



//*********************************����������*********************************

//�������Ƽ��
void FunctionPaintMonitor(CoorSys& coorSys)
{
	if (IfPaintFunction && RePaintFunction) {
		coorSys.DrawRi_angle();
		RePaintFunction = false;
		IfDynDemo = false;
	}
}

//�������ƴ�������
void FunctionPaintTrigger(MOUSEMSG* msg, CoorSys& coorSys)
{
	if (!IfPaintFunction) return;
	static POINT oldmousexy;
	POINT mousexy = { msg->x,msg->y };
	RECT FunctionRect = coorSys.GetPaintRect();
	static POINT oldOri;
	static double rotatea = 0;
	static double rotateb = 0;
	switch (msg->uMsg) {
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
		oldmousexy = mousexy;
		oldOri = coorSys.GetOri();
		return;

	case WM_MOUSEMOVE:
		if (PtInRect(&FunctionRect, oldmousexy)) {
			if (msg->mkLButton && msg->mkCtrl) {
				coorSys.SetOri(oldOri.x + mousexy.x - oldmousexy.x, oldOri.y + mousexy.y - oldmousexy.y);
				FlushMouseMsgBuffer();				//��������Ϣ���棬���Լ��ߵ����Ч��
				RePaintFunction = true;
			}
			else if (msg->mkMButton) {
				if (msg->mkCtrl) {
					rotateb = 0;
					rotatea = (mousexy.x - oldmousexy.x) / 300.0;
				}
				else if (msg->mkShift) {
					rotatea = 0;
					rotateb = (oldmousexy.y - mousexy.y) / 300.0;
				}
				else {
					rotatea = (mousexy.x - oldmousexy.x) / 300.0;
					rotateb = (oldmousexy.y - mousexy.y) / 300.0;
				}
				coorSys.RotateCoor(rotatea,rotateb);
				oldmousexy = mousexy;
				FlushMouseMsgBuffer();				//��������Ϣ���棬���Լ��ߵ����Ч��
				RePaintFunction = true;
			}
		}
		return;
	case WM_MOUSEWHEEL:
		//douPOINT oldxy = POINTCoo(*Ori, mousexy, *pro);
		if (msg->mkCtrl) coorSys.SetPro(coorSys.GetPro() + msg->wheel / 12);
		else coorSys.SetPro(coorSys.GetPro() + msg->wheel / 120);
		//douPOINT newxy = POINTCoo(*Ori, mousexy, *pro);

		//if (newxy.x < oldxy.x)	Ori->x += *pro *(newxy.x - oldxy.x)-0.5;		//�������������ԭ�����겹��
		//else Ori->x += *pro *(newxy.x - oldxy.x)+0.5;
		//if (newxy.y < oldxy.y) Ori->y -= *pro *(newxy.y - oldxy.y) - 0.5;
		//else Ori->y -= *pro *(newxy.y - oldxy.y) + 0.5;

		//oldmousexy.x = msg->x; oldmousexy.y = msg->y;							//ˢ��ԭ�㣬���������϶�ʱ�����Ŵ����϶�����ͼ��ƫ��
		//oldOri = *Ori;
		RePaintFunction = true;
	}
}
//��ӡĿ������괥������
void PrintDCooEventTrigger(MOUSEMSG msg, RECT FunctionRect, CoorSys &coorSys, bool angle)
{
	if (!IfPaintFunction || msg.mkCtrl) return;
	static POINT oldmousexy = { -1,-1 };
	POINT mousexy = { msg.x,msg.y };

	switch (msg.uMsg) {
	case WM_RBUTTONUP:
		RePaintFunction = true;
		return;
	case WM_LBUTTONDOWN:
		oldmousexy = mousexy;								//��¼��갴��ʱ������
		if (PtInRect(&FunctionRect, mousexy)) {				//ֻ�е��������Ч�����£����������¼�
			coorSys.DrawRi_angle();
			coorSys.RecordImage();
			coorSys.PrintMouseTarget(mousexy);
			//PrintDCooEvent(FunctionRect, mousexy, Ori, pro);
			RePaintFunction = false;
		}
		return;
	case WM_MOUSEMOVE:
		if (msg.mkLButton && PtInRect(&FunctionRect, oldmousexy)) {//ֻ�е���갴��ʱ������Ч���򣬲��������¼�
			coorSys.DrawRi_angle();
			coorSys.PrintMouseTarget(mousexy);
			//PrintDCooEvent(FunctionRect, mousexy, Ori, pro);
			RePaintFunction = false;
			FlushMouseMsgBuffer();				//��������Ϣ���棬���Լ��ߵ����Ч��
		}
		return;
	}
}
//*********************************����������*********************************
