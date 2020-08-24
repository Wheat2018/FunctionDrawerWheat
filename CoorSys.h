/*By.Wheat*/
/*最新日期：2017年07月07日*/


#pragma once
#include "ExpCal.h"

#include <float.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <iomanip>

#include <graphics.h>
//#include <conio.h>

#define TEXT_TYPE_LEVELMID (0x01)					//居中打印
#define TEXT_TYPE_LEVELLEFT (0x01<<1)				//居中左靠
#define TEXT_TYPE_VERTICALMID (0x01<<2)				//垂直居中
#define TEXT_TYPE_VERTICALTOP (0x01<<3)				//垂直上靠


#define SCAPROCOUNTS 20				//标尺数

#define ScaPlaLENTH 7.0				//标尺刻度线长度
bool IfDynDemo = false;


struct douPOINT {
	double x, y;
	douPOINT() { ; }
	douPOINT(int x, int y) { this->x = x; this->y = y; }
	douPOINT(double x, double y) { this->x = x; this->y = y; }
	douPOINT(POINT& point) { x = point.x; y = point.y; }

	double Module() { return sqrt(x*x + y*y); }
	double SquareModule() { return x*x + y*y; }

	friend douPOINT operator* (douPOINT left, int right) { return douPOINT(right * left.x, right * left.y); }
	friend douPOINT operator* (double left, douPOINT right) { return douPOINT(left* right.x, left * right.y); }
	friend douPOINT operator/ (douPOINT left, double right) { return douPOINT(left.x / right, left.y / right); }
	friend double operator* (douPOINT left, douPOINT right) { return left.x * right.x + left.y * right.y; }
	friend douPOINT operator+ (douPOINT left, douPOINT right) { return douPOINT(left.x + right.x, left.y + right.y); }
	friend douPOINT operator- (douPOINT left, douPOINT right) { return douPOINT(left.x - right.x, left.y - right.y); }
	friend douPOINT operator- (douPOINT right) { return douPOINT(-right.x, -right.y); }
	friend bool operator== (douPOINT left, double right) { return (left.x == left.y&& left.y == right) ? true : false; }
	friend bool operator< (douPOINT left, double right) { return (left.Module() < right) ? true : false; }
};

struct douPOINT3D {
	double x, y, z;
	douPOINT3D() { ; }
	douPOINT3D(double x, double y, double z) { this->x = x, this->y = y, this->z = z; }
	void Unitizate() { double mod = sqrt(x*x + y*y + z*z);	x /= mod, y /= mod, z /= mod; }
	friend douPOINT3D operator* (douPOINT3D left, int right) { return douPOINT3D(right * left.x, right * left.y, right * left.z); }
	friend douPOINT3D operator* (double left, douPOINT3D right) { return douPOINT3D(left* right.x, left * right.y, left * right.z); }
	friend douPOINT3D operator+ (douPOINT3D left, douPOINT3D right) { return douPOINT3D(left.x + right.x, left.y + right.y, left.z + right.z); }
};

class CoorSys {
private:
	//---------属性信息-----------
	const douPOINT Ex0 = { 0,0 }, Ey0 = { 1,0 }, Ez0 = { 0,-1 };						//内置X Y Z轴投影向量
	douPOINT3D X0, Y0, Z0;;																//原始X Y Z轴算术单位向量
	const POINT ori0;
	douPOINT3D light1, light2;	//主光源和次光源
	DWORD* const pMem = GetImageBuffer();
	RECT screen;
	RECT paint;																			//画布区域
	int width, height;																	//画布长宽
	
	//---------坐标信息-----------
	douPOINT3D X = X0, Y = Y0, Z = Z0;													//当前X Y Z轴算术单位向量
	douPOINT Ex, Ey, Ez;																//当前X Y Z轴投影向量
	POINT ori;																			//【原点】原点坐标
	int pro = DefaultPro;																//【比例】pro个像素点表示一个单位长度
	bool holdOn = false;
	struct {
		double minValue, maxValue;														//坐标轴区间
		double step;																	//描点步长
	}Xmes, Ymes, Zmes;
	double *depth = nullptr;

	//---------表达式-------------
	vector<Expression> expVec;
	
	//---------提速操作-----------
	DWORD* paintImage;
	bool recorded = false;

	void RefreshExyz();																	//刷新投影向量
	void pixel(int x, int y, COLORREF color);											//描点
	void Line_Bresenham(int xa, int ya, int xb, int yb, bool dash);						//画线
	void LineMath(douPOINT3D a, douPOINT3D b, bool dash, bool indepth, bool lineWithoutRecord);
	void LinePS(POINT point, double slope);											//画点斜式直线
	void Line_ColorToColor(int xa, int ya, COLORREF color1, double depth1, int xb, int yb, COLORREF color2, double depth2, bool fillSw, bool indepth);
	void PrintTriangle(douPOINT3D A, COLORREF color1, douPOINT3D B, COLORREF color2, douPOINT3D C, COLORREF color3, douPOINT3D D, COLORREF color4);

	//画标尺相关
	bool PrintText(const char text[], POINT location, byte type, bool record);		//在屏幕上打印字符
	bool IsRECTOverlap(RECT & rect1, RECT & rect2);										//判断两个矩形是否有重叠部分
	static int DePlace(float* digit);													//计算小数的数量级，并截断剩余数据，保证仅有一个有效数字,这个操作是为了防止标尺数据累加，但有小数截断过程，避免这个过程产生过大误差）	
	static void CutTail(char digit[], int deplace);										//将储存了小数的字符数组保留deplace位小数

