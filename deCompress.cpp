#define _CRT_SECURE_NO_DEPRECATE

#include"deCompress.h"
#include<iostream>

using namespace std;

int deCompress(const char* filename)
{
	cout << "------Huffman�ļ���ѹ------" << endl;
	HEAD head;
	InitHead(filename, head);
	cout << "���ڶ�ȡ�ļ�����" << endl;
	FILE* in = fopen(filename, "rb");
	if (in == NULL)
	{
		cout << "fail";
		return 0;
	}
	fread(&head, sizeof(HEAD), 1, in);
	/*cout << head.type << "/" << head.length << "/" << head.weight << endl;*/
	cout << "��ȡ�ɹ���" << endl;
	fclose(in);
	in = NULL;


	cout << "��ѹ�С���" << endl;
	int n = 256;
	int m = 2 * n - 1;
	int fSize = head.length;

	//cout << fSize << endl;

	HuffmanTree pHT = new HTNode[m + 1];	//����Huffman��
	CreateHuffmanTree(pHT, head.weight, n);
	//cout << "�������ɵ�Huffman��" << endl;
	//if (TestHufTree(pHT))
	//	cout << "������ϣ�" << endl;

	//char** pHC = new char* [n + 1];
	//HuffmanCoding(pHC, pHT);
	//cout << "���ԣ���ʾ�ֽڵ�Huffman����-2" << endl;
	//cout << "Byte\tHuffmanCode" << endl;
	//TestHufCode(511, pHT, pHC);

	//���ļ��������ݶ�
	char* code=NULL;
	code = ReadFile(filename);
	//cout << strlen(pBuffer) / 8;
	int codenum = strlen(code);
	char* fBuffer;
	fBuffer=DeCode(pHT, code,fSize,codenum);
	//���������ɵõ�ֵ

	  //���ɽ�ѹ�ļ�����
		//�Զ���������ʽ���ļ�
	FILE* out = fopen("dellk_main.bmp", "wb");
	//дѹ����ı���
	fwrite(fBuffer, sizeof(char), fSize, out);
	//�ر��ļ����ͷ��ļ�ָ��
	fclose(out);
	out = NULL;

	//delete[] pHC;
	delete[] pHT;
	delete code;
	delete[] fBuffer;
	cout << "��ѹ�ɹ���" << endl;
	return 1;
}

char* ReadFile(const char* filename) {
	FILE* file_ptr;
	char* binaryString;
	long file_size;
	int ch;
	int i = 0;
	int k = 0;

	// �Զ����ƶ�ģʽ���ļ�
	file_ptr = fopen(filename, "rb");
	if (file_ptr == NULL) {
		printf("�޷����ļ� %s\n", filename);
		exit(1);
	}

	//fseek(file_ptr, sizeof(HEAD), SEEK_SET);
	// �ƶ��ļ�ָ�뵽�ļ�ĩβ����ȡ�ļ���С
	fseek(file_ptr, 0, SEEK_END);
	file_size = ftell(file_ptr);
	fseek(file_ptr, 0, SEEK_SET);
	fseek(file_ptr, sizeof(HEAD), SEEK_SET);

	// �����ڴ����洢�������ַ���
	binaryString = (char*)malloc((file_size * 8 ) * sizeof(char)-sizeof(HEAD));
	if (binaryString == NULL) {
		printf("�ڴ����ʧ��\n");
		exit(1);
	}

	// ��ȡ�ļ����ݲ�ת��Ϊ�������ַ���
	while ((ch = fgetc(file_ptr)) != EOF)
	{
		for (int j = 7; j >= 0; j--) 
		{
			binaryString[i++] = (ch & (1 << j)) ? '1' : '0';	
		}
	}
	
	// �ر��ļ�
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
			cout << "����" << endl;
			return NULL;
		}
	}
	return buffer;
}