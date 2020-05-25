#include <APIServer.h>
#include <mongoose.h>

#include <iostream>
#include <memory>
#include <thread>
//
#include "declaration.h"
//
#include "BPtree.h"
//
#include "create.h"
//
#include "insert.h"
//
#include "delete.h"
//
#include "display.h"
//
#include "search.h"
//
#include "drop.h"

using namespace std;

void help() {
	std::cout <<
		R"(                        WELCOME
select the query to implement

1.show all tables in database
2.create table
3.insert into table
4.drop table
5.display table contents
6.search table or search inside table
7.help
8.quit
)";
}

int take_input_option() {
	string option;

	fflush(stdout);
	fflush(stdin);
	printf("\n select the query to implement\n");
	help();
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

//starting the system
void start_system() {
	system("clear");

	//cout<<"\t\tType h for help and q for quit\n\n";
	input();
}

static const char* s_http_port = "80";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection* nc, int ev, void* p) {
	if (ev == MG_EV_HTTP_REQUEST) {
		mg_serve_http(nc, (struct http_message*)p, s_http_server_opts);
	}
}

int staticserver() {
	struct mg_mgr mgr;
	struct mg_connection* nc;

	mg_mgr_init(&mgr, NULL);
	printf("Starting static server on port %s\n", s_http_port);
	nc = mg_bind(&mgr, s_http_port, ev_handler);
	if (nc == NULL) {
		printf("Failed to create listener\n");
		return 1;
	}

	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = "./www";	 // Serve current directory
	s_http_server_opts.enable_directory_listing = "yes";

	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);

	return 0;
}

// 初始化APIServer静态类成员
mg_serve_http_opts APIServer::s_server_option;
std::string APIServer::s_web_dir = "./www";
std::unordered_map<std::string, ReqHandler> APIServer::s_handler_map;
std::unordered_set<mg_connection*> APIServer::s_websocket_session_set;
int apiserver() {
	std::string port = "7998";
	auto server = std::shared_ptr<APIServer>(new APIServer);
	server->Init(port);
	server->Start();
}

int main(int argc, char* argv[]) {
	_db_path = "./table/";
	std::thread t(staticserver);
	std::thread t1(apiserver);

	_db_path = "./table/";
	// showTables();
	start_system();

	t.join();
	t1.join();

	return 0;
}