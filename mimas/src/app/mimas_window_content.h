#ifndef MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_
#define MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_

#include "mimas.h"
#include "m_look_and_feel.h"
#include "oscit/osc_command.h"
#include "device_browser.h"
#include "root_proxy_tree_item.h"
#include "m_root_proxy.h"
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

  Component *workspace() {
    return workspace_;
  }

  inline bool action_mode() {
    return editor_mode_ == ActionMode;
  }

  inline bool edit_mode() {
    return editor_mode_ == EditMode;
  }

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
  DeviceBrowser *device_browser_;

  /** Border used to resize the window.
  */
  ResizableBorderComponent *border_;

  /** Main workspace and view port: this is where all the work
  * is done.
  */
  Component *workspace_;

  /** Scrollable container that contains the main workspace.
  */
  Viewport *workspace_port_;

  /** Top toolbar.
  */
  Component *toolbar_;

  /** Button to enter/exit edit mode.
  */
  TextButton *edit_mode_button_;

  // (prevent copy constructor and operator= being generated..)
  MimasWindowContent (const MimasWindowContent&);
  const MimasWindowContent& operator= (const MimasWindowContent&);
};


#endif // MIMAS_SRC_APP_MIMAS_WINDOW_CONTENT_H_
