#pragma once

#include <dataNode.h>
#include <insert.h>

#include "declaration.h"
void search() {
	//search for particular table or any specific entry inside table
	cout << "1.search table\n2.search particular entry in table\n\n";
	int ch;
	cin >> ch;
	char *tab;
	tab = (char *)malloc(sizeof(char) * MAX_NAME);
	if (ch == 1) {
		cout << "enter table name: ";
		cin >> tab;
		//check if table exists
		int check = search_table(tab);
		if (check == 1)
			printf("\n%s exists!!!\n", tab);
		else
			printf("\n%s doesn't exist in table entry\n\n", tab);
	} else if (ch == 2) {
		cout << "enter table name: ";
		cin >> tab;
		int check = search_table(tab);
		if (check == 1) {
			printf("%s exists!!!\n\nEnter key to search\n\n", tab);
			//open %s meta data file and display column details;
			table inp1;
			FILE *fp = open_file(tab, const_cast<char *>("r"));
			int i = 0;
			while (fread(&inp1, sizeof(table), 1, fp)) {
				for (i = 0; i < inp1.count; i++) {
					cout << inp1.col[i].col_name << "(" << inp1.col[i].type << "),size:" << inp1.col[i].size;
					cout << "\t";
				}
			}
			int pri_int;
			int c;
			char d[MAX_NAME];
			char pri_char[MAX_NAME];
			BPtree mytree(tab);
			int ret = 0;
			if (inp1.col[0].type == INT) {
				cout << "\nenter key[col 0] to search\n";
				cin >> pri_int;
				ret = mytree.get_record(pri_int);
				if (ret == BPTREE_SEARCH_NOT_FOUND) {
					printf("\nSearching (%d) -> NOT FOUND !!", pri_int);
				} else {
					//printf("\nSearch (%d) exists -> record num = %d", pri_int, ret);
					printf("\n %d exists \n\n", pri_int);
					TASQL::dataNode("table/" + std::string(tab) + "/file" + std::to_string(ret) + ".dats", inp1).out(cout);
				}
			} else if (inp1.col[0].type == VARCHAR) {
				cout << "\nenter  key[col 0] to search\n";
				cin >> pri_char;
				void *arr[MAX_NAME];
				arr[0] = (char *)malloc(sizeof(char) * MAX_NAME);
				arr[0] = pri_char;
				ret = mytree.get_record(*(int *)arr[0]);
				if (ret == BPTREE_SEARCH_NOT_FOUND) {
					printf("\nSearching (%s) -> NOT FOUND !!", pri_char);
				} else {
					// printf("\nSearching (%s) -> record num = %d", pri_char, ret);
					printf("\n%s exists\n\n", pri_char);
					TASQL::dataNode("table/" + std::string(tab) + "/file" + std::to_string(ret) + ".dats", inp1).out(cout);
				}
			}

		} else {
			printf("%s doesn't exists!!!\n\n", tab);
		}
	} else {
		cout << "wrong input!!!\n\n";
		return;
	}
	free(tab);
}

neb::CJsonObject getItem(neb::CJsonObject &obj) {
	BPtree mytree("ss");
	std::string key;
	obj.Get("bookID", key);
	int ret = mytree.get_record(atoi(key.c_str()));
	if (ret < 0) {
		return neb::CJsonObject(R"({"statu":"404","info":"没有找到目标数据"})");
	}
	table inp1;
	FILE *fp = open_file("ss", const_cast<char *>("r"));
	int i = 0;
	fread(&inp1, sizeof(table), 1, fp);

	return TASQL::dataNode(("table/ss/file" + std::to_string(ret) + ".dats"), inp1).getObj();
}