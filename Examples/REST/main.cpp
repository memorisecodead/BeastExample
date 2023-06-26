#include "RESTExample.hpp"
#include "SimpleRESTAPI.hpp"

int main()
{
	//const std::string host = "scooterlabs.com";
	//const std::string port = "80";

	//const std::string target = "/echo?input=test";

	//asio::io_context context;

	//RESTExample example(context, host, port);

	//example.SendHTTPRequest(http::verb::get, target);
	//example.SendHTTPRequest(http::verb::put, target);
	//example.SendHTTPRequest(http::verb::post, target);
	//example.SendHTTPRequest(http::verb::delete_, target);

	{
		asio::io_context context(1);
		std::string address{"127.0.0.1"};
		uint16_t port = 80;

		SimpleRESTAPI example(context,address, port);

		auto request = example.CreateSimpleRequest();
		example.SendResponse(request);
	}
	return EXIT_SUCCESS;
}
