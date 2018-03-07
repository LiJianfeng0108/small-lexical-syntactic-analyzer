//:LexicalAnalyzer.cpp
//����ԭ���һ�׶Σ��ʷ�����
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

vector<string> vline;//���δ�����һ���о���
vector<string> vword;//����Է���Ĵ�
vector<string> vnum;//�������
vector<string> vide;//��ű�ʶ��
vector<int> vid;//������
vector<int> vvalue;//�������

int id;//���


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


//�жϿո�
bool isSpace(char c)
{
	if(c == 32)
		return true;
	return false;
}
//�ж��ַ�
bool isLet(char c)
{
	if((c>=65 && c<=90) ||(c>=97 &&c<=122))
		return true;
	return false;
}
//�ж�����
bool isNum(char c)
{
	if(c>=48 && c<=57)
		return true;
	return false;
}
//�жϽ�������
bool isOpOrSep(char c)
{
	if((c>=40 && c<=45) || (c==47) ||(c>=59 &&c<=62) || (c==123) || (c==125))
		return true;
	return false;
}
//�����ֱ�id
int getId(string s)
{
	//�����
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
	//���
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
	//�ؼ���
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
	else if(isNum(s[0]))//������ֻҪ�ַ����������������ַ�������������
		return 31;
	//��ʶ��
	return 30;


}
//�ʷ�����
void analyzeLexical()
{
	string word;
	//�жϲ��ִ�
	for(int i = 0; i < vline.size(); i++)
	{	
		int j = 0;
		
		while(j<vline[i].length())
		{	
			//�жϿո�
			while(isSpace(vline[i][j]))
			{
				j++;
			}
			
			//�ж���ĸ
			if(isLet(vline[i][j]))
			{
				while(isLet(vline[i][j])||isNum(vline[i][j]))
				{
					word += vline[i][j];
					j++;
						
				}
			}
			if(word!="")//���wordΪ�գ���˵��wordû�������ݣ�����vector�������
			{
				vword.push_back(word);
				word = "";
				continue;
			}
			
			//�ж�����
			while(isNum(vline[i][j]))
			{
				word += vline[i][j];
				j++;
			}
			if(word!="")//ͬ��
			{
				vword.push_back(word);
				word = "";
				continue;
			}
			
			//�жϽ�������������==��>=��<=
			while(isOpOrSep(vline[i][j]))
			{
				if((vline[i][j] == '>' || vline[i][j] == '<' || vline[i][j] == '=')//�жϴ��ַ� 
					&& (j+1)<vline[i].length() && (vline[i][j+1] == '=')//�ж���һ���ַ�
					&& (word.length()<=1))//������������
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
			
			//δ֪�ַ�ֱ�ӱ���
			if((!isLet(vline[i][j])&&!isNum(vline[i][j])&&!isOpOrSep(vline[i][j])&&!isSpace(vline[i][j]))
				&&j<vline[i].length())//���жϺܹؼ�,���жϵ��ַ���ĳ�����һ��
			{
				cout<<"��"<<i+1<<"����δ֪�ַ�"<<endl;
				exit(0);
			}
		}	
	}
	
}
//������ʶ�������������ԣ�����
void createTable()
{
	//������Դ���vector
	int ideCount = 0;//��ʶ�����ж�Ӧλ��
	int numCount = 0;//�������ж�Ӧλ��
	int count = -1;//-1��ʾ��ʶ������û�ظ�,����ֵΪ�ֱ���������λ��

	for(int i = 0; i < vword.size(); i++)
	{
		id = getId(vword[i]);
		vid.push_back(id);//�������id

		//��������value����������ʶ���ͳ�����
		if(id<30)//������,���,�ؼ��ֵ�����value
			vvalue.push_back(0);
		else if(id==30)//��ʶ��
		{
			for(int j = 0; j < vide.size(); j++)
			{
				if(vide[j] == vword[i])//��ʶ�����Ѿ���Ŵ˵���
				{
					count = j;
					break;
				}
			}
			if(count == -1)//δ�ظ�
			{
				vide.push_back(vword[i]);
				vvalue.push_back(ideCount);
				ideCount++;
			}
			else//�ظ�
			{
				vvalue.push_back(count);
				count = -1;
			}
				
		}
		else//����
		{
			for(int j = 0; j < vnum.size(); j++)
			{
				if(vnum[j] == vword[i])//�������Ѿ���Ŵ˵���
				{
					count = j;
					break;
				}
			}
			if(count == -1)//δ�ظ�
			{
				vnum.push_back(vword[i]);
				vvalue.push_back(numCount);
				numCount++;
			}
			else//�ظ�
			{
				vvalue.push_back(count);
				count = -1;
			}
		}

	}
}
//�����ļ�txt
void createFile()
{
	ofstream out("�ʷ��������.txt");
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
	//�ʷ�����:
	//string path = "LexicalAnalyzer.cpp";
	ifstream in("����������.txt");
	string line;
	
	//��һ���л�ȡ��������
	while(getline(in, line))
		vline.push_back(line);
	
	//�ʷ�����(������==,=�Ĳ�ͬ����,δ֪�ַ�����)
	analyzeLexical();
		
	//�������
	cout<<"------------------------------------------------------------------------------"<<endl;
	cout<<"�﷨����: ********************************************************************"<<endl<<endl;
	cout<<"���"<<"\t"<<"ֵ"<<endl;
	for(int j = 0; j < vword.size(); j++)
	{
		id = getId(vword[j]);
		PR(id, vword[j]);
	}
	cout<<"��Ϊ�����ݲ���...\n\n\n";
	
	//������ʶ�������������ԣ������������ظ����ݵĴ���
	createTable();
	
	//�����������
	vector<LAResult> vrs;
	LAResult rs;
	cout<<"���"<<"\t"<<"����"<<endl;
	for(int l = 0; l < vword.size(); l++)
	{	
		vrs.push_back(rs);
		vrs[l].id = vid[l];
		vrs[l].value = vvalue[l];
		vrs[l].output();
	}
	cout<<"�����ʼ�����..."<<endl;

	//�����ļ�txt
	createFile();
	cout<<"\n��������ѵ���..."<<endl;
	
	cout<<"\n\n���ֵ:\nǰ30:����õ����,���,�ؼ���,\n����:30�����ʶ��,31������.\n����ֵ:\n"
		<<"�㵰:���,���,�ؼ��ֵ�Ĭ��ֵ,\n����:Ϊ��ʶ��,�����ڶ�Ӧ��λ��."<<endl<<endl;
	cout<<"------------------------------------------------------------------------------"<<endl;



	//�﷨����
	cout<<"�﷨����: ********************************************************************"<<endl<<endl<<"�Ե�һ��������ʾΪ׼\n"<<endl;
	SyntacticAnalyzer();

	
}///:~
