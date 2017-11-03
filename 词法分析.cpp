//词法分析器C语言实现

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include <cctype>
using namespace std;

const int MAX_INPUT_BUFFER = 512;//默认输入缓存区

ifstream file;//打开文件

//vector<string>keyword;//预先记录关键字

map<string, int> keyword = {
	{ "auto",0 },
	{ "double",0 },
	{ "int",0 },
	{ "struct",0 },
	{ "break",0 },
	{ "else",0 },
	{ "long",0 },
	{ "switch",0 },
	{ "case",0 },
	{ "enum",0 },
	{ "register",0 },
	{ "typedef",0 },
	{ "char",0 },
	{ "extern",0 },
	{ "return",0 },
	{ "union",0 },
	{ "const",0 },
	{ "float",0 },
	{ "short",0 },
	{ "unsigned",0 },
	{ "continue",0 },
	{ "for",0 },
	{ "signed",0 },
	{ "void",0 },
	{ "default",0 },
	{ "goto",0 },
	{ "sizeof",0 },
	{ "volatile",0 },
	{ "do",0 },
	{ "if",0 },
	{ "while",0 },
	{ "static",0 }
};

vector<string>comment;//存储注释
map<string, int> id;//标识符
vector<string>pre;//预处理文件

int sum_char = 0;//字符数
int sum_word = 0;//单词数
int sum_line = 0;//行数
int current_line = 0;//当前行数

bool in_comment = false;//标识 此行是否在注释中

char buffer[MAX_INPUT_BUFFER];//缓存区

int big_bracket = 0;//大括号
int small_bracket = 0;//小括号

void sum(void)//计算字符数 行数 存注释
{
	file.open("test.c");
	int state = 0;//状态转换 0:正常 1:输入一个/  2:输入/*
	char temp;//暂存
	string temp_comment;//暂存注释
	file.seekg(0);//文件指针回到头
	while (!file.eof())//遍历全文
	{

		switch (state)
		{
		case 0:
			file.get(temp);
			sum_char++;//增加字符
			if ('\n' == temp)
				sum_line++;//增加行
			if ('/' == temp)
				state = 1;
			break;
		case 1://前一个字符是 /
			file.get(temp);
			sum_char++;//增加字符
			if ('/' == temp)//单行注释
			{
				temp_comment.clear();//清空注释缓存
				while ('\n' != temp)//输完整行
				{
					file.get(temp);
					sum_char++;//增加字符
					temp_comment.append(1, temp);//添加注释
				}
				sum_line++;//行增加
				comment.push_back(temp_comment);//添加到注释记录表
				state = 0;//状态回归
			}
			else
				if ('*' == temp)//多行注释
				{
					temp_comment.clear();
					state = 2;
				}
				else//不是注释
				{
					if ('\n' == temp)
						sum_line++;//增加行
					state = 0; //状态回归
				}
			break;
		case 2:
			file.get(temp);
			sum_char++;//增加字符
			if ('*' == temp)
			{
				file.get(temp);//再取一个
				sum_char++;//增加字符
				if ('/' == temp)//结束多行注释
				{
					comment.push_back(temp_comment);//存入注释表
					state = 0;
				}
				else//还在注释中
				{
					temp_comment.append(1, '*');
					temp_comment.append(1, temp);
					if ('\n' == temp)
						sum_line++;
				}
			}
			else
			{
				if (file.eof())//当注释到尾时
				{
					if (!temp_comment.empty())//不为空时存入最后一个注释
					{
						comment.push_back(temp_comment);
						return;
					}
				}
				if ('\n' == temp)
					sum_line++;
				temp_comment.append(1, temp);

			}
			break;

		default:
			break;
		}
	}
}

