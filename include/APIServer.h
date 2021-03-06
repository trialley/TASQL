﻿#pragma once

#include <CJsonObject.h>
#include <mongoose.h>
#include <search.h>
#include <string.h>

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
neb::CJsonObject getTable();
neb::CJsonObject getItem();
void deleteId(int id);
// 定义http返回callback
typedef void OnRspCallback(mg_connection* c, std::string);
// 定义http请求handler
using ReqHandler = std::function<bool(std::string, std::string, mg_connection* c, OnRspCallback)>;

class APIServer {
public:
	APIServer() {}
	~APIServer() {}
	void Init(const std::string& port);								   // 初始化设置
	bool Start();													   // 启动APIServer
	bool Close();													   // 关闭
	void AddHandler(const std::string& url, ReqHandler req_handler);   // 注册事件处理函数
	void RemoveHandler(const std::string& url);						   // 移除时间处理函数
	static std::string s_web_dir;									   // 网页根目录
	static mg_serve_http_opts s_server_option;						   // web服务器选项
	static std::unordered_map<std::string, ReqHandler> s_handler_map;  // 回调函数映射表

private:
	// 静态事件响应函数
	static void OnHttpWebsocketEvent(mg_connection* connection, int event_type, void* event_data);

	static void HandleHttpEvent(mg_connection* connection, http_message* http_req);
	static void SendHttpRsp(mg_connection* connection, std::string rsp);

	static int isWebsocket(const mg_connection* connection);  // 判断是否是websoket类型连接
	static void HandleWebsocketMessage(mg_connection* connection, int event_type, websocket_message* ws_msg);
	static void SendWebsocketMsg(mg_connection* connection, std::string msg);  // 发送消息到指定连接
	static void BroadcastWebsocketMsg(std::string msg);						   // 给所有连接广播消息
	static std::unordered_set<mg_connection*> s_websocket_session_set;		   // 缓存websocket连接

	std::string m_port;	 // 端口
	mg_mgr m_mgr;		 // 连接管理器
};

#include <APIServer.h>
#include <create.h>
#include <declaration.h>
#include <insert.h>

#include <utility>

void APIServer::Init(const std::string& port) {
	m_port = port;
	s_server_option.enable_directory_listing = "yes";
	s_server_option.document_root = s_web_dir.c_str();

	// TODO：其他http设置
}

bool APIServer::Start() {
	mg_mgr_init(&m_mgr, NULL);
	mg_connection* connection = mg_bind(&m_mgr, m_port.c_str(), APIServer::OnHttpWebsocketEvent);
	if (connection == NULL)
		return false;
	// for both http and websocket
	mg_set_protocol_http_websocket(connection);

	printf("starting http server at port: %s\n", m_port.c_str());
	// loop
	while (true) {
		mg_mgr_poll(&m_mgr, 5000);	// ms
	}

	return true;
}

void APIServer::OnHttpWebsocketEvent(mg_connection* connection, int event_type, void* event_data) {
	// 区分http和websocket
	if (event_type == MG_EV_HTTP_REQUEST) {
		http_message* http_req = (http_message*)event_data;
		HandleHttpEvent(connection, http_req);

	} else if (event_type == MG_EV_WEBSOCKET_HANDSHAKE_DONE ||
			   event_type == MG_EV_WEBSOCKET_FRAME ||
			   event_type == MG_EV_CLOSE) {
		websocket_message* ws_message = (struct websocket_message*)event_data;
		HandleWebsocketMessage(connection, event_type, ws_message);
	}
}

// ---- simple http ---- //
static bool route_check(http_message* http_msg, std::string&& route_prefix) {
	if (mg_vcmp(&http_msg->uri, route_prefix.c_str()) == 0)
		return true;
	else
		return false;

	// TODO: 还可以判断 GET, POST, PUT, DELTE等方法
	//mg_vcmp(&http_msg->method, "GET");
	//mg_vcmp(&http_msg->method, "POST");
	//mg_vcmp(&http_msg->method, "PUT");
	//mg_vcmp(&http_msg->method, "DELETE");
}

void APIServer::AddHandler(const std::string& url, ReqHandler req_handler) {
	if (s_handler_map.find(url) != s_handler_map.end())
		return;

	s_handler_map.insert(std::make_pair(url, req_handler));
}

void APIServer::RemoveHandler(const std::string& url) {
	auto it = s_handler_map.find(url);
	if (it != s_handler_map.end())
		s_handler_map.erase(it);
}

