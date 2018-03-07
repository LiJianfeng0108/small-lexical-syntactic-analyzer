//:syntacticAnalyzer.h
//语法分析头文件
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

extern vector<int> vid;//LexicalAnalyzer.cpp声明定义的
int i = 0;
int sym;
int lineNum = 1;//句号
bool pass = true;//是否通过语法检测标志


void SyntacticAnalyzer()
{
	sym = vid[i];

	while(i<vid.size())//每以结束符结束本句话语法分析后, 继续下面的语法分析, 直至全部分析完毕
	{
		
		E();
		//if(pass&&(vid[i] == 14))//当前是;且pass为true才输出无错误
		//	cout<<"第 "<<lineNum<<" 句没有语法错误\n";
		//cout<<i<<endl;
		if(vid[i] == 14)//如果当前是;则句号加1
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
	if((sym == 0) || (sym == 1))//0是+, 1是-
	{
		Advance();
		T();
		E1();
	}
	else if((sym == 11) || (sym == 14))//sym属于follow(e1)={),#} (假设把;当作#, ;的id是14)
	{
		
	}
	else
	{
		pass = false;
		cout<<"第 "<<lineNum<<" 句语法错误, 类型: errorE1, 出现未知运算符\n";
		
	}
}

void T()
{
	F();
	T1();
}

void T1()
{
	if((sym == 2) || (sym == 3))//2是*, 3是/
	{
		Advance();
		F();
		T1();
	}
	else if((sym == 0) || (sym == 1) || (sym == 11) || (sym == 14))//sym属于follow(t1)={+, -, ), #} (假设把;当作#, ;的id是14)
	{
		
	}
	else
	{
		pass = false;
		cout<<"第 "<<lineNum<<" 句语法错误, 类型: errorT1, 没有使用正确的运算符\n";
	}
}

void F()
{
	if(sym == 10)//10是(
	{
		Advance();
		E();
		if(sym == 11)////11是)
			Advance();
		else
		{
			pass = false;
			//错误类型判断
			if((vid[i-1]==30) || (vid[i-1]==31))
				cout<<"第 "<<lineNum<<" 句语法错误, 类型: errorF, 缺少)\n";
			else
				cout<<"第 "<<lineNum<<" 句语法错误, 类型: errorF, 没有使用正确的运算符\n";
		}

	}
	else if((sym == 30) || (sym == 31) )//30是标识符ide, 31是常数num
		Advance();
	else
	{
		pass = false;
		//错误类型判断
		if(vid[i-1]==11)
			cout<<"第 "<<lineNum<<" 句语法错误, 类型: errorFF, 缺少(\n";
		else if((vid[i-1]==0) || (vid[i-1]==1) || (vid[i-1]==2) || (vid[i-1]==3))
			cout<<"第 "<<lineNum<<" 句语法错误, 类型: errorFF, 缺少标识符/常数\n";
		else if((vid[i-1]==30) || (vid[i-1]==31))
			cout<<"第 "<<lineNum<<" 句语法错误, 类型: errorFF, 缺少标识符/常数\n";
		else
			cout<<"第 "<<lineNum<<" 句语法错误, 类型: errorFF, 没有使用正确的运算符\n";
	}
}