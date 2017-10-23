#include"header.h"
#include"Formula.h"
int main()
{
	Formula fomula; // 定义合式公式类
	cin >> fomula; // 输入合式公式

	cout << endl << "***********算符优先级算法:" << endl;
	fomula.algoXHForm(); // 计算析取和取范式 , 算符优先级算法
	cout << fomula; // 输出

	cout << endl << "***********逆波兰算法:" << endl;
	fomula.algoXHFormByFix(); // 逆波兰算法
	cout << fomula; // 输出

	system("pause");
	return 0;
}