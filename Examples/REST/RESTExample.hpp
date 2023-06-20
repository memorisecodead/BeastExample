#pragma once 

#include "Beast/beast.hpp"

class RESTExample
{
	const std::string host;
	const std::string port;

	tcp::socket socket;
	tcp::resolver resolver;
public:
	RESTExample(asio::io_context& context, const std::string str_host, const std::string str_port) 
		: host(str_host)
		, port(str_port)
		, socket(context)
		, resolver(context)
	{}

	~RESTExample() = default;

	RESTExample(RESTExample const& other) = delete;
	RESTExample& operator=(RESTExample const& other) = delete;
	RESTExample(RESTExample&& other) noexcept = delete;
	RESTExample& operator=(RESTExample&& other) noexcept = delete;

	void SendRequest(http::verb verb,const std::string& target)
	{
		asio::connect(socket, resolver.resolve(host, port));

		auto Request = SetRESTRequest<http::string_body>(verb, target);

		http::write(socket, Request);

		readResponce();

		closeConnection();
	}

private:
	template<typename Body>
	const http::request<Body> SetRESTRequest(http::verb verb, const std::string& target,
		unsigned int version = 11)
	{
		switch (verb)
		{
		case http::verb::get:
		{
			// Create an HTTP requester specifying the target
			http::request<Body> req(verb, target, version);

			// Set HTTP header fields
			req.set(http::field::host, host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			return req;
		}
		break;

		case http::verb::put:
		{
			// Set up an HTTP PUT request message
			http::request<http::string_body> req{verb, target, version};
			req.set(http::field::host, host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			req.set(http::field::content_type, "text/plain");
			req.body() = "Hello, REST Example!";

			return req;
		}
		break;

		case http::verb::post:
		{
			// Set up an HTTP POST request message
			http::request<Body> req{verb, target, version};
			req.set(http::field::host, host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			req.set(http::field::content_type, "application/x-www-form-urlencoded");
			req.body() = "param1=value1&param2=value2";

			return req;
		}
		break;

		case http::verb::delete_:
		{
			// Set up an HTTP DELETE request message
			http::request<Body> req{http::verb::delete_, target, 11};
			req.set(http::field::host, host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			return req;
		}
		break;

		default:
			std::cout << "No implementation for "
				<< verb << std::endl; break;
		}

		return http::request<Body>();
	}

	void readResponce()
	{
		// This buffer is used for reading and must be persisted
		boost::beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(socket, buffer, res);

		// Write the message to standard out
		std::cout << res << std::endl;
	}

	void closeConnection()
	{
		beast::error_code ec;

		// close the connection
		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

		// not_connected happens sometimes
		// so don't bother reporting it.
		if (ec && ec != beast::errc::not_connected)
			throw beast::system_error{ec};

		// If we get here then the connection is closed gracefully
	}
};
