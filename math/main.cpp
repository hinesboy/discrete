#include"header.h"
#include"Formula.h"
int main()
{
	Formula fomula; // �����ʽ��ʽ��
	cin >> fomula; // �����ʽ��ʽ

	cout << endl << "***********������ȼ��㷨:" << endl;
	fomula.algoXHForm(); // ������ȡ��ȡ��ʽ , ������ȼ��㷨
	cout << fomula; // ���

	cout << endl << "***********�沨���㷨:" << endl;
	fomula.algoXHFormByFix(); // �沨���㷨
	cout << fomula; // ���

	system("pause");
	return 0;
}