#include "Beast.Static/beast.hpp"

#include "HTTPClient.Static/Session.hpp"

int main()
{
   //std::cout << "Example:\n" <<
   //     "    http-client-async www.example.com 80 /\n" <<
   //     "    http-client-async www.example.com 80 / 1.0\n\n";

    // The io_context is required for all I/O
    asio::io_context ioc;

    // Launch the asynchronous operation
    std::make_shared<Session>(ioc)->run();

    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();

    return EXIT_SUCCESS;
}