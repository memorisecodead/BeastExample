#include "Beast/beast.hpp"

#include "HTTPServer.hpp"
#include "HTTPServerInfo.hpp"

int main()
{
    std::unique_ptr<HTTPServer> webserver(std::make_unique<HTTPServer>(
        HTTPServerInfo::HostAddress::local,
        HTTPServerInfo::Port::test));

    webserver->start();

    return EXIT_SUCCESS;
}
