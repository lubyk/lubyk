/*
   QVariant variantFromLua(lua_State *L, int index);
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
#include "lk/Socket.h"

/** Bind socket to a specific interface.
 * @return bound port
 */
int lk::Socket::bind(const char *localhost, int port) {
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
    throw dub::Exception("Could not getaddrinfo for %s:%i.", local_host_.c_str(), port);
  }

  if (socket_fd_ != -1) {
    ::close(socket_fd_);
    socket_fd_ = -1;
  }

  // we use getaddrinfo to stay IPv4/IPv6 agnostic
  socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  // printf("socket --> %i\n", socket_fd_);
  if (socket_fd_ == -1) {
    throw dub::Exception("Could not create socket for %s:%i (%s).", local_host_.c_str(), port, strerror(errno));
  }
  if (non_blocking_) setNonBlocking();

  // bind to port
  if (::bind(socket_fd_, res->ai_addr, res->ai_addrlen)) {
    throw dub::Exception("Could not bind socket to %s:%i (%s).", local_host_.c_str(), port, strerror(errno));
  }

  if (port == 0) {
    local_port_ = get_port(socket_fd_);
  } else {
    local_port_ = port;
  }

  freeaddrinfo(res);

  return local_port_;
}

bool lk::Socket::connect(const char *host, int port) {
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
    throw dub::Exception("Could not getaddrinfo for %s:%i (%s).", host, port, gai_strerror(status));
  }

  if (socket_fd_ != -1) {
    // printf("close(%i)\n", socket_fd_);
    ::close(socket_fd_);
    socket_fd_ = -1;
  }

  // we use getaddrinfo to stay IPv4/IPv6 agnostic
  socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  // printf("socket --> %i\n", socket_fd_);
  if (socket_fd_ == -1) {
    throw dub::Exception("Could not create socket for %s:%i (%s).", host, port, strerror(errno));
  }
  if (non_blocking_) setNonBlocking();

  // connect
  if (::connect(socket_fd_, res->ai_addr, res->ai_addrlen)) {
    if (errno == EINPROGRESS) {
      return false; // wait for 'write' and try again later
    }
    throw dub::Exception("Could not connect socket to %s:%i (%s).", host, port, strerror(errno));
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
  return true;
}

void lk::Socket::connectFinish() {
  if (socket_fd_ == -1) {
    throw dub::Exception("Should only be called after 'connect' (no socket).");
  }
  // After select(2) indicates writability, use getsockopt(2) to read the 
  // SO_ERROR option at level SOL_SOCKET to determine whether connect() 
  // completed successfully (SO_ERROR is zero) or unsuccessfully 
  // (SO_ERROR is one of the usual error codes listed here, 
  // explaining the reason for the failure).

  int err;
  socklen_t len = sizeof(err);
  if (getsockopt(socket_fd_, SOL_SOCKET, SO_ERROR, &err, &len)) {
    throw dub::Exception("Could not finalize connection (%s).", strerror(errno));
  }

  if (err) {
    throw dub::Exception("Could not finalize connection (%s).", strerror(err));
  }
}

/** Start listening for incoming connections.
 */
void lk::Socket::listen(int backlog) {
  if (local_port_ == -1)
    throw dub::Exception("Listen called before bind.");

  if (::listen(socket_fd_, backlog)) {
    throw dub::Exception("Could not listen (%s).", strerror(errno));
  }
}

/** Accept a new incomming connection.
 * @return a new lk.Socket connected to the remote end.
 */
LuaStackSize lk::Socket::accept(lua_State *L) {
  // <self>
  lua_pop(L, 1);

  if (local_port_ == -1)
    throw dub::Exception("Accept called before bind.");

  struct sockaddr sa;
  memset(&sa, 0, sizeof(struct sockaddr));
  // length has to be in a variable
  socklen_t sa_len = sizeof(sa);

  int fd;

  fd = ::accept(socket_fd_, &sa, &sa_len);
  //printf("[%p] accept(%i) --> %i\n", this, socket_fd_, fd);
  
  if (fd == -1) {
    throw dub::Exception("Error while accepting connection (%s).", strerror(errno));
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
    throw dub::Exception("Could not get remote host name (%s).", strerror(errno));
  }

  Socket *new_socket = new Socket(fd, local_host_.c_str(), remote_host, remote_port);

  new_socket->pushobject(L, new_socket, "lk.Socket", true);
  return 1;
}


  /** Receive a message encoded by msgpack (blocks).
   * For a server, this should typically be used inside the loop.
   * We pass the lua_State to avoid mixing thread contexts.
   */
