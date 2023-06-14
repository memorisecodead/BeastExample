#pragma once 

#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http; 
namespace beast = boost::beast;
namespace asio = boost::asio;
namespace websocket = beast::websocket;
