#include "Formula.h"

mapElements Formula::operatorPRI = { {'(' , 6} , {'&' , 5} , {'|' , 4}  , {'^' , 3} , {'~' , 2} , {')' , 1} }; // ���ȼ����ռ���
Formula::Formula():value(-1),elementLength(0)
{
}
Formula::Formula(string express):express(express),value(-1),elementLength(0)
{
	// TODO: ��μ��
	resolveExpress(); // ����express��ʽ
}

Formula::~Formula()
{
}
// �ж��ַ��Ƿ�Ϊ��д��ĸ
bool isUpper(char c)
{
	if (c >= 'A' && c<= 'Z')
	{
		return true;
	}
	return false;
}
// ����express��ʽ ��ȡ�� ���⣨ȥ�أ���Ϣ
void Formula::resolveExpress()
{
	int length = express.size();
	for (int i = 0 ; i < length ; i++)
	{
		char c = express[i];
		if (isUpper(c)) // ������
		{
			// �Ѿ����ڵ�����
			if (elements.count(c) == 0)
			{
				elementLength++;
				elements.insert({c , 0});
			}
		}
	}
}
// �ݼ���
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
// value�Լ��޸ĵ�ǰ������ֵ
void Formula::incValues()
{
	value++;
	int i = 0;
	mapIt mapit = elements.begin();
	while (mapit != elements.end())
	{
		int temp = value & _pow(2, i); // ��λ�� ���㵱ǰλ��Ϊ 0 / 1
		mapit->second = temp > 0 ? 1 : 0;
		mapit++;
		i++;
	}
}
// ����������
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
// iterator ����
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
// ��ջ����
void Formula::pushStack(string::iterator & leftvalue , string::iterator & leftsign ,string::iterator & rightvalue ,string::iterator & rightsign , int & result , bool flag )
{
	// leftvalue �� leftsign ��ջ
	if (flag)
	{
		// left��ջ , )������ջ��
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
		result = -1; // ����result
	}
	// ��������ջ ! / ��
	else
	{
		operators.push(*leftvalue);
	}
	incIterator(leftvalue , leftsign , rightvalue , rightsign , flag);
}
// ��ջ����
void Formula::popStack(int & result)
{
	char op = operators.top();
	operators.pop();
	while (op != '(')
	{
		int value = values.top();
		values.pop();
		// ���� reuslt = result $ value
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
			// result = ��result
			result = !result;
		}
	}
}
// ������ȼ��㷨
bool Formula::algoExpress()
{
	string::iterator leftsign, rightsign , leftvalue , rightvalue; 
	leftvalue = express.begin();
	leftsign = leftvalue + 1;
	rightvalue = leftsign + 1;
	rightsign = rightvalue + 1;
	int result = - 1;
	while (leftsign < express.end()) // ������־
	{
		// ע��һ���������һ������������߽�������end / )��
		while (!isUpper(*leftvalue))  // A
		{
		
			if (*leftvalue == '!' && isUpper(*(leftsign))) // !Aֱ�Ӽ���  !(A ��ջ
			{
				// ���з����� , ����ջ
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
			// )))...ѭ��
			while (rightvalue < express.end() && *rightvalue == ')')
			{
				incIterator(leftvalue, leftsign, rightvalue, rightsign, false);
				popStack(result);
			 }
			// ��ջ����
		}
		if (rightvalue < express.end() && isUpper(*rightvalue) && operatorPRI[*leftsign] >= (rightsign == express.end()?operatorPRI[*leftsign]:operatorPRI[*rightsign])) // A$B$
		{
			// A$B ���� �� ����ջ
			result = operatorAlgo(*leftsign , result < 0 ? elements[*leftvalue] : result , elements[*rightvalue]);
			// ����������
			incIterator(leftvalue , leftsign , rightvalue , rightsign , true);
		}
		else if (rightvalue == express.end())
		{
			// A) ������ ����ջ
			incIterator(leftvalue , leftsign , rightvalue , rightsign , true);
		}
		else
		{
			// A$B( ��������ջ
			pushStack(leftvalue , leftsign , rightvalue , rightsign , result , true);
		}
	}
	// ȡ��ջ�����ݽ�������
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
// **���㷨���㷨
void Formula::algoValues()
{
	int times = _pow(2 , elementLength);
	for (int i = 0 ; i < times ; i ++)
	{
		incValues(); // ����������ֵ
		// ���ȼ��㷨���������ʽΪ T / F 
		bool result = algoExpress();
		printElementValue();
		cout << "����ʽ���: " << result << endl;
		if (result) // ��ǰȡֵ�������
		{
			prependForm(true,xqForm);
		}
		else
		{
			prependForm(false,hqForm);
		}
	}
}
void Formula::prependForm(bool flag , string & str)
{
	string item = "(";
	mapIt mapit = elements.begin();
	while (mapit != elements.end())
	{
		if (mapit->second)
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
// ��ȡ����ȡ\��ȡ��ʽ
void Formula::algoXHForm()
{
	algoValues();
}

// cout �����ǰ����ȡֵ
void Formula::printElementValue()
{
	mapIt mapit = elements.begin();
	cout << "����ȡֵ: " << flush;
	while (mapit != elements.end())
	{
		cout << mapit->first << " : " << mapit->second << " , " << flush;
		mapit++;
	}
	cout << endl;
}
// ��μ��


// operator >>
std::istream & operator >> (std::istream & is, Formula & formula)
{
	cout << "�������ʽ��ʽ������[A-Z]�������[!&|^~]��: " << flush;
	cin >> formula.express; // TODO: ��μ��

	formula.resolveExpress(); // ������ʽ
	return is;
}
// operator << 
std::ostream & operator << (std::ostream & os , Formula & formula)
{
	os << formula.express << endl;
	cout << "����ȡ��ʽ: " << formula.xqForm << endl;
	cout << "����ȡ��ʽ: " << formula.hqForm << endl;
	return os;
}