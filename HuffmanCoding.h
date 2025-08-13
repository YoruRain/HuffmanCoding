#pragma once
#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<queue>
#include<vector>
#include<cmath>
#include<algorithm>
using namespace std;

//Huffman�����ṹ
typedef struct Node
{
	char ch;       //�ַ�
	int freq;      //����Ƶ��
	Node* lChild;  //����ָ��
	Node* rChild;  //�Һ���ָ��

	//���캯��
	Node(char c, int f) : ch(c), freq(f), lChild(NULL), rChild(NULL) {}
}HfmNode;

//����ȽϺ�������
struct Compare
{
	bool operator()(HfmNode* a, HfmNode* b)
	{
		//���ַ�����Ƶ��С�Ľ������ȼ�����Ϊ��
		return a->freq > b->freq;
	}

};


//ͳ���ַ����и��ַ�������ֵ�Ƶ�ʣ�����¼��map��
void getFrequencyFromString(string text, map<char, int>& freqMap)
{
	for (char ch : text)  //ʹ��for_each�﷨�����ַ����е��ַ�
	{
		if (freqMap.count(ch) > 0)  //���ַ���map�еĸ�������0ʱ
		{
			freqMap[ch]++;  //�������Ƶ��+1
		}
		else  //���򣬵���������0ʱ��˵��map�л�û�и��ַ�
		{
			freqMap[ch] = 1;  //������Ƶ������Ϊ1
		}
	}
}

//ͳ���ļ��и��ַ�������ֵ�Ƶ�ʣ�����¼��map��
void getFrequencyMapFromFile(string fileName, map<char, int>& map)
{
	ifstream ifs(fileName);
	if (!ifs)
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}

	char ch;
	while (ifs.get(ch))
	{
		if (map.count(ch) > 0)
		{
			map[ch]++;
		}
		else
		{
			map[ch] = 1;
		}
	}
	ifs.close();
}

bool compareByFreq(pair<char, int>& a, pair<char, int>& b)
{
	return a.second > b.second;
}

void printFreqMap(map<char, int>& freqMap)
{
	vector<pair<char, int> > vec(freqMap.begin(), freqMap.end());
	sort(vec.begin(), vec.end(), compareByFreq);
	cout << "\t�ַ�\tƵ��" << endl;
	for (auto& pair : vec)
	{
		if (pair.first == '\n')
		{
			cout << "\t " << "\\n" << "\t " << pair.second << endl;
		}
		else
		{
			cout << "\t " << pair.first << "\t " << pair.second << endl;
		}
	}
}

//ͨ��map����Huffman��
HfmNode* createHfmTree(map<char, int>& freqMap)
{
	//�������ȶ��У�Ԫ��ΪHuffman����㣬�����ַ�Ƶ��С���ȳ���
	priority_queue<Node*, vector<Node*>, Compare> pq;

	//�����ַ�-Ƶ��map
	for (auto& pair : freqMap)
	{
		HfmNode* leaf = new Node(pair.first, pair.second);  //�����½��
		pq.push(leaf);                                      //������
	}

	//���������н��󣬸���Huffman�㷨����Huffman��
	//ֱ������ֻ��һ����㣬�˽�㼴ΪHuffman���ĸ��ڵ�
	while (pq.size() > 1)
	{
		HfmNode* lchild = pq.top();  //ȡ��ͷ��㣬�˽����ַ�Ƶ����С����Ϊ���ӽ��
		pq.pop();                    //����

		HfmNode* rchild = pq.top();  //��ȡ��ͷ��㣬��Ϊ�Һ��ӽ��
		pq.pop();

		//���븸��㣬���ӽ��Ϊ����ͷ��㣬�ַ�Ƶ��Ϊ�����ӽ���ַ�Ƶ��֮��
		HfmNode* parent = new HfmNode('\0', lchild->freq + rchild->freq);
		parent->lChild = lchild;
		parent->rChild = rchild;
		pq.push(parent);  //����������
	}
	//ѭ�������󣬶��ڵ�Ψһ���ΪHuffman���ĸ���㣬���ظý����Ϊ�����õ�Huffman��
	return pq.top();
}

