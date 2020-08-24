/*By.Wheat*/
/*最新日期：2017年07月07日*/

#pragma once
#include <cmath>
#include <string.h>
#include <iostream>
#define OpMAXCHAR 20												//一个运算符最大长度
#define INF 100														//无限优先级
#define DefaultPro 10												//默认倍数
#define DefaultOri { (FunctionRect.right + FunctionRect.left) / 2 ,(FunctionRect.top + FunctionRect.bottom) / 2 }
#define PI 3.1415926
#define ExpMAXCHAR 100												//表达式最大长度

using namespace std;

//***************************************************************************
// --------------------------------------------------------------------------------------
//|		结点	|	isOpt	|	eleNum	|	prio	|			pCalMean:eleNum			|
//|-------------------------------------------------------------------------------------|
//|	数据		|	false	|	0		|	INF		|null								|
//|	自变量		|	true	|	0		|	INF		|null								|
//|	字符常量	|	true	|	0		|	INF		|exp:exp(1)、pi:3.1415926				| e p
//|	(			|	true	|	0		|	INF		|null								|
//|	)			|	true	|	0		|	0		|null								|
//|	+、-		|	true	|	2		|	1		|plus(left,right)、minus(left,right)|
//|	*、/		|	true	|	2		|	2		|multiply(le,ri)、division(le,ri)	|
//|	\、%		|	true	|	2		|	2		|intDivision(le,ri)、mod(le,ri)		|
//|	^			|	true	|	2		|	3		|pow(left,right)					|
//|	一元运算符	|	true	|	1		|	4		|plus(right)、sin(right)...			| 1+ 1- sin cos tan arcsin arccos arctan ln sqrt abs
// --------------------------------------------------------------------------------------

struct ExpNode {
	double data;													//【数据】
	char opt[OpMAXCHAR];											//【运算符】

	bool isOpt;														//【结点标识】		（自变量仍算为运算符）
	int eleNum;														//【元（目）数】	（数据与自变量元数为0）
	int prio = -1;													//【优先级】		（数据与自变量优先级为无限）(优先级为-1时表示该结点无效，为未初始化结点）
	int variableNum;												//仅自变量有，自变量在变量数组中位置
	ExpNode* next = nullptr;

	double(*pCalMean0) (void) = nullptr;
	double(*pCalMean1) (double right) = nullptr;
	double(*pCalMean2) (double left, double right) = nullptr;

private:

	int VariableSerial(const char variableFormat[], char variableName[]);			//过程函数，寻找所给变量名在变量格式串中序号，以","为分割依据

public:
	ExpNode() { ; }
	ExpNode(double data);
	ExpNode(char opt[]);
	bool IsValid() { return prio != -1; }
	bool IsData();																		//判断该结点是不是数据（包括常量、字符常量、自变量）
	bool SetVariableNum(const char variableFormat[]);									//仅自变量结点调用，设置VariableNum
	bool operator ==(const ExpNode& node);
	bool operator !=(const ExpNode& node) { return !(*this == node); }
	bool operator ==(const char opt[]);
	bool operator !=(const char opt[]) { return !(*this == opt); }

	double Calculate(const double variable[]);				//计算自变量
	double Calculate(double right) { return pCalMean1(right); }							//计算一元运算符
	double Calculate(double left, double right) { return pCalMean2(left, right); }		//计算二元运算符
};


//该实例类拥有全部支持运算符运算规则
class TemplateOptArr {
private:
	void Sort();																		//从大到小排序
																						//************************运算规则系列******************************
	static double e() { return exp(1); }
	static double pi() { return PI; }
	static double plus(double left, double right) { return left + right; }
	static double minus(double left, double right) { return left - right; }
	static double multiply(double left, double right) { return left * right; }
	static double division(double left, double right) { return left / right; }
	static double plus(double right) { return right; }
	static double minus(double right) { return -right; }
	static double mod(double left, double right) {
		if ((int)right == 0 || (int) left == -2147483648) return INFINITY;
		return (int)left % (int)right;
		//int temp = left / right;
		//return int(left - right * temp);
	}
	static double intDivision(double left, double right) { 
		if ((int)right == 0 || (int)left == -2147483648) return INFINITY;
		return (int)left / (int)right;
		//return (int)(left / right);
	}

public:
	ExpNode optArr[22];

	TemplateOptArr();
	ExpNode GetOptNode(const char opt[]);

};


TemplateOptArr templateOptArr;




inline int ExpNode::VariableSerial(const char variableFormat[], char variableName[]) {
	int n = 0, i, j, k;
	bool sw = true;
	for (i = 0; variableFormat[i] != '\0'; i++) {
		if (variableFormat[i] == ',') {
			++n;
			sw = true;
			continue;
		}

		for (j = i, k = 0;
			sw && variableName[k] != '\0' && variableName[k] == variableFormat[j];
			j++, k++);

		if (variableName[k] == '\0' && (variableFormat[j] == ',' || variableFormat[j] == '\0')) return n;
		if (sw) sw = false;
	}
	return -1;
}

inline ExpNode::ExpNode(double data) {
	this->data = data;
	isOpt = false;
	eleNum = 0;
	prio = INF;
}

inline ExpNode::ExpNode(char opt[]) {
	ExpNode node = templateOptArr.GetOptNode(opt);
	if (node.IsValid()) {
		*this = node;
		return;
	}
	strcpy(this->opt, opt);
	isOpt = true;
	eleNum = 0;
	prio = INF;
}

inline bool ExpNode::IsData() {
	if (!isOpt) return true;
	if (prio < INF) return false;
	if (*this == "(") return false;
	return true;
}

