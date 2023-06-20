#pragma once 

#include "Beast/beast.hpp"

#include <deque>

class AsioExamples
{
	asio::io_context _io_context;
	tcp::acceptor _acceptor;
	tcp::socket _socket;
	std::deque<std::thread> _threads;

public:
	AsioExamples()
	: _acceptor(_io_context, tcp::endpoint{tcp::v4(), 65003})
	, _socket(_io_context)
	{
		setup_connection();
	}

	~AsioExamples()
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

	AsioExamples(AsioExamples const& other) = delete;
	AsioExamples& operator=(AsioExamples const& other) = delete;
	AsioExamples(AsioExamples&& other) noexcept = delete;
	AsioExamples& operator=(AsioExamples&& other) noexcept = delete;

	void test_callback()
	{
		async_write_message(_socket, "[Example] Testing callback\r\n",
			[](const boost::system::error_code& error, std::size_t n)
			{
				if (!error) { std::cout << n << " bytes transferred\n"; }
				else { std::cout << "Error: " << error.message() << "\n"; }
			});
	}

	void test_future()
	{
		std::future<std::size_t> f = async_write_message(
			_socket, "[Example] Testing future\r\n", asio::use_future);

		try { std::size_t n = f.get(); std::cout << n << " bytes transferred\n"; }
		catch (std::exception& ex) { std::cout << "Error: " << ex.what() << "\n"; }
	}
private:

	// This is the simplest example of a composed asynchronous operation, where we
	// simply repackage an existing operation. The asynchronous operation
	// requirements are met by delegating responsibility to the underlying
	// operation.
	template<typename CompletionToken>
	auto async_write_message(tcp::socket& socket, const char* message, CompletionToken& token)
		// The return type of the initiating function is deduced from the combination
		// of CompletionToken type and the completion handler's signature. When the
		// completion token is a simple callback, the return type is void. However,
		// when the completion token is boost::asio::yield_context (used for stackful
		// coroutines) the return type would be std::size_t, and when the completion
		// token is boost::asio::use_future it would be std::future<std::size_t>.
	{
		// When delegating to the underlying operation we must take care to perfectly
		// forward the completion token. This ensures that our operation works
		// correctly with move-only function objects as callbacks, as well as other
		// completion token types.
		return asio::async_write(socket, asio::buffer(
			message, std::strlen(message)), std::forward<CompletionToken>(token));
	}

	void setup_connection()
	{
		_socket = _acceptor.accept();

		size_t threadsCount = std::thread::hardware_concurrency();
		threadsCount > 1 ? --threadsCount : threadsCount = 1;

		// Run the I/O service on the requested number of threads
		for (size_t i = 0; i < threadsCount; ++i)
		{
			_threads.emplace_back(std::thread([this]() { _io_context.run(); }));
		}
	}
};
