#include "Formula.h"
// ************辅助函数**************** 
// 幂计算
int _pow(int a , int b)
{
	int c = a;
	if (b == 0)
	{
		return 1;
	}
	for (int i = 1 ; i < b; i ++)
	{
		a *= c;
	}
	return a;
}
// 判断字符是否为大写字母
bool isUpper(char c)
{
	if (c >= 'A' && c<= 'Z')
	{
		return true;
	}
	return false;
}
// 操作符运算
int operatorAlgo(char op , int num1 , int num2)
{
	int result;
	switch (op)
	{
	case '&':
		result = num1 & num2;
		break;
	case '|':
		result = num1 | num2;
		break;
	case '^':
		if (num1 && !num2)
		{
			result = 0;
		}
		else
		{
			result = 1;
		}
		break;
	case '~':
		if (num1 == num2)
		{
			result = 1;
		}
		else
		{
			result = 0;
		}
		break;
	default:
		break;
	}
	return result;
}
// ***************非算法成员函数***********************
mapElements Formula::operatorPRI = { {'(' , 6} , {'&' , 5} , {'|' , 4}  , {'^' , 3} , {'~' , 2} , {')' , 1} }; // 优先级对照集合
mapElements Formula::operatorPRIByFix = { {'!' , 6}, {'&' , 5} , {'|' , 4}  , {'^' , 3} , {'~' , 2} , {')' , 1} ,{'(' , 0} }; // 优先级对照集合

Formula::Formula():value(-1),elementLength(0)
{
}
Formula::Formula(string express):express(express),value(-1),elementLength(0)
{
	// TODO: 入参检测
	resolveExpress(); // 解析express公式
}

Formula::~Formula()
{
}

// 解析express公式 获取其 命题（去重）信息
void Formula::resolveExpress()
{
	int length = express.size();
	for (int i = 0 ; i < length ; i++)
	{
		char c = express[i];
		if (isUpper(c)) // 是命题
		{
			// 已经存在的命题
			if (elements.count(c) == 0)
			{
				elementLength++;
				elements.insert({c , 0});
			}
		}
	}
}

// value自加修改当前命题真值
void Formula::incValues()
{
	value++;
	int i = 0;
	mapIt mapit = elements.begin();
	while (mapit != elements.end())
	{
		int temp = value & _pow(2, i); // 按位与 计算当前位置为 0 / 1
		mapit->second = temp > 0 ? 1 : 0;
		mapit++;
		i++;
	}
}
// 合成析取和取范式
void Formula::prependForm(bool flag , string & str)
{
	string item = "(";
	mapIt mapit = elements.begin();
	while (mapit != elements.end())
	{
		if (!(mapit->second ^ flag))
		{
			item += mapit->first;
		}
		else
		{
			item += "!";
			item += mapit->first;
		}
		item += flag ? '&':'|';
		mapit++;
	}
	item.replace(item.size() - 1,1,")");
	if (str.empty())
	{
		str.insert(0,item);
	}
	else
	{
		str.insert(0 , item + (flag?'|':'&'));
	}
}

