#include <sys/types.h>
#include <sys/socket.h>

// htons function
#include <arpa/inet.h> // TODO?? (alternative, instead)
#include <netinet/in.h>

#include <cerrno>
#include <unistd.h>

#include <csignal>

// NOTE: C++11 required
#include <thread>
#include <chrono>

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "signal_handlers.h"

const uint16_t PORT = 55555;

const uint32_t REMOTE_SRV_ADDR = INADDR_LOOPBACK;
uint16_t REMOTE_SRV_PORT = 9999;

const std::size_t MSG_BUF_SIZE = 100;

void send_message(int socket_fd, const char *message, std::size_t msg_size);
void close_socket_in_thread(int socket_fd);
extern "C" void * reading_thread_routine(void *socket_fd);

int main(int argc, char** argv)
{
    if(argc == 2) {
        REMOTE_SRV_PORT = static_cast<uint16_t>(atoi(argv[1]));
    }

    // Install signal handlers
    // The signals SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.
    if( std::signal(SIGINT, signal_handler) == SIG_ERR ) // terminal interrupt signal, ctrl + c
    {
        // Setting a signal handler can be disabled on some implementations.
        std::cerr << "Error installing the SIGINT signal handler\n"
                     /*<< "Error: " << std::strerror(errno)*/;
        std::exit(EXIT_FAILURE);
    }
    if( std::signal(SIGTERM, signal_handler) == SIG_ERR ) // termination request
    {
        std::cerr << "Error installing the SIGTERM signal handler\n";
        std::exit(EXIT_FAILURE);
    }
    if( std::signal(SIGQUIT, signal_handler) == SIG_ERR ) // terminal quit signal, ctrl + backslash
    {
        std::cerr << "Error installing the SIGQUIT signal handler\n";
        std::exit(EXIT_FAILURE);
    }
    if( std::signal(SIGTSTP, signal_handler) == SIG_ERR ) // terminal stop signal, ctrl + z
    {
        std::cerr << "Error installing the SIGTSTP signal handler\n";
        std::exit(EXIT_FAILURE);
    }
    if( std::signal(SIGCONT, signal_handler) == SIG_ERR ) // continue executing, if stopped
    {
        std::cerr << "Error installing the SIGCONT signal handler\n";
        std::exit(EXIT_FAILURE);
    }


    // create TCP/IP socket
    int sock_fd = socket(AF_INET, SOCK_STREAM /*| SOCK_NONBLOCK | SOCK_CLOEXEC*/, 0 /*IPPROTO_TCP*/);
    if( sock_fd < 0 )
    {
        std::cerr << "Error creating TCP/IP socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }


    struct sockaddr_in remote_address;
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(REMOTE_SRV_PORT);
    remote_address.sin_addr.s_addr = htonl(REMOTE_SRV_ADDR);

    if( connect(sock_fd, (struct sockaddr *) &remote_address, sizeof remote_address) != 0 )
    {
        std::cerr << "Error initiating a connection with the remote socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    // automatically bounds the socket to a random free port or to a usable shared port with the local address set to INADDR_ANY (0.0.0.0)

    std::cout << "## Connection with the server established ##" << std::endl;
    

    int remote_addr_len = sizeof remote_address;
    if( getpeername(sock_fd, (struct sockaddr *) &remote_address, (socklen_t *) &remote_addr_len) != 0 )
    {
        std::cerr << "Error retrieving peer socket name\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( remote_addr_len > sizeof remote_address ) // TODO not needed
    {
        std::cerr << "Retrieved peer address truncated because of insufficient addr buffer\n"
                     /*"Errno: " << errno*/ << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( remote_addr_len != sizeof remote_address )
    {
        std::cerr << "Retrieved peer address of wrong format\n"
                     /*"Errno: " << errno*/ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Server:\n"
                 "  Remote address: " << inet_ntoa(remote_address.sin_addr) << '\n' <<
                 "  Remote port: " << ntohs(remote_address.sin_port) << std::endl;


    struct sockaddr_in local_address;
    int local_addr_len = sizeof local_address;

    if( getsockname(sock_fd, (struct sockaddr *) &local_address, (socklen_t *) &local_addr_len) != 0 )
    {
        std::cerr << "Error retrieving local socket name\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( local_addr_len > sizeof local_address ) // TODO not needed
    {
        std::cerr << "Retrieved local address truncated because of insufficient addr buffer\n"
                     /*"Errno: " << errno*/ << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( local_addr_len != sizeof local_address )
    {
        std::cerr << "Retrieved local address of wrong format\n"
                     /*"Errno: " << errno*/ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Local host:\n"
                 "  Local port: " << ntohs(local_address.sin_port) << '\n' <<
                 "  Local address: " << inet_ntoa(local_address.sin_addr) << std::endl;
    std::cout << std::endl;


    // Receive the Welcome Message from the server. Ends with the NULL character.
    char message_buffer[MSG_BUF_SIZE];
//ssize_t bytes_received = 0;
    //bool first_msg_part = true;
/*
    do
    {
        bytes_received = recv(sock_fd, message_buffer, MSG_BUF_SIZE, 0);

        if( bytes_received < 0 )
        {
            std::cerr << "Error receiving a message from the peer\n"
                         "Error: " << std::strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }
        else if( bytes_received == 0 )
        {
            std::cout << "# The peer has performed an orderly shutdown." << std::endl;
            //break;
            goto CLOSE_SOCKET; // yup, intentional ( ͡° ͜ʖ ͡°)
        }      

        std::cout << bytes_received << " bytes received. Message:\n" <<
                     "> ";
        std::cout.write(message_buffer, bytes_received);
        std::cout << std::endl;
    }
    while( message_buffer[bytes_received - 1] != '\0' ); // not a complete message yet
    // bytes_received <= MSG_BUF_SIZE
*/

    pthread_t reading_thread_id;
    pthread_attr_t thread_attributes;
    int thread_op_result;

    if( (thread_op_result = pthread_attr_init(&thread_attributes)) != 0 )
    {
        std::cerr << "Error initializing thread attributes object\n"
                     "Error: " << std::strerror(thread_op_result) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // create thread in detached state
    if( (thread_op_result = pthread_attr_setdetachstate(&thread_attributes, PTHREAD_CREATE_DETACHED)) != 0 )
    {
        std::cerr << "Error setting the detach state attribute in thread attributes object\n"
                     "Error: " << std::strerror(thread_op_result) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if( (thread_op_result = pthread_create(&reading_thread_id, &thread_attributes, &reading_thread_routine, (void *) &sock_fd)) != 0 )
    {
        std::cerr << "Error creating new thread\n"
                     "Error: " << std::strerror(thread_op_result) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if( (thread_op_result = pthread_attr_destroy(&thread_attributes)) != 0 )
    {
        std::cerr << "Error destroying thread attributes object\n"
                     "Error: " << std::strerror(thread_op_result) << std::endl;
        std::exit(EXIT_FAILURE);
    }


    std::cout << "--You can now write messages to the server--\n"
                 "--EOF character (Ctrl + D) or empty first line terminates the input loop--" << std::endl;

    std::streamsize characters_read;
    std::cout << std::boolalpha;

    while( std::cin.get(message_buffer, MSG_BUF_SIZE) ) // delimiter = '\n'
    {
        characters_read = std::cin.gcount();    // count of characters typed in

        if( characters_read > 0 )
            send_message(sock_fd, message_buffer, characters_read);
        // with terminating '\0' => characters_read + 1

        if( std::cin.eof() )
            break;
        while( std::cin.peek() == '\n' )
        {
            std::cin.get(); // extract and discard the delim chars
        }
    }
    // EOF character (Ctrl + D) or empty first line terminates the input loop


    std::cout << "--Closing client--" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

//CLOSE_SOCKET:
    if( close(sock_fd) != 0 )
    {
        std::cerr << "Error closing socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        // TODO allow script-automated error status code distinction 
        std::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

void send_message(int socket_fd, const char *message, std::size_t msg_size)
{
    ssize_t bytes_sent = 0, total_bytes_sent = 0;
    ssize_t msg_ssize = static_cast<ssize_t>(msg_size);

    do
    {
        if( total_bytes_sent > 0 )
        {
            /*std::cout << "Sending another part of the message.\n"
                         "Bytes sent: " << total_bytes_sent << "\n" <<
                         "Total message size: " << msg_ssize << std::endl;*/
        }

        bytes_sent = send(socket_fd, message + total_bytes_sent, msg_ssize - total_bytes_sent, 0);
        // NOTE: Successful completion of a call to send() does not guarantee delivery of the message. A return value of -1 indicates only locally-detected errors. 

        if( bytes_sent < 0 )
        {
            std::cerr << "Error sending message on socket_fd: " << socket_fd << ".\n"
                         "Error: " << std::strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }

        total_bytes_sent += bytes_sent;
    }
    while( total_bytes_sent < msg_ssize );
    std::cout << "Bytes sent: " << total_bytes_sent << std::endl;
}

void close_socket_in_thread(int socket_fd)
{
    // TODO general macros for checking error conditions + throwing exceptions?
    if( close(socket_fd) != 0 )
    {
        std::cerr << "Error closing socket. Socket_fd: " << socket_fd << "\n"
                     "Error: " << std::strerror(errno) << std::endl;
        pthread_exit((void *) NULL);

        // TODO or:
        //throw std::system_error(errno, std::generic_category());
    }
}

void * reading_thread_routine(void *socket_fd)
{
    int sock_fd = *(int *) socket_fd;

    char message_buffer[MSG_BUF_SIZE];
    ssize_t bytes_received = 0;

    while( true )
    {
        bytes_received = recv(sock_fd, message_buffer, MSG_BUF_SIZE, 0);

        if( bytes_received < 0 )
        {
            std::cerr << "Error receiving a message from the peer.\n"
                         "Error: " << std::strerror(errno) << std::endl;
            //close_socket_in_thread(sock_fd);
            // stop the main thread from sending (break the sending loop) (at some error codes?)
            std::cin.setstate(std::ios_base::eofbit); // TODO signal to interrupt std::cin.get ?
            pthread_exit((void *) NULL);
        }
        else if( bytes_received == 0 )
        {
            std::cout << "# The peer has performed an orderly shutdown." << std::endl;
            // TODO stop the main thread from sending (break the sending loop)
            std::cin.setstate(std::ios_base::eofbit); // TODO signal to interrupt std::cin.get ?
            break;
        }  

        std::cout << bytes_received << " bytes received. Message:\n" <<
                     "> ";
        std::cout.write(message_buffer, bytes_received);
        std::cout << std::endl;
    }

    //close_socket_in_thread(sock_fd);

    return (void *) NULL;
}

// NOTES:
// recv:
// If a message is too long to fit in the supplied buffer, excess bytes may be discarded depending on the type of socket the message is received from. 
