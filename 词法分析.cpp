//�ʷ�������C����ʵ��

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include <cctype>
using namespace std;

const int MAX_INPUT_BUFFER = 512;//Ĭ�����뻺����

ifstream file;//���ļ�

//vector<string>keyword;//Ԥ�ȼ�¼�ؼ���

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

vector<string>comment;//�洢ע��
map<string, int> id;//��ʶ��
vector<string>pre;//Ԥ�����ļ�

int sum_char = 0;//�ַ���
int sum_word = 0;//������
int sum_line = 0;//����
int current_line = 0;//��ǰ����

bool in_comment = false;//��ʶ �����Ƿ���ע����

char buffer[MAX_INPUT_BUFFER];//������

int big_bracket = 0;//������
int small_bracket = 0;//С����

void sum(void)//�����ַ��� ���� ��ע��
{
	file.open("test.c");
	int state = 0;//״̬ת�� 0:���� 1:����һ��/  2:����/*
	char temp;//�ݴ�
	string temp_comment;//�ݴ�ע��
	file.seekg(0);//�ļ�ָ��ص�ͷ
	while (!file.eof())//����ȫ��
	{

		switch (state)
		{
		case 0:
			file.get(temp);
			sum_char++;//�����ַ�
			if ('\n' == temp)
				sum_line++;//������
			if ('/' == temp)
				state = 1;
			break;
		case 1://ǰһ���ַ��� /
			file.get(temp);
			sum_char++;//�����ַ�
			if ('/' == temp)//����ע��
			{
				temp_comment.clear();//���ע�ͻ���
				while ('\n' != temp)//��������
				{
					file.get(temp);
					sum_char++;//�����ַ�
					temp_comment.append(1, temp);//���ע��
				}
				sum_line++;//������
				comment.push_back(temp_comment);//��ӵ�ע�ͼ�¼��
				state = 0;//״̬�ع�
			}
			else
				if ('*' == temp)//����ע��
				{
					temp_comment.clear();
					state = 2;
				}
				else//����ע��
				{
					if ('\n' == temp)
						sum_line++;//������
					state = 0; //״̬�ع�
				}
			break;
		case 2:
			file.get(temp);
			sum_char++;//�����ַ�
			if ('*' == temp)
			{
				file.get(temp);//��ȡһ��
				sum_char++;//�����ַ�
				if ('/' == temp)//��������ע��
				{
					comment.push_back(temp_comment);//����ע�ͱ�
					state = 0;
				}
				else//����ע����
				{
					temp_comment.append(1, '*');
					temp_comment.append(1, temp);
					if ('\n' == temp)
						sum_line++;
				}
			}
			else
			{
				if (file.eof())//��ע�͵�βʱ
				{
					if (!temp_comment.empty())//��Ϊ��ʱ�������һ��ע��
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
	string temp_word;//�ݴ浥��
	char *ptr = buffer;//����ָ��

	while (in_comment)//����ע������
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
		if (' ' == *ptr)//���ո�
			ptr++;
		else if ('\t' == *ptr)
			ptr++;
		else if (isalpha(unsigned(*ptr)) || '_' == *ptr)//�ؼ��ֺͱ�ʶ��
		{
			temp_word.clear();
			while (isalnum(*ptr) || '_' == *ptr)
			{
				temp_word.append(1, *ptr);
				ptr++;
			}//�ո�������ַ�ʱ����
			sum_word++;

			map<string, int>::iterator ltr;
			ltr = keyword.find(temp_word);
			if (ltr != keyword.end()) {
				ltr->second++;
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �����֣�" << temp_word << endl;
			}
			else {
				ltr = id.find(temp_word);
				if (ltr != id.end())
					ltr->second++;
				else
					id.insert(pair<string, int>(temp_word, 1));
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ��ʶ����" << temp_word << endl;
			}
		}

		else if (isdigit(unsigned(*ptr)))//����
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

				cout << endl << "��" << sum_word << "�����ʣ�" << "  �Ƿ��ַ���" << temp_word << endl;
				cout << "error:�ڵ�" << current_line << "��,����" << temp_word << "�����Ƿ���" << endl;
			}
			else
			{
				if ('.' == *ptr || 'E' == *ptr || 'e' == *ptr)//С����ָ����ʽ
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

				cout << endl << "��" << sum_word << "�����ʣ�" << "  �޷�������" << temp_word << endl;
			}
		}
		else if ('#' == *ptr)//Ԥ�����ļ�
		{
			while ('\0' != *ptr)
			{
				temp_word.append(1, *ptr);
				ptr++;
			}
			pre.push_back(temp_word);
		}

		else if ('"' == *ptr)//�ַ���
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
			cout << endl << "��" << sum_word << "�����ʣ�" << "  �ַ�����" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �ӵ��ںţ�" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �ԼӺţ�" << temp_word << endl;
				ptr += 2;
			}
			else if (isdigit(*(ptr + 1)))//�з�����
			{
				temp_word = "+";
				for (int j = 1; isdigit(*(ptr + j)); j++)
					temp_word.append(1, *(ptr + j));

				sum_word++;
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �з�������" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �Ӻţ�" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �Լ��ţ�" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �����ںţ�" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ָ��ţ�" << temp_word << endl;
				ptr += 2;
			}
			else if (isdigit(*(ptr + 1)))//�з�����
			{
				temp_word = "-";
				for (int j = 1; isdigit(*(ptr + j)); j++)
					temp_word.append(1, *(ptr + j));

				sum_word++;
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �з�������" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ���ţ�" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �˵��ںţ�" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �˺ţ�" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �����ںţ�" << temp_word << endl;
				ptr += 2;
			}
			else if ('/' == *(ptr + 1))
				return;//��ע��ֱ������
			else if ('*' == *(ptr + 1))//����ע��
			{
				in_comment = true;
				ptr += 2;
				while ('\0' != *ptr)//���н���ע���Ƿ�Ҳ����
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ���ţ�" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ��ȱȽϺţ�" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ��ֵ�ţ�" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ���ڵ��ںţ�" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ���ںţ�" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  С�ڵ��ںţ�" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  С�ںţ�" << temp_word << endl;
				ptr += 2;//�ƽ�
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  �����ںţ�" << temp_word << endl;
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
				cout << endl << "��" << sum_word << "�����ʣ�" << "  ȡ���ţ�" << temp_word << endl;
				ptr += 2;//�ƽ�
			}
		}

		else if (':' == *ptr || '(' == *ptr || ')' == *ptr || ';' == *ptr || '{' == *ptr || '}' == *ptr || ',' == *ptr
			|| '[' == *ptr || ']' == *ptr || '\0' == *ptr || '\n' == *ptr)//��㲻�㵥�� 
		{

			if ('(' == *ptr)
				small_bracket++;//���
			if (')' == *ptr)
				small_bracket--;//���
			if ('{' == *ptr)
				big_bracket++;//���
			if ('}' == *ptr)
				big_bracket--;//��� 
			ptr++;//�ƽ� 
		}
		else
		{
			cout << endl << "����:�ڵ�" << current_line << "���д���" << *ptr << endl;
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
		if ('\n' == *p)//һ��
		{
			*(p + 1) = '\0';
			*(p + 2) = '\0';
			current_line++;
			word_analyse();
			if (small_bracket != 0)
			{
				cout << endl << "����:�ڵ�" << current_line << "��С���Ų�ƥ��" << endl;
				small_bracket = 0;
			}
			p = buffer;//�������
		}
		else
			p++;
	}
	if (big_bracket != 0)
		cout << endl << "����:�����Ų�ƥ�䣡" << endl;
}

void show(void)
{
	cout << endl;
	cout << "�����ż����ִ���ͳ�����£�" << endl;
	map<string, int>::iterator ltr;
	for (ltr = keyword.begin(); ltr != keyword.end(); ltr++)
		if (ltr->second != 0)
			cout << ltr->first << "\t\t" << ltr->second << endl;

	for (ltr = id.begin(); ltr != id.end(); ltr++)
		cout << ltr->first << "\t\t" << ltr->second << endl;

	cout << endl << "��������:" << sum_word << endl;
	cout << endl << "�ַ�����:" << sum_char << endl;
	cout << endl << "������is:" << sum_line << endl;
	cout << "ͷ�ļ���:" << endl;
	for (int i = 0; i<pre.size(); i++)
	{
		cout << pre.at(i);
	}
	cout << "ע����:" << endl;
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
		cout << endl << "�Ҳ����ļ���" << endl;
	else
	{
		//��ʼ���� 
		
		analyse();
		file.close();
		sum();//ͳ������ �ֽ��� ע�� 
		show();//��ʾͳ�ƽ�� 

	}

	system("pause");
	return 0;

}