	//绘制图像相关
	void DrawSca();
	void DrawScaText(douPOINT e, douPOINT3D E, douPOINT3D offset);
	void DrawName();
	void CleanScreen();
	void SetCoorMessage();
	void ResetDepth();
	bool CheckDepth(int x, int y, double dep);
	bool CheckDepth(POINT loc, double dep) { return CheckDepth(loc.x, loc.y, dep); }
	bool SetDepth(int x, int y, double dep);
	bool SetDepth(POINT loc, double dep) { return SetDepth(loc.x, loc.y, dep); }
	double GetLightInensity(douPOINT3D direction);
	DWORD SolveColor(COLORREF c1, COLORREF c2, double rate);
	POINT PrintPoint(double mathX, double mathY, double mathZ, COLORREF color);
	POINT PrintPoint(douPOINT3D mathPoint, COLORREF color) { return PrintPoint(mathPoint.x, mathPoint.y, mathPoint.z, color); }

public:
	CoorSys(POINT Ori, RECT& paint);
	void RotateCoor(double a, double b);
	void ResetCoor() { X = X0, Y = Y0, Z = Z0; pro = DefaultPro; ori = ori0; RefreshExyz(); }
	void Clean() { expVec.clear(); recorded = false; DrawRi_angle(); }
	void Set2D();
	void Set3D();
	void HoldOn() { holdOn = true; }
	void HoldOff() { holdOn= false; }
	void SetPro(int pro) { if (pro >= 1) this->pro = pro; else this->pro = 1; SetCoorMessage(); }
	void SetOri(POINT ori) { this->ori = ori; SetCoorMessage(); }
	void SetOri(int x, int y) { ori.x = x; ori.y = y; SetCoorMessage(); }

	POINT GetPrintPoint(double mathX, double mathY, double mathZ);
	POINT GetPrintPoint(douPOINT3D mathPoint) { return GetPrintPoint(mathPoint.x, mathPoint.y, mathPoint.z); }
	douPOINT GetMathXY(POINT target);
	int GetPro() { return pro; }
	POINT GetOri() { return ori; }
	Expression* GetBackExpression() { if (!expVec.empty()) return &expVec.back(); return nullptr; }
	RECT GetPaintRect() { return paint; }

	void NewExpression(char exp[], char variableFormat[]);
	void PrintMouseTarget(POINT mousexy);
	void DrawRi_angle();
	void RecordImage();

};



inline void CoorSys::RefreshExyz() {
	//生成投影向量
	Ex.x = Ex0.x * X.x + Ey0.x*X.y + Ez0.x*X.z;
	Ex.y = Ex0.y * X.x + Ey0.y*X.y + Ez0.y*X.z;
	Ey.x = Ex0.x * Y.x + Ey0.x*Y.y + Ez0.x*Y.z;
	Ey.y = Ex0.y * Y.x + Ey0.y*Y.y + Ez0.y*Y.z;
	Ez.x = Ex0.x * Z.x + Ey0.x*Z.y + Ez0.x*Z.z;
	Ez.y = Ex0.y * Z.x + Ey0.y*Z.y + Ez0.y*Z.z;
	SetCoorMessage();
}

inline void CoorSys::pixel(int x, int y, COLORREF color) {
	if (y >= 0 && y < screen.bottom && x >= 0 && x < screen.right)
		if (x >= paint.left&&x < paint.right && y >= paint.top&&y < paint.bottom)
			pMem[y*screen.right + x] = BGR(color);
}

inline void CoorSys::Line_Bresenham(int xa, int ya, int xb, int yb, bool dash = false) {
	//预判断
	//Step1.线段不经过显示区时退出
	if ((xa < paint.left && xb < paint.left || xa >= paint.right && xb >= paint.right) ||
		(ya < paint.top && yb < paint.top || ya >= paint.bottom && yb >= paint.bottom)) return;
	//Step2.排除不经过显示区线段
	if (xa == xb) {									//斜率不存在
		if (ya < paint.top) ya = paint.top;
		else if (ya >= paint.bottom) ya = paint.bottom;
		if (yb < paint.top) yb = paint.top;
		else if (yb >= paint.bottom) yb = paint.bottom;
	}

	COLORREF color = getlinecolor();
	float delta_x, delta_y, x, y;
	int dx, dy, steps;
	dx = xb - xa;
	dy = yb - ya;
	steps = max(abs(dx), abs(dy));

	if (steps > 10000) steps = 10000;
	delta_x = (float)dx / steps;
	delta_y = (float)dy / steps;
	x = xa;
	y = ya;
	pixel(x, y, color);
	for (int k = 1; k <= steps; k++) {
		x += delta_x;
		y += delta_y;
		if (dash && k % 5 == 0) {
			continue;
		}
		pixel(x, y, color);
		if ((x < paint.left && delta_x <= 0 || x >= paint.right && delta_x >= 0) ||
			(y < paint.top && delta_y <= 0 || y >= paint.bottom && delta_y >= 0)) return;
	}

}


inline void CoorSys::LineMath(douPOINT3D a, douPOINT3D b, bool dash = false, bool indepth = false, bool lineWithoutRecord = false) {
#define GETDEPTH(A) X.x*(A).x + Y.x*(A).y + Z.x*(A).z
#define SOLVE_DEPTH(d1,d2,x) (d1) * (1 - (x)) + d2 * (x)

	POINT ap = GetPrintPoint(a);
	POINT bp = GetPrintPoint(b);
	int xa = ap.x, ya = ap.y, xb = bp.x, yb = bp.y;
	double depthA = GETDEPTH(a), depthB = GETDEPTH(b);

	//预判断
	//Step1.线段不经过显示区时退出
	if ((xa < paint.left && xb < paint.left || xa >= paint.right && xb >= paint.right) ||
		(ya < paint.top && yb < paint.top || ya >= paint.bottom && yb >= paint.bottom)) return;
	//Step2.排除不经过显示区线段
	if (xa == xb) {									//斜率不存在
		if (ya < paint.top) ya = paint.top;
		else if (ya >= paint.bottom) ya = paint.bottom;
		if (yb < paint.top) yb = paint.top;
		else if (yb >= paint.bottom) yb = paint.bottom;
	}

	COLORREF color = getlinecolor();
	float delta_x, delta_y, x, y;
	int dx, dy, steps;
	dx = xb - xa;
	dy = yb - ya;
	steps = max(abs(dx), abs(dy));

	if (steps > 10000) steps = 10000;
	delta_x = (float)dx / steps;
	delta_y = (float)dy / steps;
	x = xa;
	y = ya;
	if (!indepth || (lineWithoutRecord && CheckDepth(x, y, depthA)) || SetDepth(x, y, depthA))
		pixel(x, y, color);
	for (int k = 1; k <= steps; k++) {
		x += delta_x;
		y += delta_y;
		if (dash && k % 5 == 0) {
			continue;
		}
		if (!indepth || (lineWithoutRecord && CheckDepth(x, y, SOLVE_DEPTH(depthA, depthB, (double)k / steps)))
			|| SetDepth(x, y, SOLVE_DEPTH(depthA, depthB, (double)k / steps)))
			pixel(x, y, color);
		if ((x < paint.left && delta_x <= 0 || x >= paint.right && delta_x >= 0) ||
			(y < paint.top && delta_y <= 0 || y >= paint.bottom && delta_y >= 0)) return;
	}
}

