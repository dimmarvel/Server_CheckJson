#pragma once
namespace ServerClient {
	using namespace boost::asio;

	extern io_service service;

	class Client : public boost::enable_shared_from_this<Client>, boost::noncopyable
	{
	private:
		static const int	length_mess = 1024;
		char				write_buff[length_mess];
		ip::tcp::socket		client_sock;
	public:
		Client();
		void start_client();
		void handle_connect(const boost::system::error_code& err);
		void do_close_sock();
		void do_write();
		void handle_write(const boost::system::error_code& err);
		void do_read_answer();
		void handle_read_answer(const boost::system::error_code& err);

	};

	class Server : public boost::enable_shared_from_this<Server>, boost::noncopyable
	{
	private:
		static const int						length_message = 1024;
		char									read_buf[length_message];
		ip::tcp::socket							server_socket;
		boost::scoped_ptr<ip::tcp::acceptor>	acceptr;
	public:
		Server();
		void start_server();
		void handle_accept(const boost::system::error_code& err);
		void do_close();
		void do_server_read();
		void handle_read(const boost::system::error_code& err);
		void do_write_answer();
		void handle_write_answer(const boost::system::error_code& err);
	};
}