void print_impl(HfmNode* T, bool lChild, string indent)
{
	if (T->rChild)
	{
		print_impl(T->rChild, false, indent + (lChild ? "|     " : "      "));
	}
	cout << indent;
	cout << (lChild ? '\\' : '/');
	cout << "----";
	if (T->ch != '\0')
	{
		if (T->ch == '\n')
		{
			cout << "\\n" << "(" << T->freq << ")" << endl;
		}
		else
		{
			cout << T->ch << "(" << T->freq << ")" << endl;
		}
	}
	else
	{
		cout << "��" << endl;
	}
	//cout << T->ch << endl;
	if (T->lChild)
	{
		print_impl(T->lChild, true, indent + (lChild ? "      " : "|     "));
	}
}

//ͼ�λ���ӡHuffman��
void printTree(HfmNode* T)
{
	if (T->rChild)
	{
		print_impl(T->rChild, false, "");
	}
	if (T->ch != '\0')
	{
		if (T->ch == '\n')
		{
			cout << "\\n" << "(" << T->freq << ")" << endl;
		}
		else
		{
			cout << T->ch << "(" << T->freq << ")" << endl;
		}
	}
	else
	{
		cout << "��" << endl;
	}
	if (T->lChild)
	{
		print_impl(T->lChild, true, "");
	}
}

//����Huffman���Ĵ�Ȩ·�����ȣ�ʹ�� �������з�֧���Ȩֵ֮�� ������
int WPL(HfmNode* T)
{
	//��TΪ��֧���
	if (T && T->lChild && T->rChild)
	{
		//����T���ַ�Ƶ�ʣ����������������еķ�֧�����ַ�Ƶ��
		return T->freq + WPL(T->lChild) + WPL(T->rChild);
	}
	return 0;  //���򣬷���0
}

//��Huffman��������Huffman�������¼��map��
void getHfmCodes(HfmNode* T, map<char, string>& hfmCodes, string curCode = "")
{
	//Huffman���ĸ�Ҷ�ӽ�㼴������Ч�ַ���Ƶ�ʵĽ��
	if (!T->lChild && !T->rChild)
	{
		hfmCodes[T->ch] = curCode;  //��map�ж�Ӧkey�ڴ��������Ϊvalue
		return;
	}
	//ÿ�������֧��ʱ���������"0"��ÿ�����ҷ�֧��ʱ���������"1"
	getHfmCodes(T->lChild, hfmCodes, curCode + "0");
	getHfmCodes(T->rChild, hfmCodes, curCode + "1");
}

bool compareByLength(pair<char, string>& a, pair<char, string>& b)
{
	return a.second.length() < b.second.length();
}

//��ӡHuffman�����
void printHfmCodes(map<char, string>& hfmCodes)
{
	vector<pair<char, string> > vec(hfmCodes.begin(), hfmCodes.end());
	sort(vec.begin(), vec.end(), compareByLength);
	cout << "\t�ַ�\t����" << endl;
	for (auto& pair : vec)
	{
		if (pair.first == '\n')
		{
			cout << "\t " << "\\n" << "\t " << pair.second << endl;
		}
		else
		{
			cout << "\t " << pair.first << "\t " << pair.second << endl;
		}
	}
}

//ʹ��Huffman�������ı����б��룬���ر������ַ���
string encodeTextToString(string& text, map<char, string> hfmCodes)
{
	string encodedText;
	for (char ch : text)
	{
		//���ַ����еĸ����ַ�����map���ҵ���Ӧ�ı��룬���뵽�����ַ�����
		encodedText += hfmCodes.at(ch);
	}
	return encodedText;  //���ر����ַ���
}

//ʹ��Huffman�������ı����б��룬��д���ı��ļ�
void encodedTextToFile(string originFile, string encodedFile, map<char, string> hfmCodes)
{
	ifstream ifs(originFile);
	ofstream ofs(encodedFile);
	if (!ifs.is_open() || !ofs.is_open())
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}

	char ch;
	while (ifs.get(ch))
	{
		ofs << hfmCodes.at(ch);
	}
	ifs.close();
	ofs.close();
}

//mapת�ã���<string, char>����ת��Ϊ<char, string>����
map<string, char> reverseMap(map<char, string>& originalMap)
{
	map<string, char> reversedMap;
	for (auto& pair : originalMap)
	{
		reversedMap[pair.second] = pair.first;
	}
	return reversedMap;
}