inline void CoorSys::LinePS(POINT point, double slope) {
	int x1, x2, y1, y2;
	if (isinf(slope)) {
		x1 = x2 = point.x;
		y1 = paint.top;
		y2 = paint.bottom;
	}
	else if (slope <= 1 && slope >= -1) {
		x1 = paint.left;
		x2 = paint.right;
		y1 = point.y + (x1 - point.x)*slope;
		y2 = point.y + (x2 - point.x)*slope;
	}
	else {
		y1 = paint.top;
		y2 = paint.bottom;
		x1 = point.x + (y1 - point.y) / slope;
		x2 = point.x + (y2 - point.y) / slope;
	}

	Line_Bresenham(x1, y1, x2, y2);
}

inline void CoorSys::Line_ColorToColor(int xa, int ya, COLORREF color1,double depth1, int xb, int yb, COLORREF color2, double depth2, bool fillSw = false, bool indepth = false) {
#define SOLVE_R(c1,c2,x) (DWORD(int((c2 & 0xff) * (x)) + int((c1 & 0xff) * (1 - (x)))))
#define SOLVE_G(c1,c2,x) (DWORD(int(((c2 & 0xff00) >> 8) * (x))+ int(((c1 & 0xff00) >> 8) * (1 - (x)))) << 8)
#define SOLVE_B(c1,c2,x) (DWORD(int(((c2 & 0xff0000) >>16)* (x)) + int(((c1 & 0xff0000) >> 16) * (1 - (x)))) << 16)
#define SOLVE_COLOR(color1,color2,x) DWORD(SOLVE_R(color1, color2, x) | SOLVE_G(color1, color2, x) | SOLVE_B(color1, color2, x))
	//预判断
	//Step1.线段不经过显示区时退出
	if ((xa < paint.left && xb < paint.left || xa >= paint.right && xb >= paint.right) ||
		(ya < paint.top && yb < paint.top || ya >= paint.bottom && yb >= paint.bottom)) return;
	//Step2.排除不经过显示区线段
	if (xa == xb) {									//斜率不存在
		if (ya < paint.top) ya = paint.top;
		else if (ya >= paint.bottom) ya = paint.bottom;
		if (yb < paint.top) yb = paint.top;
		else if (yb >= paint.bottom) yb = paint.bottom;
	}

	float delta_x1, delta_y1, x, y;
	int dx1, dy1, steps1;
	dx1 = xb - xa;
	dy1 = yb - ya;
	steps1 = max(abs(dx1), abs(dy1));

	if (steps1 > 10000) return/*steps1 = 10000*/;
	delta_x1 = (float)dx1 / steps1;
	delta_y1 = (float)dy1 / steps1;
	x = xa;
	y = ya;
	if (!indepth || SetDepth(x, y, depth1))
		pixel(x, y, color1);
	double dep, rate; COLORREF color;
	for (int k = 1; k <= steps1; k++) {
		x += delta_x1;
		y += delta_y1;
		rate = (double)k / steps1;
		dep = depth1 * (1 - rate) + depth2 * rate;
		color = SolveColor(color1, color2, rate);
		if (!indepth || SetDepth(x, y, dep)) 
			pixel(x, y, color);
		if (indepth && fillSw) {
			if (SetDepth(x+1, y, dep)) 
				pixel(x+1, y, color);
			if (SetDepth(x-1, y, dep)) 
				pixel(x-1, y, color);
			if (SetDepth(x, y, dep)) 
				pixel(x, y+1, color);
			if (SetDepth(x-1, y, dep)) 
				pixel(x-1, y+1, color);
			if (SetDepth(x+1, y, dep)) 
				pixel(x+1, y+1, color);
			if (SetDepth(x, y, dep)) 
				pixel(x, y-1, color);
			if (SetDepth(x-1, y, dep)) 
				pixel(x-1, y-1, color);
			if (SetDepth(x+1, y, dep)) 
				pixel(x+1, y-1, color);
		}
		if ((x < paint.left && delta_x1 <= 0 || x >= paint.right && delta_x1 >= 0) ||
			(y < paint.top && delta_y1 <= 0 || y >= paint.bottom && delta_y1 >= 0)) return;
	}
}
double operator *(POINT a, POINT b) { return a.x * b.x + a.y * b.y; }
POINT operator /(POINT a, int b) { return { a.x / b, a.y / b }; }
POINT operator -(POINT a, POINT b) { return { a.x - b.x, a.y - b.y }; }
double operator ~(POINT a) { return atan2(a.y, a.x); }
double operator !(POINT a) { return sqrt(a.x*a.x + a.y*a.y); }

