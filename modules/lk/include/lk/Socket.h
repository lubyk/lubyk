/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/
#ifndef LUBYK_INCLUDE_LK_SOCKET_H_
#define LUBYK_INCLUDE_LK_SOCKET_H_

#include "lubyk.h"
#include "lubyk/msgpack.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>  // getaddrinfo
#include <arpa/inet.h> // inet_ntop

#include <errno.h>  // errno
#include <string.h> // strerror

#include <string>

#include "lua_cpp_helper.h"

// How many pending connections should wait for 'accept'.
#define BACKLOG 10
// recv buffer size
#define MAX_BUFF_SIZE 8192

using namespace lubyk;

typedef int LuaStackSize;

namespace lk {

/** Listen for incoming messages on a given port.
 *
 * @dub lib_name:'Socket_core'
 *      string_format:'%%s:%%d --> %%s:%%d'
 *      string_args:'(*userdata)->localHost(), (*userdata)->localPort(), (*userdata)->remoteHost(), (*userdata)->remotePort()'
 */
class Socket : public LuaCallback
{
  Thread thread_;
  int socket_fd_;
  int socket_type_;
  std::string local_host_;
  int local_port_;
  std::string remote_host_;
  int remote_port_;

  // buffer management
  /** Number of bytes already received in the buffer.
   */
  int buffer_length_;

  /** Bytes already used in the buffer.
   * If buffer_i_ == buffer_length_: there is no more data in the buffer.
   */
  int buffer_i_;

  /** Buffer that contains received data not yet used by Lua.
   */
  char buffer_[MAX_BUFF_SIZE];
public:

  enum SocketType {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
  };

  Socket(lubyk::Worker *worker, int socket_type)
    : LuaCallback(worker),
      // When changing this, also change the private
      // constructor.
      socket_fd_(-1),
      socket_type_(socket_type),
      local_host_("*"),
      local_port_(-1),
      remote_host_("?"),
      remote_port_(-1),
      buffer_length_(0),
      buffer_i_(0) {
  }

  ~Socket() {
    kill();
    close();
  }

  void close() {
    if (socket_fd_ != -1) {
      // printf("close(%i)\n", socket_fd_);
      ::close(socket_fd_);
      socket_fd_ = -1;
    }
  }

  /** Bind socket to a specific interface.
   * @return bound port
   */
  int bind(const char *localhost = NULL, int port = 0) {
    char port_str[10];
    snprintf(port_str, 10, "%i", port);

    if (localhost) {
      local_host_ = localhost;
      if (local_host_ == "" || local_host_ == "*") {
        // bind to any interface
        localhost = NULL;
        local_host_ = "*";
      }
    }

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    // we do not care if we get an IPv4 or IPv6 address
    hints.ai_family = AF_UNSPEC;
    // TCP
    // TODO: Use socket_type_ info
    hints.ai_socktype = SOCK_STREAM;
    // fill our own IP
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port_str, &hints, &res)) {
      throw Exception("Could not getaddrinfo for %s:%i.", local_host_.c_str(), port);
    }

    if (socket_fd_ != -1) {
      ::close(socket_fd_);
    }

    // we use getaddrinfo to stay IPv4/IPv6 agnostic
    socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // printf("socket --> %i\n", socket_fd_);
    if (socket_fd_ == -1) {
      throw Exception("Could not create socket for %s:%i (%s).", local_host_.c_str(), port, strerror(errno));
    }

    // bind to port
    if (::bind(socket_fd_, res->ai_addr, res->ai_addrlen)) {
      throw Exception("Could not bind socket to %s:%i (%s).", local_host_.c_str(), port, strerror(errno));
    }

    if (port == 0) {
      local_port_ = get_port(socket_fd_);
    } else {
      local_port_ = port;
    }

    freeaddrinfo(res);

