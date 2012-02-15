#include "mimas/Application.h"
#include <csignal>

#include <QtGui/QFileDialog>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>

static char arg0[] = "Lubyk";
static char arg1[] = "-style";
static char arg2[] = "Plastique";
static char *app_argv[] = {&arg0[0], &arg1[0], &arg2[0], NULL};
static int   app_argc   = (int)(sizeof(app_argv) / sizeof(app_argv[0])) - 1;

using namespace mimas;

pthread_key_t Application::sAppKey = NULL;

Application::Application()
   : QApplication(app_argc, app_argv) {
  // create a key to find 'app' object in current thread
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  if (!sAppKey) pthread_key_create(&sAppKey, NULL);
  // Avoid nasty number parsing bugs (0.5 not parsed)
  QLocale::setDefault(QLocale::c());
  setlocale(LC_NUMERIC, "C");
  MIMAS_DEBUG_CC
}


int Application::exec() {
  // register interrupt signal
  pthread_setspecific(sAppKey, (void*)this);
  signal(SIGINT, terminate);
  return QApplication::exec();
}

void Application::processEvents(int maxtime) {
  QCoreApplication::sendPostedEvents();
  QCoreApplication::processEvents(QEventLoop::AllEvents, maxtime);
}

/** OS Signal catch.
 */
void Application::terminate(int sig) {
  void *app = pthread_getspecific(sAppKey);
  ((Application*)app)->quit();
}

bool Application::event(QEvent *event) {
#ifdef __MACOSX_CORE__
  if (event->type() == QEvent::FileOpen) {
    // macosx file open event
    QFileOpenEvent *fop = static_cast<QFileOpenEvent*>(event);
    QString file = fop->file();
    lua_State *L = lua_;

    if (!pushLuaCallback("openFile")) {
      // printf("Called 'openFile' for '%s' but Lua method is not defined for 'app'.\n", file.toUtf8().data());
      return false;
    }

    lua_pushstring(L, file.toUtf8().data());
    // <func> <self> <'file'>
    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'openFile' callback: %s\n", lua_tostring(L, -1));
    }
    return true;
  }
#endif
  return QApplication::event(event);
}
