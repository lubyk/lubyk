#include "mimas.h"
#include "m_component.h"

#include "m_view_proxy.h"

MComponent::MComponent(MViewProxy *view_proxy, const std::string &name)
    : Component(String(name.c_str())),
      mimas_(view_proxy->mimas()),
      view_proxy_(view_proxy),
      root_proxy_(view_proxy->root_proxy()) {}

MComponent::MComponent(MViewProxy *view_proxy, const Value &def)
    : mimas_(view_proxy->mimas()),
      view_proxy_(view_proxy),
      root_proxy_(view_proxy->root_proxy()) {
  update(def);
}

void MComponent::update(const Value &def) {

  // ========================================== x, y, width, height

  if (def.has_key("x") || def.has_key("y") || def.has_key("width") || def.has_key("height")) {
    setBounds(
      def["x"].get_real(getX()),
      def["y"].get_real(getY()),
      def["width"].get_real(getWidth()),
      def["height"].get_real(getHeight())
    );
  }

  // ========================================== hue

  if (def.has_key("hue")) {
    set_hue(def["hue"].get_real());
  }
}

void MComponent::set_hue(float hue) {
  if (hue < 0 || hue >= 360) hue = 0;
  //                     hue           sat  bri  alpha
  border_color_ = Colour(hue / 360.0f, 1.0f, 1.0f, 1.0f);
  fill_color_   = Colour(hue / 360.0f, 0.5f, 0.5f, 1.0f);
}