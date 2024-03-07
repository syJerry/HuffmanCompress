#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <stdlib.h>
#include "Compress.h"
#include"Huffman.h"

using namespace std;

#define OK 1
#define ERROR 0

const int SIZE = 256;

//ɨ���ļ��ͳ�ʼ��ͷ�ļ�����Ϣ
int InitHead(const char* pFilname, HEAD& sHead)
{
	strcpy(sHead.type, "HUF");		//�ļ�����
	sHead.length = 0;				//Դ�ļ�����
	for (int i = 0; i < SIZE; i++)
	{
		sHead.weight[i] = 0;		//Ȩֵ
	}

	//�Զ���������ʽ���ļ�
	FILE* in = fopen(pFilname, "rb");


	//ɨ���ļ������Ȩ��
	int ch;
	while ((ch = fgetc(in)) != EOF)
	{
		sHead.weight[ch]++;
		sHead.length++;
	}

	//�ر��ļ�
	fclose(in);
	in = NULL;
	return OK;
}

//�õ������ļ�
int Compress(const char* pFilename)
{
	//�򿪲�ɨ���ļ�
	cout << "���ڶ�ȡ�ļ�����" << endl;
	int weight[256] = { 0 };  //���ļ�����ȡȨ��
	FILE* in = fopen(pFilename, "rb");
	if (in == NULL)
	{
		cout << "failed" << endl;
		return 0;
	}
	int ch;
	while ((ch = getc(in)) != EOF)
	{
		weight[ch]++;
	}
	//����ɨ��������ʾ256���ֽڳ��ֵĴ���
	//TestWeight(weight);

	cout << "�ļ���ȡ��ϣ�\n" << endl << endl;
	fclose(in);

	//����������Huffman��
	int i;
	int n = 256;						//Huffman������n��Ҷ�ӽڵ�
	int m = 2 * n - 1;					//��ô����2n+1���ڵ�

	HuffmanTree pHT = new HTNode[m + 1];	//����Huffman��
	CreateHuffmanTree(pHT, weight, n);

		//cout << "�������ɵ�Huffman��" << endl;
		//TestHufTree(pHT);

		//����Huffman����
	char** pHC = new char* [n + 1]; //����
	for (int i = 1; i <= n; i++)
		pHT[i].weight = weight[i - 1];


	HuffmanCoding(pHC, pHT);

	//cout << "���ԣ���ʾ�ֽڵ�Huffman����" << endl;
	//cout << "Byte\tHuffmanCode" << endl;
	//TestHufCode(511, pHT, pHC);


	//������뻺������С
	int nSize = 0;
	for (int i = 0; i < 256; i++)
		nSize += weight[i] * strlen(pHC[i + 1]);
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;


	//�Ա����ļ�����ѹ��
	char* pBuffer = NULL;
	pBuffer = new char[nSize];
	memset(pBuffer, 0, (nSize) * sizeof(char));
	Encode(pFilename, pHC, pBuffer, nSize);
	if (!pBuffer) {
		return ERROR;
	}
	HEAD sHead;
	InitHead(pFilename, sHead);
	cout << "Ŀ���ļ���С��" << sHead.length << "�ֽ�" << endl;
	int afterlen = WriteFile(pFilename, sHead, pBuffer, nSize);
	cout << "ѹ���ļ���С��" << afterlen << "�ֽ�  \n����ͷ�ļ�sHead��С��" << sizeof(sHead) << "�ֽ�" << endl;
	cout << "ѹ�����ʣ�" << (double)afterlen * 100 / sHead.length << "%" ;

	delete[] pHT;
	delete[] pHC; 
	delete[] pBuffer;

	return OK;
}

//ʵ��ѹ������
int Encode(const char* pFilname, const HuffmanCode pHC, char* pBuffer, const int nSize)
{
	cout << "ѹ����********" << endl;
	//���ļ�
	FILE* in = fopen(pFilname, "rb");

	char cd[SIZE] = { 0 };		//������
	int pos = 0;				//������ָ��
	int ch;
	//ɨ���ļ�
	while ((ch = fgetc(in)) != EOF)
	{
		strcat(cd, pHC[ch + 1]);
		//ѹ������
		while (strlen(cd) >= 8)
		{
			pBuffer[pos++] = Str2byte(cd);
			for (int i = 0; i < SIZE - 8; i++)
			{
				cd[i] = cd[i + 8];
			}
		}
	}
	int lastByteBits = strlen(cd);

	if (strlen(cd) > 0)
	{
		pBuffer[pos++] = Str2byte(cd);
	}
	fclose(in);		//�ر��ļ�

	return OK;
}

//����ѹ���ļ�
int WriteFile(const char* pFilename, const HEAD sHead, const char* pBuffer, const int nSize)
{
	//�����ļ���
	char filename[256] = { 0 };
	strcpy(filename, pFilename);
	strcat(filename, ".huf");

	//�Զ���������ʽ���ļ�
	FILE* out = fopen(filename, "wb");

	//д��Ϣͷ
	fwrite(&sHead, sizeof(HEAD), 1, out);

	//дѹ����ı���
	fwrite(pBuffer, sizeof(char), nSize, out);

	fclose(out);
	out = NULL;

	cout << "����ѹ���ļ���" << filename << endl;
	int len = sizeof(HEAD) + nSize;
	return len;
}

//���ַ���ת�����ֽ�
char Str2byte(const char* pBinStr)
{
	char b = 0x00;
	for (int i = 0; i < 8; i++)
	{
		b = b << 1;		//����һλ
		if (pBinStr[i] == '1')
		{
			b = b | 0x01;
		}
	}
	return b;
}


int TestWeight(int weight[]) {

	cout << "��ʾ256���ֽڳ��ֵĴ�����" << endl;
	cout << "Byte\t" << "Weight\t" << endl;

	for (int i = 0; i < 256; i++) {
		printf("0x%02X\t%d\n", i, weight[i]);
		//cout << i << "\t" << weight[i] << endl;
	}
	return OK;
}