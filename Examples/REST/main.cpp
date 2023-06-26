#include "SimpleRESTAPI.hpp"

int main()
{
	asio::io_context context(1);
	std::string address{"127.0.0.1"};
	uint16_t port = 80;

	SimpleRESTAPI example(context, address, port);

	auto request = example.CreateSimpleRequest();
	example.SendSimpleResponse(request);

	return EXIT_SUCCESS;
}