inline void CoorSys::PrintTriangle(douPOINT3D A, COLORREF color1, douPOINT3D B, COLORREF color2, douPOINT3D C, COLORREF color3, douPOINT3D D, COLORREF color4) {
#define RATE_R(c,x) (DWORD(int((c & 0xff) * (x))))
#define RATE_G(c,x) (DWORD(int(((c & 0xff00) >> 8) * (x))) << 8)
#define RATE_B(c,x) (DWORD(int(((c & 0xff0000) >>16)* (x))) << 16)
#define RATE_COLOR(color,x) DWORD(RATE_R(color, x) | RATE_G(color, x) | RATE_B(color, x))

#define MatrixDet3(a11,a12,a13,a21,a22,a23,a31,a32,a33) \
	a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - \
	a11 * a23 * a32 - a12 * a21 * a33 - a13 * a22 * a31

	POINT a = GetPrintPoint(A);
	POINT b = GetPrintPoint(B);
	POINT c = GetPrintPoint(C);
	POINT d = GetPrintPoint(D);
	double D0, D1, D2, D3;
	D0 = MatrixDet3(A.x, A.y, 1, B.x, B.y, 1, C.x, C.y, 1);
	D1 = MatrixDet3(A.z, A.y, 1, B.z, B.y, 1, C.z, C.y, 1);
	D2 = MatrixDet3(A.x, A.z, 1, B.x, B.z, 1, C.x, C.z, 1);
	//D3 = MatrixDet3(A.x, A.y, A.z, B.x, B.y, B.z, C.x, C.y, C.z);
	douPOINT3D En;
	En.x = D1 / D0, En.y = D2 / D0, En.z = -1;
	En = En.x*X + En.y*Y + En.z*Z;
	if (En.x < 0) En = En * -1;
	//cout << "En(" << En.x << ", " << En.y << ", " << En.z << ")" << endl;

	double intensity = GetLightInensity(En);

	color1 = RATE_COLOR(color1, intensity);
	color2 = RATE_COLOR(color2, intensity);
	color3 = RATE_COLOR(color3, intensity);
	color4 = RATE_COLOR(color4, intensity);

	////预判断
	////Step1.线段不经过显示区时退出
	//if ((a.x < paint.left && b.x < paint.left || a.x >= paint.right && b.x >= paint.right) ||
	//	(a.y < paint.top && b.y < paint.top || a.y >= paint.bottom && b.y >= paint.bottom)) return;
	////Step2.排除不经过显示区线段
	//if (a.x == b.x) {									//斜率不存在
	//	if (a.y < paint.top) a.y = paint.top;
	//	else if (a.y >= paint.bottom) a.y = paint.bottom;
	//	if (b.y < paint.top) b.y = paint.top;
	//	else if (b.y >= paint.bottom) b.y = paint.bottom;
	//}
	float delta_x1, delta_y1, x1, y1;
	int dx1, dy1, steps;
	float delta_x2, delta_y2, x2, y2;
	int dx2, dy2;
	dx1 = b.x - a.x;
	dy1 = b.y - a.y;
	dx2 = d.x - c.x;
	dy2 = d.y - c.y;
	steps = max(max(max(abs(dx1), abs(dy1)), abs(dx2)), abs(dy2));

	if (steps > 10000) return/*steps1 = 10000*/;
	delta_x1 = (float)dx1 / steps;
	delta_y1 = (float)dy1 / steps;
	delta_x2 = (float)dx2 / steps;
	delta_y2 = (float)dy2 / steps;
	x1 = a.x;
	y1 = a.y;
	x2 = c.x;
	y2 = c.y;
	double depthA = GETDEPTH(A);
	double depthB = GETDEPTH(B);
	double depthC = GETDEPTH(C);
	double depthD = GETDEPTH(D);
	double rate;
	for (int k = 1; k <= steps; k++) {
		rate = (double)k / steps;
		x1 += delta_x1;
		y1 += delta_y1;
		x2 += delta_x2;
		y2 += delta_y2;
		Line_ColorToColor(x1, y1,
			SolveColor(color1, color2, rate), SOLVE_DEPTH(depthA, depthB, rate),
			x2, y2, SolveColor(color3, color4, rate), SOLVE_DEPTH(depthC, depthD, rate), true, true);
	}
	Line_ColorToColor(a.x, a.y,
		color1, depthA,
		c.x, c.y, color3, depthC, false, true);
	Line_ColorToColor(d.x, d.y,
		color4, depthD,
		b.x, b.y, color2, depthB, false, true);

}

inline bool CoorSys::PrintText(const char text[], POINT location, byte type = 0, bool record = false) {
	static RECT lastRect = { 0,0,0,0 };
	int x, y;
	if (type & 1)
		x = location.x - textwidth(text) / 2;
	else if ((type >> 1) & 1)
		x = location.x - textwidth(text);
	else
		x = location.x;

	if ((type >> 2) & 1)
		y = location.y - textheight(text) / 2;
	else if((type >> 3) & 1)
		y = location.y - textheight(text);
	else 
		y = location.y ;

	if (record) {
		RECT rect = { x  ,y ,x + textwidth(text),y + textheight(text) };
		if (IsRECTOverlap(rect, lastRect)) return false;
		lastRect = rect;
	}

	outtextxy(x, y, text);
	return true;
}

inline bool CoorSys::IsRECTOverlap(RECT & rect1, RECT & rect2) {
#define InRECT(x,y,rect) x >= rect.left && x < rect.right && y >= rect.top && y < rect.bottom
	if (InRECT(rect2.left, rect2.top, rect1))
		return true;
	if (InRECT(rect2.right, rect2.top, rect1))
		return true;
	if (InRECT(rect2.left, rect2.top, rect1))
		return true;
	if (InRECT(rect2.right, rect2.bottom, rect1))
		return true;
	return false;
}

inline int CoorSys::DePlace(float * digit) {
	if (abs(*digit) >= 1 || digit == 0) {
		*digit = (int)*digit;
		return 0;
	}
	for (int i = 1; ; i++) {
		if (abs(*digit *= 10) >= 1) {
			*digit = (int)*digit;
			*digit /= pow(10, i);
			return i;
		}
	}
}

