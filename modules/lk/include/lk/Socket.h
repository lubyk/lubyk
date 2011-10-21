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
// recv buffer size (must be at least SIZEOF_SIZE)
#define MAX_BUFF_SIZE 8196
// 4 = size needed to encode SIZE_MAX
#define SIZEOF_SIZE 4

using namespace lubyk;

typedef int LuaStackSize;

namespace lk {

/** Listen for incoming messages on a given port.
 *
 * @dub lib_name:'Socket_core'
 *      filename:'Socket_core/Socket_core'
 *      string_format:'%%s:%%d --> %%s:%%d'
 *      string_args:'(*userdata)->localHost(), (*userdata)->localPort(), (*userdata)->remoteHost(), (*userdata)->remotePort()'
 */
class Socket : public LuaObject
{
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

  /** Non-blocking flag when setNonBlocking is called before creating the
   * socket.
   */
  bool non_blocking_;
public:

  enum SocketType {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
  };

  Socket(int socket_type)
      // When changing this, also change the private
      // constructor.
      : socket_fd_(-1)
      , socket_type_(socket_type)
      , local_host_("*")
      , local_port_(-1)
      , remote_host_("?")
      , remote_port_(-1)
      , buffer_length_(0)
      , buffer_i_(0)
      , non_blocking_(false) {
  }

  ~Socket() {
    close();
  }

  void close() {
    if (socket_fd_ != -1) {
      printf("close(%i)\n", socket_fd_);
      ::close(socket_fd_);
      socket_fd_ = -1;
    }
  }

  /** Bind socket to a specific interface.
   * @return bound port
   */
  int bind(const char *localhost = NULL, int port = 0);

  /** Connect to a remote socket.
   * @return false if the socket is not ready and we should waitWrite and 'connectFinish'.
   */
  bool connect(const char *host, int port);

  /** Finish connecting for NON-BLOCKING sockets.
   */
  void connectFinish();

  /** Start listening for incoming connections.
   */
  void listen();

  /** Accept a new incomming connection.
   * @return a new lk.Socket connected to the remote end.
   */
  LuaStackSize accept(lua_State *L);

  void setRecvTimeout(int timeout) {
    setTimeout(timeout, SO_RCVTIMEO);
  }
    
  void setSendTimeout(int timeout) {
    setTimeout(timeout, SO_SNDTIMEO);
  }

  void setNonBlocking() {
    non_blocking_ = true;
    if (socket_fd_ != -1) {
      int x;
      x = fcntl(socket_fd_, F_GETFL, 0);
      if (-1 == fcntl(socket_fd_, F_SETFL, x | O_NONBLOCK)) {
        throw Exception("Could not set non-blocking (%s).", strerror(errno));
      }
    }
  }
  
  /** Receive a message encoded by msgpack (blocks).
   * For a server, this should typically be used inside the loop.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  // ENABLE ONLY IF NEEDED (needs to be rewritten in Lua)
  // LuaStackSize recvMsg(lua_State *L);

  /** Receive a raw string (not encoded by msgpack).
   * This IO call blocks.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  LuaStackSize recv(lua_State *L);

  /** Send raw bytes without encoding with msgpack.
   * param: string to send.
   * @return number of bytes sent.
   */
  int send(lua_State *L);

  /** Send a message packed with msgpack.
   * Varying parameters.
   */
  // ENABLE ONLY IF NEEDED (needs to be rewritten in Lua)
  // void sendMsg(lua_State *L);

  /** Request = remote call (uses msgpack to encode data).
   * This is really just a sendMsg with a recvMsg.
   */
  // ENABLE ONLY IF NEEDED (needs to be rewritten in Lua)
  // LuaStackSize request(lua_State *L) {
  //   sendMsg(L);
  //   return recvMsg(L);
  // }

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

  int fd() const {
    return socket_fd_;
  }

private:
   /** Create a socket with an existing file descriptor.
    * This is used as the result of an 'accept()' call.
    */
   Socket(int fd, const char *local_host, const char *remote_host, int remote_port)
       : socket_fd_(fd)
       , local_host_(local_host)
       , local_port_(get_port(fd))
       , remote_host_(remote_host)
       , remote_port_(remote_port)
       , buffer_length_(0)
       , buffer_i_(0)
       , non_blocking_(false) {
  }

  static int get_port(int fd);

  void run(Thread *runner);

  int recvLine(lua_State *L);

  int recvBytes(lua_State *L, int sz);

  int recvAll(lua_State *L);

  void setTimeout(int timeout, int opt_name);
};
} // lk

#endif // LUBYK_INCLUDE_LK_SOCKET_H_
