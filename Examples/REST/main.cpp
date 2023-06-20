#include "RESTExample.hpp"

int main()
{
	const std::string host = "scooterlabs.com";
	const std::string port = "80";

	const std::string target = "/echo?input=test";

	asio::io_context context;

	RESTExample example(context, host, port);
	example.SendRequest(http::verb::get, target);
	example.SendRequest(http::verb::put, target);
	example.SendRequest(http::verb::post, target);
	example.SendRequest(http::verb::delete_, target);

	return EXIT_SUCCESS;
}
