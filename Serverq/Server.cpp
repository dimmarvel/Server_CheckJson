#include"Precompiled.h"
#include"Json_parser.h"
#include"Server.h"

namespace ServerClient {
	using namespace boost::asio;
	io_service service;

	Client::Client() : client_sock(service) { /*std::cout << "#_DEBUG_# Connect Client sock to servise\n";*/ };

	void Client::start_client() {
		client_sock.async_connect(
			ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 9090),
			boost::bind(&Client::handle_connect, shared_from_this(), _1));
		/*std::cout << "#_DEBUG_# Client started success\n";*/
	}

	void Client::handle_connect(const boost::system::error_code& err) {
		if (err) {
			std::cout << "Error connect: " << err << std::endl;
			do_close_sock();
		}
		do_write();
	}

	void Client::do_close_sock() {
		std::cout << "Closing socket\n";
		if (client_sock.is_open())
		{
			client_sock.close();
		}
		throw std::logic_error("timeout");
	}

	void Client::do_write() {
		std::cout << "Input json( \\ - end of input ):\n";
		int i = 0;
		char ch;
		//std::cin.getline(write_buff, length_mess);
		while (ch = std::cin.get()) {
			if (ch == '\\') {
				write_buff[i] = '\0';
				break;
			}
			write_buff[i++] = ch;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		async_write(client_sock, buffer(write_buff, length_mess),
			boost::bind(&Client::handle_write, shared_from_this(), _1));
	}

	void Client::handle_write(const boost::system::error_code& err) {
		if (err) {
			std::cout << "Write error: " << err << std::endl;
			do_close_sock();
		}
		do_read_answer();
	}

	void Client::do_read_answer() {
		/*std::cout << "#_DEBUG_# Answer from Server reading...\n";*/
		async_read(client_sock, buffer(write_buff, length_mess),
			boost::bind(&Client::handle_read_answer, shared_from_this(), _1));
	}

	void Client::handle_read_answer(const boost::system::error_code& err) {

		if (err) {
			std::cout << "Read answer errror: " << err << std::endl;
			do_close_sock();
		}
		/*std::cout << "#_DEBUG_# Answer from Server received!\n";*/
		std::cout << "Answer from server: " << std::string(write_buff) << std::endl;
		do_write();
	}

	//--------------------------------------------------------------------------------------------------------
	
	Server::Server() : server_socket(service){};

	void Server::start_server() {
		/*std::cout << "#_DEBUG_# Server started." << std::endl;*/
		acceptr.reset(new ip::tcp::acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 9090)));
		acceptr->async_accept(server_socket,
			boost::bind(&Server::handle_accept, shared_from_this(), _1));
	}

	void Server::handle_accept(const boost::system::error_code& err) {
		if (err) {
			std::cout << "Error in accept: " << err << std::endl;
			do_close();
		}
		do_server_read();
	}

	void Server::do_close() {

		/*std::cout << "#_DEBUG_# Close socket" << std::endl;*/
		if (server_socket.is_open()) {
			server_socket.close();
		}

		if (acceptr && acceptr->is_open()) {
			acceptr->close();
		}

		throw std::logic_error("timeout");
	}

	void Server::do_server_read() {
		async_read(server_socket,
			buffer(read_buf, length_message), boost::bind(&Server::handle_read, shared_from_this(), _1));
	}

	void Server::handle_read(const boost::system::error_code& err) {
		if (err) {
			std::cout << "Read error: " << err << std::endl;
			do_close();
		}
		//std::cout << "#_DEBUG_# Server get message from -> Client!\n"
		//	<< "Client(server): " << std::string(read_buf) << std::endl;
		do_write_answer();
	}

	void Server::do_write_answer() {
		/*std::cout << "#_DEBUG_# Answer from Server was sent to client..." << std::endl;*/

		strcpy(read_buf, Json_parser::check_valid(read_buf).c_str());

		async_write(server_socket,
			buffer(read_buf), boost::bind(&Server::handle_write_answer, shared_from_this(), _1));
	}

	void Server::handle_write_answer(const boost::system::error_code& err) {
		if (err) {
			std::cout << "Error in write: " << err << std::endl;
			do_close();
		}
		do_server_read();
	}

};