// cout 输出当前命题取值
void Formula::printElementValue()
{
	mapIt mapit = elements.begin();
	cout << "命题取值: " << flush;
	while (mapit != elements.end())
	{
		cout << mapit->first << " : " << mapit->second << " , " << flush;
		mapit++;
	}
	cout << endl;
}
// **计算法则算法
void Formula::switchAlgo(bool flag)
{
	xqForm.clear();
	hqForm.clear();
	value = 0;
	int times = _pow(2 , elementLength);
	for (int i = 0 ; i < times ; i ++)
	{
		incValues(); // 自增命题真值
		// 优先级算法计算出表达式为 T / F 
		bool result;
		if (flag)
		{
			result = algoExpress();
		}
		else
		{
			result = algoExpressByFix();
		}
		printElementValue();
		cout << "表达式结果: " << result << endl << endl;
		if (result) // 当前取值情况成立
		{
			prependForm(true,xqForm);
		}
		else
		{
			prependForm(false,hqForm);
		}
	}
}
// ***********算符优先级算法*********************
void Formula::algoXHForm()
{
	switchAlgo(true);
}
// iterator 自增
void Formula::incIterator(string::iterator & leftvalue, string::iterator & leftsign, string::iterator & rightvalue, string::iterator & rightsign, bool flag)
{
	if (flag)
	{
		leftsign = rightsign;
		leftvalue = rightvalue;
		if (rightvalue < express.end())
		{
			rightvalue++;
		}
		if (rightsign < express.end())
		{
			rightsign++;
		}
	}
	else
	{
		leftsign++;
		leftvalue++;
	}
	if (rightvalue < express.end())
	{
		rightvalue++;
	}
	if (rightsign < express.end())
	{
		rightsign++;
	}
}
// 入栈运算
void Formula::pushStack(string::iterator & leftvalue , string::iterator & leftsign ,string::iterator & rightvalue ,string::iterator & rightsign , int & result , bool flag )
{
	// leftvalue 和 leftsign 入栈
	if (flag)
	{
		// left入栈 , )不放入栈中
		if (*leftsign != ')')
		{
			operators.push(*leftsign);
		}
		if (result < 0)
		{
			values.push(elements[*leftvalue]);
		}
		else
		{
			values.push(result);
		}
		result = -1; // 重置result
	}
	// 仅符号入栈 ! / （
	else
	{
		operators.push(*leftvalue);
	}
	incIterator(leftvalue , leftsign , rightvalue , rightsign , flag);
}
// 出栈运算
void Formula::popStack(int & result)
{
	char op = operators.top();
	operators.pop();
	while (op != '(')
	{
		int value = values.top();
		values.pop();
		// 运算 reuslt = result $ value
		result = operatorAlgo(op , result , value);
		op = operators.top();
		operators.pop();
	}
	if (!operators.empty())
	{
		op = operators.top();
		if (op =='!') //!(A$B)
		{
			operators.pop();
			// result = ！result
			result = !result;
		}
	}
}
// 算符优先级算法
bool Formula::algoExpress()
{
	string::iterator leftsign, rightsign , leftvalue , rightvalue; 
	leftvalue = express.begin();
	leftsign = leftvalue + 1;
	rightvalue = leftsign + 1;
	rightsign = rightvalue + 1;
	int result = - 1;
	while (leftsign < express.end()) // 结束标志
	{
		// 注：一个命题后面一定是运算符或者结束符（end / )）
		while (!isUpper(*leftvalue))  // A
		{
		
			if (*leftvalue == '!' && isUpper(*(leftsign))) // !A直接计算  !(A 入栈
			{
				// 进行非运算 , 不入栈
				result = !elements[*leftsign];
				incIterator(leftvalue, leftsign, rightvalue, rightsign, false);
			} 
			else
			{
				pushStack(leftvalue , leftsign , rightvalue , rightsign ,result , false);
			}
		}
		if (*leftsign ==')') // A)
		{
			if (result < 0 ) // (A)
			{
				result = elements[*leftvalue];
			}
			popStack(result);
			// )))...循环
			while (rightvalue < express.end() && *rightvalue == ')')
			{
				incIterator(leftvalue, leftsign, rightvalue, rightsign, false);
				popStack(result);
			 }
			// 出栈运算
		}
		if (rightvalue < express.end() && isUpper(*rightvalue) && operatorPRI[*leftsign] >= (rightsign == express.end()?operatorPRI[*leftsign]:operatorPRI[*rightsign])) // A$B$
		{
			// A$B 运算 ， 不入栈
			result = operatorAlgo(*leftsign , result < 0 ? elements[*leftvalue] : result , elements[*rightvalue]);
			// 迭代器自增
			incIterator(leftvalue , leftsign , rightvalue , rightsign , true);
		}
		else if (rightvalue == express.end())
		{
			// A) 不运算 不入栈
			incIterator(leftvalue , leftsign , rightvalue , rightsign , true);
		}
		else
		{
			// A$B( 不运算入栈
			pushStack(leftvalue , leftsign , rightvalue , rightsign , result , true);
		}
	}
	// 取出栈内数据进行运算
	while (!operators.empty() && !values.empty())
	{
		int value = values.top();
		values.pop();
		char op = operators.top();
		operators.pop();
		result = operatorAlgo(op , result , value);
	}
	return result?true:false;
}

// *******************逆波兰（后缀）算法********************
void Formula::algoXHFormByFix()
{
	// 获取后缀表达式
	getSubFixExpress();
	switchAlgo(false);
}

bool Formula::algoExpressByFix()
{
	stackValue valStack;
	string::iterator exip = subfixExpress.begin();
	while (exip < subfixExpress.end())
	{
		if (isUpper(*exip))
		{
			valStack.push(elements[*exip]);
		}
		else
		{
			if (*exip == '!')
			{
				int val = valStack.top();
				valStack.pop();
				valStack.push(!val);
			} 
			else
			{
				int val1 = valStack.top();
				valStack.pop();
				int val2 = valStack.top();
				valStack.pop();
				valStack.push(operatorAlgo(*exip , val1 , val2));
			}
		}
		exip++;
	}
	return valStack.top();
}
void Formula::getSubFixExpress()
{
	stackOperator opStack;
	string::iterator exip = express.begin();
	while (exip < express.end())
	{
		if (isUpper(*exip))
		{
			subfixExpress += *exip;
		}
		else
		{
			// 判断是否直接入栈，空栈和（ 直接入栈
			if (!opStack.empty() && *exip != '(')
			{
				// 判断是否需要出栈，*exip的优先级低时候需要先出栈
				while (!opStack.empty() && operatorPRIByFix[*exip] <= operatorPRIByFix[opStack.top()])
				{
					subfixExpress += opStack.top();
					opStack.pop();
				}
				if (*exip == ')')
				{
					opStack.pop();
				}
				else
				{
					opStack.push(*exip);
				}
			}
			else
			{
				opStack.push(*exip);
			}
		}
		exip++;
	}

	while (!opStack.empty())
	{
		subfixExpress += opStack.top();
		opStack.pop();
	}
}
// *******************友元重载/输入输出**********************
// operator >>
std::istream & operator >> (std::istream & is, Formula & formula)
{
	cout << "请输入合式公式（命题[A-Z]、运算符[!&|^~]）: " << flush;
	cin >> formula.express; 
	formula.resolveExpress(); // 解析公式
	return is;
}
// operator << 
std::ostream & operator << (std::ostream & os , Formula & formula)
{
	os <<"合式公式: "<< formula.express << endl;
	cout << "主析取范式: " << formula.xqForm << endl;
	cout << "主和取范式: " << formula.hqForm << endl;
	return os;
}