inline void CoorSys::CutTail(char digit[], int deplace) {
	int i = 0;
	for (; digit[i] != '.' && digit[i] != '\0'; ++i);
	if (deplace <= 0) digit[i] = '\0';
	else digit[i + 1 + deplace] = '\0';
}

inline void CoorSys::DrawSca() {
	//创建一个矩形区域
	HRGN rgn = CreateRectRgn(paint.left, paint.top, paint.right, paint.bottom);
	//将该矩形区域设置为裁剪区
	setcliprgn(rgn);


	setlinecolor(RGB(255, 132, 0));			//坐标线颜色
	settextcolor(WHITE);					//刻度文字颜色
	setbkmode(TRANSPARENT);					//设置文字背景为透明
	settextstyle(20, 0, "等线");			//设置字体样式


	DrawScaText(Ex, douPOINT3D(1, 0, 0), ScaPlaLENTH / pro * douPOINT3D(0, 0, 1));		//画x坐标线
	
	DrawScaText(Ey, douPOINT3D(0, 1, 0), ScaPlaLENTH / pro * douPOINT3D(1, 0, 0));		//画y坐标线

	DrawScaText(Ez, douPOINT3D(0, 0, 1), ScaPlaLENTH / pro * douPOINT3D(1, 0, 0));		//画z坐标线

	//不再使用rgn，清理rgn占用的系统资源
	DeleteObject(rgn);
	//恢复裁剪区
	setcliprgn(NULL);

}

inline void CoorSys::DrawName() {
	settextcolor(WHITE);				//文字颜色
	setbkmode(TRANSPARENT);				//设置文字背景为透明
	settextstyle(20, 0, "等线");		//设置字体样式
	setlinecolor(LIGHTGRAY);			//边框颜色
	setlinestyle(PS_SOLID);				//边框实线
	setfillcolor(BLACK);				//填充颜色

	stringstream ss;
	ss << "比例:" << pro << ends;
	string& prop = ss.str();
	fillrectangle(paint.left + 5,
		paint.top + 2,
		paint.left + textwidth(prop.c_str()) + 15,
		paint.top + textheight(prop.c_str()) + 2);		//图像名填充背景
	outtextxy(paint.left + 10, paint.top + 2, prop.c_str());

	char *expc, *varF;
	int top = paint.top + textheight(prop.c_str()) + 6;
	int expCounts = expVec.size();
	for (int i = 0; i < expCounts; i++) {

		expc = expVec[i].GetExpression();
		varF = expVec[i].GetVariableFormat();
		ss.str("");
		ss << "f";
		if (i) ss << i + 1;
		ss << "(" << varF << ")=" << expc << ends;
		string& name = ss.str();
		fillrectangle(paint.left + 5,
			top,
			paint.left + textwidth(name.c_str()) + 15,
			top + textheight(name.c_str()));							//图像名填充背景
		outtextxy(paint.left + 10, top, name.c_str());
		top += 4 + textheight(name.c_str());
	}
}

inline void CoorSys::CleanScreen() {
	setlinecolor(WHITE);															//画布边框颜色
	setlinestyle(PS_SOLID);															//边框实线
	setfillcolor(RGB(30, 30, 30));													//画布填充颜色
	fillrectangle(paint.left - 1, paint.top - 1, paint.right, paint.bottom);		//清空剪裁区
	ResetDepth();
}

inline void CoorSys::SetCoorMessage() {
#define SETCOORMESSAGE(mes,e) \
	if (e < 1e-6) { \
		mes.maxValue = mes.minValue = 0; \
		mes.step = 1; \
	} \
	else if (abs(e.x) >= abs(e.y)) { \
		mes.minValue = (paint.left - ori.x) / (pro * e.x); \
		mes.maxValue = (paint.right - ori.x) / (pro * e.x); \
		mes.step = 1 / (pro * e.x); \
	} \
	else { \
		mes.minValue = (paint.top - ori.y) / (pro * e.y); \
		mes.maxValue = (paint.bottom - ori.y) / (pro * e.y); \
		mes.step = 1 / (pro * e.y); \
	} \
	if (mes.maxValue < mes.minValue) {\
		double temp = mes.maxValue;\
		mes.maxValue = mes.minValue;\
		mes.minValue = temp;\
	} \

	
	if (Ey == 0) {						//坐标是二维的
		SETCOORMESSAGE(Xmes, Ex);
		Ymes.maxValue = Ymes.minValue = 0;
		Ymes.step = 1;
	}
	else {								//坐标是三维的
		Xmes.maxValue = Ymes.maxValue = 10;
		Xmes.minValue = Ymes.minValue = -10;
		Xmes.step = Ymes.step = 0.1;
	}
	SETCOORMESSAGE(Zmes, Ez);

	recorded = false;					//坐标更改，需要重新记录图像
}

inline void CoorSys::ResetDepth() {
	double *p = depth;
	for (int i = paint.top; i < paint.bottom; i++) 
		for (int j = paint.left; j < paint.right; j++, p++) 
			*p = -INFINITY;
}

inline bool CoorSys::CheckDepth(int x, int y, double dep) {
	if (InRECT(x, y, paint)) {
		double & temp = depth[(y - paint.top) * width + x - paint.left];
		if (temp >= dep)
			return false;
		return true;
	}
	return false;
}

inline bool CoorSys::SetDepth(int x, int y, double dep) {
	if (InRECT(x, y, paint)) {
		double & temp = depth[(y - paint.top) * width + x - paint.left];
		if (temp >= dep)
			return false;
		temp = dep;
		return true;
	}
	return false;
}

inline double CoorSys::GetLightInensity(douPOINT3D direction) {
	direction.Unitizate();
	double inensity1 = direction.x*light1.x + direction.y * light1.y + direction.z*light1.z;
	if (inensity1 < 0.1) inensity1 = 0.1;
	double inensity2 = direction.x*light2.x + direction.y * light2.y + direction.z*light2.z;
	if (inensity2 < 0.1) inensity2 = 0.1;
	return 0.9 * inensity1 + 0.1 * inensity2;
}

