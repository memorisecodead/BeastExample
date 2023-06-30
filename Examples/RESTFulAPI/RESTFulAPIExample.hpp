#pragma once

#include "Beast/beast.hpp"

/*
* @brief RESTFulExample
* @details 
*/
class RESTFulExample
{
	std::string address;
	uint16_t port;

	tcp::socket socket;
	tcp::acceptor acceptor;
public:
	RESTFulExample(asio::io_context& context,
		std::string str_address, uint16_t _port)
		: address(str_address)
		, port(_port)
		, socket(context)
		, acceptor(context, { boost::asio::ip::make_address(address), port })
	{
		acceptor.accept(socket);
	}

	~RESTFulExample() = default;

	RESTFulExample(RESTFulExample const& other) = delete;
	RESTFulExample& operator=(RESTFulExample const& other) = delete;
	RESTFulExample(RESTFulExample&& other) noexcept = delete;
	RESTFulExample& operator=(RESTFulExample&& other) noexcept = delete;

	http::request<http::string_body> CreateRequest(const std::string& target)
	{
		http::request<http::string_body> request;
		request.set(http::field::host, "127.0.0.1");
		request.set(http::field::user_agent, "Boost Beast");
		request.method(http::verb::get);
		request.target(target);
		request.body() = "Hello, example!";

		std::cout << request.method() << std::endl;
		std::cout << request.target() << std::endl;

		return request;
	}

	void ReadRequest(http::request<http::string_body> request)
	{
		boost::beast::flat_buffer buffer;
		boost::system::error_code ec;

		http::read(socket, buffer, request, ec);

		if (ec == http::error::end_of_stream)
		{
			std::cerr << "Error: " << ec.message() << "\n";
		}
	}

	void HandleRequest(http::request<http::string_body> request)
	{
		boost::system::error_code ec;

		if (request.method() == http::verb::get && request.target() == "/hello")
		{
			// Send a response
			http::response<http::string_body> response{http::status::ok, request.version()};
			response.set(http::field::server, "Boost Beast");
			response.set(http::field::content_type, "text/plain");
			response.keep_alive(request.keep_alive());
			response.body() = "Hello, example!";
			response.prepare_payload();

			http::write(socket, response, ec);

			if (ec)
			{
				std::cerr << ec.message() << "\n";
			}
		}
		else if (request.method() == http::verb::get && request.target() == "/goodbye")
		{
			http::response<http::string_body> response{http::status::ok, request.version()};
			response.set(http::field::server, "Boost Beast");
			response.set(http::field::content_type, "text/plain");
			response.keep_alive(request.keep_alive());
			response.body() = "Goodbye, example!";
			response.prepare_payload();

			http::write(socket, response, ec);

			if (ec)
			{
				std::cerr << ec.message() << "\n";
			}
		}
		else
		{
			// Send a "not found" response
			http::response<http::string_body> response{http::status::not_found, request.version()};
			response.set(http::field::server, "Boost Beast");
			response.set(http::field::content_type, "text/plain");
			response.keep_alive(request.keep_alive());
			response.body() = "Not found";
			response.prepare_payload();

			http::write(socket, response, ec);

			if (ec)
			{
				std::cerr << ec.message() << "\n";
			}
		}
	}
};
