/*By.Wheat*/
/*�������ڣ�2017��07��07��*/

#pragma once
#include "ExpressionScan.h"


class ExpStack {
private:
	ExpNode* top = nullptr;
	ExpNode* bottom = nullptr;
	int count = 0;
public:
	ExpStack() { ; }
	ExpStack(const ExpStack & expStack);
	void Clear();														//���ջ
	bool IsEmpty() { return !top; }										//�Ƿ��ջ

	//��ջ��ѹ��ɹ�����true��
	bool Push(ExpNode& node);
	bool Push(double data) { return this->Push(ExpNode(data)); }
	bool Push(char opt[], int eleNum, int prio) { return this->Push(ExpNode(opt)); }

	//��ջ
	ExpNode Pop();

	ExpNode GetTop();													//��ȡջ����Ϣ����
	int GetLen() { return count; }										//��ȡջ����
	void Invert();														//��תջ
	void Print();														//�����ô�ӡ����

	~ExpStack() { Clear(); }
};

class Expression {
private:
	ExpStack infixExpStack;
	ExpStack postfixExpStack;
	ExpStack temp;
	double* newTemp = nullptr;

	char* exp = nullptr;
	char* variableFormat = nullptr;
	ExpNode* postfixExpArr = nullptr;
	int postfixExpArrLen;
public:
	Expression() { ; }
	Expression(const Expression& exp);
	Expression(const ExpStack& expStack) :infixExpStack(expStack) { TransformInfixToPostfix(infixExpStack); }
	Expression(const char exp[], const char variableFormat[]) { SetExpression(exp, variableFormat); }
	~Expression() { Clear(); }
	void Print() { postfixExpStack.Print(); }
	void SetExpression(const char exp[], const char variableFormat[]);
	void SetVariableFormat(const char variableFormat[]);
	char* GetExpression() { return exp; }
	char* GetVariableFormat() { return variableFormat; }
	double Calculate(const double variable[]);
	void Clear();
private:
	void ExpressionScan();													//���ʽɨ��
	bool Postfix_TempAction();												//��׺ת�����õ�����ʱ����
	void TransformInfixToPostfix(ExpStack & infixExpStack);					//��׺ջת���ɺ�׺ջ
	void PostfixStack_To_PostfixArr();										//��׺ջת���ɺ�׺����
};




inline ExpStack::ExpStack(const ExpStack & expStack) {
	ExpNode* p = expStack.top;
	ExpNode* q = nullptr;
	for (; p; p = p->next) {
		if (!top) {
			q = top = new ExpNode;
			*top = *p;
		}
		else {
			q->next = new ExpNode;
			*q->next = *p;
			q = q->next;
		}
		++count;
	}
}

inline void ExpStack::Clear() {
	while (top) {
		ExpNode* p = top;
		top = top->next;
		delete p;
	}
	bottom = nullptr;
	count = 0;
}

inline bool ExpStack::Push(ExpNode & node) {
	if (!node.IsValid()) return false;								//�����Чʱ�˳�

	if (!top) {
		bottom = top = new ExpNode;
		*top = node;
	}
	else {
		ExpNode* p = new ExpNode;
		*p = node;
		p->next = top;
		top = p;
	}
	++count;
	return true;
}

inline ExpNode ExpStack::Pop() {
	if (this->IsEmpty()) return ExpNode();

	ExpNode node = *top;										//����ջ�ڴ棬�Ա㺯���ս�ʱ����
	node.next = nullptr;										//Ĩȥ��ַ����������

	ExpNode* p = top;											//��ջ���������ٶ��ڴ�
	top = top->next;
	if (!top) bottom = nullptr;
	--count;
	delete p;

	return node;
}

inline ExpNode ExpStack::GetTop() {
	if (this->IsEmpty()) return ExpNode();

	ExpNode node = *top;
	node.next = nullptr;										//Ĩȥ��ַ����������

	return node;
}



inline void ExpStack::Invert() {
	ExpStack expStack(*this);
	this->Clear();
	while (!expStack.IsEmpty()) {
		this->Push(expStack.Pop());
	}
}

inline void ExpStack::Print() {
	for (ExpNode* p = top; p; p = p->next) {
		if (p->isOpt)
			cout << p->opt << " ";
		else
			cout << p->data << " ";
	}
	cout << endl;
}

