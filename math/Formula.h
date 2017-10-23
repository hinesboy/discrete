#pragma once
/**
* 合式公式运算类
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

typedef map<char , int> mapElements; // 命题map
typedef mapElements::value_type mapValue; // 命题map的元素类型
typedef mapElements::iterator mapIt;

typedef stack<int> stackValue; // 真值栈
typedef stack<char> stackOperator; // 运算符栈

class Formula
{
	static mapElements operatorPRI; // 算符优先级算法对照集合
	static mapElements operatorPRIByFix;// 逆波兰优先级对照集合
private:
	string express; // 合式公式字符串
	string subfixExpress;// 合式公式后缀表达式
	intValues value; // 用int类型的bit 计算真值表
	int elementLength; // 命题实际个数(去重)
	mapElements elements; // {命题:真值}
	stackValue values; // 存放value的栈 - 用于优先级运算
	stackOperator operators; // 存放运算符的栈 - 用于优先级运算

public:
	string xqForm; // 主析取范式
	string hqForm; // 主和取范式

private:
	void resolveExpress(); // 解析合式公式获取命题信息
	void incValues(); // value自增根据bit位获取命题真值
	void printElementValue(); // cout输出当前命题的真值
	void prependForm(bool , string &); // 合成析取和取范式字符串
	void switchAlgo(bool); // 选择使用何种算法运算结果 , true：算符优先级

	// 算符优先级算法
    bool algoExpress();// 核心算法
	void pushStack(string::iterator &,string::iterator &,string::iterator & , string::iterator &, int & , bool); // 入栈运算
	void incIterator(string::iterator &,string::iterator &,string::iterator & , string::iterator & , bool); // 指针自增
	void popStack(int &); // 出栈运算
	
	// 逆波兰算法
	bool algoExpressByFix(); // 逆波兰命题出栈运算
	void getSubFixExpress(); // 将合式公式转化为后缀表达式

public:
	Formula();
	Formula(string);
	void algoXHForm(); // 算符优先级算法
	void algoXHFormByFix(); // 逆波兰算法
	~Formula();

	friend std::ostream & operator << (std::ostream & , Formula &); // 重载输出运算符
	friend std::istream & operator >> (std::istream & , Formula &); // 重载输入运算符
};