void APIServer::SendHttpRsp(mg_connection* connection, std::string rsp) {
	mg_printf(connection, "%s", "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin:*\r\nTransfer-Encoding: chunked\r\n\r\n");

	mg_printf_http_chunk(connection, "%s", rsp.c_str());

	mg_send_http_chunk(connection, "", 0);
}
void APIServer::HandleHttpEvent(mg_connection* connection, http_message* http_req) {
	std::string req_str = std::string(http_req->message.p, http_req->message.len);
	printf("got request: %s\n", req_str.c_str());

	std::string url = std::string(http_req->uri.p, http_req->uri.len);
	std::string body = std::string(http_req->body.p, http_req->body.len);
	auto it = s_handler_map.find(url);
	if (it != s_handler_map.end()) {
		ReqHandler handle_func = it->second;
		handle_func(url, body, connection, &APIServer::SendHttpRsp);
	}
	//开始解析输入
	neb::CJsonObject data(body);
	std::string bookID;
	std::string bookAuthor;
	std::string bookName;
	std::string bookLeftNum;
	std::string bookTotalNum;
	if (!data.Get("bookID", bookID)) {
		std::cout << "no type\n";
	} else {
		std::cout << bookID << std::endl;
	}
	if (!data.Get("bookName", bookName)) {
		std::cout << "no type\n";
	} else {
		std::cout << bookName << std::endl;
	}
	if (!data.Get("bookAuthor", bookAuthor)) {
		std::cout << "no type\n";
	} else {
		std::cout << bookAuthor << std::endl;
	}
	if (!data.Get("bookLeftNum", bookLeftNum)) {
		std::cout << "no type\n";
	}
	if (!data.Get("bookTotalNum", bookTotalNum)) {
		std::cout << "no type\n";
	} else {
		std::cout << bookLeftNum << std::endl;
	}
	neb::CJsonObject obj;
	obj.Add("bookID", bookID);
	obj.Add("bookName", bookName);
	obj.Add("bookAuthor", bookAuthor);
	obj.Add("bookLeftNum", bookLeftNum);
	obj.Add("bookTotalNum", bookTotalNum);
	std::string result;

	if (route_check(http_req, std::string("/getItem"))) {
		result = getItem(obj).ToString();
	} else if (route_check(http_req, std::string("/getTable"))) {
		result = getTable().ToString();
	} else if (route_check(http_req, std::string("/getBTree"))) {
		result = getBTree().ToString();
	} else if (route_check(http_req, std::string("/showTables"))) {
		result = showTables().ToString();
	} else if (route_check(http_req, std::string("/createItem"))) {	 //采编入库
		insertObj(obj);

		result = R"(
			{
				"type":"info",
				"result":"success"
			}
		)";
	} else if (route_check(http_req, std::string("/deleteItem"))) {	 //注销
		deleteId(atoi(bookID.c_str()));
	} else if (route_check(http_req, std::string("/borrowItem"))) {
		borrowObj(obj, -1, false);

	} else if (route_check(http_req, std::string("/returnItem"))) {
		borrowObj(obj, 1, false);

	} else {
		mg_printf(
			connection,
			"%s",
			"HTTP/1.1 501 Not Implemented\r\n"	// 也可以用HTTP/2.0
			"Content-Length: 0\r\n\r\n");
	}
	SendHttpRsp(connection, result);
}

// ---- websocket ---- //
int APIServer::isWebsocket(const mg_connection* connection) {
	return connection->flags & MG_F_IS_WEBSOCKET;
}

void APIServer::HandleWebsocketMessage(mg_connection* connection, int event_type, websocket_message* ws_msg) {
	if (event_type == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
		printf("client websocket connected\n");
		// 获取连接客户端的IP和端口
		char addr[32];
		mg_sock_addr_to_str(&connection->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
		printf("client addr: %s\n", addr);

		// 添加 session
		s_websocket_session_set.insert(connection);

		SendWebsocketMsg(connection, "client websocket connected");
	} else if (event_type == MG_EV_WEBSOCKET_FRAME) {
		mg_str received_msg = {
			(char*)ws_msg->data, ws_msg->size};

		char buff[1024] = {0};
		strncpy(buff, received_msg.p, received_msg.len);  // must use strncpy, specifiy memory pointer and length

		// do sth to process request
		printf("received msg: %s\n", buff);

		SendWebsocketMsg(connection, "send your msg back: " + std::string(buff));

	} else if (event_type == MG_EV_CLOSE) {
		if (isWebsocket(connection)) {
			printf("client websocket closed\n");
			// 移除session
			if (s_websocket_session_set.find(connection) != s_websocket_session_set.end())
				s_websocket_session_set.erase(connection);
		}
	}
}

void APIServer::SendWebsocketMsg(mg_connection* connection, std::string msg) {
	mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, msg.c_str(), strlen(msg.c_str()));
}

void APIServer::BroadcastWebsocketMsg(std::string msg) {
	for (mg_connection* connection : s_websocket_session_set)
		mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, msg.c_str(), strlen(msg.c_str()));
}

bool APIServer::Close() {
	mg_mgr_free(&m_mgr);
	return true;
}
