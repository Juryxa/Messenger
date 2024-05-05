#define _CRT_SECURE_NO_WARNINGS

#include "../header/Server.h"
#include "../header/UserModel.h"

#include<uwebsockets/App.h>

#include <iostream>

void Server::run()
{
	uWS::SSLApp({
	.key_file_name = "../misc/key.pem",
	.cert_file_name = "../misc/cert.pem",
	.passphrase = "dima15042004"
	})
		.post("/signup", [&](auto* res, auto* req) {
			request_handler_->HandleSignUp(res, req);

		}).post("/login", [&](auto* res, auto* req){
			request_handler_->HandleLogIn(res, req);

		/*}).post("/client/DisplayUsers", [&](auto* res, auto* req) {
			request_handler_->HandledDisplayUsers(res, req);*/

		}).post("/client/SearchUser", [&](auto* res, auto* req) {
			request_handler_->HandleSearchUser(res, req);
		})

		.ws<WebSocketUser>("/*",{
		.compression = uWS::SHARED_COMPRESSOR,
		.maxPayloadLength = 10 * 1024,
		.idleTimeout = 666,
		.maxBackpressure = 1 * 1024 * 1024,

		.open = [&](auto* ws){
			  messager_handler_->ConnectedUser(ws);
		},

		.message = [&](auto* ws, std::string_view message, uWS::OpCode){
			 messager_handler_->ProcessMessage(ws,message);
		},

		.close = [&](auto* ws, int code, std::string_view message){
			messager_handler_->DisconnectedUser(ws,code,message);
		}

		})
		.options("/*", [&](auto* res, auto* req) 
		{
			// for policy CORS and not only
			res->writeHeader("Access-Control-Allow-Origin", "*");
			res->writeHeader("Access-Control-Allow-Headers", "Content-Type");
			res->writeHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
			res->writeStatus("200 OK");
			res->end();
		})
		.listen(this->port_, [&](const auto* listenSocket)
		{
			if (listenSocket) {
				std::cout << "Server is running" << '\n';
			}
			else {
				std::cout << "Server Failed" << '\n';
			}
		}).run();
}
