/*
  ==============================================================================

   This file is part of the MIMAS project (http://rubyk.org/mimas)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef MIMAS_SRC_DEVICE_BROWSER_M_PATH_LIST_H_
#define MIMAS_SRC_DEVICE_BROWSER_M_PATH_LIST_H_
#include "mimas.h"

#include <vector>

class MimasWindowContent;
class MBrowser;

class MPathList : public ListBox, public ListBoxModel {
public:
  MPathList(MBrowser *browser);

  void set_container(Object *object);

  // ======== ListBoxModel callbacks =========== //

  virtual int getNumRows();

  virtual void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);

  // ======== ListBox callbacks     ============ //
  virtual void listBoxItemClicked(int row, const MouseEvent &e);

  virtual void paint(Graphics &g);

private:
  MimasWindowContent *mimas_;
  MBrowser *browser_;

  /** Object whose children are currently displayed in the
   * list.
   * FIXME: callback on object delete !
   */
  Object *container_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_M_PATH_LIST_H_
