#pragma once
#include <BPtree.h>
#include <declaration.h>
#include <file_handler.h>

void delete_command(char tname[], int key) {
	table *temp;
	int ret;
	BPtree obj(tname);
	//open meta data
	FILE *fp = open_file(tname, const_cast<char *>("r"));
	temp = (table *)malloc(sizeof(table));
	fread(temp, sizeof(table), 1, fp);

	//insert into table and write to btree file nodes
	ret = obj.delete_record(key);
	if (ret == BPTREE_INSERT_ERROR_EXIST) {
		cout << "key not exists" << std::endl;
		return;
	}

	free(temp);
}

void deletei() {
	char *tab;
	tab = (char *)malloc(sizeof(char) * MAX_PATH + 1);
	cout << "enter table name: ";
	cin >> tab;
	int check = search_table(tab);
	if (check == 0) {
		printf("Table %s not exists\n", tab);
		return;
	}

	else {
		cout << "\nTable exists enter data\n\n";
		char dir[100];
		strcpy(dir, "./table/");
		strcat(dir, tab);
		strcat(dir, "/meta");
		table inp1;
		int count;
		//read column details from file;
		FILE *fp = open_file(tab, const_cast<char *>("r"));
		int i = 0;
		while (fread(&inp1, sizeof(table), 1, fp)) {
			printf("------------------------------------\n");
			cout << "\ninsert the following details ::\n";
			printf("\n------------------------------------\n");
			count = inp1.count;
			for (i = 0; i < inp1.count; i++) {
				cout << inp1.col[i].col_name << "(" << inp1.col[i].type << "),size:" << inp1.col[i].size;
				cout << "\t";
			}
		}
		printf("\n------------------------------------\n");
		//enter data;
		char var[MAX_NAME + 1];
		void *data[MAX_ATTR];
		//void *data1[MAX_ATTR];

		//input data for the table of desired datatype 1.int 2.varchar;
		int size = 0;
		int total = 0;
		for (int i = 0; i < count; i++) {
			if (inp1.col[i].type == INT) {
				data[i] = (int *)malloc(sizeof(int));
				total += sizeof(int);
				string inp_int;
				cin >> inp_int;
				if (inp_int.length() > (unsigned)inp1.col[i].size) {
					printf("\nwrong input, size <= %d\nexiting...\n", inp1.col[i].size);
					return;
				} else {
					//verify if entered input is integer and not a string;
					int num = 0;
					int factor_10 = 1;
					for (int j = inp_int.length() - 1; j >= 0; j--) {
						if (inp_int[j] < 48 || inp_int[j] > 57) {
							printf("\nwrong input, input should be integer\nexiting...\n");
							return;
						} else {
							num += (inp_int[j] - 48) * factor_10;
							factor_10 = factor_10 * 10;
						}
					}
					*((int *)data[i]) = num;
				}
				size++;
			} else if (inp1.col[i].type == VARCHAR) {
				//cout<<"inside varchar\n";
				data[i] = malloc(sizeof(char) * (MAX_NAME + 1));
				int flag = 1;
				while (flag) {
					cin >> var;
					total += sizeof(char) * (MAX_NAME + 1);
					if (strlen(var) > (unsigned int)inp1.col[i].size) {
						cout << "error\nEntered size of string is greater than specified \n";
					} else
						flag = 0;
				}
				strcpy((char *)(data[i]), var);
				//cout<<(char *)data[1]<<endl;
				size++;
			}
		}
		insert_command(tab, data, total);
	}
	free(tab);
}

void deleteId(int id) {
	char *tab;
	tab = (char *)malloc(sizeof(char) * MAX_PATH + 1);
	cout << "enter table name: " << std::endl;
	strcpy(tab, "ss");
	int check = search_table(tab);
	if (check == 0) {
		printf("Table %s not exists\n", tab);
		return;
	} else {
		cout << "\nTable exists enter data\n\n";
		char dir[100];
		strcpy(dir, "./table/");
		strcat(dir, tab);
		strcat(dir, "/meta");
		delete_command(tab, id);
	}
	free(tab);
}