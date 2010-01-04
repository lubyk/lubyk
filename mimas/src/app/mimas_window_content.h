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

#ifndef MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_
#define MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_

#include "mimas.h"
#include "m_look_and_feel.h"
#include "oscit/osc_command.h"
#include "m_browser.h"
#include "m_proxy_factory.h"
#include "m_theme.h"

#define TOOLBAR_HEIGHT 30

class MimasWindowContent : public Component, public ButtonListener {
public:
  enum EditorMode {
    ActionMode = 0,
    EditMode,
    FrozenMode,
    LastMode
  };

  MimasWindowContent();
  ~MimasWindowContent();

  virtual void paint(Graphics& g);
  virtual void resized();
  virtual void buttonClicked(Button *button);

  juce_UseDebuggingNewOperator

  const Colour &color(const MTheme::ColorId color_id) const {
    return themes_[is_day_mode_].color_for_id(color_id);
  }

  const Colour &bg_color() const {
    return themes_[is_day_mode_].color_for_id((MTheme::ColorId)editor_mode_);
  }

  float font_size() const {
    return themes_[is_day_mode_].font_size();
  }

  Component *workspace() {
    return workspace_;
  }

  Component *workspace_port() {
    return workspace_port_;
  }

  inline bool action_mode() {
    return editor_mode_ == ActionMode;
  }

  inline bool edit_mode() {
    return editor_mode_ == EditMode;
  }

  void resize_except(Component *skip_component);

private:
  /* ================== APP STATE    ================= */
  EditorMode editor_mode_;

  /** This holds two array of colors used by the various paint methods.
   */
  MTheme *themes_;

  /** Holds true if we should use the "day" theme (light).
   */
  bool is_day_mode_;

  /* ================== MAIN WIDGETS ================= */

  /** Our main working tree (holds commands and proxies).
   */
  Root work_tree_;

  /* ================== MAIN WIDGETS ================= */

  /** Device browser (currently a TreeView, but should be replaced by a custom view
   * holding a device list, a breadcrumbs path and a method list).
   */
  MBrowser *device_browser_;

  /** Border used to resize the window.
  */
  ResizableBorderComponent *border_;

  /** Top toolbar.
  */
  Component *toolbar_;

  /** Button to enter/exit edit mode.
  */
  TextButton *edit_mode_button_;

  /** Main workspace and view port: this is where all the work
  * is done.
  */
  Component *workspace_;

  /** Scrollable container that contains the main workspace.
  */
  Viewport *workspace_port_;

  // (prevent copy constructor and operator= being generated..)
  MimasWindowContent (const MimasWindowContent&);
  const MimasWindowContent& operator= (const MimasWindowContent&);
};


#endif // MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_
