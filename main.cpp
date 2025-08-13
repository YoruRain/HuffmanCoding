#include "HuffmanCoding.h"

int main()
{
	char n = '#';
	while(n)
	{
		cout << "              ___________________________________________________________________________ " << endl;
		cout << "             |                              ���ݽṹ�γ����                             |" << endl;
		cout << "             |                         ����Huffman�������ļ�ѹ��                         |" << endl;
		cout << "             |                                                                           |" << endl;
		cout << "             |1.���������ı����б��롢����                                               |" << endl;
		cout << "             |                                                                           |" << endl;
		cout << "             |2.ʹ���ı��ļ����б��롢����                                               |" << endl;
		cout << "             |                                                                           |" << endl;
		cout << "             |___________________________________________________________________________|" << endl;
		cout << endl;
		cout << "������Ҫִ�еĹ��ܣ�1��2�������� Q �˳�ϵͳ����";
		cin >> n;
		switch (n)
		{
		case '1':
		{
			string str;
			cout << "��������������ı���";
			cin.ignore();
			getline(cin, str);
			stringCodingInAll(str);

			system("pause");
			system("cls");
			break;
		}
		case '2':
		{
			string text1 = "text_HighFreq.txt";
			string text2 = "text_EvenFreq.txt";
			string encoded1 = "encoded_HighFreq.txt";
			string encoded2 = "encoded_EvenFreq.txt";
			string decoded1 = "decoded_HighFreq.txt";
			string decoded2 = "dncoded_EvenFreq.txt";
			cout << "\n����ʹ��ƽ���ַ�Ƶ���ı� " << text2 << " ���б��롢����" << endl;
			cout << endl;
			system("pause");
			cout << text2 << "�ı��ļ����ݣ�" << endl;
			printFile(text2);
			cout << endl;
			fileCodingInAll(text2, encoded2, decoded2);

			system("pause");
			cout << "\n����ʹ�ø�Ƶ�ַ��ı� " << text1 << " ���б��롢����" << endl;
			cout << endl;
			system("pause");
			cout << text1 << "�ı��ļ����ݣ�" << endl;
			printFile(text1);
			cout << endl;
			fileCodingInAll(text1, encoded1, decoded1);

			system("pause");
			system("cls");
			break;
		}
		case 'Q':
		case 'q':
			cout << "���˳�ϵͳ��ллʹ�ã�" << endl;
			exit(0);
		default:
			break;
		}
	}

	return 0;
}