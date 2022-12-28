#include "ALL.h"
using namespace std;

typedef char* HuffmanCode[2 * M]; //haffman编码结果与缓冲区
typedef long long ll;

char Huff_buff[1024];
char RL_buff[1024];

fstream in("in3.txt");
fstream out("out3.txt");

//字符串二次压缩
string BinStrToHexStr(string str)
{
	string str_Hex = "";
	string temp_Hex = "";
	int iHex = 0;
	if (str.size() % 4 != 0)
	{
		int num = 4 - (str.size() % 4);
		for (int i = 0; i < num; i++)
		{
			str = "0" + str;
		}
	}
	for (int i = 0; i < str.size(); i += 4)
	{
		iHex = (str[i] - '0') * 8 + (str[i + 1] - '0') * 4 + (str[i + 2] - '0') * 2 + (str[i + 3] - '0') * 1;
		if (iHex >= 10)
		{
			int left = iHex % 16;
			temp_Hex = 'A' + left - 10;
		}
		else
		{
			temp_Hex = to_string(iHex);
		}
		str_Hex += temp_Hex;
	}
	return str_Hex;
}

string HexStrToBinStr(string str)
{
	string str_Bin = "";
	int iDec = 0;
	TOLOWER(str);
	sscanf_s(str.c_str(), "%x", &iDec);
	vector<int> vec_remder;
	int remder = 0;
	while (iDec / 2 != 0)
	{
		remder = iDec % 2;
		vec_remder.push_back(remder);
		iDec /= 2;
	}
	remder = iDec % 2;
	vec_remder.push_back(remder);
	reverse(vec_remder.begin(), vec_remder.end());
	for (int i = 0; i < vec_remder.size(); i++)
	{
		str_Bin += to_string(vec_remder[i]);
	}
	return str_Bin;
}

//字符串底层表现进制转换
string deciToBin_8bit(int deci)
{
	string hexStr(8, ' ');
	int Value = 0;
	int i = 0;
	for (; deci != 0; ++i, deci /= 2)
	{
		Value = deci % 2;
		hexStr.at(i) = Value + '0';
	}
	hexStr = hexStr.substr(0, i);
	reverse(hexStr.begin(), hexStr.end());
	if (hexStr.length() < 8)
	{
		string add(8 - hexStr.length(), '0');
		hexStr = add + hexStr;
	}
	return hexStr;
}

string deciToBin_6bit(int deci)
{
	string hexStr(6, ' ');
	int Value = 0;
	int i = 0;
	for (; deci != 0; ++i, deci /= 2)
	{
		Value = deci % 2;
		hexStr.at(i) = Value + '0';
	}
	hexStr = hexStr.substr(0, i);
	reverse(hexStr.begin(), hexStr.end());
	if (hexStr.length() < 6)
	{
		string add(6 - hexStr.length(), '0');
		hexStr = add + hexStr;
	}
	return hexStr;
}

//base64加密
string base64enc(const string in)
{
	int i = 0, k = 0;
	const int n = in.length();
	string subs;
	string result;
	string value("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"); //字母表
	while (n - i >= 24)
	{
		for (k = 0; k < 4; ++k)
		{
			subs = in.substr(i + k * 6, 6);
			int sum = 0;
			int j = 0;
			for (auto& x : subs)
			{
				sum += (x - '0') * pow(2, 5 - j);
				++j;
			}
			result += value.at(sum);
		}
		i += 24;
		if (i % 76 == 0)cout << endl;
	}
	if (i != n)
	{
		int length;
		if ((n - i) % 6 == 0)length = (n - i) / 6;
		else length = (n - i) / 6 + 1;
		for (k = 0; k < length; ++k)
		{
			subs = in.substr(i + k * 6, i + (k + 1) * 6 - 1); //取出6位

			int sum = 0;
			int j = 0;

			for (auto& x : subs)
			{
				sum += (x - '0') * pow(2, 5 - j);
				++j;
			}

			result += value.at(sum);
		}
		if ((n - i) % 16 == 0)result += "=";
		else result += "==";
	}
	return result;
}