// ENABLE ONLY IF NEEDED (needs to be rewritten in Lua)
//
//   LuaStackSize lk::Socket::recvMsg(lua_State *L) {
//     // printf("recvMsg len:%i, i:%i\n", buffer_length_, buffer_i_);
//     // TODO: second param = timeout ?
//     char *msg_buffer  = NULL;
//     char *tmp_buffer  = NULL;
//     size_t sz;
// 
//     // unlock while waiting
// 
//       // Receive buffer length
//       char *sz_ptr = (char*)&sz;
//       for(int i=0; i < SIZEOF_SIZE; ++i) {
//         if (buffer_i_ >= buffer_length_) {
//           // eaten all buffer, fetch more
//           // ***********************************************
//           // This is where we need our coroutine to do coroutine.yield('read', socket_fd_)
//           // ***********************************************
//           buffer_length_ = ::recv(socket_fd_, buffer_, SIZEOF_SIZE - i, 0);
//           if (buffer_length_ == 0) {
//             // connection closed
//             return 0;
//           } else if (buffer_length_ < 0) {
//             buffer_length_ = 0;
//             throw dub::Exception("Could not receive (%s).", strerror(errno));
//           }
//           buffer_i_ = 0;
//         }
//         sz_ptr[i] = *(buffer_ + buffer_i_);
//         ++buffer_i_;
//       }
// 
//       sz = ntohs(sz);
//       // printf("Receiving %lu bytes...\n", sz);
//       if (sz > SIZE_MAX) {
//       throw dub::Exception("Invalid message size (%lu bytes).", sz);
//     }
// 
//     char *recv_buffer = NULL;
// 
//     // available content in buffer
//     size_t avail = buffer_length_ - buffer_i_;
//     size_t done = 0;
// 
//     // allocate buffer if sz > MAX_BUFF_SIZE or we have stuff in buffer_
//     if (sz <= avail) {
//       // everything is in the internal buffer
//       // printf("All in tmp buffer (%lu).\n", avail);
//       msg_buffer = buffer_ + buffer_i_;
//       done = sz;
//       buffer_i_ += done;
//     } else if (sz > MAX_BUFF_SIZE || avail) {
//       // printf("Using tmp buffer (%lu).\n", avail);
//       tmp_buffer = (char*)malloc(sz);
//       if (!tmp_buffer) {
//         throw dub::Exception("Could not allocate buffer (%lu bytes).", sz);
//       }
//       msg_buffer  = tmp_buffer;
//       recv_buffer = msg_buffer + avail;
// 
//       if (avail) {
//         done = avail;
//         memcpy(tmp_buffer, buffer_ + buffer_i_, done);
//         buffer_i_ = buffer_length_;
//       }
//     } else {
//       // internal buffer empty
//       msg_buffer  = buffer_;
//       recv_buffer = buffer_;
//     }
// 
// 
//     while (done < sz) {
//       int received = ::recv(socket_fd_, recv_buffer, sz - done, 0);
//       if (received == 0) {
//         if (tmp_buffer) free(tmp_buffer);
//         return 0;
//       } else if (received < 0) {
//         if (tmp_buffer) free(tmp_buffer);
//         throw dub::Exception("Could not receive (%s).", strerror(errno));
//       }
//       done += received;
//     }
// 
//   int arg_size = msgpack_bin_to_lua(L, msg_buffer, sz);
//   if (tmp_buffer) free(tmp_buffer);
//   return arg_size;
// }

/** Receive a raw string (not encoded by msgpack).
 * This IO call blocks.
 * We pass the lua_State to avoid mixing thread contexts.
 */
LuaStackSize lk::Socket::recv(lua_State *L) {
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
      throw dub::Exception("Bad mode to recv (should be '*a' or '*l' but found '%s')", mode);
    }
  } 
  // receive a single line (not returning \r or \n).
  return recvLine(L);

  return 1;
}

/** Send raw bytes without encoding with msgpack.
 * param: string to send.
 */
int lk::Socket::send(lua_State *L) {
  // <string>
  size_t size;
  const char *data = luaL_checklstring(L, -1, &size);
  // printf("send string (%lu bytes)\n", size);
  // send raw bytes
  int sent = ::send(socket_fd_, data, size, 0);
  if (sent == -1) {
    if (errno == EAGAIN) {
      sent = 0;
    } else {
      throw dub::Exception("Could not send message (%s).", strerror(errno));
    };
  }
  return sent;
}

