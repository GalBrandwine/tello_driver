#include "AsyncServer.hpp"

server::server(asio::io_context &io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)),
      sender_endpoint_(asio::ip::address_v4::from_string("192.168.10.1"), 8889)
{
    do_receive();
}

void server::do_receive()
{
    socket_.async_receive_from(
        asio::buffer(data_, max_length), sender_endpoint_,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0)
            {
                do_send(data_, bytes_recvd);
                std::cout << "bytes_recvd: " << bytes_recvd << "\n";
                is_bytes_recvd = true;
            }
            else
            {
                do_receive();
            }
        });
}

void server::do_send(char *data, std::size_t length)
{
    socket_.async_send_to(
        asio::buffer(data, length), sender_endpoint_,
        [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            do_receive();
        });
}
