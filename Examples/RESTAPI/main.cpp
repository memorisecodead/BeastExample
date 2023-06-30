#include "RESTAPIExample.hpp"

int main()
{
	asio::io_context context(1);
	std::string address{"127.0.0.1"};
	uint16_t port = 80;

	RESTAPIExample example(context, address, port);

	auto request = example.CreateRequest();
	example.SendResponse(request);

	return EXIT_SUCCESS;
}