//ͨ��Huffman�����Ա����ı����н��룬���ؽ������ַ���
string decodeTextToString(string text, map<char, string> originalCodes)
{
	//��map����ת�ã��� �ַ�-���� mapת��Ϊ ����-�ַ� map
	map<string, char> hfmCodes = reverseMap(originalCodes);

	string curCode;
	string decodedText;
	for (char ch : text)
	{
		curCode += ch;  //���ö����Ʊ�ʾ�ı����������ǰ�ַ�����

		//�����ǰ�ַ�����ʾ�ı����� ����-�ַ� map��
		if (hfmCodes.count(curCode))
		{
			decodedText += hfmCodes[curCode];  //����Ӧ������ַ������������ַ�����
			curCode.clear();  //��յ�ǰ�ַ�����Ϊ��һ�ֽ�����׼��
		}
	}
	return decodedText;  //���ؽ����ַ���
}
//ͨ��Huffman�����Ա����ı����н��룬��д���ı��ļ�
void decodeTextToFile(string encodedFile, string decodedFile, map<char, string> originalCodes)
{
	ifstream ifs(encodedFile);
	ofstream ofs(decodedFile);
	if (!ifs.is_open() || !ofs.is_open())
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	map<string, char> hfmCodes = reverseMap(originalCodes);

	char ch;
	string curCode;
	while (ifs.get(ch))
	{
		curCode += ch;
		if (hfmCodes.count(curCode))
		{
			ofs << hfmCodes[curCode];
			curCode.clear();
		}
	}
	ifs.close();
	ofs.close();
}

//ͳ��Huffman���У���Ч�ַ�����
int charCount(HfmNode* T)
{
	if (T)
	{
		//��TΪҶ�ӽ��
		if (!T->lChild && !T->rChild)
		{
			return T->freq;  //����T���ַ�Ƶ��
		}
		//��TΪ��֧��㣬�򷵻������������е�Ҷ�ӽ����ַ�Ƶ��֮��
		return charCount(T->lChild) + charCount(T->rChild);
	}
	return 0;  //��TΪ�ս�㣬�򷵻�0
}

//ͳ��Huffman���У����ֵ��ַ�����
int leafCount(HfmNode* T)
{
	if (T)
	{
		if (!T->lChild && !T->rChild)
		{
			return 1;
		}
		return leafCount(T->lChild) + leafCount(T->rChild);
	}
	return 0;
}

//ͨ��������Huffman��������ѹ����
void printRatio(HfmNode* T)
{
	//ʹ�õȳ���ʽ����ʱ���ļ���СΪ�����ַ��� * ÿ���ַ���ʹ�õ���С������λ��
	double oriSize = charCount(T) * ceil(log2(leafCount(T)));

	//ʹ��Huffman����ʱ���ļ���С��ΪHuffman���Ĵ�Ȩ·�����ȣ���WPL
	double encSize = WPL(T);

	//ѹ���� = ������ļ���С / δ�����ļ���С
	double ratio =  encSize / oriSize;

	cout << "ʹ�õȳ���ʽ���룬�ļ���СΪ��" << oriSize << endl;
	cout << "ʹ��Huffman���룬�ļ���СΪ��" << encSize << endl;
	cout << "ѹ���ȣ�" << ratio << endl;
}

//�Ƚ��ļ������Ƿ���ͬ
bool compareFiles(string file1, string file2)
{
	ifstream ifs1(file1);
	ifstream ifs2(file2);

	if (!ifs1.is_open() || !ifs2.is_open())
	{
		cout << "���ļ�ʧ�ܣ�" << endl;
		return false;
	}

	char ch1, ch2;
	int sameCount = 0;
	int totalCount = 0;

	while (ifs1.get(ch1) && ifs2.get(ch2))
	{
		totalCount++;
		if (ch1 == ch2)
		{
			sameCount++;
		}
		else
		{
			cout << "�ļ����ݲ���ȫ��ͬ" << endl;
			return false;
		}
	}

	double similarity = (double)sameCount / totalCount;
	cout << "�ļ� " << file1 << " ���ļ� " << file2 << " �����ƶ�Ϊ��" << 100 * similarity << "%" << endl;

	return true;
}

//�Ƚ��ַ����Ƿ���ͬ
bool compareStrings(string str1, string str2)
{
	if (str1 == str2)
	{
		cout << "�ַ��� " << str1 << " ���ַ��� " << str2 << " ��ȫ��ͬ" << endl;
		return true;
	}
	else
	{
		cout << "ԭ�ַ��� " << str1 << " �������ַ��� " << str2 << " ����ȫ��ͬ" << endl;
		return false;
	}
}