void word_analyse(void)
{
	string temp_word;//暂存单词
	char *ptr = buffer;//处理指针

	while (in_comment)//多行注释问题
	{
		if ('\0' == *ptr)
			return;
		if (('*' == *ptr) && ('/' == *(ptr + 1)))
		{
			ptr += 2;
			in_comment = false;
		}
		ptr++;
	}
	while ('\0' != *ptr)
	{
		if (' ' == *ptr)//消空格
			ptr++;
		else if ('\t' == *ptr)
			ptr++;
		else if (isalpha(unsigned(*ptr)) || '_' == *ptr)//关键字和标识符
		{
			temp_word.clear();
			while (isalnum(*ptr) || '_' == *ptr)
			{
				temp_word.append(1, *ptr);
				ptr++;
			}//空格或其他字符时弹出
			sum_word++;

			map<string, int>::iterator ltr;
			ltr = keyword.find(temp_word);
			if (ltr != keyword.end()) {
				ltr->second++;
				cout << endl << "第" << sum_word << "个单词：" << "  保留字：" << temp_word << endl;
			}
			else {
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));
				cout << endl << "第" << sum_word << "个单词：" << "  标识符：" << temp_word << endl;
			}
		}

		else if (isdigit(unsigned(*ptr)))//数字
		{
			temp_word.clear();
			while (isdigit(unsigned(*ptr)))
			{
				temp_word.append(1, *ptr);
				ptr++;
			}
			if (isalpha(unsigned(*ptr)) || '_' == *ptr)
			{
				while (' ' == *ptr && '\0' == *ptr)
				{
					temp_word.append(1, *ptr);
					ptr++;
				}
				sum_word++;

				cout << endl << "第" << sum_word << "个单词：" << "  非法字符：" << temp_word << endl;
				cout << "error:在第" << current_line << "行,单词" << temp_word << "命名非法！" << endl;
			}
			else
			{
				if ('.' == *ptr || 'E' == *ptr || 'e' == *ptr)//小数和指数形式
				{
					temp_word.append(1, *ptr);
					ptr++;
					while (isdigit(*ptr))
					{
						temp_word.append(1, *ptr);
						ptr++;
					}
				}
				sum_word++;

				cout << endl << "第" << sum_word << "个单词：" << "  无符号数：" << temp_word << endl;
			}
		}
		else if ('#' == *ptr)//预处理文件
		{
			while ('\0' != *ptr)
			{
				temp_word.append(1, *ptr);
				ptr++;
			}
			pre.push_back(temp_word);
		}

		else if ('"' == *ptr)//字符串
		{
			temp_word.clear();
			ptr++;
			while ('"' != *ptr)
			{
				temp_word.append(1, *ptr);
				ptr++;
			}
			ptr++;
			sum_word++;
			cout << endl << "第" << sum_word << "个单词：" << "  字符串：" << temp_word << endl;
		}

		else if ('+' == *ptr)
		{
			temp_word.clear();
			if ('+' == *(ptr + 1))
			{
				temp_word = "++";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  加等于号：" << temp_word << endl;
				ptr += 2;
			}
			else if ('=' == *(ptr + 1))
			{
				temp_word = "+=";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  自加号：" << temp_word << endl;
				ptr += 2;
			}
			else if (isdigit(*(ptr + 1)))//有符号数
			{
				temp_word = "+";
				for (int j = 1; isdigit(*(ptr + j)); j++)
					temp_word.append(1, *(ptr + j));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  有符号数：" << temp_word << endl;
				ptr += 2;//推进
			}
			else
			{
				temp_word = "+";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  加号：" << temp_word << endl;
				ptr += 2;//推进
			}
		}

		else if ('-' == *ptr)
		{
			temp_word.clear();
			if ('-' == *(ptr + 1))
			{
				temp_word = "--";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  自减号：" << temp_word << endl;
				ptr += 2;
			}
			else if ('=' == *(ptr + 1))
			{
				temp_word = "-=";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  减等于号：" << temp_word << endl;
				ptr += 2;
			}
			else if ('>' == *(ptr + 1))
			{
				temp_word = "->";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  指针号：" << temp_word << endl;
				ptr += 2;
			}
			else if (isdigit(*(ptr + 1)))//有符号数
			{
				temp_word = "-";
				for (int j = 1; isdigit(*(ptr + j)); j++)
					temp_word.append(1, *(ptr + j));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  有符号数：" << temp_word << endl;
				ptr += 2;//推进
			}
			else
			{
				temp_word = "-";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  减号：" << temp_word << endl;
				ptr += 2;//推进
			}
		}

		else if ('*' == *ptr)
		{
			temp_word.clear();
			if ('=' == *(ptr + 1))
			{
				temp_word = "*=";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  乘等于号：" << temp_word << endl;
				ptr += 2;
			}
			else
			{
				temp_word = "*";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  乘号：" << temp_word << endl;
				ptr += 2;//推进
			}
		}
		else if ('/' == *ptr)
		{
			//temp_word.clear();
			if ('=' == *(ptr + 1))
			{
				temp_word = "/=";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  除等于号：" << temp_word << endl;
				ptr += 2;
			}
			else if ('/' == *(ptr + 1))
				return;//行注释直接跳出
			else if ('*' == *(ptr + 1))//多行注释
			{
				in_comment = true;
				ptr += 2;
				while ('\0' != *ptr)//此行结束注释是否也结束
				{
					if (('*' == *ptr) && ('/' == *(ptr + 1)))
					{
						in_comment = false;
						break;
					}
					ptr++;
				}
			}
			else
			{
				temp_word = "/";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  除号：" << temp_word << endl;
				ptr += 2;//推进
			}
		}

		else if ('=' == *ptr)
		{
			temp_word.clear();
			if ('=' == *(ptr + 1))
			{
				temp_word = "==";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  相等比较号：" << temp_word << endl;
				ptr += 2;
			}
			else
			{
				temp_word = "=";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  赋值号：" << temp_word << endl;
				ptr += 2;//推进
			}
		}

		else if ('>' == *ptr)
		{
			temp_word.clear();
			if ('=' == *(ptr + 1))
			{
				temp_word = ">=";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  大于等于号：" << temp_word << endl;
				ptr += 2;
			}
			else
			{
				temp_word = ">";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  大于号：" << temp_word << endl;
				ptr += 2;//推进
			}
		}

		else if ('<' == *ptr)
		{
			temp_word.clear();
			if ('=' == *(ptr + 1))
			{
				temp_word = "<=";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  小于等于号：" << temp_word << endl;
				ptr += 2;
			}
			else
			{
				temp_word = "<";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  小于号：" << temp_word << endl;
				ptr += 2;//推进
			}
		}

		else if ('!' == *ptr)
		{
			temp_word.clear();
			if ('=' == *(ptr + 1))
			{
				temp_word = "!=";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  不等于号：" << temp_word << endl;
				ptr += 2;
			}
			else
			{
				temp_word = "!";

				map<string, int>::iterator ltr;
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));

				sum_word++;
				cout << endl << "第" << sum_word << "个单词：" << "  取反号：" << temp_word << endl;
				ptr += 2;//推进
			}
		}

		else if (':' == *ptr || '(' == *ptr || ')' == *ptr || ';' == *ptr || '{' == *ptr || '}' == *ptr || ',' == *ptr
			|| '[' == *ptr || ']' == *ptr || '\0' == *ptr || '\n' == *ptr)//标点不算单词 
		{

			if ('(' == *ptr)
				small_bracket++;//查错
			if (')' == *ptr)
				small_bracket--;//查错
			if ('{' == *ptr)
				big_bracket++;//查错
			if ('}' == *ptr)
				big_bracket--;//查错 
			ptr++;//推进 
		}
		else
		{
			cout << endl << "错误:在第" << current_line << "行有错误：" << *ptr << endl;
			ptr++;
		}
	}
}

