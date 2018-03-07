//:LexicalAnalyzer.cpp
//编译原理第一阶段：词法分析
//{L}syntacticAnalyzer.h
#include "syntacticAnalyzer.h"
#include<iostream>
#include<fstream>
#include<string>
#include <strstream>
#include<vector>
using namespace std;

#define PR(arg1, arg2) \
	cout<<"( "<<arg1<<",\t"<<arg2<<" )"<<endl;

vector<string> vline;//存放未处理的一行行句子
vector<string> vword;//存放以分组的词
vector<string> vnum;//存放整数
vector<string> vide;//存放标识符
vector<int> vid;//存放类别
vector<int> vvalue;//存放属性

int id;//类别


class LAResult
{
public:
	int id;
	int value;
public:
	LAResult();
	LAResult(int idd, int val);
	output();
};
LAResult::LAResult(int idd, int val)
{
	id = idd;
	value = val;
}
LAResult::LAResult()
{
	id = -1;
	value = -1;
}
LAResult::output()
{
	PR(id, value);
}


//判断空格
bool isSpace(char c)
{
	if(c == 32)
		return true;
	return false;
}
//判断字符
bool isLet(char c)
{
	if((c>=65 && c<=90) ||(c>=97 &&c<=122))
		return true;
	return false;
}
//判断数字
bool isNum(char c)
{
	if(c>=48 && c<=57)
		return true;
	return false;
}
//判断界符和算符
bool isOpOrSep(char c)
{
	if((c>=40 && c<=45) || (c==47) ||(c>=59 &&c<=62) || (c==123) || (c==125))
		return true;
	return false;
}
//返回种别id
int getId(string s)
{
	//运算符
	if(s=="+")
		return 0;
	else if(s=="-")
		return 1;
	else if(s=="*")
		return 2;
	else if(s=="/")
		return 3;
	else if(s=="=")
		return 4;
	else if(s=="==")
		return 5;
	else if(s==">")
		return 6;
	else if(s==">=")
		return 7;
	else if(s=="<")
		return 8;
	else if(s=="<=")
		return 9;
	//界符
	else if(s=="(")
		return 10;
	else if(s==")")
		return 11;
	else if(s=="{")
		return 12;
	else if(s=="}")
		return 13;
	else if(s==";")
		return 14;
	else if(s==",")
		return 15;
	//关键字
	else if(s=="main")
		return 16;
	else if(s=="int")
		return 17;
	else if(s=="char")
		return 18;
	else if(s=="string")
		return 19;
	else if(s=="void")
		return 20;
	else if(s=="while")
		return 21;
	else if(s=="for")
		return 22;
	else if(s=="if")
		return 23;
	else if(s=="else")
		return 24;
	else if(s=="ifstream")
		return 25;
	else if(s=="ofstream")
		return 26;
	else if(s=="cout")
		return 27;
	else if(s=="cin")
		return 28;
	else if(s=="class")
		return 29;
	else if(isNum(s[0]))//整数、只要字符串包含整数，此字符串就是整数串
		return 31;
	//标识符
	return 30;


}
//词法分析
void analyzeLexical()
{
	string word;
	//判断并分词
	for(int i = 0; i < vline.size(); i++)
	{	
		int j = 0;
		
		while(j<vline[i].length())
		{	
			//判断空格
			while(isSpace(vline[i][j]))
			{
				j++;
			}
			
			//判断字母
			if(isLet(vline[i][j]))
			{
				while(isLet(vline[i][j])||isNum(vline[i][j]))
				{
					word += vline[i][j];
					j++;
						
				}
			}
			if(word!="")//如果word为空，则说明word没存入数据，则不向vector添加数据
			{
				vword.push_back(word);
				word = "";
				continue;
			}
			
			//判断数字
			while(isNum(vline[i][j]))
			{
				word += vline[i][j];
				j++;
			}
			if(word!="")//同上
			{
				vword.push_back(word);
				word = "";
				continue;
			}
			
			//判断界符和算符，包括==，>=，<=
			while(isOpOrSep(vline[i][j]))
			{
				if((vline[i][j] == '>' || vline[i][j] == '<' || vline[i][j] == '=')//判断此字符 
					&& (j+1)<vline[i].length() && (vline[i][j+1] == '=')//判断下一个字符
					&& (word.length()<=1))//最多存两个长度
				{
					
					word += vline[i][j];
					word += vline[i][j+1];
					j = j+2;
				}
				else
				{
					word += vline[i][j];
					j++;
				}
				vword.push_back(word);
				word = "";
				continue;
			}
			
			//未知字符直接报错
			if((!isLet(vline[i][j])&&!isNum(vline[i][j])&&!isOpOrSep(vline[i][j])&&!isSpace(vline[i][j]))
				&&j<vline[i].length())//此判断很关键,如判断的字符在某行最后一个
			{
				cout<<"第"<<i+1<<"行有未知字符"<<endl;
				exit(0);
			}
		}	
	}
	
}
//建立标识符，常数表，属性，类别表
void createTable()
{
	//类别、属性存入vector
	int ideCount = 0;//标识符表中对应位置
	int numCount = 0;//常数表中对应位置
	int count = -1;//-1表示标识符或常数没重复,其他值为分别在两表中位置

	for(int i = 0; i < vword.size(); i++)
	{
		id = getId(vword[i]);
		vid.push_back(id);//保存类别id

		//保存属性value，并建立标识符和常数表
		if(id<30)//存放算符,界符,关键字的属性value
			vvalue.push_back(0);
		else if(id==30)//标识符
		{
			for(int j = 0; j < vide.size(); j++)
			{
				if(vide[j] == vword[i])//标识符表已经存放此单词
				{
					count = j;
					break;
				}
			}
			if(count == -1)//未重复
			{
				vide.push_back(vword[i]);
				vvalue.push_back(ideCount);
				ideCount++;
			}
			else//重复
			{
				vvalue.push_back(count);
				count = -1;
			}
				
		}
		else//常数
		{
			for(int j = 0; j < vnum.size(); j++)
			{
				if(vnum[j] == vword[i])//常数表已经存放此单词
				{
					count = j;
					break;
				}
			}
			if(count == -1)//未重复
			{
				vnum.push_back(vword[i]);
				vvalue.push_back(numCount);
				numCount++;
			}
			else//重复
			{
				vvalue.push_back(count);
				count = -1;
			}
		}

	}
}
//生成文件txt
void createFile()
{
	ofstream out("词法分析结果.txt");
	string outWord, outWord2, allWords;
	for(int i = 0; i < vword.size(); i++)
	{
		strstream soutWord, soutWord2;
		soutWord<<vid[i];
		soutWord>>outWord;
		soutWord2<<vvalue[i];
		soutWord2>>outWord2;
		allWords += "( "+outWord+",\t"+outWord2+" )"+"\n";
	}
	out<<allWords;
}

