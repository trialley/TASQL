

#include <CJsonObject.h>

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
				FILE *fpr;
				char *str;
				str = (char *)malloc(sizeof(char) * MAX_PATH);
				sprintf(str, "table/%s/file%d.dat", tab, i);
				//cout<<str<<endl;
				fpr = fopen(str, "r");
				//cout<<*(int *)data1[0]<<" ";
				//fread(data2,temp->count,tot,fpr);
				//fread(data2,1,tot,fpr);
				for (int j = 0; j < temp->count; j++) {
					if (temp->col[j].type == INT) {
						fread(&c, 1, sizeof(int), fpr);
						cout << c << setw(20);
					} else if (temp->col[j].type == VARCHAR) {
						fread(d, 1, sizeof(char) * MAX_NAME, fpr);
						cout << d << setw(20);
					}
				}
				cout << "\n\n";

				free(str);
				fclose(fpr);
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

neb::CJsonObject getBTree() {
	neb::CJsonObject data(R"(
		{
			"type":"BTree",
			"data":{

			}
		}
	)");
	auto createBTree = [&]() -> void {
		//search for particular table or any specific entry inside table
		cout << "1.search table\n2.search particular entry in table\n\n";
		char *tab = (char *)malloc(sizeof(char) * MAX_NAME);

		cout << "enter table name: ";
		tab = "ss";
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
					//print the details of the particular row;
					FILE *fpz;
					char *str1;
					printf("\n------------------------------------\n");
					str1 = (char *)malloc(sizeof(char) * MAX_PATH);
					sprintf(str1, "table/%s/file%d.dat", tab, ret);
					fpz = fopen(str1, "r");

					for (int j = 0; j < inp1.count; j++) {
						if (inp1.col[j].type == INT) {
							fread(&c, 1, sizeof(int), fpz);
							cout << c << "\t";
						} else if (inp1.col[j].type == VARCHAR) {
							fread(d, 1, sizeof(char) * MAX_NAME, fpz);
							cout << d << "\t";
						}
					}
					printf("\n------------------------------------\n");
					fclose(fpz);
					free(str1);
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

					//print the details of the particular row;
					FILE *fpz;
					char *str1;
					str1 = (char *)malloc(sizeof(char) * MAX_PATH);
					sprintf(str1, "table/%s/file%d.dat", tab, ret);
					fpz = fopen(str1, "r");
					printf("\n------------------------------------\n");
					for (int j = 0; j < inp1.count; j++) {
						if (inp1.col[j].type == INT) {
							fread(&c, 1, sizeof(int), fpz);
							cout << c << "\t";
						} else if (inp1.col[j].type == VARCHAR) {
							fread(d, 1, sizeof(char) * MAX_NAME, fpz);
							cout << d << "\t";
						}
					}
					printf("\n-------------------------------------\n");
					fclose(fpz);
					free(str1);
				}
			}

		} else {
			printf("%s doesn't exists!!!\n\n", tab);
		}
		free(tab);
	};

	createBTree();
	return data;
}