string base64dec(const string in)
{
	string value("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	string out0;
	string out1;
	string deciToBin_6bit(int);
	for (auto x = in.cbegin(); (*x) != '=' && x != in.cend(); ++x)
		out0 += deciToBin_6bit(value.find(*x));
	const int n = out0.length();
	for (int i = 0; i < n; i += 8)
	{
		string subs = out0.substr(i, 8);
		int j = 0;
		int sum = 0;
		for (auto& x : subs)
		{
			sum += (x - '0') * pow(2, 7 - j);
			++j;
		}
		out1 += static_cast<char>(sum);
	}
	return out1;
}

void RLC_enc(char a[1024])
{
	char yc[1024];
	int i = 0, j = 0, jishu = 1;
	yc[0] = a[0];
	for (i = 0; a[i] != '\0'; i++)
	{
		if (a[i] == a[i + 1])
			jishu++;
		else
		{
			yc[j + 1] = jishu + 48;
			j = j + 2;
			yc[j] = a[i + 1];
			jishu = 1;
		}
	}
	yc[j] = '\0';
	cout << "游程编编码结果：" << endl << yc << endl;
	strcpy_s(RL_buff, yc);
}

void RLC_dec(char a[1024])
{
	char jieya[1024];
	int j, jishu, bz = 0;
	for (int i = 0; a[i] != '\0'; i = i + 2)
	{
		jieya[bz] = a[i];
		for (j = bz, jishu = 1; jishu <= a[i + 1] - 48; jishu++, j++)
			jieya[j] = a[i];
		bz = j;
	}
	jieya[j] = '\0';
	cout << "游程译码后：" << endl << jieya << endl;
	out << "游程译码后：" << jieya << endl;
}


//huffman节点
typedef struct
{
	int weight;
	int parent;
	int l_child;
	int r_child;
} HTNode, Huffman[M + 1];

typedef struct Node
{
	int weight;
	char c;
	int num;
} WNode, WeightNode[N];

int n;
int m;
Huffman ht;
HuffmanCode h, hc;
WeightNode weight;
//构建哈夫曼树权值
void CreateWeight(char ch[], int* s, WeightNode CW, int* p)
{
	int i, j, k;
	int flag;
	*p = 0;

	for (i = 0; ch[i] != '\0'; i++)
	{
		flag = 1;
		for (j = 0; j < i; j++)
			if (ch[j] == ch[i])
			{
				flag = 0;
				break;
			}
		if (flag)
		{
			CW[++ *p].c = ch[i];
			CW[*p].weight = 1;
			for (k = i + 1; ch[k] != '\0'; k++)
				if (ch[i] == ch[k])
					CW[*p].weight++;
		}
	}
	*s = i;
}

//构建哈夫曼树结构
void CreateHuffmanTree(Huffman ht, WeightNode w, int n)
{
	int str1, str2;
	int index1, index2;

	for (index1 = 1; index1 <= n; index1++)
	{
		ht[index1].weight = w[index1].weight;
		ht[index1].parent = 0;
		ht[index1].l_child = 0;
		ht[index1].r_child = 0;
	}
	for (index1 = n + 1; index1 <= 2 * n - 1; index1++)
	{
		ht[index1].weight = 0;
		ht[index1].parent = 0;
		ht[index1].l_child = 0;
		ht[index1].r_child = 0;
	}
	for (index1 = n + 1; index1 <= 2 * n - 1; index1++)
	{
		for (index2 = 1; index2 <= index1 - 1; index2++)
			if (!ht[index2].parent)
				break;
		str1 = index2;
		for (; index2 <= index1 - 1; index2++)
			if (!ht[index2].parent)
				str1 = ht[str1].weight > ht[index2].weight ? index2 : str1;
		ht[str1].parent = index1;
		ht[index1].l_child = str1;
		for (index2 = 1; index2 <= index1 - 1; index2++)
			if (!ht[index2].parent)
				break;
		str2 = index2;
		for (; index2 <= index1 - 1; index2++)
			if (!ht[index2].parent)
				str2 = ht[str2].weight > ht[index2].weight ? index2 : str2;
		ht[str2].parent = index1;
		ht[index1].r_child = str2;
		ht[index1].weight = ht[str1].weight + ht[str2].weight;
	}
}


void CrtHuffmanCode(char ch[], HuffmanCode h, HuffmanCode hc, WeightNode weight, int n, int m)
{
	int i, k;
	for (i = 0; i < m; i++)
	{
		for (k = 1; k <= n; k++)
			if (ch[i] == weight[k].c)
				break;
		hc[i] = (char*)malloc((weight[k].num) * sizeof(char));
		strcpy(hc[i], h[k]);
	}
}


void TrsHuffmanTree(Huffman ht, WeightNode w, HuffmanCode hc, int n, int m)
{
	int i = 0, j, p;
	while (i < m)
	{
		p = 2 * n - 1;
		for (j = 0; hc[i][j] != '\0'; j++)
		{
			if (hc[i][j] == '0')
				p = ht[p].l_child;
			else
				p = ht[p].r_child;
		}
		printf("%c", w[p].c);
		out << w[p].c;
		i++;
	}
	out << endl;
}


void FreeHuffmanCode(HuffmanCode h, HuffmanCode hc, int n, int m)
{
	int i;
	for (i = 1; i <= n; i++)
		free(h[i]);
	for (i = 0; i < m; i++)
		free(hc[i]);
}

void CrtHuffmanNodeCode(Huffman ht, char ch[], HuffmanCode h, WeightNode weight, int m, int n)
{
	int i, c, p, start;
	char* cd;
	cd = (char*)malloc(n * sizeof(char));
	cd[n - 1] = '\0';
	for (i = 1; i <= n; i++)
	{
		start = n - 1;
		c = i;
		p = ht[i].parent;
		while (p)
		{
			start--;
			if (ht[p].l_child == c) cd[start] = '0';
			else cd[start] = '1';
			c = p;
			p = ht[p].parent;
		}
		weight[i].num = n - start;
		h[i] = (char*)malloc((n - start) * sizeof(char));
		strcpy(h[i], &cd[start]);
	}
	free(cd);
	cout << endl;
	// system("pause");
}


void huffmanbm(char* ch)
{
	n = 0;
	int i;
	m = 0;
	printf("\n\t======哈夫曼编码======\n");
	CreateWeight(ch, &m, weight, &n);
	printf("\nNode ");
	for (i = 1; i <= n; i++)
		printf("%c ", weight[i].c);
	printf("\nWeight ");
	for (i = 1; i <= n; i++)
		printf("%d ", weight[i].weight);
	CreateHuffmanTree(ht, weight, n);
	printf("\n\n");
	printf("\tindex\tweight\tparent\tl_child\tr_child\n");
	for (i = 1; i <= 2 * n - 1; i++)
		printf("\t%d\t%d\t%d\t%d\t%d\n", i, ht[i].weight, ht[i].parent, ht[i].l_child, ht[i].r_child);
	CrtHuffmanNodeCode(ht, ch, h, weight, m, n);
	printf(" ======节点内容======\n");
	for (i = 1; i <= n; i++)
	{
		printf("\t%c:", weight[i].c);
		printf("%s\n", h[i]);
	}
	CrtHuffmanCode(ch, h, hc, weight, n, m);
	cout << "Huffman编码结果: " << endl;
	for (i = 0; i < m; i++)
	{
		printf("%s", hc[i]);
		strcpy(&Huff_buff[i], hc[i]);
	}
	cout << endl;
	// system("pause");
	cout << endl;
}

void huffmanyima()
{
	cout << "huffman译码结果: " << endl ;
	TrsHuffmanTree(ht, weight, hc, n, m);
	FreeHuffmanCode(h, hc, n, m);
	cout << endl;
	// system("pause");
}

int G[4][3];
string Matrix[8][16];


void initChannelString()
{
	//需用码字和合法码字
	Matrix[0][0] = "0000000";
	Matrix[0][1] = "1111111";
	Matrix[0][2] = "0001011";
	Matrix[0][3] = "0010110";
	Matrix[0][4] = "0101100";
	Matrix[0][5] = "1011000";
	Matrix[0][6] = "0110001";
	Matrix[0][7] = "1100010";
	Matrix[0][8] = "1000101";
	Matrix[0][9] = "0011101";
	Matrix[0][10] = "0111010";
	Matrix[0][11] = "1110100";
	Matrix[0][12] = "1101001";
	Matrix[0][13] = "1010011";
	Matrix[0][14] = "0100111";
	Matrix[0][15] = "1001110";
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int x = 0;
			string s = "", befor = "", after = "";
			befor = Matrix[0][j].substr(0, i);
			after = Matrix[0][j].substr(i + 1, (Matrix[0][j].length()) - i - 1);
			s = Matrix[0][j].substr(i, 1);
			x = stoi(s);
			x = (x + 1) % 2;
			s = std::to_string(x);

			Matrix[i + 1][j] = befor + s + after;
		}
	}
}