inline DWORD CoorSys::SolveColor(COLORREF c1, COLORREF c2, double rate) {
	DWORD r1 = c1 & 0xff;
	DWORD r2 = c2 & 0xff;
	DWORD r = r1 * (1 - rate) + r2 * rate;
	DWORD g1 = c1 & 0xff00; g1 >>= 8;
	DWORD g2 = c2 & 0xff00; g2 >>= 8;
	DWORD g = g1 * (1 - rate) + g2 * rate;
	DWORD b1 = c1 & 0xff0000; b1 >>= 16;
	DWORD b2 = c2 & 0xff0000; b2 >>= 16;
	DWORD b = b1 * (1 - rate) + b2 * rate; 
	g <<= 8;
	b <<= 16;
	return b | g | r;
}


void CoorSys::DrawScaText(douPOINT e, douPOINT3D E, douPOINT3D offset) {

	stringstream ss;
	float scapro;						//标尺刻度最小分度值
	int level;							//分度值的数量级

	double minV, maxV;
	if (e < 1e-6) return;
	else if (abs(e.x) >= abs(e.y)) {
		scapro = width / (pro * e.x) / SCAPROCOUNTS;
		level = DePlace(&scapro);
		minV = (paint.left - 10 - ori.x) / (pro * e.x);
		maxV = (paint.right + 10 - ori.x) / (pro * e.x);
	}
	else {
		scapro = height / (pro * e.y) / SCAPROCOUNTS;
		level = DePlace(&scapro);
		minV = (paint.top - 10 - ori.y) / (pro * e.y);
		maxV = (paint.bottom + 10 - ori.y) / (pro * e.y);
	}
	
	LineMath(minV * E, maxV * E, false, true, true);
	int type = 0;
	douPOINT offset2D = { pro*(offset.x * Ex.x + offset.y * Ey.x + offset.z * Ez.x),
		+pro*(offset.x  * Ex.y + offset.y  * Ey.y + offset.z * Ez.y) };
	if (offset2D.x != 0 || offset2D.y != 0) {
		int a = atan2(-offset2D.y, offset2D.x) * 4 / PI;
		int b = atan2(-0.5, 1) * 4 / PI;
		if (a == 0)
			type = TEXT_TYPE_VERTICALMID;
		else if (a == 1 || a == 2)
			type = TEXT_TYPE_VERTICALTOP | TEXT_TYPE_LEVELMID;
		else if (a == 3 || a == 4 || a == -3)
			type = TEXT_TYPE_VERTICALMID | TEXT_TYPE_LEVELLEFT;
		else if (a == -2 || a == -1)
			type = TEXT_TYPE_LEVELMID;
	}
	//FlushBatchDraw();
	int min = minV / scapro, max = maxV / scapro;
	if (min > max) swap(min, max);
	ss << setiosflags(ios::fixed) << setprecision(level);				//设置字符流中小数的精度
	for (min; min <= max; min++) {
		ss.str("");
		ss  << min *scapro << ends;										//将数字转化为字符串
		douPOINT3D nowSca = min  * scapro * E;
		if (CheckDepth(GetPrintPoint(nowSca + offset), GETDEPTH((nowSca + offset)))) {
			if (PrintText(ss.str().c_str(), GetPrintPoint(nowSca + offset), type, false))
				LineMath(nowSca, nowSca + offset, false, true, true);
		}
	}
}

CoorSys::CoorSys(POINT ori, RECT & paint):ori0(ori) {
	GetClipBox(GetImageHDC(), &screen);
	this->paint = paint;
	this->ori = ori;
	width = paint.right - paint.left;
	height = paint.bottom - paint.top;
	paintImage = new DWORD[width * height];
	depth = new double[width * height];
	//light1 = { 1,0,0}, light2 = { sqrt(2) / 2,sqrt(2) / 2,0 };	//主光源和次光源
	light1 = { 0.5,-0.5,sqrt(2) / 2 }, light2 = { sqrt(2) / 2,sqrt(2) / 2,0 };	//主光源和次光源
	Set3D();
}

inline void CoorSys::RotateCoor(double a, double b) {
	//内置初始单位向量：Ex0 = { -sqrt(2) / 4,sqrt(2) / 4 }, Ey0 = { 1,0 }, Ez0 = { 0,-1 }
	double m, n;
	
	m = X.x *cos(a) - X.y*sin(a);	//水平旋转
	n = X.y *cos(a) + X.x*sin(a);
	X.x = m; X.y = n;
	m = X.x *cos(b) - X.z*sin(b);	//垂直旋转
	n = X.z *cos(b) + X.x*sin(b);
	X.x = m; X.z = n;

	m = Y.x *cos(a) - Y.y*sin(a);	//水平旋转
	n = Y.y *cos(a) + Y.x*sin(a);
	Y.x = m; Y.y = n;
	m = Y.x *cos(b) - Y.z*sin(b);	//垂直旋转
	n = Y.z *cos(b) + Y.x*sin(b);
	Y.x = m; Y.z = n;

	m = Z.x *cos(a) - Z.y*sin(a);	//水平旋转
	n = Z.y *cos(a) + Z.x*sin(a);
	Z.x = m; Z.y = n;
	m = Z.x *cos(b) - Z.z*sin(b);	//垂直旋转
	n = Z.z *cos(b) + Z.x*sin(b);
	Z.x = m; Z.z = n;

	RefreshExyz();
}

inline void CoorSys::Set2D() { 
	X = X0 = { 0,1,0 }, Y = Y0 = { -1,0,0 }, Z = Z0 = { 0,0,1 };
	RefreshExyz();
}

inline void CoorSys::Set3D() {
	X = X0 = { sqrt(3) / 3, -sqrt(2) / 2, -sqrt(6) / 6 };
	Y = Y0 = { sqrt(3) / 3, sqrt(2) / 2, -sqrt(6) / 6 };
	Z = Z0 = { sqrt(3) / 3,0,sqrt(6) / 3 };
	RefreshExyz();
}

