#include "RESTFulAPIExample.hpp"

int main()
{
	asio::io_context context(1);
	std::string address{"127.0.0.1"};
	uint16_t port = 80;

	RESTFulExample example(context, address, port);

	std::string target = "/hello";
	auto request = example.CreateRequest(target);

	example.ReadRequest(request);
	example.HandleRequest(request);

	return EXIT_SUCCESS;
}
