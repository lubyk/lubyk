#include "mimas.h"
#include "m_view_proxy.h"

#include <string>
#include <iostream>

#include "m_device_view.h"
#include "m_slider.h"
#include "m_pad.h"

void MViewProxy::build_view(const std::string &view_def) {
  std::cout << "BUILDING FROM\n" << view_def << "\n";
  Value def; // TODO: why doesn't "Value def(Json(view_def));" work ?
  def.build_from_json(view_def.c_str());
  if (!def.is_hash()) {
    std::cerr << "Json document is not a hash '" << url() << "': " << def << "\n";
    return;
  }

  MessageManagerLock mml;

  if (view_) delete view_;

  view_ = new MDeviceView(root_proxy_->remote_location().name());
  set_bounds_from_hash(view_, def);

  // TODO: resize view

  workspace_->addAndMakeVisible(view_);

  Value parts_value = def["parts"];
  if (!parts_value.is_hash()) {
    error("'parts' attribute is not a hash. Found", parts_value);
    return;
  }

  Hash *parts = parts_value.hash_;
  Hash::const_iterator it, end = parts->end();
  Value part;

  for (it = parts->begin(); it != end; ++it) {
    if (parts->get(*it, &part) && part.is_hash()) {
      Value klass = part["class"];
      if (!klass.is_string()) {
        error("'class' attribute missing in", part);
        continue;
      }
      if (klass.str() == "Slider") {
        build_slider(part);
      } else if (klass.str() == "Pad") {
        build_pad(part);
      } else {
        error("Unknown class", klass);
      }
    }
  }
}

void MViewProxy::build_slider(const Value &def) {
  std::cout << "build_slider\n";
  const Value connect_path = def["connect"];
  if (!connect_path.is_string()) {
    error("invalid 'connect' attribute in", def);
    return;
  }

  Value err;
  MObjectProxy *proxy = TYPE_CAST(MObjectProxy, root_->find_or_build_object_at(connect_path.str(), &err));
  if (!proxy) {
    error("could not connect", err);
    return;
  }

  MSlider *slider = new MSlider(connect_path.str());
  proxy->connect(slider);

//  slider->setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);
  set_bounds_from_hash(slider, def);
  if (slider->getWidth() > slider->getHeight()) {
    slider->set_slider_type(MSlider::HorizontalSliderType);
  } else {
    slider->set_slider_type(MSlider::VerticalSliderType);
  }

  view_->addAndMakeVisible(slider);
}

void MViewProxy::build_pad(const Value &def) {
  std::cout << "build_pad\n";
  Value connect_path_x = def["connect_x"];
  Value connect_path_y = def["connect_y"];
  if (!connect_path_x.is_string()) {
    error("invalid 'connect_x' attribute in", def);
    return;
  }
  if (!connect_path_y.is_string()) {
    error("invalid 'connect_y' attribute in", def);
    return;
  }


  Value err;
  MObjectProxy *proxy_x = TYPE_CAST(MObjectProxy, root_->find_or_build_object_at(connect_path_x.str(), &err));
  if (!proxy_x) {
    error("could not connect x", err);
    return;
  }

  MObjectProxy *proxy_y = TYPE_CAST(MObjectProxy, root_->find_or_build_object_at(connect_path_y.str(), &err));
  if (!proxy_y) {
    error("could not connect y", err);
    return;
  }

  MPad *pad = new MPad(connect_path_x.str());
  proxy_x->connect(pad->range_x());
  proxy_y->connect(pad->range_y());

  set_bounds_from_hash(pad, def);
  view_->addAndMakeVisible(pad);
}

void MViewProxy::error(const char *message, const Value &context) {
  std::cerr << "Error in '" << url() << "': '" << message << " " << context << ".\n";
}

void MViewProxy::set_bounds_from_hash(Component *component, const Value &def) {
  component->setBounds(
    def["x"].get_real(),
    def["y"].get_real(),
    def["width"].get_real(),
    def["height"].get_real()
  );
}