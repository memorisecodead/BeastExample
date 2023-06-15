#include "HTTPExamples.Static/HTTPExamples.hpp"

int main() 
{
    const std::string host = "scooterlabs.com";
    const std::string port = "80";

    const std::string target = "/echo?input=test";
    const std::string optionTarget = "/path/to/resource";

    HTTPExamples example(host, port);

    example.OPTIONSRequest(optionTarget);
    example.GETRequest(target);
    example.DELETERequest(target);
    example.PUTRequest(target);
    example.HEADRequest(target);

    return EXIT_SUCCESS;
}
