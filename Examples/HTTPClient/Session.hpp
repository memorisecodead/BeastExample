#pragma once

#include "Beast/beast.hpp"
#include "HTTPClientInfo.hpp"

#include "Utility/Utility.hpp"

// Performs an HTTP GET and prints the response
class Session : public std::enable_shared_from_this<Session>
{
    tcp::resolver _resolver;
    beast::tcp_stream _stream;
    beast::flat_buffer _buffer; // (Must persist between reads)
    http::request<http::empty_body> _req;
    http::response<http::string_body> _res;

public:
    // Objects are constructed with a strand to
    // ensure that handlers do not execute concurrently.
    explicit Session(asio::io_context& ioc)
        : _resolver(asio::make_strand(ioc))
        , _stream(asio::make_strand(ioc))
    {}

    // Start the asynchronous operation
    void run()
    {
        auto const port = std::to_string(HTTPClientInfo::Port::test);

        // Set up an HTTP GET request message
        _req.version(11);
        _req.method(http::verb::get);
        _req.target("/");
        _req.set(http::field::host, HTTPClientInfo::HostAddress::local);
        _req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        std::cout << "[ClientSession][log] Look up the domain name!"
            << std::endl;

        // Look up the domain name
        _resolver.async_resolve(
            HTTPClientInfo::HostAddress::local, port, 
            beast::bind_front_handler(&Session::on_resolve, shared_from_this()));
    }

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results)
    {
        if (ec)
            return fail(ec, "resolve");

        std::cout << "[ClientSession][log] Make the connection on the IP address we get from a lookup!"
            << std::endl;

        // Set a timeout on the operation
        _stream.expires_after(std::chrono::seconds(30));

        // Make the connection on the IP address we get from a lookup
        _stream.async_connect(results, 
            beast::bind_front_handler(&Session::on_connect, shared_from_this()));
    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
    {
        if (ec)
            return fail(ec, "connect");

        std::cout << "[ClientSession][log] Send the HTTP request to the remote host!"
            << std::endl;

        // Set a timeout on the operation
        _stream.expires_after(std::chrono::seconds(30));

        // Send the HTTP request to the remote host
        http::async_write(_stream, _req,
            beast::bind_front_handler(&Session::on_write, shared_from_this()));
    }

    void on_write(beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec)
            return fail(ec, "write");

        std::cout << "[ClientSession][log] Receive the HTTP response!"
            << std::endl;

        // Receive the HTTP response
        http::async_read(_stream, _buffer, _res,
            beast::bind_front_handler(&Session::on_read, shared_from_this()));
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec)
            return fail(ec, "read");

        std::cout << "[ClientSession][log] Write the message to standard out!"
            << std::endl;

        // Write the message to standard out
        std::cout << _res << std::endl;

        // Gracefully close the socket
        _stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // not_connected happens sometimes so don't bother reporting it.
        if (ec && ec != beast::errc::not_connected)
            return fail(ec, "shutdown");

        // If we get here then the connection is closed gracefully
    }
};
