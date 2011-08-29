#include "mimas/Application.h"
#include <csignal>

#include <QtGui/QFileDialog>

static char arg0[] = "mimas";
static char arg1[] = "-style";
static char arg2[] = "Plastique";
static char *app_argv[] = {&arg0[0], &arg1[0], &arg2[0], NULL};
static int   app_argc   = (int)(sizeof(app_argv) / sizeof(app_argv[0])) - 1;

using namespace mimas;

pthread_key_t Application::sAppKey = NULL;

Application::Application()
   : QApplication(app_argc, app_argv) {
  // create a key to find 'app' object in current thread
  if (!sAppKey) pthread_key_create(&sAppKey, NULL);
}


int Application::exec() {
  ScopedUnlock unlock(worker_);
  // register interrupt signal
  pthread_setspecific(sAppKey, (void*)this);
  signal(SIGINT, terminate);
  return QApplication::exec();
}

/** OS Signal catch.
 */
void Application::terminate(int sig) {
  void *app = pthread_getspecific(sAppKey);
  ((Application*)app)->quit();
}