inline Expression::Expression(const Expression & exp) {
	postfixExpArrLen = exp.postfixExpArrLen;
	postfixExpArr = new ExpNode[postfixExpArrLen];
	for (int i = 0; i < postfixExpArrLen; i++)
		postfixExpArr[i] = exp.postfixExpArr[i];

	this->exp = new char[strlen(exp.exp) + 1];
	strcpy(this->exp, exp.exp);

	variableFormat = new char[strlen(exp.variableFormat) + 1];
	strcpy(variableFormat, exp.variableFormat);

	newTemp = new double[postfixExpArrLen];
}

inline void Expression::SetExpression(const char exp[], const char variableFormat[]) {
	this->Clear();
	this->exp = new char[strlen(exp) + 1];
	this->variableFormat = new char[strlen(variableFormat) + 1];
	strcpy(this->exp, exp);
	strcpy(this->variableFormat, variableFormat);
	ExpressionScan();

#ifdef DEBUG
	infixExpStack.Print();
#endif // DEBUG

	TransformInfixToPostfix(infixExpStack);

#ifdef DEBUG
	postfixExpStack.Print();
#endif // DEBUG

	PostfixStack_To_PostfixArr();
}

inline void Expression::SetVariableFormat(const char variableFormat[]) {
	if (this->variableFormat) delete[] this->variableFormat;
	this->variableFormat = new char[strlen(variableFormat) + 1];
	strcpy(this->variableFormat, variableFormat);
	for (int i = 0; i < postfixExpArrLen; i++) {
		if (postfixExpArr[i].isOpt && postfixExpArr[i].prio == INF && postfixExpArr[i].pCalMean0 == nullptr)
			postfixExpArr[i].SetVariableNum(this->variableFormat);
	}
}

inline double Expression::Calculate(const double variable[]) {
	//Ϊ���Ч�ʣ��˴�ȫ��ʹ��������ʽ�ľ�̬ջ
	int n=0;
	for (int i = 0; i < postfixExpArrLen; i++)
	{
		if (!postfixExpArr[i].isOpt)
			newTemp[n++] = postfixExpArr[i].data;
		else {
			switch (postfixExpArr[i].eleNum)
			{
			case 0:
				newTemp[n++] = postfixExpArr[i].Calculate(variable);
				continue;
			case 1:
				newTemp[--n] = postfixExpArr[i].Calculate(newTemp[n]);
				++n;
				continue;
			case 2:
				n -= 2;
				newTemp[n] = postfixExpArr[i].Calculate(newTemp[n], newTemp[n + 1]);
				++n;
				continue;
			}
		}
	}
	return newTemp[0];
}

inline void Expression::Clear() {
	if (exp) { delete[] exp; exp = nullptr; }
	if (variableFormat) { delete[] variableFormat; variableFormat = nullptr; }
	if (postfixExpArr) { delete[] postfixExpArr; postfixExpArr = nullptr; }
	if (newTemp) { delete[] newTemp; newTemp = nullptr; }
	infixExpStack.Clear();
	postfixExpStack.Clear();
	temp.Clear();
}