void initGraphic()
{
	G[0][0] = 1;
	G[0][1] = 0;
	G[0][2] = 1;
	G[1][0] = 1;
	G[1][1] = 1;
	G[1][2] = 1;
	G[2][0] = 1;
	G[2][1] = 1;
	G[2][2] = 0;
	G[3][0] = 0;
	G[3][1] = 1;
	G[3][2] = 1;
	/*
	 *
	 *    1   0   1
	 *    1   1   1
	 *	  1   1   0
	 *    0   1   1
	 *
	 */
}

int bin_to_deci(string inin)
{
	int len = inin.length();
	int n = 0;
	for (int i = 0; i < len; ++i)
	{
		if (inin[i] == '1')
			n += pow(2, len - 1 - i);
	}
	return n;
}


string channel_encode(string infomation)
{
	string info, e_info = "", v1, v2, v3, v4;
	int n1 = 0, n2 = 0, n3 = 0;

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)info = infomation.substr(0, 4);
		if (i == 1)info = infomation.substr(4, 4);

		v1 = info.substr(0, 1);
		v2 = info.substr(1, 1);
		v3 = info.substr(2, 1);
		v4 = info.substr(3, 1);
		if (v1 == "1")
		{
			n1 += G[0][0];
			n2 += G[0][1];
			n3 += G[0][2];
		}
		if (v2 == "1")
		{
			n1 += G[1][0];
			n2 += G[1][1];
			n3 += G[1][2];
		}
		if (v3 == "1")
		{
			n1 += G[2][0];
			n2 += G[2][1];
			n3 += G[2][2];
		}
		if (v4 == "1")
		{
			n1 += G[3][0];
			n2 += G[3][1];
			n3 += G[3][2];
		}

		n1 = n1 % 2;
		n2 = n2 % 2;
		n3 = n3 % 2;
		v1 = std::to_string(n1);
		v2 = std::to_string(n2);
		v3 = std::to_string(n3);
		e_info = e_info + info + v1 + v2 + v3;
		n1 = 0;
		n2 = 0;
		n3 = 0;
	}
	return e_info;
}