inline POINT CoorSys::GetPrintPoint(double mathX, double mathY, double mathZ) {
	POINT printPoint = { ori.x + pro*(mathX * Ex.x + mathY * Ey.x + mathZ * Ez.x),
		ori.y + pro*(mathX * Ex.y + mathY * Ey.y + mathZ * Ez.y) };
	return printPoint;
}

inline douPOINT CoorSys::GetMathXY(POINT target) {
	douPOINT b = ((douPOINT)target - ori) / pro;
	if (Ey < 1e-6)
		return douPOINT(Ex*b/Ex.SquareModule(), 0.0);
	if (Ex < 1e-6)
		return douPOINT(0.0, Ey*b / Ey.SquareModule());

	double D, D1, D2;
	D = Ex.x * Ey.y - Ex.y * Ey.x;
	D1 = b.x * Ey.y - b.y * Ey.x;
	D2 = b.y * Ex.x - b.x * Ex.y;
	return douPOINT(D1 / D, D2 / D);
}

inline void CoorSys::NewExpression(char exp[], char variableFormat[]) {
	if (!holdOn)
		this->expVec.clear();
	Expression a(exp, variableFormat);
	this->expVec.push_back(a);
	recorded = false;								//表达式组更改，需要重新记录图像
}

inline POINT CoorSys::PrintPoint(double mathX, double mathY, double mathZ, COLORREF color) {
	POINT printPoint = GetPrintPoint(mathX, mathY, mathZ);
	pixel(printPoint.x, printPoint.y, color);
	return printPoint;
}

inline void CoorSys::PrintMouseTarget(POINT mousexy) {
#define GET_SCAPRO_LEVEL(e,scapro,level) \
	if (e < 1e-6) level = 0; \
	else if (abs(e.x) >= abs(e.y)) { \
		scapro = width / (pro * e.x) / SCAPROCOUNTS; \
		level = DePlace(&scapro) + 2; \
	} \
	else { \
		scapro = height / (pro * e.y) / SCAPROCOUNTS; \
		level = DePlace(&scapro)+ 2; \
	} 


	settextcolor(WHITE);				//文字颜色
	setbkmode(TRANSPARENT);				//设置文字背景为透明
	settextstyle(20, 0, "等线");		//设置字体样式
	setlinecolor(GREEN);				//辅助线颜色

	douPOINT mathXY = GetMathXY(mousexy);
	douPOINT3D coo = { mathXY.x,mathXY.y,0 };
	
	if (coo.x) LineMath(douPOINT3D(coo.x, coo.y, 0), douPOINT3D(coo.x, 0, 0), 0, 1, 1);
	if (coo.y) LineMath(douPOINT3D(coo.x, coo.y, 0), douPOINT3D(0, coo.y, 0), 0, 1, 1);

	float scaproX, scaproY, scaproZ;
	int levelX, levelY, levelZ;
	GET_SCAPRO_LEVEL(Ex, scaproX, levelX);
	GET_SCAPRO_LEVEL(Ey, scaproY, levelY);
	GET_SCAPRO_LEVEL(Ez, scaproZ, levelZ);

	LOGFONT textstyle;
	gettextstyle(&textstyle);
	int fontheight = textstyle.lfHeight + 2;

	stringstream ss;
	ss << setiosflags(ios::fixed);								//设置字符流setprecision操作为保留小数
	int expCounts = expVec.size();
	POINT loc = mousexy;
	if (mousexy.y < paint.top + 1) loc.y = paint.top + 1;
	if (mousexy.y + expCounts * fontheight > paint.bottom)
		loc.y = paint.bottom - expCounts * fontheight;


	for (int i = 0; i < expCounts; i++) {
		coo.z = expVec[i].Calculate(&coo.x);
		
		setlinecolor(RED);				//辅助线颜色
		LineMath(coo, douPOINT3D(coo.x, coo.y, 0), true, 1, 1);
		setlinecolor(GREEN);				//辅助线颜色
		LineMath(coo, douPOINT3D(coo.x, 0, coo.z), true, 1, 1);
		LineMath(coo, douPOINT3D(0, coo.y, coo.z), true, 1, 1);
		if (coo.y) LineMath(douPOINT3D(0, 0, coo.z), douPOINT3D(coo.x, 0, coo.z), true, 1, 1);
		if (coo.x) LineMath(douPOINT3D(0, 0, coo.z), douPOINT3D(0, coo.y, coo.z), true, 1, 1);
		if (coo.x) LineMath(douPOINT3D(coo.x, 0, coo.z), douPOINT3D(coo.x, 0, 0), true, 1, 1);
		if (coo.y) LineMath(douPOINT3D(0, coo.y, coo.z), douPOINT3D(0, coo.y, 0), true, 1, 1);

		ss.str("");												//清空字符流
		ss << "(" << setprecision(levelX) << coo.x <<", "
			<< setprecision(levelY) << coo.y <<", " 
			<< setprecision(levelZ) << coo.z <<")"<< ends;
		string& cooText = ss.str();

		int textwid = textwidth(cooText.c_str());
		if (loc.x + textwid + 2 > paint.right)
			loc.x = paint.right - textwidth(cooText.c_str()) - 2;
		
		if (loc.x < paint.left + 2) loc.x = paint.left + 2;

		setlinecolor(WHITE);			//边框颜色
		setfillcolor(BLACK);				//填充颜色
		fillrectangle(loc.x - 2, loc.y - 2, loc.x + textwidth(cooText.c_str()) + 2, loc.y + fontheight);
		
		PrintText(cooText.c_str(), loc);
		loc.y += fontheight;
	}
	FlushBatchDraw();
}

