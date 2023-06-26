#pragma once

#include "Beast/beast.hpp"

class SimpleRESTAPI
{
	std::string address;
	uint16_t port;

	tcp::socket socket;
	tcp::acceptor acceptor;
public:
	SimpleRESTAPI(asio::io_context& context,
		std::string str_address, uint16_t _port)
		: address(str_address)
		, port(_port)
		, socket(context)
		, acceptor(context, { boost::asio::ip::make_address(address), port})
	{
		acceptor.accept(socket);
	}

	SimpleRESTAPI(SimpleRESTAPI const& other) = delete;
	SimpleRESTAPI& operator=(SimpleRESTAPI const& other) = delete;
	SimpleRESTAPI(SimpleRESTAPI&& other) noexcept = delete;
	SimpleRESTAPI& operator=(SimpleRESTAPI&& other) noexcept = delete;

	const http::request<http::string_body> CreateSimpleRequest()
	{
        boost::beast::flat_buffer buffer;
        http::request<http::string_body> request;
        boost::system::error_code ec;
        http::read(socket, buffer, request, ec);
        if (ec == http::error::end_of_stream) 
		{
			std::cerr << "Error: " << ec.message() << "\n";
			return http::request<http::string_body>();
        }
        if (ec) 
		{
            std::cerr << "Error: " << ec.message() << "\n";
			return http::request<http::string_body>();
        }

		return request;
	}

	const http::response<http::string_body> SendResponse(http::request<http::string_body>& request)
	{
		http::response<http::string_body> response{http::status::ok, request.version()};
		response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response.set(http::field::content_type, "text/html");
		response.keep_alive(request.keep_alive());
		response.body() = "Hello, RESTAPI Example!";
		response.prepare_payload();

		boost::system::error_code ec;
		http::write(socket, response, ec);
		if (ec) 
		{
			std::cerr << "Error: " << ec.message() << "\n";
			return http::response<http::string_body>();
		}
		if (!request.keep_alive()) 
		{
			return http::response<http::string_body>();
		}

		return response;
	}
};
