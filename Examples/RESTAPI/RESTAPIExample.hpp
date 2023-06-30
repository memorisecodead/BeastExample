#pragma once

#include "Beast/beast.hpp"

/*
* @brief class SimpleRESTAPI
* @details This example creates a simple web server that listens
*          on the port specified as the command line argument and responds
*		   to HTTP requests with a status of 200 OK and the response body "Hello, RESTAPI Example!".
*/
class RESTAPIExample
{
	std::string address;
	uint16_t port;

	tcp::socket socket;
	tcp::acceptor acceptor;
public:
	RESTAPIExample(asio::io_context& context,
		std::string str_address, uint16_t _port)
		: address(str_address)
		, port(_port)
		, socket(context)
		, acceptor(context, { boost::asio::ip::make_address(address), port })
	{
		acceptor.accept(socket);
	}

	~RESTAPIExample() = default;

	RESTAPIExample(RESTAPIExample const& other) = delete;
	RESTAPIExample& operator=(RESTAPIExample const& other) = delete;
	RESTAPIExample(RESTAPIExample&& other) noexcept = delete;
	RESTAPIExample& operator=(RESTAPIExample&& other) noexcept = delete;

	/*
	* @details Uses boost::beast::flat_buffer to read request from socket
	*		   and http::request object to parse request.
	*		   If the request is successfully parsed,
	*		   a response is created using the http::response object.
	*/
	http::request<http::string_body> CreateRequest()
	{
		boost::beast::flat_buffer buffer;

		http::request<http::string_body> request{};
		request.set(http::field::host, "127.0.0.1");
		request.set(http::field::user_agent, "Boost Beast");

		boost::system::error_code ec;
		http::read(socket, buffer, request, ec);

		if (ec == http::error::end_of_stream)
		{
			std::cerr << "Error: " << ec.message() << "\n";
			return http::request<http::string_body>();
		}

		return request;
	}

	/*
	* @details In this example we set the response status to http::status::ok,
	*		   the Server header to the current version of Boost.Beast and the content type to text/html.
	*		   We also set the keep-alive flag for the response to maintain a persistent connection.
	*		   We then set the body of the response to "Hello, RESTAPI Example!" and prepare it for sending
	*		   using the prepare_payload() method.
	*		   We send the response to the client using the http::write() method
	*/
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
			return response;
		}

		return response;
	}
};
