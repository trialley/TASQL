//
#include "declaration.h"
//
#include "BPtree.h"
//
#include "create.h"
//
#include "insert.h"
//
#include "display.h"
//
#include "search.h"
//
#include <APIServer.h>

#include "drop.h"
using namespace std;

void help() {
	std::cout <<
		R"(TASQL
myDB is a simple database design engine in which you can implement basic queries.

QUERIES SUPPORTED ARE::
1.create a new table
2.insert data into existing table
3.drop table
4.search in the table

1.For creating table
a>enter the table name
b>enter no. of columns
c>enter col name,datatype(1.INT 2.VARCHAR) and maximum size for it.

2.For inserting data into table
a>enter table name
b>it will display how many details to be filled
c>enter all the details
d>your data is inserted into the table

3.For deleting table just enter the table name

4.For search into table
a>you can search for a particular table if it exists or not
b>b>You can search for a particular entry if it exists in the table or not
c>For particular entry searching , search is based on primary key, so enter col[0] value of table to search

---------------------------------------------------------------------------------------------------
                        designed by::

                        MANDEEP SINGH
                        PAWAN SHEORAN
)";
}

int take_input_option() {
	string option;
	fflush(stdout);
	fflush(stdin);
	printf("\n select the query to implement\n");
	printf("\n1.show all tables in database\n2.create table\n3.insert into table\n4.drop table\n5.display table contents\n6.search table or search inside table\n7.help\n8.quit\n\n");
	cin >> option;
	if (option.length() > 1) {
		printf("\nwrong input\nexiting...\n\n");
		exit(0);
	} else {
		if (option[0] > 48 && option[0] < 58) {
			return option[0] - 48;
		} else {
			printf("\nwrong input\nexiting...\n\n");
			exit(0);
		}
	}
}

void input() {
	int c = take_input_option();
	while (c < 9 && c > 0) {
		switch (c) {
		case 1:
			showTables();
			break;
		case 2:
			create();
			break;
		case 3:
			insert();
			break;
		case 4:
			drop();
			break;
		case 5:
			displayTable();
			break;
		case 6:
			search();
			break;
		case 8:
			printf("\nexiting...\n");
			printf("\n\t\t good bye!!!\n\n");
			exit(0);
			break;
		case 7:
			help();
			break;
		default:
			printf("\nplease choose a correct option\n");
			break;
		}
		c = take_input_option();
	}
}
// static const char* s_http_port = "80";
// static struct mg_serve_http_opts s_http_server_opts;
// 初始化APIServer静态类成员
mg_serve_http_opts APIServer::s_server_option;
std::string APIServer::s_web_dir = "./www";
std::unordered_map<std::string, ReqHandler> APIServer::s_handler_map;
std::unordered_set<mg_connection*> APIServer::s_websocket_session_set;
//starting the system
void start_system() {
	system("clear");
	printf("\n\t\t\tWELCOME\n\n");
	printf("\t\tWelcome to myDB monitor \n\n");
	//cout<<"\t\tType h for help and q for quit\n\n";
	input();
}

int main() {
	_db_path = "./table/";
	// showTables();
	start_system();
	return 0;
}
