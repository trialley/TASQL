#pragma once

#ifndef DEFINITIONS
#define DEFINITIONS 1
#include <CJsonObject.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#define fe first
#define se second
#define pb push_back
#define INT 1
#define VARCHAR 2

#define BPTREE_MAX_FILE_PATH_SIZE 1000
#define BPTREE_MAX_KEYS_PER_NODE 5
#define BPTREE_INSERT_SUCCESS 1
#define BPTREE_INSERT_ERROR_EXIST 2
#define BPTREE_SEARCH_NOT_FOUND -1

#define ERROR 0
#define SUCCESS 1

#define MAX_VARCHAR 50
#define MAX_ATTR 30
#define MAX_NAME 50
#define MULTIPLICITY 10
#define MAX_PATH 1000

#define MAX_NODE 30

using namespace std;
struct col_details {
	char col_name[20];
	int type;
	int size;
};

struct table {
	int prefix[MAX_ATTR + 1];
	col_details col[MAX_ATTR + 1];

	int count;		//no. of attributes
	char name[20];	//NAME OF TABLE
	int size;		//size of record
	int data_size;
	int BLOCKSIZE;
	FILE *fp;
	void *blockbuf;
	int rec_count;
};

neb::CJsonObject showTables();
neb::CJsonObject getBTree();
// TODO:  在此处引用程序需要的其他头文件
#ifdef _WIN32
#define filenamecut(x) (strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x)
#elif __linux
#define filenamecut(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)
#else
#PushError "No suitable filename cutter for your opearting system.\n"
#endif
#define LOG \
	std::cout << __FILE__ << ":" << __LINE__ << ":1 " << __func__ << "() "
#endif

////////////////////新面向对象
namespace TASQL {
class dataNode;
}