inline void CoorSys::RecordImage() {
#define enumPAINTIMAGE_HEAD(pStore,pMemory)\
	DWORD* p = paintImage; \
	int d = screen.right - screen.left - width; \
	DWORD* pM = pMem + paint.top * screen.right + paint.left; \
	for (int i = paint.top ; i < paint.bottom; i++) { \
		for (int j = paint.left; j < paint.right; j++,p++,pM++) {
#define enumPAINTIMAGE_TAIL(pStore,pMemory)\
		} \
		pM += d; \
	}

	if (recorded) return;
	enumPAINTIMAGE_HEAD(p, pM);
			*p = *pM;
	enumPAINTIMAGE_TAIL(p, pM);
	recorded = true;
}

inline void CoorSys::DrawRi_angle() {
#ifdef DEBUG
	clock_t start, finish;
	start = clock();

#endif // DEBUG


	if (recorded) {
		enumPAINTIMAGE_HEAD(p, pM);
		*pM = *p;
		enumPAINTIMAGE_TAIL(p, pM);
		FlushBatchDraw();

#ifdef DEBUG
		finish = clock();
		double Time = (double)(finish - start) / 1000;
		printf("用时：%f秒\n", Time);

#endif // DEBUG

		return;
	}


	CleanScreen();
	setlinecolor(WHITE);				//图像画线颜色

#define MINCOLOR LIGHTCYAN
#define MAXCOLOR MAGENTA

//#define MINCOLOR WHITE
//#define MAXCOLOR WHITE
//
	int Xcounts = (Xmes.maxValue - Xmes.minValue) / Xmes.step + 1;
	int Ycounts = (Ymes.maxValue - Ymes.minValue) / Ymes.step + 1;
	double* data = new double[Xcounts * Ycounts];

	for (auto i : expVec) {
		double maxV = 1, minV = 0, *pdata = data;

		for (double y = Ymes.minValue; y <= Ymes.maxValue; y += Ymes.step) {
			for (double x = Xmes.minValue; x <= Xmes.maxValue; x += Xmes.step) {
				double xy[] = { x,y };
				*pdata = i.Calculate(xy);
				if (!isinf(*pdata) && !_isnan(*pdata)) {
					if (*pdata > maxV) maxV = *pdata;
					if (*pdata < minV) minV = *pdata;
				}
				++pdata;
			}
		}

		pdata = data;

		if (Xcounts <= 0 || Ycounts <= 0) continue;
		else if (Xcounts == 1) {
			douPOINT3D point1 = { Xmes.minValue, Ymes.minValue, *(pdata++) }, point2;
			POINT p1 = GetPrintPoint(point1), p2;
			COLORREF colorP1 = SolveColor(MINCOLOR, MAXCOLOR, (point1.z - minV) / (maxV - minV)), colorP2;

			for (double y = Ymes.minValue; y < Ymes.maxValue; y += Ymes.step) {
				point2 = { Xmes.minValue, y + Ymes.step, *(pdata++) };
				p2 = GetPrintPoint(point2);
				colorP2 = SolveColor(MINCOLOR, MAXCOLOR, (point2.z - minV) / (maxV - minV));
				Line_ColorToColor(p1.x, p1.y, colorP1, 0, p2.x, p2.y, colorP2, 0);
				point1 = point2, p1 = p2, colorP1 = colorP2;
			}
			continue;
		}
		else if (Ycounts == 1) {
			douPOINT3D point1 = { Xmes.minValue, Ymes.minValue, *(pdata++) }, point2;
			POINT p1 = GetPrintPoint(point1), p2;
			COLORREF colorP1 = SolveColor(MINCOLOR, MAXCOLOR, (point1.z - minV) / (maxV - minV)), colorP2;

			for (double x = Xmes.minValue; x <= Xmes.maxValue; x += Xmes.step) {
				point2 = { x + Xmes.step, Ymes.minValue, *(pdata++) };
				p2 = GetPrintPoint(point2);
				colorP2 = SolveColor(MINCOLOR, MAXCOLOR, (point2.z - minV) / (maxV - minV));
				Line_ColorToColor(p1.x, p1.y, colorP1, 0, p2.x, p2.y, colorP2, 0);
				point1 = point2, p1 = p2, colorP1 = colorP2;
			}
			continue;
		}

		else {
			
			double* pdata2;
			douPOINT3D point1, point2, point3, point4;
			COLORREF colorP1, colorP2, colorP3, colorP4;

			int line = 1, row = 1;
			for (double y = Ymes.minValue; y < Ymes.maxValue; y += Ymes.step) {

				if (y + Ymes.step <= Ymes.maxValue) {
					pdata2 = pdata + Xcounts;

					point1 = { Xmes.minValue, y, *(pdata++) };
					colorP1 = SolveColor(MINCOLOR, MAXCOLOR, (point1.z - minV) / (maxV - minV));

					point2 = { Xmes.minValue, y + Ymes.step, *(pdata2++) };
					colorP2 = SolveColor(MINCOLOR, MAXCOLOR, (point2.z - minV) / (maxV - minV));

					for (double x = Xmes.minValue; x < Xmes.maxValue; x += Xmes.step) {

						point3 = { x + Xmes.step, y, *(pdata++) }; row++;
						colorP3 = SolveColor(MINCOLOR, MAXCOLOR, (point3.z - minV) / (maxV - minV));

						point4 = { x + Xmes.step, y + Ymes.step, *(pdata2++) };
						colorP4 = SolveColor(MINCOLOR, MAXCOLOR, (point4.z - minV) / (maxV - minV));

						PrintTriangle(point1, colorP1, point2, colorP2, point3, colorP3, point4, colorP4);

						point1 = point3, colorP1 = colorP3;
						point2 = point4, colorP2 = colorP4;
						//FlushBatchDraw();
						if (row >= Xcounts) break;
					}
				}
				else
					pdata2 = nullptr;
				pdata = data + Xcounts* (line++);
				row = 1;
			}
		}
	}
	delete[] data;

	DrawSca();
	DrawName();
	FlushBatchDraw();

#ifdef DEBUG
	finish = clock();
	double Time = (double)(finish - start) / 1000;
	printf("用时：%f秒\n", Time);

#endif // DEBUG


}



