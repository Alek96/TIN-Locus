extern "C" { // avoid C++ name mangling; in fact not needed (over-protective?) as these are system libraries with use in C++ code in mind (no doubt)
  #include <sys/types.h>
  #include <sys/socket.h>
}

// htons function
extern "C" {
  #include <arpa/inet.h> // TODO?? (alternative, instead)
  #include <netinet/in.h>
}

#include <cerrno>
extern "C" {
  #include <unistd.h> // standard symbolic constants and types (e.g. STDOUT_FILENO)
}

#include <csignal>

// NOTE: C++11 required
#include <thread>
#include <chrono>
// NOTE: for std::literals::chrono_literals C++14 required

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "signal_handlers.h"

const uint16_t LISTEN_PORT = 9999;
const int BACKLOG_SIZE = 5;

const int SOCKET_OPTION_ON = 1;

const std::size_t MSG_BUF_SIZE = 100;

// TODO? define special, non-standard types if lacking on build platform
/*#ifndef socklen_t
#define socklen_t int
#endif
// typedef int socklen_t;
*/

void send_message(int socket_fd, const char *message, std::size_t msg_size);

int main(int argc, char** argv)
{
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

    std::cout << "Socket fd: " << sock_fd << std::endl;

    if( setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (const void *) &SOCKET_OPTION_ON, sizeof SOCKET_OPTION_ON) != 0 )
    {
        std::cerr << "Error setting the SO_REUSEADDR socket option\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    // TODO? assert on getsockopt == true?


    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(LISTEN_PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);   // bind to all local interfaces

    if( bind(sock_fd, (struct sockaddr *) &address, sizeof address) != 0 )
    {
        std::cerr << "Error binding name to the socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int addr_len = sizeof address;
    if( getsockname(sock_fd, (struct sockaddr *) &address, (socklen_t *) &addr_len) != 0 )
    {
        std::cerr << "Error retrieving socket name\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( addr_len > sizeof address ) // TODO not needed
    {
        std::cerr << "Retrieved local address truncated because of insufficient addr buffer\n"
                     /*"Errno: " << errno*/ << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( addr_len != sizeof address )
    {
        std::cerr << "Retrieved local address of wrong format\n"
                     /*"Errno: " << errno*/ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Socket bound to local port: " << ntohs(address.sin_port) << '\n' <<
                 "Local address: " << inet_ntoa(address.sin_addr) << std::endl;
    std::cout << std::endl;


    if( listen(sock_fd, BACKLOG_SIZE) != 0 )
    {
        std::cerr << "Error listening for connections on the socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int con_sock_fd;
    struct sockaddr_in peer_address;
    int peer_addr_len = sizeof peer_address;
    con_sock_fd = accept(sock_fd, (struct sockaddr *) &peer_address, (socklen_t *) &peer_addr_len);

    if( con_sock_fd < 0 )
    {
        std::cerr << "Error accepting a connection on the socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( peer_addr_len > sizeof peer_address ) // TODO not needed
    {
        std::cerr << "Peer address truncated because of insufficient addr buffer\n"
                     /*"Errno: " << errno*/ << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( peer_addr_len != sizeof peer_address )
    {
        std::cerr << "Peer address of wrong format\n"
                     /*"Errno: " << errno*/ << std::endl;
        std::exit(EXIT_FAILURE);
    }
    

    std::cout << "Connected peer:\n"
                 "  Remote address: " << inet_ntoa(peer_address.sin_addr) << "\n" <<
                 "  Remote port:    " << ntohs(peer_address.sin_port) << "\n"
                 "  socket_fd: " << con_sock_fd << std::endl;
    std::cout << std::endl;


    char weclome_message[] = "\n"
                             "This is a welcome message.\n"
                             "Hello. You've just connected to the TestTcp server.\n"
                             "\n";
                             //"Your client id is " 
    send_message(con_sock_fd, weclome_message, sizeof weclome_message);


    // shut down the sending part of the full-duplex TCP connection (disallow further transmissions)
    if( shutdown(sock_fd, SHUT_RD) != 0 )
    {
        std::cerr << "Error shuting down transmissions on the local socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }


    char message_buffer[MSG_BUF_SIZE];
    ssize_t bytes_received = 0;

    while( true )
    {
        bytes_received = recv(con_sock_fd, message_buffer, MSG_BUF_SIZE, 0);

        if( bytes_received < 0 )
        {
            std::cerr << "Error receiving a message from the peer\n"
                         "Error: " << std::strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }
        else if( bytes_received == 0 )
        {
            std::cout << "# The peer has performed an orderly shutdown." << std::endl;
            break;
        }  

        std::cout << bytes_received << " bytes received. Message:\n" <<
                     "> ";
        std::cout.write(message_buffer, bytes_received);
        std::cout << std::endl; 
    }


    std::cout << "--Closing server--" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));


    // TODO general macros for checking error conditions + throwing exceptions?
    if( close(con_sock_fd) != 0 )
    {
        std::cerr << "Error closing connected socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if( close(sock_fd) != 0 )
    {
        std::cerr << "Error closing listen socket\n"
                     "Error: " << std::strerror(errno) << std::endl;
        // TODO allow script-automated error status code distinction 
        std::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

void send_message(int socket_fd, const char *message, std::size_t msg_size)
{
    // TODO? memcpy message? - can the message buffer be modified during this function execution?
    // TODO? or serialize string? / string's buffer? -- string as func argument
    ssize_t bytes_sent = 0, total_bytes_sent = 0; //characters? (unicode?)
    ssize_t msg_ssize = static_cast<ssize_t>(msg_size);

    do
    {
        if( total_bytes_sent > 0 )
        {
            std::cout << "Sending another part of the message.\n"
                         "Bytes sent: " << total_bytes_sent << "\n" <<
                         "Total message size: " << msg_ssize << std::endl;
        }

        bytes_sent = send(socket_fd, message + total_bytes_sent, msg_ssize - total_bytes_sent, 0); // TODO flags
        // TODO sendmsg
        if( bytes_sent < 0 )
        {
            std::cerr << "Error sending message on socket_fd: " << socket_fd << ".\n"
                         "Error: " << std::strerror(errno) << std::endl;
            // TODO throw exception --> clean-up
            std::exit(EXIT_FAILURE);
        }

        total_bytes_sent += bytes_sent;
    }
    while( total_bytes_sent < msg_ssize );
    std::cout << "Message sent on socket_fd: " << socket_fd << "\n"
                 "Bytes sent: " << total_bytes_sent << std::endl;
}


// TODO
// SO_REUSEADDR flag:
// A TCP local socket address that has been bound is unavailable for some time after closing, unless the SO_REUSEADDR flag has been set. Care should be taken when using this flag as it makes TCP less reliable. 

// TODO
// accept errno
// Linux accept() (and accept4()) passes already-pending network errors on the new socket as an error code from accept(). This behavior differs from other BSD socket implementations. For reliable operation the application should detect the network errors defined for the protocol after accept() and treat them like EAGAIN by retrying. In the case of TCP/IP, these are ENETDOWN, EPROTO, ENOPROTOOPT, EHOSTDOWN, ENONET, EHOSTUNREACH, EOPNOTSUPP, and ENETUNREACH. 

// TODOSYNC
// close socket
// It is probably unwise to close file descriptors while they may be in use by system calls in other threads in the same process. Since a file descriptor may be reused, there are some obscure race conditions that may cause unintended side effects. 

// TODO
// blocking send call:
// TODO
// nonblocking socket send + select:
// When the message does not fit into the send buffer of the socket, send() normally blocks, unless the socket has been placed in nonblocking I/O mode. In nonblocking mode it would fail with the error EAGAIN or EWOULDBLOCK in this case. The select(2) call may be used to determine when it is possible to send more data.

// TODOSEC
// close open sockets after each error. or do it automatically (SOCK_CLOEXEC flag)

// TODO
// C++ chrono clock:
// The standard recommends that a steady clock is used to measure the duration. If an implementation uses a system clock instead, the wait time may also be sensitive to clock adjustments.

// TODOWARY
// exit function:
// Stack is not unwound - destructors of variables with automatic storage duration are not called. 

// TODOWARY
// closing connection:
// SO_LINGER socket option:
// When enabled, a close(2) or shutdown(2) will not return until all queued messages for the socket have been successfully sent or the linger timeout has been reached. Otherwise, the call returns immediately and the closing is done in the background. When the socket is closed as part of exit(2), it always lingers in the background.


// nonblocking accept:
// If no pending connections are present on the queue, and the socket is not marked as nonblocking, accept() blocks the caller until a connection is present. If the socket is marked nonblocking and no pending connections are present on the queue, accept() fails with the error EAGAIN or EWOULDBLOCK. 
// select, poll, SIGIO:
// In order to be notified of incoming connections on a socket, you can use select(2) or poll(2). A readable event will be delivered when a new connection is attempted and you may then call accept() to get a socket for that connection. Alternatively, you can set the socket to deliver SIGIO when activity occurs on a socket; see socket(7) for details.
// accept after SIGIO/select/poll:
// There may not always be a connection waiting after a SIGIO is delivered or select(2) or poll(2) return a readability event because the connection might have been removed by an asynchronous network error or another thread before accept() is called. If this happens then the call will block waiting for the next connection to arrive. To ensure that accept() never blocks, the passed socket sockfd needs to have the O_NONBLOCK flag set (see socket(7)).
// explicitly set flags:
// On Linux, the new socket returned by accept() does not inherit file status flags such as O_NONBLOCK and O_ASYNC from the listening socket. This behavior differs from the canonical BSD sockets implementation. Portable programs should not rely on inheritance or noninheritance of file status flags and always explicitly set all required flags on the socket returned from accept().  

// accept4(sock_fd, &peer_address, &peer_addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC)
// nonblocking accept, accept4 is a nonstandard Linux extension

// nonblocking recv:
// If no messages are available at the socket, the receive calls wait for a message to arrive, unless the socket is nonblocking (see fcntl(2)), in which case the value -1 is returned and the external variable errno is set to EAGAIN or EWOULDBLOCK. The receive calls normally return any data available, up to the requested amount, rather than waiting for receipt of the full amount requested.
// The select(2) or poll(2) call may be used to determine when more data arrives. 