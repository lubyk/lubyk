#ifndef MIMAS_SRC_WIDGETS_M_THEME_H_
#define MIMAS_SRC_WIDGETS_M_THEME_H_

class MTheme {
public:
  MTheme() {
    colors_ = new Colour[LastColorId];
  }

  ~MTheme() {
    delete[] colors_;
  }

  enum ColorId {
    WorkspaceActionBG = 0,
    WorkspaceEditBG,
    WorkspaceFrozenBG,
    WorkspaceBorder,
    
    Disconnected,
    
    // MGhostComponent
    GhostBG,
    GhostBorder,

    BrowserSelectedItem,
    BrowserLabel,

    ToolbarBG,

    DeviceBorder,
    DeviceLabel,

    LastColorId,
  };

  const Colour &color_for_id(const ColorId id) const {
    return colors_[id];
  }

  /** Load the "day" colors into the theme.
   * TODO: load from file...
   */
  void set_day_theme();

  /** Load the "night" colors into the theme.
   * TODO: load from file...
   */
  void set_night_theme();
private:
  Colour *colors_;
};

#endif // MIMAS_SRC_WIDGETS_M_THEME_H_
