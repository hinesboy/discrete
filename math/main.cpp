#include"header.h"
#include"Formula.h"
int main()
{
	Formula fomula; // 定义合式公式类
	cin >> fomula; // 输入合式公式

	fomula.algoXHForm();

	cout << fomula;
	system("pause");
	return 0;
}