    return local_port_;
  }

  void connect(const char *host, int port) {
    char port_str[10];
    snprintf(port_str, 10, "%i", port);
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    // we do not care if we get an IPv4 or IPv6 address
    hints.ai_family = AF_UNSPEC;
    // TCP
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ( (status = getaddrinfo(host, port_str, &hints, &res)) ) {
      throw Exception("Could not getaddrinfo for %s:%i (%s).", host, port, gai_strerror(status));
    }

    if (socket_fd_ != -1) {
      // printf("close(%i)\n", socket_fd_);
      ::close(socket_fd_);
    }

    // we use getaddrinfo to stay IPv4/IPv6 agnostic
    socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // printf("socket --> %i\n", socket_fd_);
    if (socket_fd_ == -1) {
      throw Exception("Could not create socket for %s:%i (%s).", host, port, strerror(errno));
    }

    // connect
    if (::connect(socket_fd_, res->ai_addr, res->ai_addrlen)) {
      throw Exception("Could not connect socket to %s:%i (%s).", host, port, strerror(errno));
    }

    char info[INET6_ADDRSTRLEN];
    if (res->ai_family == AF_INET) {
      inet_ntop(res->ai_family, &(((struct sockaddr_in*)res)->sin_addr), info, sizeof(info));
    } else {
      inet_ntop(res->ai_family, &(((struct sockaddr_in6*)res)->sin6_addr), info, sizeof(info));
    }
    // printf("Connected to %s:%i (%s)\n", host, port, info);

    freeaddrinfo(res);

    remote_host_ = host;
    remote_port_ = port;
  }

  /** Start listening for incoming connections.
   */
  void listen() {
    if (local_port_ == -1)
      throw Exception("Listen called before bind.");

    if (::listen(socket_fd_, BACKLOG)) {
      throw Exception("Could not listen (%s).", strerror(errno));
    }
  }

  /** Accept a new incomming connection.
   * @return a new lk.Socket connected to the remote end.
   */
  LuaStackSize accept(lua_State *L) {

    if (local_port_ == -1)
      throw Exception("Accept called before bind.");

    struct sockaddr sa;
    memset(&sa, 0, sizeof(struct sockaddr));
    // length has to be in a variable
    socklen_t sa_len = sizeof(sa);

    int fd;

    { ScopedUnlock unlock(worker_);
      fd = ::accept(socket_fd_, &sa, &sa_len);

      // printf("accept(%i) --> %i\n", socket_fd_, fd);
    }

    if (fd == -1) {
      throw Exception("Error while accepting connection (%s).", strerror(errno));
    }

    // get remote name / port
    int remote_port;
    if (sa.sa_family == AF_INET) {
      remote_port = ntohs(((struct sockaddr_in *)&sa)->sin_port);
    } else {
      remote_port = ntohs(((struct sockaddr_in6 *)&sa)->sin6_port);
    }

    char remote_host[NI_MAXHOST];

    if (getnameinfo(&sa, sizeof(sa), remote_host, sizeof(remote_host), NULL, 0, 0)) {
      throw Exception("Could not get remote host name (%s).", strerror(errno));
    }

    Socket *new_socket = new Socket(worker_, fd, local_host_.c_str(), remote_host, remote_port);

    lua_pushclass<Socket>(L, new_socket, "lk.Socket");
    return 1;
  }

  void setRecvTimeout(int timeout) {
    setTimeout(timeout, SO_RCVTIMEO);
  }
    
  void setSendTimeout(int timeout) {
    setTimeout(timeout, SO_SNDTIMEO);
  }

  /** Receive a message encoded by msgpack (blocks).
   * For a server, this should typically be used inside the loop.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  // LuaStackSize recvMsg(lua_State *L) {
  //   // TODO: second param = timeout ?
  //   // FIXME: We need to pass size of msg pack in first line ! (if there is no other way to 
  //   // encode a "packet" with msgpack)
  //   // unlock while waiting
  //   { lubyk::ScopedUnlock unlock(worker_);
  //     buffer_length_ = ::recv(socket_fd_, &buffer_, MAX_BUFF_SIZE, 0);
  //     if (buffer_length_ == 0) {
  //       return 0;
  //     } else if (buffer_length_ < 0) {
  //       buffer_length_ = 0;
  //       throw Exception("Could not receive (%s).", strerror(errno));
  //     }
  //   }

  //   buffer_length_ = 0;
  //   buffer_i_ = 0;
  //   int arg_size = msgpack_bin_to_lua(L, buffer, len);
  //   return arg_size;
  // }

  /** Receive a raw string (not encoded by msgpack).
   * This IO call blocks.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  LuaStackSize recv(lua_State *L) {
    if (lua_isnumber(L, 3)) {
      setRecvTimeout(lua_tonumber(L, 3));
    }

    if (lua_isnumber(L, 2)) {
      // <self> <num_bytes> [<timeout>]
      return recvBytes(L, lua_tonumber(L, 2));
    } else if (lua_isstring(L, 2)) {
      // <self> <mode> [<timeout>]
      const char *mode = lua_tostring(L, 2);
      if (mode[0] == '*' && mode[1] == 'a') {
        return recvAll(L);
      } else if (mode[0] == '*' && mode[1] == 'l') {
        return recvLine(L);
      } else {
        throw Exception("Bad mode to recv (should be '*a' or '*l' but found '%s')", mode);
      }
    } 
    // receive a single line (not returning \r or \n).
    return recvLine(L);

    return 1;
  }

  /** Send raw bytes without encoding with msgpack.
   * param: string to send.
   */
  void send(lua_State *L) {
    // <string>
    size_t size;
    const char *data = luaL_checklstring(L, -1, &size);
    // send raw bytes
    if (::send(socket_fd_, data, size, 0) == -1) {
      throw Exception("Could not send message (%s).", strerror(errno));
    }
  }

  // /** Send a message packed with msgpack.
  //  * Varying parameters.
  //  */
  // void sendMsg(lua_State *L) {
  //   msgpack_sbuffer* buffer;
  //   msgpack_lua_to_bin(L, &buffer, 1);
  //   // FIXME: We need to encode a "packet" with msgpack (size of msg on first line for
  //   // example.
  //   if (::send(socket_fd_, buffer->data, buffer->size, 0) == -1) {
  //     throw Exception("Could not send message (%s).", strerror(errno));
  //   }
  //   free_msgpack_msg(NULL, buffer);
  // }

  /** Request = remote call (uses msgpack to encode data).
   * This is really just a sendMsg with a recvMsg.
   */
  // LuaStackSize request(lua_State *L) {
  //   sendMsg(L);
  //   return recvMsg(L);
  // }

  /** Execute a loop in a new thread.
   */
  void loop(lua_State *L) {
    set_lua_callback(L);
    thread_.startThread<Socket, &Socket::run>(this, NULL);
  }

  /** Halt loop.
   */
  void quit() {
    // sets should_run to false but does not interrupt
    thread_.quit();
  }

  /** Interrupt loop.
   */
  void kill() {
    // get out of blocking recv
    thread_.send_signal(SIGINT);
  }

  const char *localHost() const {
    return local_host_.c_str();
  }

  int localPort() const {
    return local_port_;
  }

  const char *remoteHost() const {
    return remote_host_.c_str();
  }

  int remotePort() const {
    return remote_port_;
  }

  int port() const {
    return local_port_;
  }

