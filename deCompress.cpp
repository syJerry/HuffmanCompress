#define _CRT_SECURE_NO_DEPRECATE

#include"deCompress.h"
#include<iostream>

using namespace std;

int deCompress(const char* filename)
{
	cout << "------Huffman文件解压------" << endl;
	HEAD head;
	InitHead(filename, head);
	cout << "正在读取文件……" << endl;
	FILE* in = fopen(filename, "rb");
	if (in == NULL)
	{
		cout << "fail";
		return 0;
	}
	fread(&head, sizeof(HEAD), 1, in);
	/*cout << head.type << "/" << head.length << "/" << head.weight << endl;*/
	cout << "读取成功！" << endl;
	fclose(in);
	in = NULL;


	cout << "解压中……" << endl;
	int n = 256;
	int m = 2 * n - 1;
	int fSize = head.length;

	//cout << fSize << endl;

	HuffmanTree pHT = new HTNode[m + 1];	//定义Huffman树
	CreateHuffmanTree(pHT, head.weight, n);
	//cout << "测试生成的Huffman树" << endl;
	//if (TestHufTree(pHT))
	//	cout << "测试完毕！" << endl;

	//char** pHC = new char* [n + 1];
	//HuffmanCoding(pHC, pHT);
	//cout << "测试：显示字节的Huffman编码-2" << endl;
	//cout << "Byte\tHuffmanCode" << endl;
	//TestHufCode(511, pHT, pHC);

	//从文件读入数据段
	char* code=NULL;
	code = ReadFile(filename);
	//cout << strlen(pBuffer) / 8;
	int codenum = strlen(code);
	char* fBuffer;
	fBuffer=DeCode(pHT, code,fSize,codenum);
	//遍历树生成得到值

	  //生成解压文件缓存
		//以二进制流形式打开文件
	FILE* out = fopen("dellk_main.bmp", "wb");
	//写压缩后的编码
	fwrite(fBuffer, sizeof(char), fSize, out);
	//关闭文件，释放文件指针
	fclose(out);
	out = NULL;

	//delete[] pHC;
	delete[] pHT;
	delete code;
	delete[] fBuffer;
	cout << "解压成功！" << endl;
	return 1;
}

char* ReadFile(const char* filename) {
	FILE* file_ptr;
	char* binaryString;
	long file_size;
	int ch;
	int i = 0;
	int k = 0;

	// 以二进制读模式打开文件
	file_ptr = fopen(filename, "rb");
	if (file_ptr == NULL) {
		printf("无法打开文件 %s\n", filename);
		exit(1);
	}

	//fseek(file_ptr, sizeof(HEAD), SEEK_SET);
	// 移动文件指针到文件末尾，获取文件大小
	fseek(file_ptr, 0, SEEK_END);
	file_size = ftell(file_ptr);
	fseek(file_ptr, 0, SEEK_SET);
	fseek(file_ptr, sizeof(HEAD), SEEK_SET);

	// 分配内存来存储二进制字符串
	binaryString = (char*)malloc((file_size * 8 ) * sizeof(char)-sizeof(HEAD));
	if (binaryString == NULL) {
		printf("内存分配失败\n");
		exit(1);
	}

	// 读取文件内容并转换为二进制字符串
	while ((ch = fgetc(file_ptr)) != EOF)
	{
		for (int j = 7; j >= 0; j--) 
		{
			binaryString[i++] = (ch & (1 << j)) ? '1' : '0';	
		}
	}
	
	// 关闭文件
	fclose(file_ptr);

	return binaryString;
}

char* DeCode(HuffmanTree pHT, char* code,int fSize,int codenum)
{
	char* buffer = (char*)malloc(fSize*sizeof(char));
	int i = 0;//codenum
	int j = 0;//buffer
	int pos = 0;
	while (i < codenum)
	{
		pos = 2*256-1;
		while (pHT[pos].lchild!=0 && pHT[pos].rchild!=0)
		{
			if (code[i]=='0')
			{
				pos = pHT[pos].lchild;
				i++;
			}
			else
			{
				pos = pHT[pos].rchild;
				i++;
			}
		}

		if (pos <= 256 && pos != 0 && j<fSize)
		{
			buffer[j++] = pos-1;
		}
		else if (j == fSize)
		{
			return buffer;
		}
		else
		{
			cout << "出错！" << endl;
			return NULL;
		}
	}
	return buffer;
}