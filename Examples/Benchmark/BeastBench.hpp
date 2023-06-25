#pragma once

#include "Beast/beast.hpp"

#include "benchmark/benchmark.h"

void beast_test(benchmark::State& state)
{
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace asio = boost::asio;
    using tcp = asio::ip::tcp;

    asio::io_context ioc;

    tcp::resolver resolver(ioc);
    tcp::socket socket(ioc);

    tcp::resolver::query query("www.google.com", "80");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    boost::asio::connect(socket, endpoint_iterator);

    while (state.KeepRunningBatch(state.range_x()))
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            auto start = std::chrono::high_resolution_clock::now();

            beast::http::request<beast::http::string_body> req{http::verb::get, "/", 11};
            req.set(http::field::host, "www.google.com");
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            http::write(socket, req);

            beast::flat_buffer buffer;
            http::response<http::dynamic_body> res;
            http::read(socket, buffer, res);

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            state.SetIterationTime(elapsed_seconds.count());
        }
    }
}