/** Send a message packed with msgpack.
 * Varying parameters.
 */
// void lk::Socket::sendMsg(lua_State *L) {
//   msgpack_sbuffer* buffer;
//   msgpack_lua_to_bin(L, &buffer, 1);
//   if (buffer->size > SIZE_MAX) {
//     throw dub::Exception("Message too big to send in a single packet (%lu bytes).", buffer->size);
//   }
//   // printf("Sending %lu bytes...\n", buffer->size);
//   size_t sz = htons(buffer->size);
// 
//   if (::send(socket_fd_, (void*)&sz, SIZEOF_SIZE, 0) == -1) {
//     throw dub::Exception("Could not send message size (%s).", strerror(errno));
//   }
// 
//   if (::send(socket_fd_, buffer->data, buffer->size, 0) == -1) {
//     throw dub::Exception("Could not send message (%s).", strerror(errno));
//   }
//   free_msgpack_msg(NULL, buffer);
// }


int lk::Socket::get_port(int fd) {
  // get bound port
  struct sockaddr sa;
  memset(&sa, 0, sizeof(struct sockaddr));
  // length has to be in a variable
  socklen_t sa_len = sizeof(sa);
  if (getsockname(fd, &sa, &sa_len)) {
    throw dub::Exception("Could not get bound port (%s).", strerror(errno));
  }
  if (sa.sa_family == AF_INET) {
    return ntohs(((struct sockaddr_in *)&sa)->sin_port);
  } else {
    return ntohs(((struct sockaddr_in6 *)&sa)->sin6_port);
  }
}

int lk::Socket::recvLine(lua_State *L) {
  luaL_Buffer buffer;
  luaL_buffinit(L, &buffer);

  while (true) {
    while (buffer_i_ < buffer_length_) {
      char c = buffer_[buffer_i_++];
      // printf("recvLine (%c)\n", c);
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
    buffer_length_ = ::recv(socket_fd_, buffer_, MAX_BUFF_SIZE, 0);
    if (buffer_length_ == 0) {
      // connection closed
      return 0;
    } else if (buffer_length_ < 0) {
      buffer_length_ = 0;
      throw dub::Exception("Could not receive (%s).", strerror(errno));
    }
    buffer_i_ = 0;
  }                             
  return 0;
}

int lk::Socket::recvBytes(lua_State *L, int sz) {
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
    // we prefer not reading too much so that we might simplify read operation
    // with a single pushlstring
    buffer_length_ = ::recv(socket_fd_, buffer_, sz < MAX_BUFF_SIZE ? sz : MAX_BUFF_SIZE, 0);
    if (buffer_length_ == 0) {
      // connection closed
      // abort
      return 0;
    } else if (buffer_length_ < 0) {
      buffer_length_ = 0;
      throw dub::Exception("Could not receive (%s).", strerror(errno));
    }
    
    luaL_addlstring(&buffer, buffer_, buffer_length_);

    sz -= buffer_length_;
    buffer_i_ = buffer_length_;
  }                             
  
  luaL_pushresult(&buffer);
  return 1;
}

int lk::Socket::recvAll(lua_State *L) {
  luaL_Buffer buffer;
  luaL_buffinit(L, &buffer);

  if (buffer_i_ < buffer_length_) {
    luaL_addlstring(&buffer, buffer_ + buffer_i_, buffer_length_ - buffer_i_);
  }

  while (true) {
    // read more data
    buffer_length_ = ::recv(socket_fd_, buffer_, MAX_BUFF_SIZE, 0);
    if (buffer_length_ == 0) {
      // connection closed
      // done
      return 0;
    } else if (buffer_length_ < 0) {
      buffer_length_ = 0;
      if (errno == EAGAIN) {
        break;
      } else {
        throw dub::Exception("Could not receive (%s).", strerror(errno));
      }
    }
    luaL_addlstring(&buffer, buffer_, buffer_length_);
  }                             
  
  luaL_pushresult(&buffer);
  return 1;
}

void lk::Socket::setTimeout(int timeout, int opt_name) {
  if (socket_fd_ == -1) {
    throw dub::Exception("Cannot set timeout before bind or connect.");
  }
  struct timeval tv;
  tv.tv_sec = timeout/1000;
  tv.tv_usec = (timeout % 1000) * 1000;
  if (setsockopt(socket_fd_, SOL_SOCKET, opt_name, (char*)&tv, sizeof tv) == -1) {
    throw dub::Exception("Could not set timeout (%s).", strerror(errno));
  }
}
