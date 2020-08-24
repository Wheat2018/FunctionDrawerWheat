/*By.Wheat*/
/*最新日期：2017年07月07日*/

#pragma once
#include "CoorSys.h"
#include "ExpCal.h"


typedef struct {
	IMAGE image;
	RECT rect;
}myIMAGE;
//************************************************************************
//**																	**
//**							重要的全局变量							**
//**																	**
//************************************************************************
bool RePaintFunction = false;					//当RePaintFunction为true时会重绘一次图像
bool IfPaintFunction = false;					//当IfPaintFunction为false时不允许绘制图像（可用来定格已绘制的图像）
bool RePaintExpBox = true;

//************************************************************************
//**																	**
//**							自定义函数								**
//**																	**
//************************************************************************

//载入图像函数
void LoadmyIMAGE(myIMAGE* image, LPCTSTR path)
{
	loadimage(&image->image, path, image->rect.right - image->rect.left, image->rect.bottom - image->rect.top, 0);
}
//打印图像函数
void PutmyIMAGE(myIMAGE* image)
{
	putimage(image->rect.left, image->rect.top, &image->image);
}

//*********************************↓触发事件↓*********************************

void CleanEvent(void* coorSys) {
	((CoorSys*)coorSys)->Clean();
	FlushBatchDraw();
}

//NewExp按钮事件
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
	if (InputBox(a, ExpMAXCHAR, "输入表达式f(x)=", "输入表达式", a, 0, 0, 0)) {
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
//Reset按钮事件
void ResetCoorEvent(void* coorSys)
{
	CoorSys* cs = ((CoorSys*)coorSys);
	cs->ResetCoor();
	RePaintFunction = true;
}
//Savebmp按钮事件
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

//Load事件
void LoadEvent(myIMAGE load[], int frame) {
	static int i = 0;
	static clock_t t = 0, tlast = clock();
	if (t == 0) {
		PutmyIMAGE(&load[i++]);						//首次调用无条件执行
		FlushBatchDraw();
	}
	if (((t = clock()) - tlast) >= 500) {			//500毫秒一帧
		tlast = t;
		PutmyIMAGE(&load[i++]);
		FlushBatchDraw();
	}
	if (i >= frame) i = 0;
}

//*********************************↑触发事件↑*********************************



//*********************************↓触发器↓*********************************

//函数绘制监控
void FunctionPaintMonitor(CoorSys& coorSys)
{
	if (IfPaintFunction && RePaintFunction) {
		coorSys.DrawRi_angle();
		RePaintFunction = false;
		IfDynDemo = false;
	}
}

//函数绘制触发函数
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
				FlushMouseMsgBuffer();				//清空鼠标信息缓存，可以极高地提高效率
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
				FlushMouseMsgBuffer();				//清空鼠标信息缓存，可以极高地提高效率
				RePaintFunction = true;
			}
		}
		return;
	case WM_MOUSEWHEEL:
		//douPOINT oldxy = POINTCoo(*Ori, mousexy, *pro);
		if (msg->mkCtrl) coorSys.SetPro(coorSys.GetPro() + msg->wheel / 12);
		else coorSys.SetPro(coorSys.GetPro() + msg->wheel / 120);
		//douPOINT newxy = POINTCoo(*Ori, mousexy, *pro);

		//if (newxy.x < oldxy.x)	Ori->x += *pro *(newxy.x - oldxy.x)-0.5;		//基于四舍五入的原点坐标补偿
		//else Ori->x += *pro *(newxy.x - oldxy.x)+0.5;
		//if (newxy.y < oldxy.y) Ori->y -= *pro *(newxy.y - oldxy.y) - 0.5;
		//else Ori->y -= *pro *(newxy.y - oldxy.y) + 0.5;

		//oldmousexy.x = msg->x; oldmousexy.y = msg->y;							//刷新原点，避免正在拖动时滚动放大，再拖动发生图像偏移
		//oldOri = *Ori;
		RePaintFunction = true;
	}
}
//打印目标点坐标触发函数
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
		oldmousexy = mousexy;								//记录鼠标按下时的坐标
		if (PtInRect(&FunctionRect, mousexy)) {				//只有当鼠标在有效区域按下，才允许触发事件
			coorSys.DrawRi_angle();
			coorSys.RecordImage();
			coorSys.PrintMouseTarget(mousexy);
			//PrintDCooEvent(FunctionRect, mousexy, Ori, pro);
			RePaintFunction = false;
		}
		return;
	case WM_MOUSEMOVE:
		if (msg.mkLButton && PtInRect(&FunctionRect, oldmousexy)) {//只有当鼠标按下时是在有效区域，才允许触发事件
			coorSys.DrawRi_angle();
			coorSys.PrintMouseTarget(mousexy);
			//PrintDCooEvent(FunctionRect, mousexy, Ori, pro);
			RePaintFunction = false;
			FlushMouseMsgBuffer();				//清空鼠标信息缓存，可以极高地提高效率
		}
		return;
	}
}
//*********************************↑触发器↑*********************************
