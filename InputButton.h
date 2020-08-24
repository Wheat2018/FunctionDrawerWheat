/*�����ߣ���ۼ��߿����Ӷ�*/
/*�������ڣ�2017��01��16��*/

//ע���ò��ִ��뱾Ӧ����MouseEvent.h��������Գ����ʶ�������
#pragma once
#include "MouseEvent.h"
//��By.�����ģ����Ķ���

void ExpBoxPaintMonitor(RECT ExpBoxRect, Expression& exp, bool angle)
{
	if (RePaintExpBox) {
		/*�������ʽ��ʾ��*/
		setfillcolor(RGB(221, 221, 221));
		setlinecolor(RGB(104, 104, 104));
		settextstyle(20, 0, "����");		//����������ʽ
		settextcolor(BLACK);
		setlinestyle(PS_SOLID);				//����ʵ��

		fillrectangle(ExpBoxRect.left, ExpBoxRect.top, ExpBoxRect.right, ExpBoxRect.bottom);
		ExpBoxRect.left += 5;

		if (angle) outtextxy(ExpBoxRect.left , ExpBoxRect.top, "��(x)=");
		else outtextxy(ExpBoxRect.left , ExpBoxRect.top, "f(x)=");

		ExpBoxRect.left += textwidth("��(x)=");
		drawtext(exp.GetExpression(), &ExpBoxRect, DT_LEFT | DT_WORD_ELLIPSIS);

		FlushBatchDraw();
		RePaintExpBox = false;
	}
}
void InputBottonloading(RECT InputBottonRect)
{
	LOGFONT f = { 0 ,0,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,0,ANTIALIASED_QUALITY,0,0 };
	settextstyle(&f);					//����������ʽ

	//Step1.��¼һЩ����
	int ButtonWidth = (InputBottonRect.right - InputBottonRect.left) / 3;						//��ť���
	int ButtonHeight = (InputBottonRect.bottom - InputBottonRect.top) / 12;					//��ť�߶�

	//Step2.������
	setlinecolor(RGB(104, 104, 104));									//���û�����ɫ���߿�����滭�߶����õ���
	setfillcolor(RGB(221, 221, 221));									//���õ�ǰ�����ɫ
	fillrectangle(InputBottonRect.left, InputBottonRect.top, InputBottonRect.right - 1, InputBottonRect.bottom - 1);		//������

	InputBottonRect.top += ButtonHeight;
	//Step3.����
	line(InputBottonRect.left, InputBottonRect.top, InputBottonRect.right, InputBottonRect.top);
	line(InputBottonRect.left, InputBottonRect.top + ButtonHeight, InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + ButtonHeight);
	for (int y = InputBottonRect.top + 2 * ButtonHeight; y <= InputBottonRect.bottom - ButtonHeight; y += ButtonHeight) {
		line(InputBottonRect.left, y, InputBottonRect.left + 3 * ButtonWidth, y);//������
	}

	/*������*/
	line(InputBottonRect.left + ButtonWidth, InputBottonRect.top + ButtonHeight, InputBottonRect.left + ButtonWidth, InputBottonRect.top + 6 * ButtonHeight);
	line(InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top, InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 6 * ButtonHeight);
	line(InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 6 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 11 * ButtonHeight);

	//Step4.��ӡ�ַ�
	setbkmode(TRANSPARENT);				//�������ֱ���Ϊ͸��
	settextcolor(BLACK);

	settextstyle(15, 0, "����");		//����������ʽ
	outtextxy(
		InputBottonRect.left + 2 * ButtonWidth + ButtonWidth / 2 - textwidth("Enter") / 2,
		InputBottonRect.top + ButtonHeight - textheight("Enter") / 2,
		"Enter");
	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 2 - textwidth("BackSpace") / 2,
		InputBottonRect.top - ButtonHeight + ButtonHeight / 2 - textheight("BackSpace") / 2,
		"BackSpace");


	settextstyle(20, 0, "����");		//����������ʽ
	outtextxy(
		InputBottonRect.left + ButtonWidth - textwidth('C') / 2,			//�����ӡ���xֵ��ȡ�ð�ť���е㣬��ȥ��ӡ�ַ���ȵ�һ�룩
		InputBottonRect.top + ButtonHeight / 2 - textheight('C') / 2,			//�����ӡ���yֵ��ȡ�ð�ť���е㣬��ȥ��ӡ�ַ��߶ȵ�һ�룩
		'C');


	outtextxy(
		InputBottonRect.left + ButtonWidth / 2 - textwidth('+') / 2,
		InputBottonRect.top + ButtonHeight + ButtonHeight / 2 - textheight('+') / 2,
		'+');

	outtextxy(
		InputBottonRect.left + ButtonWidth + ButtonWidth / 2 - textwidth('-') / 2,
		InputBottonRect.top + ButtonHeight + ButtonHeight / 2 - textheight('-') / 2,
		'-');

	outtextxy(
		InputBottonRect.left + ButtonWidth / 2 - textwidth('*') / 2,
		InputBottonRect.top + 2 * ButtonHeight + ButtonHeight / 2 - textheight('*') / 2,
		'*');

	outtextxy(
		InputBottonRect.left + ButtonWidth + ButtonWidth / 2 - textwidth('/') / 2,
		InputBottonRect.top + 2 * ButtonHeight + ButtonHeight / 2 - textheight('/') / 2,
		'/');

	outtextxy(
		InputBottonRect.left + 2 * ButtonWidth + ButtonWidth / 2 - textwidth('0') / 2,
		InputBottonRect.top + 2 * ButtonHeight + ButtonHeight / 2 - textheight('0') / 2,
		'0');

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			outtextxy(
				InputBottonRect.left + ButtonWidth / 2 + j * ButtonWidth - textwidth('1' + i * 3 + j) / 2,
				InputBottonRect.top + ButtonHeight / 2 + (3 + i) * ButtonHeight - textheight('1' + i * 3 + j) / 2,
				'1' + i * 3 + j);
		}
	}

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth('(') / 2,
		InputBottonRect.top + 6 * ButtonHeight + ButtonHeight / 2 - textheight('(') / 2,
		'(');

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth(')') / 2,
		InputBottonRect.top + 6 * ButtonHeight + ButtonHeight / 2 - textheight(')') / 2,
		')');

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth("x^2") / 2,
		InputBottonRect.top + 7 * ButtonHeight + ButtonHeight / 2 - textheight("x^2") / 2,
		"x^2");

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth('^') / 2,
		InputBottonRect.top + 7 * ButtonHeight + ButtonHeight / 2 - textheight('^') / 2,
		'^');

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth("sqrt") / 2,
		InputBottonRect.top + 8 * ButtonHeight + ButtonHeight / 2 - textheight("sqrt") / 2,
		"sqrt");

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth("ln") / 2,
		InputBottonRect.top + 8 * ButtonHeight + ButtonHeight / 2 - textheight("ln") / 2,
		"ln");

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth("sin") / 2,
		InputBottonRect.top + 9 * ButtonHeight + ButtonHeight / 2 - textheight("sin") / 2,
		"sin");

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth("cos") / 2,
		InputBottonRect.top + 9 * ButtonHeight + ButtonHeight / 2 - textheight("cos") / 2,
		"cos");

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth("tan") / 2,
		InputBottonRect.top + 10 * ButtonHeight + ButtonHeight / 2 - textheight("tan") / 2,
		"tan");

	outtextxy(
		InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth('x') / 2,
		InputBottonRect.top + 10 * ButtonHeight + ButtonHeight / 2 - textheight('x') / 2,
		'x');
	FlushBatchDraw();

}