inline void Expression::ExpressionScan() {
	char* exp = new char[strlen(this->exp) + 1];
	strcpy(exp, this->exp);
	ExpNode* temp = new ExpNode[strlen(exp)];
	int* flag = new int[strlen(exp)];
	for (unsigned int i = 0; i < strlen(exp); i++) flag[i] = -1;
	int n = 0;

	//ɨ�������ı�
	char* data = new char[strlen(exp) + 1];
	data[0] = '\0';
	for (int i = 0; ; i++) {
		if ((exp[i] >= '0'&&exp[i] <= '9') || exp[i] == '.') {
			char a[2] = { exp[i],'\0' };
			strcat(data, a);
			exp[i] = ' ';
		}
		else if (data[0]) {
			double dat;
			sscanf(data, "%lf", &dat);
			temp[n] = ExpNode(dat);
			flag[n] = i - 1;
			data[0] = '\0';
			++n;
		}
		if (exp[i] == '\0') break;
	}
	delete[] data;

	//ɨ����Ա��������
	for (int i = 0; i < sizeof(templateOptArr.optArr) / sizeof(ExpNode); i++) {
		char* p = strstr(exp, templateOptArr.optArr[i].opt);
		while (p) {
			temp[n] = templateOptArr.optArr[i];
			flag[n] = p - exp;
			int x = strlen(temp[n].opt);
			for (unsigned int j = 0; j < strlen(temp[n].opt); j++) *(p + j) = ' ';
			p = strstr(exp, templateOptArr.optArr[i].opt);
			++n;
		}
	}

	//��ʣ���ֽڹ�Ϊ�Ա���
	char*p = strtok(exp, " ");
	while (p) {
		temp[n] = ExpNode(p);
		temp[n].SetVariableNum(variableFormat);
		flag[n] = p - exp;
		p = strtok(NULL, " ");
		++n;
	}

	//��С��������
	bool isChange = true;
	while (isChange) {
		isChange = false;
		for (int i = 0; i < n - 1; i++) {
			if (flag[i]>flag[i + 1]) {
				ExpNode tempNode = temp[i];
				temp[i] = temp[i + 1];
				temp[i + 1] = tempNode;
				isChange = true;
				int tempInt = flag[i];
				flag[i] = flag[i + 1];
				flag[i + 1] = tempInt;
			}
		}
	}

	//������׺ջ
	for (int i = 0; i < n; i++) {
		//���+��-�ڱ��ʽͷ����ǰ��Ϊ�������ҷ������ţ�+��-����ΪһԪ�����1+��1-
		if (temp[i] == "+" || temp[i] == "-") {
			if (i == 0 || (!temp[i - 1].IsData() && temp[i - 1] != ")")) {
				char opt[3] = { '1',temp[i].opt[0] ,'\0' };
				temp[i] = ExpNode(opt);
			}
		}

		infixExpStack.Push(temp[i]);

		//������ݻ������ź���Ϊ�Ƕ�Ԫ������ҷ������ţ���һ���˺�
		if (temp[i].IsData() || temp[i] == ")") {
			if (i + 1 < n) {
				if (temp[i + 1].eleNum != 2 && temp[i + 1] != ")")
					infixExpStack.Push(ExpNode("*"));
			}
		}
	}
	infixExpStack.Invert();

	//�����ڴ�
	delete[] exp;
	delete[] temp;
	delete[] flag;
}

inline bool Expression::Postfix_TempAction() {
	if (temp.IsEmpty()) return false;

	ExpNode node = temp.Pop();
	if (postfixExpStack.GetTop().prio == 0) postfixExpStack.Pop();
	else if (postfixExpStack.GetTop().prio < node.prio) temp.Push(postfixExpStack.Pop());

	return postfixExpStack.Push(node);
}

inline void Expression::TransformInfixToPostfix(ExpStack & infixExpStack) {
	//������׺���ʽ
	while (!infixExpStack.IsEmpty()) {
		ExpNode node = infixExpStack.Pop();

		if (node.eleNum == 0) {														//����׺ջ��ȡ��0Ԫ�����
			if (node.isOpt && !strcmp(node.opt, "(")) {								//ȡ�õ���������

				Expression innerExp;
				innerExp.TransformInfixToPostfix(infixExpStack);					//��������׺���ʽת������뱾�����׺ջ��
				while (!innerExp.postfixExpStack.IsEmpty())
					postfixExpStack.Push(innerExp.postfixExpStack.Pop());

				postfixExpStack.Push(")", 0, 0);									//����һ�������Ž��

			}
			else if (node.isOpt && !strcmp(node.opt, ")")) 	break;					//ȡ�õ���������
			else postfixExpStack.Push(node);

			while (Postfix_TempAction()) { ; };
		}
		else if (node.eleNum == 1){
			temp.Push(node);
		}
		else {
			if (postfixExpStack.GetTop().prio == 0) postfixExpStack.Pop();
			else if (postfixExpStack.GetTop().prio < node.prio) temp.Push(postfixExpStack.Pop());
			temp.Push(node);
		}

	}

	if (postfixExpStack.GetTop().prio == 0) postfixExpStack.Pop();
	while (!temp.IsEmpty())
		postfixExpStack.Push(temp.Pop());
	postfixExpStack.Invert();
}

inline void Expression::PostfixStack_To_PostfixArr() {
	postfixExpArrLen = postfixExpStack.GetLen();
	postfixExpArr = new ExpNode[postfixExpArrLen];
	newTemp = new double[postfixExpArrLen];
	for (int i = 0; !postfixExpStack.IsEmpty(); i++) {
		postfixExpArr[i] = postfixExpStack.Pop();
	}
}
