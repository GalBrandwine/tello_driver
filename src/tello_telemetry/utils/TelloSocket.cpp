#include "TelloSocket.hpp"

void TelloSocket::do_receive()
{
    m_tello_socket->async_receive_from(
        boost::asio::buffer(data_, max_length), m_sender_endpoint,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0)
            {
                m_bytes_recvd = bytes_recvd;
                m_is_bytes_received = true;
                // std::cout << "In do_receive callback::bytes_recvd::" << m_bytes_recvd << "\n";
            }
            else
            {
                m_is_bytes_received = false;
            }
        });
}

int TelloSocket::Receive(std::vector<unsigned char> &data)
{
    std::lock_guard<std::mutex> lock(m_sendM);
    if (m_is_bytes_received)
    {
        int bytes_recvd = m_bytes_recvd;
        m_bytes_recvd = 0;
        std::copy(data_, data_ + bytes_recvd, data.begin());
        memset(data_, 0, sizeof(data_));
        do_receive();
        return bytes_recvd;
    }
    return 0;
}

void TelloSocket::Send(const std::string &cmd)
{
    std::lock_guard<std::mutex> lock(m_sendM);
    m_tello_socket->async_send_to(
        boost::asio::buffer(cmd, cmd.length()),
        m_sender_endpoint,
        [this](std::error_code error, std::size_t bytes_transferred) {
            // std::cout << "In Send callback::bytes_transferred::" << bytes_transferred << "\n";
            do_receive();
        });
}

TelloSocket::~TelloSocket()
{
    work_io_context_.stop();
    if (work_thread_.joinable())
    {
        work_thread_.join();
    }
    delete (m_tello_socket);
    std::cout << "TelloSocket Destrtucting.\n";
}

TelloSocket::TelloSocket(const std::string &droneIp, const short droneCommandPort, const short droneDataPort)
    : work_io_context_(),
      work_(boost::asio::make_work_guard(work_io_context_)),
      work_thread_(std::thread([&]() {
          work_io_context_.run();
          std::cout << "work_io_context_ done working!\n";
      }))
{
    m_tello_socket = new boost::asio::ip::udp::socket(work_io_context_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), droneDataPort));
    m_sender_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string(droneIp), droneCommandPort);
}