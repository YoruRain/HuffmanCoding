#include "HuffmanCoding.h"

int main()
{
	char n = '#';
	while(n)
	{
		cout << "              ___________________________________________________________________________ " << endl;
		cout << "             |                              数据结构课程设计                             |" << endl;
		cout << "             |                         ——Huffman树编码文件压缩                         |" << endl;
		cout << "             |                                                                           |" << endl;
		cout << "             |1.交互输入文本进行编码、解码                                               |" << endl;
		cout << "             |                                                                           |" << endl;
		cout << "             |2.使用文本文件进行编码、解码                                               |" << endl;
		cout << "             |                                                                           |" << endl;
		cout << "             |___________________________________________________________________________|" << endl;
		cout << endl;
		cout << "请输入要执行的功能（1或2）（输入 Q 退出系统）：";
		cin >> n;
		switch (n)
		{
		case '1':
		{
			string str;
			cout << "请输入欲编码的文本：";
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
			cout << "\n即将使用平均字符频率文本 " << text2 << " 进行编码、解码" << endl;
			cout << endl;
			system("pause");
			cout << text2 << "文本文件内容：" << endl;
			printFile(text2);
			cout << endl;
			fileCodingInAll(text2, encoded2, decoded2);

			system("pause");
			cout << "\n即将使用高频字符文本 " << text1 << " 进行编码、解码" << endl;
			cout << endl;
			system("pause");
			cout << text1 << "文本文件内容：" << endl;
			printFile(text1);
			cout << endl;
			fileCodingInAll(text1, encoded1, decoded1);

			system("pause");
			system("cls");
			break;
		}
		case 'Q':
		case 'q':
			cout << "已退出系统，谢谢使用！" << endl;
			exit(0);
		default:
			break;
		}
	}

	return 0;
}