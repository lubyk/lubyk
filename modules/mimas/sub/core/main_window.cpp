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

#include "mimas/MainWindow.h"
#include "mimas/Painter.h"

#include <QtGui/QFileDialog>

namespace mimas {


void MainWindow::testMenus(bool inplace, lua_State *L) {
  if (inplace) {
    // we do not have access to the "App" OS X menu: it is not in the menuBar.
    printf("in place\n");
    QMenuBar *bar = menuBar();

    QAction *foobar = new QAction(QString("settings"), this);
    foobar->setShortcut(QKeySequence("Ctrl+W"));
    QObject::connect(foobar, SIGNAL(triggered()), this, SLOT(close()));
    QMenu *foo = bar->addMenu(QString("foo"));
    foo->addAction(foobar);

    QObjectList list = bar->children();
  } else {
    MenuBar *bar = new MenuBar(this);
    bar->luaInit(L, bar, "mimas.MenuBar");
    QAction *foobar = new QAction(QString("Foobar"), this);
    foobar->setShortcut(QKeySequence("Ctrl+W"));
    QObject::connect(foobar, SIGNAL(triggered()), this, SLOT(close()));
    QMenu *foo = bar->addMenu(QString("foo"));
    foo->addAction(foobar);
    setMenuBar(bar);
  }
}

void MainWindow::paintEvent(QPaintEvent *event) {
  Painter *p = new Painter(this);
  if (!parent()) {
    // window
    p->QPainter::fillRect(rect(), palette().color(QPalette::Window));
  }
  Widget::paint(this, p, width(), height());
  delete p;
  QMainWindow::paintEvent(event);
}

LuaStackSize MainWindow::getOpenFileName(const char *caption,
                        const char *the_base_dir,
                        const char *the_filter,
                        int options,
                        lua_State *L) {
  QString base_dir(the_base_dir);
  if (base_dir.isEmpty()) base_dir = QString();

  QString filter(the_filter);
  if (filter.isEmpty()) filter = QString();

  QString path = QFileDialog::getOpenFileName(this, caption, base_dir, filter, 0, (QFileDialog::Option)options);
  if (path.isNull()) {
    return 0;
  } else {
    lua_pushstring(L, path.toUtf8().data());
    return 1;
  }
}

LuaStackSize MainWindow::getExistingDirectory(const char *caption,
                        const char *the_base_dir,
                        int options,
                        lua_State *L) {
  QString base_dir(the_base_dir);
  if (base_dir.isEmpty()) base_dir = QString();

  QString path = QFileDialog::getExistingDirectory(this, caption, base_dir, (QFileDialog::Option)options);
  if (path.isNull()) {
    return 0;
  } else {
    lua_pushstring(L, path.toUtf8().data());
    return 1;
  }
}

} // mimas
