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
using std::regex_match;
using std::to_string;

typedef map<char , int> mapElements; // ����map
typedef mapElements::value_type mapValue; // ����map��Ԫ������
typedef mapElements::iterator mapIt;

typedef stack<int> stackValue; // ��ֵջ
typedef stack<char> stackOperator; // �����ջ

class Formula
{
	static mapElements operatorPRI; // ������ȼ��㷨���ռ���
	static mapElements operatorPRIByFix;// �沨�����ȼ����ռ���
private:
	string express; // ��ʽ��ʽ�ַ���
	string subfixExpress;// ��ʽ��ʽ��׺���ʽ
	intValues value; // ��int���͵�bit ������ֵ��
	int elementLength; // ����ʵ�ʸ���(ȥ��)
	mapElements elements; // {����:��ֵ}
	stackValue values; // ���value��ջ - �������ȼ�����
	stackOperator operators; // ����������ջ - �������ȼ�����

public:
	string xqForm; // ����ȡ��ʽ
	string hqForm; // ����ȡ��ʽ

private:
	void resolveExpress(); // ������ʽ��ʽ��ȡ������Ϣ
	void incValues(); // value��������bitλ��ȡ������ֵ
	void printElementValue(); // cout�����ǰ�������ֵ
	void prependForm(bool , string &); // �ϳ���ȡ��ȡ��ʽ�ַ���
	void switchAlgo(bool); // ѡ��ʹ�ú����㷨������ , true��������ȼ�

	// ������ȼ��㷨
    bool algoExpress();// �����㷨
	void pushStack(string::iterator &,string::iterator &,string::iterator & , string::iterator &, int & , bool); // ��ջ����
	void incIterator(string::iterator &,string::iterator &,string::iterator & , string::iterator & , bool); // ָ������
	void popStack(int &); // ��ջ����
	
	// �沨���㷨
	bool algoExpressByFix(); // �沨�������ջ����
	void getSubFixExpress(); // ����ʽ��ʽת��Ϊ��׺���ʽ

public:
	Formula();
	Formula(string);
	void algoXHForm(); // ������ȼ��㷨
	void algoXHFormByFix(); // �沨���㷨
	~Formula();

	friend std::ostream & operator << (std::ostream & , Formula &); // ������������
	friend std::istream & operator >> (std::istream & , Formula &); // �������������
};

