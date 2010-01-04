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

#ifndef MIMAS_SRC_PROXY_M_VIEW_PROXY_H_
#define MIMAS_SRC_PROXY_M_VIEW_PROXY_H_
#include "m_object_proxy.h"
#include "m_component.h"

class MimasWindowContent;
class MUpdateViewProxy;
class MDeviceView;

class MViewProxy : public MObjectProxy {
public:
  /** Class signature. */
  TYPED("Object.ObjectProxy.MObjectProxy.MViewProxy")

  MViewProxy(MimasWindowContent *mimas, const std::string &name, const Value &type);

  virtual ~MViewProxy();

  /** ViewProxy is ready: force sync and add view to workspace.
   */
  virtual void adopted();

  virtual void value_changed() {
    if (!value_.is_hash()) {
      std::cerr << "'" << url() << "' error: value should be a Hash. Found " << value_ << "\n";
      return;
    }

    MessageManagerLock mml;
    update_view(value_);
  }

  /** This method is called by widgets when something changed on our side. If the
   * remote implements the "update" method, the latter is called, otherwise we just
   * send and update with the full view definition.
   */
  void update_remote(const Value& hash);

  virtual Object *build_child(const std::string &name, const Value &type, Value *error);

  MDeviceView *view() {
    if (!view_) {
      update_view(value_);
    }

    // TODO: resize view
    // this should be done in the caller of the view() method.
    //workspace_->addAndMakeVisible(view_);
    return view_;
  }

  /** Return the current window's content manager.
   */
  MimasWindowContent *mimas() {
    return mimas_;
  }

  /** Return the current tree's root.
   */
  RootProxy *root_proxy() {
    return root_proxy_;
  }

  /** Read a (partial) view definition and build or update the
   * view components.
   */
  void update_view(const Value &def);

private:

  MDeviceView *view_;

  /** Simple object to handle messages to the method "this_view/update".
   */
  MUpdateViewProxy *update_view_proxy_;
};

#endif // MIMAS_SRC_PROXY_M_VIEW_PROXY_H_