#pragma once 

#include "Beast/beast.hpp"

/*
* @brief class HTTPExamples created /
*		 for understand how http is works reasons.
* @details There are many other requests not covered here. 
*		   All other queries are formed in a similar way, /
*		   so you can operate to create others based on the proposed implementation. 
*/
class HTTPExamples
{
	tcp::socket _socket;
	tcp::resolver _resolver;

	const std::string _host;
	const std::string _port;

public:
	HTTPExamples(asio::io_context& context, const std::string host, const std::string port)
		: _host(host)
		, _port(port)
		, _socket(context)
		, _resolver(context)
	{}

	~HTTPExamples() = default;

	HTTPExamples(HTTPExamples const& other) = delete;
	HTTPExamples& operator=(HTTPExamples const& other) = delete;
	HTTPExamples(HTTPExamples&& other) noexcept = delete;
	HTTPExamples& operator=(HTTPExamples&& other) noexcept = delete;

	/*
	* @brief Is used to request a resource from the server. 
	*		 This request does not change the state of the server and can be cached.
	*/
	void GETRequest(const std::string target)
	{
		// Resolve the address and connect
		asio::connect(_socket, _resolver.resolve(_host, _port));

		auto getRequest = createRequest(http::verb::get, target);

		// Sending Requests via a Connected Socket
		http::write(_socket, getRequest);

		// Part responsible for reading the response
		readResponce();

		// close the connection
		closeConnection();

		std::cout << "HTTP GET Request Had finished\n\n";
	}

	/*
	* @brief Is used to send data to the server for processing. 
	*		 This request can change the state of the server and cannot be cached.
	*/
	void POSTRequest(const std::string target)
	{
		// Resolve the address and connect
		asio::connect(_socket, _resolver.resolve(_host, _port));

		auto postRequest = createRequest(http::verb::post, target);

		// Sending Requests via a Connected Socket
		http::write(_socket, postRequest);

		// Part responsible for reading the response
		readResponce();

		// close the connection
		closeConnection();

		std::cout << "HTTP POST Request Had finished\n\n";
	}

	/*
	* @brief Is used to delete the resource on the server.
	*/
	void DELETERequest(const std::string target)
	{
		// Resolve the address and connect
		asio::connect(_socket, _resolver.resolve(_host, _port));

		auto deleteRequest = createEmptyRequest(http::verb::delete_, target);

		// Sending Requests via a Connected Socket
		http::write(_socket, deleteRequest);

		// Part responsible for reading the response
		readResponce();

		// close the connection
		closeConnection();

		std::cout << "HTTP DELETE Request Had finished\n\n";
	}

	/*
	* @brief Is used to upload content to the server. 
	*		 This request can create or replace a resource on the server.
	*/
	void PUTRequest(const std::string target)
	{
		// Resolve the address and connect
		asio::connect(_socket, _resolver.resolve(_host, _port));

		auto putRequest = createRequest(http::verb::put, target);

		// Sending Requests via a Connected Socket
		http::write(_socket, putRequest);

		// Part responsible for reading the response
		readResponce();

		// close the connection
		closeConnection();

		std::cout << "HTTP PUT Request Had finished\n\n";
	}

	/*
	* @brief The resource is used to obtain metadata about the resource, /
	*		 without actually transmitting it.
	*/
	void HEADRequest(const std::string target)
	{
		// Resolve the address and connect
		asio::connect(_socket, _resolver.resolve(_host, _port));

		auto headRequest = createEmptyRequest(http::verb::head, target);

		// Sending Requests via a Connected Socket
		http::write(_socket, headRequest);

		// Part responsible for reading the response
		readResponce();

		// close the connection
		closeConnection();

		std::cout << "HTTP HEAD Request Had finished\n\n";
	}

	/*
	* @brief Is used to get the list of methods supported /
	*		 by the server for a particular resource.
	*/
	void OPTIONSRequest(const std::string target)
	{
		// Resolve the address and connect
		asio::connect(_socket, _resolver.resolve(_host, _port));

		auto optionRequest = createEmptyRequest(http::verb::options, target);

		// Sending Requests via a Connected Socket
		http::write(_socket, optionRequest);

		// Part responsible for reading the response
		readResponce();

		// close the connection
		closeConnection();

		std::cout << "HTTP OPTIONS Request Had finished\n\n";
	}

private:
	/*
	* @brief Create an HTTP requester specifying the target
	*/
	const http::request<http::string_body> createRequest(http::verb verb,
		const std::string& target, unsigned int version = 11) const
	{
		// Create a special HTTP requester specifying the target
		if (verb == http::verb::post)
		{
			// Set up an HTTP POST request message
			http::request<http::string_body> req{verb, target, version};
			req.set(http::field::host, _host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			req.set(http::field::content_type, "application/x-www-form-urlencoded");
			req.body() = "param1=value1&param2=value2";

			return req;
		}
		if (verb == http::verb::put)
		{
			// Set up an HTTP PUT request message
			http::request<http::string_body> req{verb, target, version};
			req.set(http::field::host, _host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			req.set(http::field::content_type, "text/plain");
			req.body() = "Hello, Beast Examples!";

			return req;
		}

		// Create an HTTP requester specifying the target
		http::request<http::string_body> req(verb, target, version);

		// Set HTTP header fields
		req.set(http::field::host, _host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		return req;
	}

	/*
	* @brief Create an HTTP requester specifying the target
	* @details It's has empty because some http::verb request does not return server response body.
	*		   Examples: ** delete_, head, options, etc... **
	*		   If you want the response body for another type of request, /
	*		   you should use http::response<http::dynamic_body> or http::response<http::buffer_body> /
	*		   depending on the desired size of the response.
	*/ 
	const http::request<http::empty_body> createEmptyRequest(http::verb verb,
		const std::string& target, unsigned int version = 11) const
	{
		if (verb == http::verb::delete_)
		{
			// Set up an HTTP DELETE request message
			http::request<http::empty_body> req{http::verb::delete_, target, 11};
			req.set(http::field::host, _host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			return req;
		}
		if (verb == http::verb::head)
		{
			// Set up an HTTP HEAD request message
			http::request<http::empty_body> req{http::verb::head, target, 11};
			req.set(http::field::host, _host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			return req;
		}
		if (verb == http::verb::options)
		{
			// Set up an HTTP OPTIONS request message
			http::request<http::empty_body> req{http::verb::options, target, 11};
			req.set(http::field::host, _host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			return req;
		}

		return http::request<http::empty_body>();
	}

	/*
	* @brief Part responsible for reading the response
	*/
	void readResponce()
	{
		// This buffer is used for reading and must be persisted
		boost::beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(_socket, buffer, res);

		// Write the message to standard out
		std::cout << res << std::endl;
	}

	/*
	* @brief Close the connection
	*/
	void closeConnection()
	{
		beast::error_code ec;

		// close the connection
		_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

		// not_connected happens sometimes
		// so don't bother reporting it.
		if (ec && ec != beast::errc::not_connected)
			throw beast::system_error{ec};

		// If we get here then the connection is closed gracefully
	}
};
