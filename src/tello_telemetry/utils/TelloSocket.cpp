#include "TelloSocket.hpp"

void TelloSocket::in_data_worker()
{
    while (m_keep_running)
    {
        do_receive();
        m_is_bytes_received = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void TelloSocket::do_receive()
{

    m_bytes_recvd = m_tello_socket->receive_from(
        boost::asio::buffer(data_, max_length), m_sender_endpoint);
}

int TelloSocket::Receive(std::vector<unsigned char> &data)
{

    if (m_is_bytes_received)
    {
        int bytes_recvd = m_bytes_recvd;
        m_bytes_recvd = 0;
        std::copy(data_, data_ + bytes_recvd, data.begin());
        memset(data_, 0, sizeof(data_));
        // do_receive();
        return bytes_recvd;
    }
    return 0;
}

void TelloSocket::Send(const std::string &cmd)
{
    auto msg = cmd;

    // std::lock_guard<std::mutex> lock(m_sendM);
    m_tello_socket->send_to(boost::asio::buffer(msg, msg.length()), m_sender_endpoint); /* ,
        [this](std::error_code error, std::size_t bytes_transferred) {
        }); */
}

TelloSocket::~TelloSocket()
{

    m_tello_socket->shutdown(m_tello_socket->shutdown_both);

    work_io_context_.stop();
    // if (work_thread_.joinable())
    // {
    //     work_thread_.join();
    // }

    delete (m_tello_socket);

    if (m_in_socket_thread.joinable())
    {
        m_any_revieved = true;
        m_keep_running = false;
        m_in_socket_thread.join();
    }
    std::cout << "TelloSocket Destrtucting.\n";
}

TelloSocket::TelloSocket(const std::string &droneIp, const short droneCommandPort, const short droneDataPort)
{
    m_tello_socket = new boost::asio::ip::udp::socket(work_io_context_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), droneDataPort));
    m_sender_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string(droneIp), droneCommandPort);

    m_in_socket_thread = std::thread(std::bind(&TelloSocket::in_data_worker, this));
}