inline bool ExpNode::SetVariableNum(const char variableFormat[]) {
	variableNum = VariableSerial(variableFormat, opt);
	if (variableNum >= 0) return true;
	cout << "未定义变量'" << opt << "'" << endl;
	return false;
}

inline bool ExpNode::operator==(const ExpNode & node) {
	if (!node.isOpt && !isOpt) return true;
	if (!strcmp(node.opt, opt) && node.eleNum == eleNum
		&& node.prio == prio) return true;
}

inline bool ExpNode::operator==(const char opt[]) {
	if (!strcmp(this->opt, opt)) return true;
	return false;
}

inline double ExpNode::Calculate(const double variable[]) {
	if (pCalMean0 != nullptr) return pCalMean0();

	if (variableNum >= 0) return variable[variableNum];
	//cout << "未定义变量'" << opt << "'!" << endl;
	return 0;
}

inline void TemplateOptArr::Sort() {
	bool isChange = true;
	while (isChange) {
		isChange = false;
		for (int i = 0; i < sizeof(optArr) / sizeof(ExpNode) - 1; i++) {
			if (strlen(optArr[i].opt) < strlen(optArr[i + 1].opt)) {
				ExpNode temp = optArr[i];
				optArr[i] = optArr[i + 1];
				optArr[i + 1] = temp;
				isChange = true;
			}
		}
	}
}

inline TemplateOptArr::TemplateOptArr() {
	int i;

	i = 0;
	//常量e							true						0						INF
	strcpy(optArr[i].opt, "exp");		optArr[i].isOpt = true;		optArr[i].eleNum = 0;	optArr[i].prio = INF;
	optArr[i].pCalMean0 = e;

	i = 1;
	//常量p							true						0						INF
	strcpy(optArr[i].opt, "pi");		optArr[i].isOpt = true;		optArr[i].eleNum = 0;	optArr[i].prio = INF;
	optArr[i].pCalMean0 = pi;

	i = 2;
	// (							true						0						INF
	strcpy(optArr[i].opt, "(");		optArr[i].isOpt = true;		optArr[i].eleNum = 0;	optArr[i].prio = INF;

	i = 3;
	// )							true						0						0
	strcpy(optArr[i].opt, ")");		optArr[i].isOpt = true;		optArr[i].eleNum = 0;	optArr[i].prio = 0;

	i = 4;
	// +							true						2						1
	strcpy(optArr[i].opt, "+");		optArr[i].isOpt = true;		optArr[i].eleNum = 2;	optArr[i].prio = 1;
	optArr[i].pCalMean2 = plus;

	i = 5;
	// -							true						2						1
	strcpy(optArr[i].opt, "-");		optArr[i].isOpt = true;		optArr[i].eleNum = 2;	optArr[i].prio = 1;
	optArr[i].pCalMean2 = minus;

	i = 6;
	// *							true						2						2
	strcpy(optArr[i].opt, "*");		optArr[i].isOpt = true;		optArr[i].eleNum = 2;	optArr[i].prio = 2;
	optArr[i].pCalMean2 = multiply;

	i = 7;
	// /							true						2						2
	strcpy(optArr[i].opt, "/");		optArr[i].isOpt = true;		optArr[i].eleNum = 2;	optArr[i].prio = 2;
	optArr[i].pCalMean2 = division;

	i = 8;
	// ^							true						2						3
	strcpy(optArr[i].opt, "^");		optArr[i].isOpt = true;		optArr[i].eleNum = 2;	optArr[i].prio = 3;
	optArr[i].pCalMean2 = pow;

	i = 9;
	// 1+							true						1						4
	strcpy(optArr[i].opt, "1+");	optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = plus;

	i = 10;
	// 1-							true						1						4
	strcpy(optArr[i].opt, "1-");	optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = minus;

	i = 11;
	// sin							true						1						4
	strcpy(optArr[i].opt, "sin");	optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = sin;

	i = 12;
	// cos							true						1						4
	strcpy(optArr[i].opt, "cos");	optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = cos;

	i = 13;
	// tan							true						1						4
	strcpy(optArr[i].opt, "tan");	optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = tan;

	i = 14;
	// arcsin						true						1						4
	strcpy(optArr[i].opt, "arcsin"); optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = asin;

	i = 15;
	// arccos						true						1						4
	strcpy(optArr[i].opt, "arccos"); optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = acos;

	i = 16;
	// arctan						true						1						4
	strcpy(optArr[i].opt, "arctan"); optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = atan;

	i = 17;
	// ln							true						1						4
	strcpy(optArr[i].opt, "ln");	optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = log;

	i = 18;
	// sqrt							true						1						4
	strcpy(optArr[i].opt, "sqrt");	optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = sqrt;

	i = 19;
	// abs							true						1						4
	strcpy(optArr[i].opt, "abs");	optArr[i].isOpt = true;		optArr[i].eleNum = 1;	optArr[i].prio = 4;
	optArr[i].pCalMean1 = abs;

	i = 20;
	// mod							true						2						2
	strcpy(optArr[i].opt, "%");	optArr[i].isOpt = true;		optArr[i].eleNum = 2;	optArr[i].prio = 2;
	optArr[i].pCalMean2 = mod;

	i = 21;
	// inte							true						2						2
	strcpy(optArr[i].opt, "\\");	optArr[i].isOpt = true;		optArr[i].eleNum = 2;	optArr[i].prio = 2;
	optArr[i].pCalMean2 = intDivision;

	Sort();

}

inline ExpNode TemplateOptArr::GetOptNode(const char opt[]) {
	for (int i = 0; i < sizeof(optArr) / sizeof(ExpNode); i++) {
		if (!strcmp(optArr[i].opt, opt)) return optArr[i];
	}
	return ExpNode();
}