void main()
{
	//词法分析:
	//string path = "LexicalAnalyzer.cpp";
	ifstream in("待处理代码段.txt");
	string line;
	
	//先一行行获取到容器中
	while(getline(in, line))
		vline.push_back(line);
	
	//词法分析(包括对==,=的不同处理,未知字符处理)
	analyzeLexical();
		
	//测试输出
	cout<<"------------------------------------------------------------------------------"<<endl;
	cout<<"语法分析: ********************************************************************"<<endl<<endl;
	cout<<"类别"<<"\t"<<"值"<<endl;
	for(int j = 0; j < vword.size(); j++)
	{
		id = getId(vword[j]);
		PR(id, vword[j]);
	}
	cout<<"作为下数据参照...\n\n\n";
	
	//建立标识符，常数表，属性，类别表（包括对重复数据的处理）
	createTable();
	
	//建立对象，输出
	vector<LAResult> vrs;
	LAResult rs;
	cout<<"类别"<<"\t"<<"属性"<<endl;
	for(int l = 0; l < vword.size(); l++)
	{	
		vrs.push_back(rs);
		vrs[l].id = vid[l];
		vrs[l].value = vvalue[l];
		vrs[l].output();
	}
	cout<<"对象初始化完毕..."<<endl;

	//生成文件txt
	createFile();
	cout<<"\n分析结果已导出..."<<endl;
	
	cout<<"\n\n类别值:\n前30:定义好的算符,界符,关键字,\n其他:30代表标识符,31代表常数.\n属性值:\n"
		<<"零蛋:算符,界符,关键字的默认值,\n其他:为标识符,常数在对应表位置."<<endl<<endl;
	cout<<"------------------------------------------------------------------------------"<<endl;



	//语法分析
	cout<<"语法分析: ********************************************************************"<<endl<<endl<<"以第一个错误提示为准\n"<<endl;
	SyntacticAnalyzer();

	
}///:~
