#define _CRT_SECURE_NO_DEPRECATE

#include "Huffman.h"
#include<iostream>
#include<malloc.h>

#define OK 1
#define ERROR 0

using namespace std;

//����Huffman��
int CreateHuffmanTree(HuffmanTree pHT, int weight[], int n) {
	int s1, s2, i;
	int m = 2 * n - 1;

	for (i = 1; i <= n; i++) {
		pHT[i].weight = weight[i - 1];
		pHT[i].lchild = 0;
		pHT[i].rchild = 0;
		pHT[i].parent = 0;
	}
	for (i = n + 1; i <= m; i++) {
		pHT[i].parent = 0;
		pHT[i].lchild = 0;
		pHT[i].rchild = 0;
		pHT[i].weight = 0;
	}

		//cout << "���Գ�ʼ��Huffman������б�" << endl;
		//if (TestHufTree(pHT))
		//	cout << "������ϣ�" << endl;

	for (i = n + 1; i <= m; i++)
	{
		//ѡ��parentΪ0��weight��С��������㣬����ŷֱ�Ϊs1��s2
		Select(pHT, i - 1, s1, s2);

		//�޸�s1��s2��ָ��
		pHT[s1].parent = i;
		pHT[s2].parent = i; 

		//�޸�i�������Һ���ָ��
		pHT[i].lchild = s1;
		pHT[i].rchild = s2;

		//�޸�Ȩֵ
		pHT[i].weight = pHT[s1].weight + pHT[s2].weight; 
	}

		//cout << "��������Huffman������б�" << endl;
		//if (TestHufTree(pHT))
		//	cout << "������ϣ�" << endl;
	return OK;
}


void Select(HuffmanTree& pHT, int i, int& s1, int& s2) {
	int minValue = INT_MAX;

	//�ҵ���С��һ��Ȩֵ
	for (int j = 1; j <= i; j++) {
		if (pHT[j].parent == 0 && pHT[j].weight < minValue) {
			minValue = pHT[j].weight;
			s1 = j;
		}
	}
	//	cout << "��СȨֵ��" << s1;

	minValue = INT_MAX;
	//�ҵ������ڶ�С��Ȩֵ
	for (int j = 1; j <= i; j++) {
		if (j != s1 && pHT[j].parent == 0 && pHT[j].weight < minValue) {
			minValue = pHT[j].weight;
			s2 = j;
		}
	}
}

//����Huffman���� 
int HuffmanCoding(HuffmanCode& pHC, HuffmanTree& pHT)
{
	//��ջ�ǵݹ����Huffman������Huffman����
	char cd[256] = { '\0' };	//��¼����·��
	int cdlen = 0;				//��¼��ǰ·������

	for (int i = 1; i < 512; i++) {
		pHT[i].weight = 0;	//����Huffman��ʱ�����ڵ��״̬��־
	}

	int p = 511;			//���ڵ�
	while (p != 0) {
		//����
		if (pHT[p].weight == 0) 
		{
			pHT[p].weight = 1;
			if (pHT[p].lchild != 0) {
				p = pHT[p].lchild;
				cd[cdlen++] = '0';
			}
			//�Ǽ�Ҷ�ӽڵ���ַ��ı���
			else if (pHT[p].rchild == 0) {
				pHC[p] = (char*)malloc((cdlen + 1) * sizeof(char));
				cd[cdlen] = '\0';
				strcpy(pHC[p], cd);//���Ʊ���
			}
		}
		//����
		else if (pHT[p].weight == 1) {
			pHT[p].weight = 2;
			//�Һ���ΪҶ�ӽڵ�
			if (pHT[p].rchild != 0) {
				p = pHT[p].rchild;
				cd[cdlen++] = '1';
			}
		}
		//�˻ظ��ڵ㣬���볤�ȼ�һ
		else {
			pHT[p].weight = 0;
			p = pHT[p].parent;
			cdlen--;
		}
	}
	return OK;
}

int TestHufTree(HuffmanTree pHT) {
	cout << "Byte\t\tWeight\tParent\tLchild\tRchild\n";
	for (int i = 1; i < 512; i++) {
		if (i <= 99) {
			cout << "pHT[" << i << "]\t\t" << pHT[i].weight << "\t" << pHT[i].parent << "\t" << pHT[i].lchild << "\t" << pHT[i].rchild << endl;
		}
		else {
			cout << "pHT[" << i << "]\t" << pHT[i].weight << "\t" << pHT[i].parent << "\t" << pHT[i].lchild << "\t" << pHT[i].rchild << endl;
		}

	}
	return OK;
}

void TestHufCode(int root, HuffmanTree& pHT, HuffmanCode& pHC) {
	if (root <= 1) return;

	if (pHT[root].lchild == 0 && pHT[root].rchild == 0)
	{
		printf("0x%02X %s\n", root - 1, pHC[root - 1]);
	}
	if (pHT[root].lchild)//��������
	{
		TestHufCode(pHT[root].lchild, pHT, pHC);
	}
	if (pHT[root].rchild)//�����Һ���
	{
		TestHufCode(pHT[root].rchild, pHT, pHC);
	}


}