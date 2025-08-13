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

//Huffman树结点结构
typedef struct Node
{
	char ch;       //字符
	int freq;      //出现频率
	Node* lChild;  //左孩子指针
	Node* rChild;  //右孩子指针

	//构造函数
	Node(char c, int f) : ch(c), freq(f), lChild(NULL), rChild(NULL) {}
}HfmNode;

//定义比较函数对象
struct Compare
{
	bool operator()(HfmNode* a, HfmNode* b)
	{
		//将字符出现频率小的结点的优先级设置为高
		return a->freq > b->freq;
	}

};


//统计字符串中各字符及其出现的频率，并记录在map中
void getFrequencyFromString(string text, map<char, int>& freqMap)
{
	for (char ch : text)  //使用for_each语法遍历字符串中的字符
	{
		if (freqMap.count(ch) > 0)  //当字符在map中的个数大于0时
		{
			freqMap[ch]++;  //将其出现频率+1
		}
		else  //否则，当个数等于0时，说明map中还没有该字符
		{
			freqMap[ch] = 1;  //将出现频率设置为1
		}
	}
}

//统计文件中各字符及其出现的频率，并记录在map中
void getFrequencyMapFromFile(string fileName, map<char, int>& map)
{
	ifstream ifs(fileName);
	if (!ifs)
	{
		cout << "文件打开失败！" << endl;
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
	cout << "\t字符\t频率" << endl;
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

//通过map创建Huffman树
HfmNode* createHfmTree(map<char, int>& freqMap)
{
	//创建优先队列，元素为Huffman树结点，结点的字符频率小的先出队
	priority_queue<Node*, vector<Node*>, Compare> pq;

	//遍历字符-频率map
	for (auto& pair : freqMap)
	{
		HfmNode* leaf = new Node(pair.first, pair.second);  //申请新结点
		pq.push(leaf);                                      //结点入队
	}

	//申请完所有结点后，根据Huffman算法创建Huffman树
	//直到队内只有一个结点，此结点即为Huffman树的根节点
	while (pq.size() > 1)
	{
		HfmNode* lchild = pq.top();  //取队头结点，此结点的字符频率最小，作为左孩子结点
		pq.pop();                    //出队

		HfmNode* rchild = pq.top();  //再取队头结点，作为右孩子结点
		pq.pop();

		//申请父结点，孩子结点为两队头结点，字符频率为两孩子结点字符频率之和
		HfmNode* parent = new HfmNode('\0', lchild->freq + rchild->freq);
		parent->lChild = lchild;
		parent->rChild = rchild;
		pq.push(parent);  //将父结点入队
	}
	//循环结束后，队内的唯一结点为Huffman树的根结点，返回该结点作为创建好的Huffman树
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
		cout << "·" << endl;
	}
	//cout << T->ch << endl;
	if (T->lChild)
	{
		print_impl(T->lChild, true, indent + (lChild ? "      " : "|     "));
	}
}

//图形化打印Huffman树
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
		cout << "·" << endl;
	}
	if (T->lChild)
	{
		print_impl(T->lChild, true, "");
	}
}

//计算Huffman树的带权路径长度（使用 计算所有分支结点权值之和 方法）
int WPL(HfmNode* T)
{
	//若T为分支结点
	if (T && T->lChild && T->rChild)
	{
		//返回T的字符频率，加上其左右子树中的分支结点的字符频率
		return T->freq + WPL(T->lChild) + WPL(T->rChild);
	}
	return 0;  //否则，返回0
}

//由Huffman树创建出Huffman编码表，记录于map中
void getHfmCodes(HfmNode* T, map<char, string>& hfmCodes, string curCode = "")
{
	//Huffman树的各叶子结点即存有有效字符及频率的结点
	if (!T->lChild && !T->rChild)
	{
		hfmCodes[T->ch] = curCode;  //在map中对应key内存入编码作为value
		return;
	}
	//每次往左分支走时，编码加上"0"；每次往右分支走时，编码加上"1"
	getHfmCodes(T->lChild, hfmCodes, curCode + "0");
	getHfmCodes(T->rChild, hfmCodes, curCode + "1");
}

bool compareByLength(pair<char, string>& a, pair<char, string>& b)
{
	return a.second.length() < b.second.length();
}

