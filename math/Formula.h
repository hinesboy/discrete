#pragma once
/**
* ��ʽ��ʽ������
*/
#include"header.h"
#include<vector>
#include<stack>
#include<map>
#include<regex>

using std::vector;
using std::stack;
using std::map;
using std::regex;
using std::regex_replace;
using std::to_string;

typedef map<char , int> mapElements; // ����map
typedef mapElements::value_type mapValue; // ����map��Ԫ������
typedef mapElements::iterator mapIt;

typedef stack<int> stackValue; // ��ֵջ
typedef stack<char> stackOperator; // �����ջ

class Formula
{
	static mapElements operatorPRI; // ��������ȼ����ռ���
private:
	string express; // ��ʽ��ʽ�ַ���
	intValues value; // ��int���͵�bit ������ֵ��
	int elementLength; // ����ʵ�ʸ���(ȥ��)
	mapElements elements; // {����:��ֵ}
	stackValue values; // ���value��ջ - �������ȼ�����
	stackOperator operators; // ����������ջ - �������ȼ�����

public:
	string xqForm; // ����ȡ��ʽ
	string hqForm; // ����ȡ��ʽ

private:
	void resolveExpress(); // ������ʽ��ʽ
	void algoValues(); // ������ֵ��
	void incValues(); // value��������bitλ��ȡ������ֵ
	bool algoExpress();// �����㷨
	void pushStack(string::iterator &,string::iterator &,string::iterator & , string::iterator &, int & , bool); // ��ջ����
	void incIterator(string::iterator &,string::iterator &,string::iterator & , string::iterator & , bool); // ָ������
	void popStack(int &); // ��ջ����
	void printElementValue(); // �����ǰ�������ֵ
	void prependForm(bool , string &);
	

public:
	Formula();
	Formula(string);
	void algoXHForm(); // ��ȡ��ȡ��ʽ
	~Formula();

	friend std::ostream & operator << (std::ostream & , Formula &); // ������������
	friend std::istream & operator >> (std::istream & , Formula &); // �������������
};