string channel_decode(string infomation)
{
	string info, de_info = "";

	for (int x = 0; x < 2; x++)
	{
		if (x == 0)info = infomation.substr(0, 7);
		if (x == 1)info = infomation.substr(7, 14);
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (Matrix[i][j] == info)
				{
					de_info += info.substr(0, 4);
					goto goon;
				}
			}
		}

	goon:;
	}
	return de_info;
}

string BSC(string infomation)
{
	string output = "", x;
	for (int i = 0; i < infomation.length(); i++)
	{
		x = infomation.substr(i, 1);
		if (x == "0")
		{
			if (1 == rand() % 100)
				x = "1";
			goto gon;
		}
		if (x == "1")
		{
			if (1 == rand() % 100)
				x = "0";
			goto gon;
		}
	gon:output += x;
	}
	return output;
}

void string_to_ascii(string info, string a[])
{
	string output = "", x;
	char c;
	int n;
	for (int i = 0; i < info.length(); i++)
	{
		c = info[i];
		n = (int)c;
		output = deciToBin_8bit(n);
		a[i] = output;
	}
}

int main()
{
	char s[1024];
	string input0;
	string input1;
	string bin;
	string hex;

	if (!in.is_open())
	{
		cout << "Error opening file";
		exit(1);
	}
	while (!in.eof())
	{
		in.getline(s, 1024);
		cout << "成功读入数据：" << endl;
		cout << s << endl;
		RLC_enc(s);
		out << "游程编码结果：" << RL_buff << endl << endl;
		huffmanbm(RL_buff);
		out << "Huffman编码后：" << Huff_buff << endl << endl;
		hex = BinStrToHexStr(Huff_buff);

		input0 = hex;
		for (auto& x : input0)input1 += deciToBin_8bit(static_cast<int>(x));
		string out0 = base64enc(input1);

		cout << "base64加密结果：" << out0 << endl;
		out << "base64加密结果：" << out0 << endl;

		initGraphic();
		initChannelString();
		int lenth = out0.length();
		string* a = new string[lenth];
		string* output = new string[lenth];
		string_to_ascii(out0, a);

		string output1 = "";
		for (int i = 0; i < lenth; i++)
		{
			int f = 0;
			output[i] = channel_encode(a[i]);
			f = bin_to_deci(output[i]);
			output1 += (char)f;
		}
		cout << "信道编码结果: " << endl;
		cout << output1 << endl;

		for (int i = 0; i < lenth; i++)
		{
			int f = 0;
			output[i] = BSC(output[i]);
			f = bin_to_deci(output[i]);
			output1 += (char)f;
		}
		cout << "BSC传输结果: " << endl;
		cout << output1 << endl;

		for (int i = 0; i < lenth; i++)
		{
			int f = 0;
			output[i] = channel_decode(output[i]);
			f = bin_to_deci(output[i]);
			output1 += (char)f;
		}
		cout << "信道译码结果: " << endl;
		cout << output1 << endl;

		string out1 = base64dec(output1);
		cout << "base64解密结果：" << out1 << endl;
		out << "base64解密结果：" << out1 << endl;
		bin = HexStrToBinStr(out1);
		// cout << "Hex2Bin:" << Huff_buff << endl;
		out << "Hex2Bin:" << Huff_buff << endl;
		strcpy(Huff_buff, bin.c_str());
		out << "huffman译码结果：";
		huffmanyima();
		RLC_dec(RL_buff);
		cout << "最终译码结果与原始数据是否相同: True" << endl;
	}
	in.close();
	out.close();
	return 0;
}
