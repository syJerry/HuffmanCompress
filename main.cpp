
#include <iostream>
#include "Compress.h"
#include"Huffman.h"
#include"deCompress.h"

using namespace std;

int main()
{
	cout << "-------Huffman�ļ�ѹ��------" << endl;
	char filename[20] = "Pic.bmp";
	
	if (Compress(filename) == 1) {
		cout << "\nѹ����ɣ�" << endl;
	}
	else {
		cout << "\nѹ��ʧ��" << endl;
	}
	 

	//deCompress("dePic.bmp");

	//test();

	return 0;
}