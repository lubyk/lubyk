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
#include "lubyk/worker.h"
#include "lubyk/cocoa.h"
#include "lubyk/lua.h"
#include "lua_cpp_helper.h"

using namespace lubyk;

#include <mach-o/dyld.h> // _NSGetExecutablePath

Worker::Worker()
    : zmq_context_(NULL)
    , zmq_context_refcount_(0) {
}

Worker *Worker::getWorker(lua_State *L) {
  // ...
  lua_pushlstring(L, "worker", 6);
  // ... <'worker'>
  lua_gettable(L, LUA_GLOBALSINDEX);
  // ... <worker>
  Worker **userdata = (Worker**)dubL_checksdata_n(L, -1, "lubyk.Worker");
  lua_pop(L, 1);
  // ...
  return *userdata;
}

Worker::~Worker() {
}

static char *getExecPath() {

  // http://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe
  uint32_t bufsize = 200;
  char *buf = (char*)malloc(bufsize * sizeof(char));
  if (_NSGetExecutablePath(buf, &bufsize)) {
    // buffer too small
    char * nbuf = (char*)realloc(buf, bufsize * sizeof(char));
    if (nbuf) {
      if (_NSGetExecutablePath(nbuf, &bufsize)) {
        // error again .... ?
        free(nbuf);
        return NULL;
      } else {
        return nbuf;
      }
    } else {
      // could not allocate memory
      free(buf);
      return NULL;
    }
  } else {
    // ok
    return buf;
  }
}
/** Get the current executable's path.
 */
LuaStackSize Worker::execPath(lua_State *L)
{
  char *path = getExecPath();
  if (path) {
    lua_pushstring(L, path);
    free(path);
    return 1;
  } else {
    return 0;
  }
}

extern "C" {
#include "lualib.h"
}

#if 0
static void startProcess(const char *string) {
  lua_State *L = lua_open();
  luaL_openlibs(L);  /* open libraries */

  if (luaL_loadstring(L, string)) {
    fprintf(stderr, "Compilation error in 'spawn': %s\n", lua_tostring(L, -1));
  } else {
    if (lua_pcall(L, 0, 0, 0)) {
      fprintf(stderr, "Error in 'spawn': %s\n", lua_tostring(L, -1));
    }
  }

  lua_close(L);
}
#endif 

/** Start a new process with the given Lua script.
 */
LuaStackSize Worker::spawn(const char *script, lua_State *L)
{
  char *path = getExecPath();
  if (path) {
    char arg1[] = "-e";
    char *argv[] = {path, arg1, const_cast<char*>(script), NULL};

    int pid = fork();
    if (pid == 0) {
      // child process
      execv(argv[0], argv);
      //startProcess(script);
      // unreachable
      exit(0);
      return 0;
    } else {
      // parent process
      free(path);
      lua_pushnumber(L, pid);
      return 1;
    }
  } else {
    // could not get executable path
    return 0;
  }
}

void Worker::exit(int status) {
  ::exit(status);
}

/** FIXME: Replace with selfpipe so that we have a FD and we can treat
 * waitpid like a non-blocking socket read.
 * This could also be used to spawn actions through pipes and get result back
 * without the need for zmq...
 *
 * http://stackoverflow.com/questions/282176/waitpid-equivalent-with-timeout
 *
int selfpipe[2];
void selfpipe_sigh(int n)
{
    write(selfpipe[1], "",1);
}
void selfpipe_setup(void)
{
    static struct sigaction act;
    if (pipe(selfpipe) == -1) { abort(); }
    fcntl(selfpipe[0],F_SETFL,fctnl(selfpipe[0],F_GETFL)|O_NONBLOCK);
    fcntl(selfpipe[1],F_SETFL,fctnl(selfpipe[1],F_GETFL)|O_NONBLOCK);
    memset(&act, 0, sizeof(act));
    act.sa_handler = selfpipe_sigh;
    act.sa_flags |= 0;
    sigaction(SIGCHLD, &act, NULL);
}
Then, your waitpid-like function looks like this:

int selfpipe_waitpid(void)
{
    static char dummy[4096];
    fd_set rfds;
    struct timeval tv;
    int died = 0, st;

    tv.tv_sec = 5;
    tv.tv_usec = 0;
    FD_ZERO(&rfds);
    FD_SET(selfpipe[0], &rfds);
    if (select(selfpipe[0]+1, &rfds, NULL, NULL, &tv) > 0) {
       while (read(selfpipe[0],dummy,sizeof(dummy)) > 0);
       while (waitpid(-1, &st, WNOHANG) != -1) died++;
    }
    return died;
}
*/
int Worker::waitpid(int pid) {
  int child_status;
  ::waitpid(pid, &child_status, 0);
  if (WIFEXITED(child_status)) {
    return WEXITSTATUS(child_status);
  } else {
    return -1;
  }
}