private:
   /** Create a socket with an existing file descriptor.
    * This is used as the result of an 'accept()' call.
    */
   Socket(lubyk::Worker *worker, int fd, const char *local_host, const char *remote_host, int remote_port)
    : LuaCallback(worker),
      socket_fd_(fd),
      local_host_(local_host),
      local_port_(get_port(fd)),
      remote_host_(remote_host),
      remote_port_(remote_port),
      buffer_length_(0),
      buffer_i_(0) {
  }

  static int get_port(int fd) {
    // get bound port
    struct sockaddr sa;
    memset(&sa, 0, sizeof(struct sockaddr));
    // length has to be in a variable
    socklen_t sa_len = sizeof(sa);
    if (getsockname(fd, &sa, &sa_len)) {
      throw Exception("Could not get bound port (%s).", strerror(errno));
    }
    if (sa.sa_family == AF_INET) {
      return ntohs(((struct sockaddr_in *)&sa)->sin_port);
    } else {
      return ntohs(((struct sockaddr_in6 *)&sa)->sin6_port);
    }
  }

  void run(Thread *runner) {
    // lua_ = LuaCallback's lua thread context
    runner->thread_ready();

    // trigger callback

    lubyk::ScopedLock lock(worker_);

    push_lua_callback();

    // lua_ = LuaCallback's thread state
    // first argument is self
    int status = lua_pcall(lua_, 1, 0, 0);

    if (status) {
      printf("Error in Socket callback: %s.\n", lua_tostring(lua_, -1));
      return;
    }
  }

  int recvLine(lua_State *L) {
    luaL_Buffer buffer;
    luaL_buffinit(L, &buffer);

    while (true) {
      while (buffer_i_ < buffer_length_) {
        char c = buffer_[buffer_i_++];
        if (c == '\n') {
          // found end of line
          // push string
          luaL_pushresult(&buffer);
          return 1;
        } else if (c != '\r') {
          // ignore \r
          // add char to lua buffer
          luaL_putchar(&buffer, c);
        }
      }
        
      // read more data
      { lubyk::ScopedUnlock unlock(worker_);
        buffer_length_ = ::recv(socket_fd_, buffer_, MAX_BUFF_SIZE, 0);
        if (buffer_length_ == 0) {
          // connection closed
          return 0;
        } else if (buffer_length_ < 0) {
          buffer_length_ = 0;
          throw Exception("Could not receive (%s).", strerror(errno));
        }
      }
      buffer_i_ = 0;
    }                             
    return 0;
  }

  int recvBytes(lua_State *L, int sz) {
    luaL_Buffer buffer;

    if (buffer_i_ < buffer_length_) {
      if (sz <= (buffer_length_-buffer_i_)) {
        // everything is in the buffer already
        lua_pushlstring(L, buffer_ + buffer_i_, sz);
        buffer_i_ += sz;
        return 1;
      }
      luaL_buffinit(L, &buffer);
      // add current content
      luaL_addlstring(&buffer, buffer_ + buffer_i_, buffer_length_ - buffer_i_);
      sz -= buffer_length_ - buffer_i_;
    } else {
      luaL_buffinit(L, &buffer);
    }

    while (sz) {
      // read more data
      { lubyk::ScopedUnlock unlock(worker_);
        // we prefer not reading too much so that we might simplify read operation
        // with a single pushlstring
        buffer_length_ = ::recv(socket_fd_, buffer_, sz < MAX_BUFF_SIZE ? sz : MAX_BUFF_SIZE, 0);
        if (buffer_length_ == 0) {
          // connection closed
          // abort
          return 0;
        } else if (buffer_length_ < 0) {
          buffer_length_ = 0;
          throw Exception("Could not receive (%s).", strerror(errno));
        }
      }
      
      luaL_addlstring(&buffer, buffer_, buffer_length_);

      sz -= buffer_length_;
      buffer_i_ = buffer_length_;
    }                             
    
    luaL_pushresult(&buffer);
    return 1;
  }

  int recvAll(lua_State *L) {
    luaL_Buffer buffer;
    luaL_buffinit(L, &buffer);

    if (buffer_i_ < buffer_length_) {
      luaL_addlstring(&buffer, buffer_ + buffer_i_, buffer_length_ - buffer_i_);
    }

    while (true) {
      // read more data
      { lubyk::ScopedUnlock unlock(worker_);
        buffer_length_ = ::recv(socket_fd_, buffer_, MAX_BUFF_SIZE, 0);
        if (buffer_length_ == 0) {
          // connection closed
          // done
          break;
        } else if (buffer_length_ < 0) {
          buffer_length_ = 0;
          throw Exception("Could not receive (%s).", strerror(errno));
        }
      }
      luaL_addlstring(&buffer, buffer_, buffer_length_);
    }                             
    
    luaL_pushresult(&buffer);
    return 1;
  }

  void setTimeout(int timeout, int opt_name) {
    if (socket_fd_ == -1) {
      throw Exception("Cannot set timeout before bind or connect.");
    }
    struct timeval tv;
    tv.tv_sec = timeout/1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    if (setsockopt(socket_fd_, SOL_SOCKET, opt_name, (char*)&tv, sizeof tv) == -1) {
      throw Exception("Could not set timeout (%s).", strerror(errno));
    }
  }
};
} // lk

#endif // LUBYK_INCLUDE_LK_SOCKET_H_
