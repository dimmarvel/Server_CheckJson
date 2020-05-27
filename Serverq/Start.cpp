#include "Precompiled.h" 
#include "Server.h"
int main() {
	boost::shared_ptr<ServerClient::Server> server = boost::make_shared<ServerClient::Server>();
	boost::shared_ptr<ServerClient::Client> client = boost::make_shared<ServerClient::Client>();
	server->start_server();
	client->start_client();
	ServerClient::service.run();
}
