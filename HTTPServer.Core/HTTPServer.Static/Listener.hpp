#pragma once

#include "Beast.Static/beast.hpp"
#include "Session.hpp"

#include "../Utility/Public/Utility.hpp"

// Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener>
{
    asio::io_context& _ioc;
    tcp::acceptor _acceptor;
    std::shared_ptr<std::string const> _doc_root;

public: 
    Listener(asio::io_context& ioc, tcp::endpoint endpoint, 
        std::shared_ptr<std::string const>& root)
        : _ioc(ioc)
        , _acceptor(ioc)
        , _doc_root(root)
    {
        beast::error_code ec;


        std::cout << "[ServerListener][log] Open the acceptor!"
            << std::endl;

        // Open the acceptor
        _acceptor.open(endpoint.protocol(), ec);
        if (ec)
        {
            fail(ec, "open");
            return;
        }

        std::cout << "[ServerListener][log] Allow address reuse!"
            << std::endl;

        // Allow address reuse
        _acceptor.set_option(asio::socket_base::reuse_address(true), ec);
        if (ec)
        {
            fail(ec, "set_option");
            return;
        }

        std::cout << "[ServerListener][log] Bind to the server address!"
            << std::endl;

        // Bind to the server address
        _acceptor.bind(endpoint, ec);
        if (ec)
        {
            fail(ec, "bind");
            return;
        }

        std::cout << "[ServerListener][log] Start listening for connections!"
            << std::endl;

        // Start listening for connections
        _acceptor.listen(asio::socket_base::max_listen_connections, ec);
        if (ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void run()
    {
        std::cout << "[ServerListener][log] Start accepting incoming connections!"
            << std::endl;

        do_accept();
    }

private:
    void do_accept()
    {
        std::cout << "[ServerListener][log] The new connection gets its own strand!"
            << std::endl;

        // The new connection gets its own strand
        _acceptor.async_accept(asio::make_strand(_ioc),
            beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
    }

    void on_accept(beast::error_code ec, tcp::socket socket)
    {
        if (ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<Session>(std::move(socket), _doc_root)->run();
        }

        // Accept another connection
        do_accept();
    }
};
