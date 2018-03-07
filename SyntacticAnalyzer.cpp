//:syntacticAnalyzer.h
//�﷨����ͷ�ļ�
#include<iostream>
#include<vector>
using namespace std;

void SyntacticAnalyzer();
void Advance();
void E();
void E1();
void T();
void T1();
void F();

extern vector<int> vid;//LexicalAnalyzer.cpp���������
int i = 0;
int sym;
int lineNum = 1;//���
bool pass = true;//�Ƿ�ͨ���﷨����־


void SyntacticAnalyzer()
{
	sym = vid[i];

	while(i<vid.size())//ÿ�Խ������������仰�﷨������, ����������﷨����, ֱ��ȫ���������
	{
		
		E();
		//if(pass&&(vid[i] == 14))//��ǰ��;��passΪtrue������޴���
		//	cout<<"�� "<<lineNum<<" ��û���﷨����\n";
		//cout<<i<<endl;
		if(vid[i] == 14)//�����ǰ��;���ż�1
			lineNum++;
		pass = true;
		Advance();
	}
}

void Advance()
{
	i++;
	sym = vid[i];
}

void E()
{
	T();
	E1();
}

void E1()
{
	if((sym == 0) || (sym == 1))//0��+, 1��-
	{
		Advance();
		T();
		E1();
	}
	else if((sym == 11) || (sym == 14))//sym����follow(e1)={),#} (�����;����#, ;��id��14)
	{
		
	}
	else
	{
		pass = false;
		cout<<"�� "<<lineNum<<" ���﷨����, ����: errorE1, ����δ֪�����\n";
		
	}
}

void T()
{
	F();
	T1();
}

void T1()
{
	if((sym == 2) || (sym == 3))//2��*, 3��/
	{
		Advance();
		F();
		T1();
	}
	else if((sym == 0) || (sym == 1) || (sym == 11) || (sym == 14))//sym����follow(t1)={+, -, ), #} (�����;����#, ;��id��14)
	{
		
	}
	else
	{
		pass = false;
		cout<<"�� "<<lineNum<<" ���﷨����, ����: errorT1, û��ʹ����ȷ�������\n";
	}
}

void F()
{
	if(sym == 10)//10��(
	{
		Advance();
		E();
		if(sym == 11)////11��)
			Advance();
		else
		{
			pass = false;
			//���������ж�
			if((vid[i-1]==30) || (vid[i-1]==31))
				cout<<"�� "<<lineNum<<" ���﷨����, ����: errorF, ȱ��)\n";
			else
				cout<<"�� "<<lineNum<<" ���﷨����, ����: errorF, û��ʹ����ȷ�������\n";
		}

	}
	else if((sym == 30) || (sym == 31) )//30�Ǳ�ʶ��ide, 31�ǳ���num
		Advance();
	else
	{
		pass = false;
		//���������ж�
		if(vid[i-1]==11)
			cout<<"�� "<<lineNum<<" ���﷨����, ����: errorFF, ȱ��(\n";
		else if((vid[i-1]==0) || (vid[i-1]==1) || (vid[i-1]==2) || (vid[i-1]==3))
			cout<<"�� "<<lineNum<<" ���﷨����, ����: errorFF, ȱ�ٱ�ʶ��/����\n";
		else if((vid[i-1]==30) || (vid[i-1]==31))
			cout<<"�� "<<lineNum<<" ���﷨����, ����: errorFF, ȱ�ٱ�ʶ��/����\n";
		else
			cout<<"�� "<<lineNum<<" ���﷨����, ����: errorFF, û��ʹ����ȷ�������\n";
	}
}