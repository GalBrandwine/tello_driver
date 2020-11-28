#include "TelloSocket.hpp"

void TelloSocket::do_receive()
{
    m_tello_socket.async_receive_from(
        asio::buffer(data_, max_length), m_sender_endpoint,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0)
            {
                m_bytes_recvd = bytes_recvd;
                m_is_bytes_received = true;
            }
            else
            {
                m_is_bytes_received = false;
                do_receive();
            }
        });
}

int TelloSocket::AsyncReceive(std::vector<unsigned char> &data)
{
    std::cout << __PRETTY_FUNCTION__ << " called!\n";
    if (m_is_bytes_received)
    {
        // data.reserve(m_bytes_recvd);
        std::copy(data_, data_ + m_bytes_recvd, data.begin());
        return m_bytes_recvd;
    }
    return 0;
}

int TelloSocket::Receive(std::vector<unsigned char> &data)
{

    size_t rec = m_tello_socket.receive(asio::buffer(data));
    return rec;
}

void TelloSocket::Send(const std::string &cmd)
{
    std::lock_guard<std::mutex> lock(m_sendM);

    m_tello_socket.async_send_to(
        asio::buffer(cmd, cmd.length()), m_sender_endpoint,
        [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            do_receive();
        });
}

TelloSocket::~TelloSocket()
{
    std::cout << "TelloSocket Destrtucting.\n";
}

TelloSocket::TelloSocket(asio::io_context &io_context, const std::string &droneIp, const short droneCommandPort, const short droneDataPort)
    : m_tello_socket(io_context, udp::endpoint(udp::v4(), droneDataPort)),
      m_sender_endpoint(asio::ip::address_v4::from_string(droneIp), droneCommandPort)
{
}