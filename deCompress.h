#pragma once
#include"Compress.h"
#include"Huffman.h"
#include<fstream>

int deCompress(const char* filename);

char* ReadFile(const char* filename);

char* DeCode(HuffmanTree pHT,char* code,int fSize,int codenum);



