#pragma once 

#include <deque>

#include "Listener.hpp"

class HTTPServer
{
	uint16_t _port;
	asio::ip::address _address;
	asio::io_context _io_context;
	std::deque<std::thread> _threads;

public:
	HTTPServer(std::string_view host, uint16_t port)
		: _address(asio::ip::make_address(host))
		, _port(port)
	{}

	HTTPServer(HTTPServer const& other) = delete;
	HTTPServer& operator=(HTTPServer const& other) = delete;
	HTTPServer(HTTPServer&& other) noexcept = delete;
	HTTPServer& operator=(HTTPServer&& other) noexcept = delete;

	~HTTPServer() { stop(); }

	void start()
	{
		std::shared_ptr<const std::string> root = std::make_shared<std::string>(".");

		// Create and launch a listening port 
		std::make_shared<Listener>(_io_context,
			tcp::endpoint{_address, _port}, root)->run();

		size_t threadsCount = std::thread::hardware_concurrency();
		threadsCount > 1 ? --threadsCount : threadsCount = 1;

		// Run the I/O service on the requested number of threads
		for (size_t i = 0; i < threadsCount; ++i)
		{
			_threads.emplace_back(std::thread([this]() { _io_context.run(); }));
		}
		std::cout << "[Server][log] Server is started!" 
			<< std::endl;

		_io_context.run();
	}

	void stop()
	{
		_io_context.stop();

		for (std::thread& thread : _threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}

		_threads.clear();
	}
};
