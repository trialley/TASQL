

#pragma once

#include <CJsonObject.h>
#include <dataNode.h>

#include <filesystem>
#include <iterator>
#include <string>

#include "declaration.h"
void displayTable() {
	char *tab;
	tab = (char *)malloc(sizeof(char) * MAX_PATH);
	cout << "enter table name to display\n";
	cin >> tab;
	int ret = search_table(tab);
	if (ret == 0) {
		printf("%s doesn't exist\n\n", tab);
		return;
	} else if (ret == 1) {
		//table exists

		table *temp;
		int count;
		int tot = 0;
		temp = (table *)malloc(sizeof(table));
		printf("\n-----------------------------------------------\n");
		FILE *fp = open_file(tab, const_cast<char *>("r"));
		if (fp) {
			fread(temp, 1, sizeof(table), fp);
			count = temp->count;
			void *data2[MAX_ATTR];
			for (int i = 0; i < count; i++) {
				cout << temp->col[i].col_name << setw(20);
				if (temp->col[i].type == INT) {
					data2[i] = (int *)malloc(sizeof(int));
					if (data2[i] == NULL) {
						cout << "NULL";
						return;
					}
					tot += sizeof(int);
				} else if (temp->col[i].type == VARCHAR) {
					data2[i] = malloc(sizeof(char) * (MAX_NAME + 1));
					if (data2[i] == NULL) {
						cout << "NULL";
						return;
					}
					tot += (sizeof(char) * (MAX_NAME + 1));
				}
			}
			fclose(fp);
			cout << "\n";
			printf("-----------------------------------------------\n");

			int c;
			char d[MAX_NAME];
			//cout<<"C........"<<c<<endl;
			for (int i = 0; i < temp->rec_count; i++) {
				TASQL::dataNode("table/" + std::string(tab) + "/file" + std::to_string(i) + ".dats", *temp).out(cout);
				cout << "\n";
			}
			printf("-----------------------------------------------\n");
			//fclose(fp);
		}
	}
}
std::string _db_path;
neb::CJsonObject showTables() {
	neb::CJsonObject data(R"(
		{
			"type":"tables",
			"tables":[]
		}
	)");

	for (auto &name : std::filesystem::directory_iterator(_db_path)) {
		//如果不想要路径加上.filename()即可.path
		if (name.is_directory()) {
			std::cout << name.path().filename() << std::endl;
			data["tables"].Add(name.path().filename());
		}
	}
	return data;
}
neb::CJsonObject createDataNode(int i) {
	neb::CJsonObject data;
	int c;
	FILE *fpr;
	table *temp;
	int count;
	int tot = 0;
	char d[MAX_NAME];
	temp = (table *)malloc(sizeof(table));
	// printf("\n-----------------------------------------------\n");
	FILE *fp = open_file("ss", "r");
	fread(temp, 1, sizeof(table), fp);
	TASQL::dataNode dnode("table/ss/file" + std::to_string(i) + ".dats", *temp);
	data.Add("name", dnode.getObj().ToFormattedString());
	return data;  //竟然忘记返回值
}
neb::CJsonObject createNode(int i) {
	neb::CJsonObject new_note;
	// new_note["data"]["name"] += (std::to_string(i) + '\n');
	std::ifstream temp_in("./table/ss/tree/tree" + std::to_string(i) + ".dat");
	int isroot;
	int indexnum;
	int subnum;
	temp_in >> isroot;
	std::string name = "";
	if (isroot == 0) {	//
		name = "root\n";
	}
	name += ("ID:");
	temp_in >> indexnum;
	int ids[indexnum];
	for (int i = 0; i < indexnum; ++i) {
		temp_in >> ids[i];
		name += (" " + std::to_string(ids[i]));
	}
	name += "\nPOS:";
	temp_in >> subnum;
	int subs[subnum];
	new_note.AddEmptySubArray("children");
	for (int i = 0; i < subnum; ++i) {
		temp_in >> subs[i];
		name += (" " + std::to_string(subs[i]));
	}
	int next;
	temp_in >> next;
	name = ("node" + std::to_string(i) + "->" + std::to_string(next)) + "\n" + name;
	new_note.Add("name", name);
	new_note.AddEmptySubArray("children");

	if (isroot == 0) {	//不是底部节点，还有子节点
		for (int i = 0; i < subnum; ++i) {
			new_note["children"].Add(createNode(subs[i]));
		}
	} else {  //是底部节点，需要添加数据子节点
		for (int i = 0; i < subnum; ++i) {
			new_note["children"].Add(createDataNode(subs[i]));
		}
	}

	return new_note;
}
neb::CJsonObject getBTree() {
	neb::CJsonObject data(R"(
		{
			"type":"BTree",
			"data":{
				"name":"root",
				"children":[]
			}
		}
	)");
	//make root
	std::ifstream temp_in("./table/ss/tree/meta_tree.dat");
	int nodesnum, root_num;
	temp_in >> nodesnum >> root_num;

	return createNode(root_num);
}

neb::CJsonObject createTableDataNode(int i) {
	neb::CJsonObject data(R"(
{
	"bookID": 4,
	"bookName": 4,
	"bookLeftNum": 4,
	"bookAuthor": 4,
	"bookTotalNum": 4
}	
	)");
	int c;
	FILE *fpr;
	table *temp;
	int count;
	int tot = 0;
	char d[MAX_NAME];
	temp = (table *)malloc(sizeof(table));
	FILE *fp = open_file("ss", "r");
	fread(temp, 1, sizeof(table), fp);
	std::string res;
	std::string strTraversing;
	TASQL::dataNode ttt(("table/ss/file" + std::to_string(i) + ".dats"), *temp);
	return ttt.getObj();  //竟然忘记返回值
}
void createTableNode(int i, neb::CJsonObject &new_note) {
	// new_note["data"]["name"] += (std::to_string(i) + '\n');
	std::ifstream temp_in("./table/ss/tree/tree" + std::to_string(i) + ".dat");
	int isroot;
	int indexnum;
	int subnum;
	temp_in >> isroot;
	std::string name = "";
	if (isroot == 0) {	//
		name = "root\n";
	}
	name += ("ID:");
	temp_in >> indexnum;
	int ids[indexnum];
	for (int i = 0; i < indexnum; ++i) {
		temp_in >> ids[i];
		name += (" " + std::to_string(ids[i]));
	}
	name += "\nPOS:";
	temp_in >> subnum;
	int subs[subnum];
	// new_note.AddEmptySubArray("children");
	for (int i = 0; i < subnum; ++i) {
		temp_in >> subs[i];
		name += (" " + std::to_string(subs[i]));
	}
	int next;
	temp_in >> next;
	name = ("node" + std::to_string(i) + "->" + std::to_string(next)) + "\n" + name;
	// new_note.Add("name", name);
	// new_note.AddEmptySubArray("children");

	if (isroot == 0) {	//不是底部节点，还有子节点
		for (int i = 0; i < subnum; ++i) {
			createTableNode(subs[i], new_note);
		}
	} else {  //是底部节点，需要添加数据子节点
		for (int i = 0; i < subnum; ++i) {
			new_note["table"].Add(createTableDataNode(subs[i]));
		}
	}
}
neb::CJsonObject getTable() {
	neb::CJsonObject data(R"(
		{
			"type":"Table",
			"table":[]
		}
	)");
	//make root
	std::ifstream temp_in("./table/ss/tree/meta_tree.dat");
	int nodesnum, root_num;
	temp_in >> nodesnum >> root_num;
	createTableNode(root_num, data);
	return data;
}