//��ӡ�ı��ļ�����
void printFile(string fileName)
{
	ifstream ifs(fileName);
	if (!ifs.is_open())
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	char ch;
	while (ifs.get(ch))
	{
		cout << ch;
	}
	cout << endl;
	ifs.close();
}

void stringCodingInAll(string text)
{
	map<char, int> freqMap;
	map<char, string> HfmCodes;

	getFrequencyFromString(text, freqMap);
	cout << "------------��ӡ�ַ���Ƶ��------------" << endl;
	printFreqMap(freqMap);
	cout << "----------��ӡ�ַ���Ƶ�ʽ���----------" << endl;
	cout << endl;

	HfmNode* T = createHfmTree(freqMap);
	cout << "------------��ӡHuffman��------------" << endl;
	printTree(T);
	cout << "----------��ӡHuffman������----------" << endl;
	cout << endl;

	cout << "��Ȩ·������WPL��" << WPL(T) << endl;
	cout << endl;

	getHfmCodes(T, HfmCodes);
	cout << "------------��ӡHuffman�����------------" << endl;
	printHfmCodes(HfmCodes);
	cout << "----------��ӡHuffman��������----------" << endl;
	cout << endl;

	string encoded = encodeTextToString(text, HfmCodes);
	cout << "------------��ӡ������ַ���------------" << endl;
	cout << encoded << endl;
	cout << "----------��ӡ������ַ�������----------" << endl;
	cout << endl;

	cout << "------------����ѹ����------------" << endl;
	printRatio(T);
	cout << "----------����ѹ���Ƚ���----------" << endl;
	cout << endl;

	string decoded = decodeTextToString(encoded, HfmCodes);
	cout << "------------��ӡ������ַ���------------" << endl;
	cout << decoded << endl;
	cout << "----------��ӡ������ַ�������----------" << endl;
	cout << endl;

	cout << "------------�ַ����Ƚ�------------" << endl;
	compareStrings(text, decoded);
	cout << "----------�ַ����ȽϽ���----------" << endl;
}

void fileCodingInAll(string oriFile, string encFile, string decFile)
{
	map<char, int> freqMap;
	map<char, string> HfmCodes;

	ifstream ifs(oriFile);
	if (!ifs.is_open())
	{
		cout << "���ļ�ʧ�ܣ�" << endl;
		return;
	}
	getFrequencyMapFromFile(oriFile, freqMap);
	cout << "\n������ӡ�ַ�����Ƶ��" << endl;
	system("pause");
	cout << "------------��ӡ�ַ���Ƶ��------------" << endl;
	printFreqMap(freqMap);
	cout << "----------��ӡ�ַ���Ƶ�ʽ���----------" << endl;
	cout << endl;

	HfmNode* T = createHfmTree(freqMap);
	cout << "\n������ӡHuffman��" << endl;
	system("pause");
	cout << "------------��ӡHuffman��------------" << endl;
	printTree(T);
	cout << "----------��ӡHuffman������----------" << endl;
	cout << endl;

	cout << "��Ȩ·������WPL��" << WPL(T) << endl;
	cout << endl;

	getHfmCodes(T, HfmCodes);
	cout << "\n������ӡHuffman�����" << endl;
	system("pause");
	cout << "------------��ӡHuffman�����------------" << endl;
	printHfmCodes(HfmCodes);
	cout << "----------��ӡHuffman��������----------" << endl;
	cout << endl;

	encodedTextToFile(oriFile, encFile, HfmCodes);
	cout << "\n������ӡ������ļ�" << endl;
	system("pause");
	cout << "------------��ӡ������ı��ļ�------------" << endl;
	printFile(encFile);
	cout << "----------��ӡ������ı��ļ�����----------" << endl;
	cout << endl;

	cout << "------------����ѹ����------------" << endl;
	printRatio(T);
	cout << "----------����ѹ���Ƚ���----------" << endl;
	cout << endl;

	decodeTextToFile(encFile, decFile, HfmCodes);
	cout << "\n������ӡ������ļ�" << endl;
	system("pause");
	cout << "------------��ӡ������ı��ļ�------------" << endl;
	printFile(decFile);
	cout << "----------��ӡ������ı��ļ�����----------" << endl;
	cout << endl;

	cout << "------------�ı��ļ��Ƚ�------------" << endl;
	compareFiles(oriFile, decFile);
	cout << "----------�ı��ļ��ȽϽ���----------" << endl;

	ifs.close();
}