void analyse(void)
{
	char *p = buffer;
	file.seekg(0);
	while (!file.eof())
	{
		file.get(*p);
		if ('\n' == *p)//一句
		{
			*(p + 1) = '\0';
			*(p + 2) = '\0';
			current_line++;
			word_analyse();
			if (small_bracket != 0)
			{
				cout << endl << "错误:在第" << current_line << "行小括号不匹配" << endl;
				small_bracket = 0;
			}
			p = buffer;//重新填充
		}
		else
			p++;
	}
	if (big_bracket != 0)
		cout << endl << "错误:大括号不匹配！" << endl;
}

void show(void)
{
	cout << endl;
	cout << "各符号及出现次数统计如下：" << endl;
	map<string, int>::iterator ltr;
	for (ltr = keyword.begin(); ltr != keyword.end(); ltr++)
		if (ltr->second != 0)
			cout << ltr->first << "\t\t" << ltr->second << endl;

	for (ltr = id.begin(); ltr != id.end(); ltr++)
		cout << ltr->first << "\t\t" << ltr->second << endl;

	cout << endl << "单词总数:" << sum_word << endl;
	cout << endl << "字符总数:" << sum_char << endl;
	cout << endl << "总行数is:" << sum_line << endl;
	cout << "头文件有:" << endl;
	for (int i = 0; i<pre.size(); i++)
	{
		cout << pre.at(i);
	}
	cout << "注释有:" << endl;
	for (int i = 0; i<comment.size(); i++)
	{
		cout << comment.at(i);
	}
	cout << endl;

}

int main(void)
{


	file.open("test.c");
	if (!file)
		cout << endl << "找不到文件！" << endl;
	else
	{
		//开始处理 
		
		analyse();
		file.close();
		sum();//统计行数 字节数 注释 
		show();//显示统计结果 

	}

	system("pause");
	return 0;

}