//��ť�����¼���������
bool FuncButton(Expression& exp, RECT InputBottonRect, MOUSEMSG msg)
{
	char* expChar = exp.GetExpression();
	static bool key = false;

	//Step1.��¼һЩ����
	int ButtonWidth = (InputBottonRect.right - InputBottonRect.left) / 3;						//��ť���
	int ButtonHeight = (InputBottonRect.bottom - InputBottonRect.top) / 12;					//��ť�߶�

																		//Step2.�ж��������
	switch (msg.uMsg) {
	case WM_LBUTTONDOWN:
		InputBottonRect.top += ButtonHeight;
		int x, y, m;
		x = (msg.x - InputBottonRect.left) / ButtonWidth;
		y = (msg.y - InputBottonRect.top) / ButtonHeight;
		m = (msg.x - InputBottonRect.left) / (ButtonWidth * 3 / 2);
		if (x < 3 && x >= 0 && y < 12 && y >= 0)
		{
			key = true;
			setfillcolor(RGB(104, 104, 104));
			setlinecolor(RGB(104, 104, 104));
			LOGFONT f = { 0 ,0,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,0,ANTIALIASED_QUALITY,0,0 };
			settextstyle(&f);					//����������ʽ
			setbkmode(TRANSPARENT);
			settextstyle(20, 0, "����");		//����������ʽ
			settextcolor(WHITE);

			if (msg.y - InputBottonRect.top < 0)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top - ButtonHeight, InputBottonRect.right, InputBottonRect.top);

				LOGFONT f;
				gettextstyle(&f);                     // ��ȡ��ǰ��������
				settextstyle(15, 0, "����");		//����������ʽ
				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 2 - textwidth("BackSpace") / 2,
					InputBottonRect.top - ButtonHeight + ButtonHeight / 2 - textheight("BackSpace") / 2,
					"BackSpace");
				settextstyle(&f);                     // ����������ʽ*/

				int i = -1;
				for (; expChar[i] != '\0'; ++i);
				if (i) expChar[i - 1] = '\0';

			}
			else if (x < 3 && x >= 2 && y < 2)
			{

				fillrectangle(InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top, InputBottonRect.left + 3 * ButtonWidth, InputBottonRect.top + 2 * ButtonHeight);

				LOGFONT f;
				gettextstyle(&f);                     // ��ȡ��ǰ��������
				settextstyle(15, 0, "����");		//����������ʽ
				outtextxy(
					InputBottonRect.left + 2 * ButtonWidth + ButtonWidth / 2 - textwidth("Enter") / 2,
					InputBottonRect.top + ButtonHeight - textheight("Enter") / 2,
					"Enter");
				settextstyle(&f);                     // ����������ʽ*/
				RePaintFunction = true;
				IfPaintFunction = true;
			}
			else if (x < 2 && y < 1)
			{

				fillrectangle(InputBottonRect.left, InputBottonRect.top, InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth - textwidth('C') / 2,
					InputBottonRect.top + ButtonHeight / 2 - textheight('C') / 2,
					'C');

				expChar[0] = '\0';

			}

			else if (x < 1 && y < 2 && y >= 1)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + ButtonHeight, InputBottonRect.left + ButtonWidth, InputBottonRect.top + 2 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth / 2 - textwidth('+') / 2,
					InputBottonRect.top + ButtonHeight + ButtonHeight / 2 - textheight('+') / 2,
					'+');

				strcat(expChar, "+");

			}

			else if (x < 2 && x >= 1 && y < 2 && y >= 1)
			{
				fillrectangle(InputBottonRect.left + ButtonWidth, InputBottonRect.top + ButtonHeight, InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 2 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth + ButtonWidth / 2 - textwidth('-') / 2,
					InputBottonRect.top + ButtonHeight + ButtonHeight / 2 - textheight('-') / 2,
					'-');

				strcat(expChar, "-");

			}

			else if (x < 1 && y < 3 && y >= 2)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + 2 * ButtonHeight, InputBottonRect.left + ButtonWidth, InputBottonRect.top + 3 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth / 2 - textwidth('*') / 2,
					InputBottonRect.top + 2 * ButtonHeight + ButtonHeight / 2 - textheight('*') / 2,
					'*');

				strcat(expChar, "*");

			}

			else if (x < 2 && x >= 1 && y < 3 && y >= 2)
			{
				fillrectangle(InputBottonRect.left + ButtonWidth, InputBottonRect.top + 2 * ButtonHeight, InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 3 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth + ButtonWidth / 2 - textwidth('/') / 2,
					InputBottonRect.top + 2 * ButtonHeight + ButtonHeight / 2 - textheight('/') / 2,
					'/');

				strcat(expChar, "/");

			}

			else if (x < 3 && x >= 2 && y < 3 && y >= 2)
			{
				fillrectangle(InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 2 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth, InputBottonRect.top + 3 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 2 * ButtonWidth + ButtonWidth / 2 - textwidth('0') / 2,
					InputBottonRect.top + 2 * ButtonHeight + ButtonHeight / 2 - textheight('0') / 2,
					'0');

				strcat(expChar, "0");
			}

			else if (x < 1 && y < 4 && y >= 3)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + 3 * ButtonHeight, InputBottonRect.left + ButtonWidth, InputBottonRect.top + 4 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth / 2 - textwidth('1') / 2,
					InputBottonRect.top + 3 * ButtonHeight + ButtonHeight / 2 - textheight('1') / 2,
					'1');

				strcat(expChar, "1");
			}

			else if (x < 2 && x >= 1 && y < 4 && y >= 3)
			{
				fillrectangle(InputBottonRect.left + ButtonWidth, InputBottonRect.top + 3 * ButtonHeight, InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 4 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth + ButtonWidth / 2 - textwidth('2') / 2,
					InputBottonRect.top + 3 * ButtonHeight + ButtonHeight / 2 - textheight('2') / 2,
					'2');

				strcat(expChar, "2");
			}

			else if (x < 3 && x >= 2 && y < 4 && y >= 3)
			{
				fillrectangle(InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 3 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth, InputBottonRect.top + 4 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 2 * ButtonWidth + ButtonWidth / 2 - textwidth('3') / 2,
					InputBottonRect.top + 3 * ButtonHeight + ButtonHeight / 2 - textheight('3') / 2,
					'3');

				strcat(expChar, "3");

			}

			else if (x < 1 && y < 5 && y >= 4)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + 4 * ButtonHeight, InputBottonRect.left + ButtonWidth, InputBottonRect.top + 5 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth / 2 - textwidth('1') / 2,
					InputBottonRect.top + 4 * ButtonHeight + ButtonHeight / 2 - textheight('4') / 2,
					'4');

				strcat(expChar, "4");
			}

			else if (x < 2 && x >= 1 && y < 5 && y >= 4)
			{
				fillrectangle(InputBottonRect.left + ButtonWidth, InputBottonRect.top + 4 * ButtonHeight, InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 5 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth + ButtonWidth / 2 - textwidth('8') / 2,
					InputBottonRect.top + 4 * ButtonHeight + ButtonHeight / 2 - textheight('5') / 2,
					'5');

				strcat(expChar, "5");
			}

			else if (x < 3 && x >= 2 && y < 5 && y >= 4)
			{
				fillrectangle(InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 4 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth, InputBottonRect.top + 5 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 2 * ButtonWidth + ButtonWidth / 2 - textwidth('6') / 2,
					InputBottonRect.top + 4 * ButtonHeight + ButtonHeight / 2 - textheight('6') / 2,
					'6');


				strcat(expChar, "6");

			}

			else if (x < 1 && y < 6 && y >= 5)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + 5 * ButtonHeight, InputBottonRect.left + ButtonWidth, InputBottonRect.top + 6 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth / 2 - textwidth('7') / 2,
					InputBottonRect.top + 5 * ButtonHeight + ButtonHeight / 2 - textheight('7') / 2,
					'7');

				strcat(expChar, "7");
			}

			else if (x < 2 && x >= 1 && y < 6 && y >= 5)
			{
				fillrectangle(InputBottonRect.left + ButtonWidth, InputBottonRect.top + 5 * ButtonHeight, InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 6 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + ButtonWidth + ButtonWidth / 2 - textwidth('8') / 2,
					InputBottonRect.top + 5 * ButtonHeight + ButtonHeight / 2 - textheight('8') / 2,
					'8');

				strcat(expChar, "8");

			}

			else if (x < 3 && x >= 2 && y < 6 && y >= 5)
			{
				fillrectangle(InputBottonRect.left + 2 * ButtonWidth, InputBottonRect.top + 5 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth, InputBottonRect.top + 6 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 2 * ButtonWidth + ButtonWidth / 2 - textwidth('9') / 2,
					InputBottonRect.top + 5 * ButtonHeight + ButtonHeight / 2 - textheight('9') / 2,
					'9');

				strcat(expChar, "9");

			}

			else if (m<1 && y<7 && y >= 6)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + 6 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 7 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth('(') / 2,
					InputBottonRect.top + 6 * ButtonHeight + ButtonHeight / 2 - textheight('(') / 2,
					'(');

				strcat(expChar, "(");
			}

			else if (m >= 1 && y<7 && y >= 6)
			{
				fillrectangle(InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 6 * ButtonHeight, InputBottonRect.right, InputBottonRect.top + 7 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth(')') / 2,
					InputBottonRect.top + 6 * ButtonHeight + ButtonHeight / 2 - textheight(')') / 2,
					')');

				strcat(expChar, ")");
			}

			else if (m<1 && y<8 && y >= 7)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + 7 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 8 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth("x^2") / 2,
					InputBottonRect.top + 7 * ButtonHeight + ButtonHeight / 2 - textheight("x^2") / 2,
					"x^2");

				strcat(expChar, "^2");

			}

			else if (m >= 1 && y<8 && y >= 7)
			{
				fillrectangle(InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 7 * ButtonHeight, InputBottonRect.right, InputBottonRect.top + 8 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth('^') / 2,
					InputBottonRect.top + 7 * ButtonHeight + ButtonHeight / 2 - textheight('^') / 2,
					'^');

				strcat(expChar, "^");
			}

			else if (m<1 && y<9 && y >= 8)
			{

				fillrectangle(InputBottonRect.left, InputBottonRect.top + 8 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 9 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth("sqrt") / 2,
					InputBottonRect.top + 8 * ButtonHeight + ButtonHeight / 2 - textheight("sqrt") / 2,
					"sqrt");

				strcat(expChar, "sqrt");
			}

			else if (m >= 1 && y<9 && y >= 8)
			{
				fillrectangle(InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 8 * ButtonHeight, InputBottonRect.right, InputBottonRect.top + 9 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth("ln") / 2,
					InputBottonRect.top + 8 * ButtonHeight + ButtonHeight / 2 - textheight("ln") / 2,
					"ln");

				strcat(expChar, "ln");

			}

			else if (m<1 && y<10 && y >= 9)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + 9 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 10 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth("sin") / 2,
					InputBottonRect.top + 9 * ButtonHeight + ButtonHeight / 2 - textheight("sin") / 2,
					"sin");

				strcat(expChar, "sin");

			}

			else if (m >= 1 && y<10 && y >= 9)
			{
				fillrectangle(InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 9 * ButtonHeight, InputBottonRect.right, InputBottonRect.top + 10 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth("cos") / 2,
					InputBottonRect.top + 9 * ButtonHeight + ButtonHeight / 2 - textheight("cos") / 2,
					"cos");

				strcat(expChar, "cos");
			}

			else if (m<1 && y<11 && y >= 10)
			{
				fillrectangle(InputBottonRect.left, InputBottonRect.top + 10 * ButtonHeight, InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 11 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 4 - textwidth("tan") / 2,
					InputBottonRect.top + 10 * ButtonHeight + ButtonHeight / 2 - textheight("tan") / 2,
					"tan");

				strcat(expChar, "tan");
			}

			else if (m >= 1 && y<11 && y >= 10)
			{
				fillrectangle(InputBottonRect.left + 3 * ButtonWidth / 2, InputBottonRect.top + 10 * ButtonHeight, InputBottonRect.right, InputBottonRect.top + 11 * ButtonHeight);

				outtextxy(
					InputBottonRect.left + 3 * ButtonWidth / 2 + 3 * ButtonWidth / 4 - textwidth('x') / 2,
					InputBottonRect.top + 10 * ButtonHeight + ButtonHeight / 2 - textheight('x') / 2,
					'x');

				strcat(expChar, "x");
			}


			FlushBatchDraw();
			return true;
		}
		return false;
	case WM_LBUTTONUP:
		if (key) {
			InputBottonloading(InputBottonRect);
			key = false;
		}
		return false;
	}
	return false;
}
