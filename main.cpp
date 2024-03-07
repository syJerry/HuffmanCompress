
#include <iostream>
#include "Compress.h"
#include"Huffman.h"
#include"deCompress.h"

using namespace std;

int main()
{
	cout << "-------HuffmanÎÄ¼þÑ¹Ëõ------" << endl;
	char filename[20] = "Pic.bmp";
	
	if (Compress(filename) == 1) {
		cout << "\nÑ¹ËõÍê³É£¡" << endl;
	}
	else {
		cout << "\nÑ¹ËõÊ§°Ü" << endl;
	}
	 

	//deCompress("dePic.bmp");

	//test();

	return 0;
}