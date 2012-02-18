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

#include "mimas/ListView.h"
#include "mimas/Painter.h"
#include <QtGui/QStyledItemDelegate>
#include <QtGui/QTextDocument>
#include <QtCore/QUrl>

namespace mimas {

/** Simple delegate to draw html in list view. Enabled
 * by setting ListView:enableHtml(true, 'extra css').
 */
class ItemHTMLDelegate : public QStyledItemDelegate {
  ListView *master_;
  QString style_;
  QTextDocument doc_;
public:
  ItemHTMLDelegate(ListView *master, QString style)
    : master_(master) 
    , style_(style) {
    }
protected:

  virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    painter->save();

    QTextDocument doc;
    doc.setDefaultStyleSheet(style_);
    doc.setHtml(options.text);

    options.text = "";
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    painter->translate(options.rect.left(), options.rect.top());
    QRect clip(0, 0, options.rect.width(), options.rect.height());
    doc.drawContents(painter, clip);
    painter->restore();
  }

  virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    QTextDocument doc;
    doc.setDefaultStyleSheet(style_);
    doc.setHtml(options.text);
    doc.setTextWidth(options.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
  }
};


bool ListView::click(QMouseEvent *event, int type) {
  lua_State *L = lua_;

  if (pushLuaCallback("select")) {
    // ... <select> <self>
    if (type == MousePress) {
      // normal click processing
      lua_pop(L, 2);
      return false;
    } else {
      // select
      QModelIndex idx = QListView::indexAt(QPoint(event->x(), event->y()));
      return select(idx, event);
    }
  }
  return Widget::click(this, event, type);
}

bool ListView::select(const QModelIndex &idx, QEvent *event) {
  lua_State *L = lua_;
  // ... <select> <self>
  if (!idx.isValid()) {
    lua_pop(L, 2);
    return false;
  }

  lua_pushnumber(L, idx.row() + 1);
  lua_pushnumber(L, idx.column() + 1);
  // ... <select> <self> <row> <col>
  int status = lua_pcall(L, 3, 1, 0);

  if (status) {
    fprintf(stderr, "Error in 'select' callback: %s\n", lua_tostring(L, -1));
  }

  if (lua_isfalse(L, -1)) {
    // We are not concerned.
    event->ignore();
  } else if (lua_istrue(L, -1)) {
    // continue processing with super
    lua_pop(L, 1);
    return false;
  }
  // processing done.
  lua_pop(L, 1);
  return true;
}

class ItemPaintDelegate : public QStyledItemDelegate {
  ListView *master_;
public:
  ItemPaintDelegate(ListView *master)
    : master_(master) {}
  
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

void ListView::enablePaintItem(bool enable) {
  if (!enable) {
    if (item_delegate_) {
      delete item_delegate_;
      item_delegate_ = NULL;
      setItemDelegate(NULL);
    }
  } else {
    if (!item_delegate_) {
      item_delegate_ = new ItemPaintDelegate(this);
      setItemDelegate(item_delegate_);
    }
  }
}

void ListView::enableHtml(bool enable, const char *css) {
  if (!enable) {
    if (item_delegate_) {
      delete item_delegate_;
      item_delegate_ = NULL;
      setItemDelegate(NULL);
    }
  } else {
    if (!item_delegate_) {
      item_delegate_ = new ItemHTMLDelegate(this, css ? QString(css) : QString());
      setItemDelegate(item_delegate_);
    }
  }
}


ListView::ListView() : item_delegate_(NULL) {
  setAttribute(Qt::WA_DeleteOnClose);
  setSelectionMode(QAbstractItemView::SingleSelection);
  // Not editable
  setEditTriggers(QAbstractItemView::NoEditTriggers);

  MIMAS_DEBUG_CC
}

ListView::~ListView() {
  if (item_delegate_) delete item_delegate_;
  MIMAS_DEBUG_GC
}

void ItemPaintDelegate::paint(
    QPainter *painter,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
  if (!master_->paintItem((Painter*)painter, option, index)) {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

bool ListView::paintItem(
    Painter *p,
    const QStyleOptionViewItem &option,
    const QModelIndex &idx) {
  
  p->setRenderHints(QPainter::Antialiasing);
  lua_State *L = lua_;

  const QRect &rect = option.rect;

  if (!pushLuaCallback("paintItem")) return false;

  // Deletable out of Lua
  lua_pushclass2<Painter>(L, p, "mimas.Painter");
  lua_pushnumber(L, rect.x());
  lua_pushnumber(L, rect.y());
  lua_pushnumber(L, rect.width());
  lua_pushnumber(L, rect.height());
  lua_pushnumber(L, idx.row() + 1);
  lua_pushnumber(L, idx.column() + 1);
  // <func> <self> <Painter> <x> <y> <width> <height> <col> <row>
  int status = lua_pcall(L, 8, 0, 0);

  if (status) {
    fprintf(stderr, "Error in 'paintItem' callback: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  return true;
}

} // mimas
