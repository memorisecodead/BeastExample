#pragma once

#include "Beast/beast.hpp"
#include "HTTPServerInfo.hpp"

#include "Utility/Utility.hpp"

// Sends a WebSocket message and prints the response
class Session : public std::enable_shared_from_this<Session>
{
    // The function object is used to send an HTTP message.
    struct HTTPLambdaSender
    {
        Session& _self;

        explicit HTTPLambdaSender(Session& self)
            : _self(self) {}

        template<bool isRequest, class Body, class Fields>
        void operator()(http::message<isRequest, Body, Fields>&& msg) const
        {
            // The lifetime of the message has to extend
            // for the duration of the async operation so
            // we use a shared_ptr to manage it.
            auto storeMessage = std::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));

            // Store a type-erased version of the shared
            // pointer in the class to keep it alive.
            _self._res = storeMessage;

            // Write the response 
            http::async_write(_self._stream,
                *storeMessage,
                beast::bind_front_handler
                (&Session::on_write, _self.shared_from_this(), storeMessage->need_eof()));
        }
    };

    beast::tcp_stream _stream;
    beast::flat_buffer _buffer;
    std::shared_ptr<std::string const> _doc_root;
    http::request<http::string_body> _req;
    std::shared_ptr<void> _res;
    HTTPLambdaSender _lambda;
public:
    // Take ownership of the stream
    explicit Session(tcp::socket&& socket, std::shared_ptr<const std::string> const& root)
        : _stream(std::move(socket))
        , _doc_root(root)
        , _lambda(*this)
    {}

    // Start the asynchronous operation
    void run()
    {
        std::cout << "[ServerSession][log] Start the asynchronous operation!"
            << std::endl;

        do_read();
    }

    // This function produces an HTTP response for the given
    // request. The type of the response object depends on the
    // contents of the request, so the interface requires the
    // caller to pass a generic lambda for receiving the response.
    template<class Body, class Allocator, class Send>
    void handle_request(beast::string_view doc_root,
        http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send)
    {
        using namespace beast;

        // Returns a bad request response
        const auto bad_request = [&req](beast::string_view why)
        {
            http::response<http::string_body> res{http::status::bad_request, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = std::string(why);
            res.prepare_payload();

            return res;
        };

        // Returns a not found response
        const auto not_found = [&req](beast::string_view target)
        {
            http::response<http::string_body> res{http::status::not_found, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "The resource '" + std::string(target) + "' was not found.";
            res.prepare_payload();
            return res;
        };

        // Returns a not found response
        const auto server_error = [&req](beast::string_view what)
        {
            http::response<http::string_body> res{http::status::internal_server_error, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "An error occurred: '" + std::string(what) + "'";
            res.prepare_payload();

            return res;
        };

        // Make sure we can handle the method
        if (req.method() != http::verb::get &&
            req.method() != http::verb::head)
        {
            return send(bad_request("Unknown HTTP-method"));
        }

        // Request path must be absolute and not contain ".."
        if (req.target().empty()
            || req.target()[0] != '/'
            || req.target().find("..") != beast::string_view::npos)
        {
            return send(bad_request("Illegal request target"));
        }

        // Build the path to the requested file
        std::string path = path_cat(doc_root, req.target());
        if (req.target().back() == '/')
        {
            path.append("index.html");
        }

        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::scan, ec);

        // Handle the case where the file doesn't exist
        if (ec == beast::errc::no_such_file_or_directory)
        {
            return send(not_found(req.target()));
        }

        // Handle an unknown error
        if (ec)
        {
            return send(server_error(ec.message()));
        }

        // Cache the size since we need it after the move
        auto const size = body.size();

        // Respond to HEAD request
        if (req.method() == http::verb::head)
        {
            http::response<http::empty_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());

            return send(std::move(res));
        }

        // Respond to GET request
        http::response<http::file_body> res{
            std::piecewise_construct,
                std::make_tuple(std::move(body)),
                std::make_tuple(http::status::ok, req.version())};

        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());

        return send(std::move(res));
    }
    
    void do_read()
    {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.

        _req = {};

        // Set the timeout
        _stream.expires_after(std::chrono::seconds(20));

        std::cout << "[ServerSession][log] Read the request!"
            << std::endl;

        // Read the request 
        http::async_read(_stream, _buffer, 
            _req, beast::bind_front_handler(&Session::on_read, shared_from_this()));
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection 
        if (ec == http::error::end_of_stream)
            return do_close();
        
        if (ec)
            return fail(ec, "read");

        // Send the response
        std::cout << "[ServerSession][log] Send the response!"
            << std::endl;

        handle_request(*_doc_root, std::move(_req), _lambda);
    }

    void on_write(bool close, beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec)
            return fail(ec, "write");

        if (close)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        std::cout << "[ServerSession][log] On write method!"
            << std::endl;

        // We're done with the response so delete it
        _res = nullptr;

        // Read another request
        do_read();
    }

    void do_close()
    {
        // Send a TCP shutdown
        beast::error_code ec;
        _stream.socket().shutdown(tcp::socket::shutdown_send, ec);

        std::cout << "[ServerSession][log] Send a TCP shutdown!"
            << std::endl;

        // At this point the connection is closed 
    }
};
