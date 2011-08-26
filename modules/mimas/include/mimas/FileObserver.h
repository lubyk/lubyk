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
#ifndef LUBYK_INCLUDE_MIMAS_FILE_OBSERVER_H_
#define LUBYK_INCLUDE_MIMAS_FILE_OBSERVER_H_

#include "mimas/mimas.h"
#include "mimas/constants.h"

using namespace lubyk;

#include <QtCore/QFileSystemWatcher>
#include <QtGui/QMouseEvent>
#include <QtCore/QPoint>

#include <iostream>


namespace mimas {

/** The DataSource is used to provide data to Views such as TableView, ListView or
 * TreeView.
 *
 */
class FileObserver : public QFileSystemWatcher, public LuaObject
{
  Q_OBJECT

public:
  FileObserver() {
    QObject::connect(this, SIGNAL(fileChanged(QString)),
                     this, SLOT(pathChangedSlot(QString)));

    //QObject::connect(this, SIGNAL(directoryChanged(QString)),
    //                 this, SLOT(pathChangedSlot(QString)));
  }

  ~FileObserver() {
    MIMAS_DEBUG_GC
  }

  void addPath(const char *path) {
    QFileSystemWatcher::addPath(QString::fromUtf8(path));
  }

  void removePath(const char *path) {
    QFileSystemWatcher::removePath(QString::fromUtf8(path));
  }

private slots:
  // connected to the pathChanged signal
	void pathChangedSlot(const QString &path) {
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    pushLuaCallback("pathChanged");
    lua_pushstring(L, path.toUtf8().data());
    // <func> <self> <path>
    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      // cannot raise an error here...
      fprintf(stderr, "Error in 'pathChanged' callback: %s\n", lua_tostring(L, -1));
    }
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_FILE_OBSERVER_H
