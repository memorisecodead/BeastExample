#pragma once

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>

#include "benchmark/benchmark.h"

void poco_test(benchmark::State& state)
{
    Poco::URI uri("http://www.google.com");
    Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/", Poco::Net::HTTPMessage::HTTP_1_1);

    while (state.KeepRunningBatch(state.range_x()))
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            auto start = std::chrono::high_resolution_clock::now();

            session.sendRequest(request);
            Poco::Net::HTTPResponse response;
            std::istream& rs = session.receiveResponse(response);

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            state.SetIterationTime(elapsed_seconds.count());
        }
    }
}
