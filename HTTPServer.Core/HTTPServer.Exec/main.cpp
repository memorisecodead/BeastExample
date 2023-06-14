#include "Beast.Static/beast.hpp"

#include "HTTPServer.Static/HTTPServer.hpp"
#include "HTTPServer.Static/HTTPServerInfo.hpp"

int main()
{
    std::unique_ptr<HTTPServer> webserver(std::make_unique<HTTPServer>(
        HTTPServerInfo::HostAddress::local,
        HTTPServerInfo::Port::test));

    webserver->start();

    return EXIT_SUCCESS;
}