//打印Huffman编码表
void printHfmCodes(map<char, string>& hfmCodes)
{
	vector<pair<char, string> > vec(hfmCodes.begin(), hfmCodes.end());
	sort(vec.begin(), vec.end(), compareByLength);
	cout << "\t字符\t编码" << endl;
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

//使用Huffman编码表对文本进行编码，返回编码后的字符串
string encodeTextToString(string& text, map<char, string> hfmCodes)
{
	string encodedText;
	for (char ch : text)
	{
		//对字符串中的各个字符，在map中找到对应的编码，加入到编码字符串中
		encodedText += hfmCodes.at(ch);
	}
	return encodedText;  //返回编码字符串
}

//使用Huffman编码表对文本进行编码，并写入文本文件
void encodedTextToFile(string originFile, string encodedFile, map<char, string> hfmCodes)
{
	ifstream ifs(originFile);
	ofstream ofs(encodedFile);
	if (!ifs.is_open() || !ofs.is_open())
	{
		cout << "文件打开失败！" << endl;
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

//map转置：由<string, char>类型转换为<char, string>类型
map<string, char> reverseMap(map<char, string>& originalMap)
{
	map<string, char> reversedMap;
	for (auto& pair : originalMap)
	{
		reversedMap[pair.second] = pair.first;
	}
	return reversedMap;
}

//通过Huffman编码表对编码文本进行解码，返回解码后的字符串
string decodeTextToString(string text, map<char, string> originalCodes)
{
	//对map进行转置：将 字符-编码 map转换为 编码-字符 map
	map<string, char> hfmCodes = reverseMap(originalCodes);

	string curCode;
	string decodedText;
	for (char ch : text)
	{
		curCode += ch;  //将用二进制表示的编码加入至当前字符串中

		//如果当前字符串表示的编码在 编码-字符 map中
		if (hfmCodes.count(curCode))
		{
			decodedText += hfmCodes[curCode];  //将对应编码的字符加入至解码字符串中
			curCode.clear();  //清空当前字符串，为下一轮解码作准备
		}
	}
	return decodedText;  //返回解码字符串
}
//通过Huffman编码表对编码文本进行解码，并写入文本文件
void decodeTextToFile(string encodedFile, string decodedFile, map<char, string> originalCodes)
{
	ifstream ifs(encodedFile);
	ofstream ofs(decodedFile);
	if (!ifs.is_open() || !ofs.is_open())
	{
		cout << "文件打开失败！" << endl;
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

//统计Huffman树中，有效字符总数
int charCount(HfmNode* T)
{
	if (T)
	{
		//若T为叶子结点
		if (!T->lChild && !T->rChild)
		{
			return T->freq;  //返回T的字符频率
		}
		//若T为分支结点，则返回其左右子树中的叶子结点的字符频率之和
		return charCount(T->lChild) + charCount(T->rChild);
	}
	return 0;  //若T为空结点，则返回0
}

//统计Huffman树中，出现的字符个数
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

//通过创建的Huffman树，计算压缩比
void printRatio(HfmNode* T)
{
	//使用等长方式编码时，文件大小为：总字符数 * 每个字符需使用的最小二进制位数
	double oriSize = charCount(T) * ceil(log2(leafCount(T)));

	//使用Huffman编码时，文件大小即为Huffman树的带权路径长度，即WPL
	double encSize = WPL(T);

	//压缩比 = 编码后文件大小 / 未编码文件大小
	double ratio =  encSize / oriSize;

	cout << "使用等长方式编码，文件大小为：" << oriSize << endl;
	cout << "使用Huffman编码，文件大小为：" << encSize << endl;
	cout << "压缩比：" << ratio << endl;
}

//比较文件内容是否相同
bool compareFiles(string file1, string file2)
{
	ifstream ifs1(file1);
	ifstream ifs2(file2);

	if (!ifs1.is_open() || !ifs2.is_open())
	{
		cout << "打开文件失败！" << endl;
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
			cout << "文件内容不完全相同" << endl;
			return false;
		}
	}

	double similarity = (double)sameCount / totalCount;
	cout << "文件 " << file1 << " 与文件 " << file2 << " 的相似度为：" << 100 * similarity << "%" << endl;

	return true;
}

//比较字符串是否相同
bool compareStrings(string str1, string str2)
{
	if (str1 == str2)
	{
		cout << "字符串 " << str1 << " 与字符串 " << str2 << " 完全相同" << endl;
		return true;
	}
	else
	{
		cout << "原字符串 " << str1 << " 与解码后字符串 " << str2 << " 不完全相同" << endl;
		return false;
	}
}

//打印文本文件内容
void printFile(string fileName)
{
	ifstream ifs(fileName);
	if (!ifs.is_open())
	{
		cout << "文件打开失败！" << endl;
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
	cout << "------------打印字符及频率------------" << endl;
	printFreqMap(freqMap);
	cout << "----------打印字符及频率结束----------" << endl;
	cout << endl;

	HfmNode* T = createHfmTree(freqMap);
	cout << "------------打印Huffman树------------" << endl;
	printTree(T);
	cout << "----------打印Huffman树结束----------" << endl;
	cout << endl;

	cout << "带权路径长度WPL：" << WPL(T) << endl;
	cout << endl;

	getHfmCodes(T, HfmCodes);
	cout << "------------打印Huffman编码表------------" << endl;
	printHfmCodes(HfmCodes);
	cout << "----------打印Huffman编码表结束----------" << endl;
	cout << endl;

	string encoded = encodeTextToString(text, HfmCodes);
	cout << "------------打印编码后字符串------------" << endl;
	cout << encoded << endl;
	cout << "----------打印编码后字符串结束----------" << endl;
	cout << endl;

	cout << "------------计算压缩比------------" << endl;
	printRatio(T);
	cout << "----------计算压缩比结束----------" << endl;
	cout << endl;

	string decoded = decodeTextToString(encoded, HfmCodes);
	cout << "------------打印解码后字符串------------" << endl;
	cout << decoded << endl;
	cout << "----------打印编码后字符串结束----------" << endl;
	cout << endl;

	cout << "------------字符串比较------------" << endl;
	compareStrings(text, decoded);
	cout << "----------字符串比较结束----------" << endl;
}

void fileCodingInAll(string oriFile, string encFile, string decFile)
{
	map<char, int> freqMap;
	map<char, string> HfmCodes;

	ifstream ifs(oriFile);
	if (!ifs.is_open())
	{
		cout << "打开文件失败！" << endl;
		return;
	}
	getFrequencyMapFromFile(oriFile, freqMap);
	cout << "\n即将打印字符及其频率" << endl;
	system("pause");
	cout << "------------打印字符及频率------------" << endl;
	printFreqMap(freqMap);
	cout << "----------打印字符及频率结束----------" << endl;
	cout << endl;

	HfmNode* T = createHfmTree(freqMap);
	cout << "\n即将打印Huffman树" << endl;
	system("pause");
	cout << "------------打印Huffman树------------" << endl;
	printTree(T);
	cout << "----------打印Huffman树结束----------" << endl;
	cout << endl;

	cout << "带权路径长度WPL：" << WPL(T) << endl;
	cout << endl;

	getHfmCodes(T, HfmCodes);
	cout << "\n即将打印Huffman编码表" << endl;
	system("pause");
	cout << "------------打印Huffman编码表------------" << endl;
	printHfmCodes(HfmCodes);
	cout << "----------打印Huffman编码表结束----------" << endl;
	cout << endl;

	encodedTextToFile(oriFile, encFile, HfmCodes);
	cout << "\n即将打印编码后文件" << endl;
	system("pause");
	cout << "------------打印编码后文本文件------------" << endl;
	printFile(encFile);
	cout << "----------打印编码后文本文件结束----------" << endl;
	cout << endl;

	cout << "------------计算压缩比------------" << endl;
	printRatio(T);
	cout << "----------计算压缩比结束----------" << endl;
	cout << endl;

	decodeTextToFile(encFile, decFile, HfmCodes);
	cout << "\n即将打印解码后文件" << endl;
	system("pause");
	cout << "------------打印解码后文本文件------------" << endl;
	printFile(decFile);
	cout << "----------打印编码后文本文件结束----------" << endl;
	cout << endl;

	cout << "------------文本文件比较------------" << endl;
	compareFiles(oriFile, decFile);
	cout << "----------文本文件比较结束----------" << endl;

	ifs.close();
}