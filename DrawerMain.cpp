/*By.Wheat*/
/*最新日期：2017年07月07日*/

//#define DEBUG
#include "MouseEvent.h"
#include "InputButton.h"
#include "Button.h"

int main()
{
	//printf("%d", StrCmp("*cossin", "tan arcsin"));
	////Function("sqrt(100-x^2)");
	////printf("%f  %f\n", Function(1,0), sqrt(10 - 1));
	//system("pause");
	//char a[ExpMAXCHAR] = { 0 };
	Expression a;
	bool angle = 0;
#ifdef DEBUG
	initgraph(1100, 650, 1);
#endif // DEBUG

#ifndef DEBUG
	initgraph(1100, 650);
#endif // !DEBUG


	//初始化界面
	LOGFONT f = { 0 ,0,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,0,ANTIALIASED_QUALITY,0,0 };
	settextstyle(&f);					//设置字体样式
	setbkcolor(RGB(77, 111, 107));
	cleardevice();

	RECT FunctionRect = { 150,10,950,610 };
	RECT InputBottonRect = { 15,50,130,530};
	RECT ExpBoxRect = { FunctionRect.left, FunctionRect.bottom + 10, FunctionRect.right, FunctionRect.bottom + 32};
	InputBottonloading(InputBottonRect);
	RECT AboutRect = { getmaxx() - 120, getmaxy()-100, getmaxx() - 10, getmaxy() };
	myIMAGE load[3] = { { 0,FunctionRect },{ 0,FunctionRect },{ 0,FunctionRect } };
	LoadmyIMAGE(&load[0], "materials\\load1.bmp");
	LoadmyIMAGE(&load[1], "materials\\load2.bmp");
	LoadmyIMAGE(&load[2], "materials\\load2.bmp");


	settextstyle(20, 0, "微软雅黑");		//设置字体样式
	settextcolor(WHITE);
	drawtext("By.Wheat\n2017.07.03", &AboutRect, DT_CENTER);
	POINT Ori = DefaultOri;																//【原点】原点坐标
	BeginBatchDraw();
	CoorSys coorSys(Ori,FunctionRect);
	buttonVec.AddButton(new Button_Base("materials\\NewNor.bmp", getmaxx() - 130, 70, getmaxx() - 10, 110, &coorSys, NewExpEvent));
	buttonVec.AddButton(new Button_Base("materials\\ResetNor.bmp", getmaxx() - 130, 120, getmaxx() - 10, 160, &coorSys, ResetCoorEvent));
	buttonVec.AddButton(new Button_Base("materials\\SavebmpNor.bmp", getmaxx() - 130, 170, getmaxx()-10, 210, &coorSys, SavebmpEvent));
	buttonVec.AddButton(new Button_Base("materials\\ClearNor.bmp", getmaxx() - 130, 220, getmaxx()-10, 260, &coorSys, CleanEvent));
	buttonVec.AddButton(new Button_Base("materials\\RectCooNor.bmp", getmaxx() - 130, 410, getmaxx()-10, 450));
	buttonVec.AddButton(new Button_Base("materials\\DynDemoNor.bmp", getmaxx() - 130, 460, getmaxx()-10, 500));
	//coorSys.Set2D();
	coorSys.HoldOn();

	//批量绘图
	MOUSEMSG msg;
	while (1)
	{
		if (!IfPaintFunction) LoadEvent(load, sizeof(load) / sizeof(load[0]));
		//if (MouseHit()/*||IfPaintFunction*/) {
			msg = GetMouseMsg();
			cout << setw(10) << '\r';
			FunctionPaintTrigger(&msg, coorSys);

			PrintDCooEventTrigger(msg, FunctionRect, coorSys, angle);


			if (FuncButton(a,InputBottonRect,msg)) RePaintExpBox=true;
		//}

		buttonVec.ButtonTrigger(msg);
		//ExpBoxPaintMonitor(ExpBoxRect, a, angle);
		FunctionPaintMonitor(coorSys);
	}

	//退出
	EndBatchDraw();

}


