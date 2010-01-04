#include "mimas.h"
#include "m_breadcrumbs.h"

#include "m_breadcrumbs_item.h"

void MBreadcrumbs::set_url(const Url &url) {
  if (url_ == url) return;
  std::cout << "set_url = " << url << "\n";
  url_ = url;
  clear();

  Url current_url(url);
  MBreadcrumbsItem *item;

  std::cout << "Build path";
  do {
    std::cout << " " << current_url;
    item = new MBreadcrumbsItem(browser_, current_url);
    paths_.push_front(item);
    addAndMakeVisible(item);
  } while(current_url.get_parent_url(&current_url));

  std::cout << "\n";
  resized();
}

void MBreadcrumbs::clear() {
  std::list<MBreadcrumbsItem*>::iterator it = paths_.begin(), end = paths_.end();
  while(it != end) {
    delete *it;
    it = paths_.erase(it);
  }
}

// FIXME: use a view port with scrollbar ?
void MBreadcrumbs::resized() {
  int width  = getWidth();
  int height = getHeight() - 2 * BREADCRUMBS_BORDER_WIDTH;
  int half_height = height / 2;
  int used_width = 0;

  std::list<MBreadcrumbsItem*>::iterator it = paths_.begin(), end = paths_.end();
  while(it != end) {

    MBreadcrumbsItem *item = *it;
    int item_width = item->min_width() + 2 * half_height; // path element overlap and we do not want text in the arrow
    std::cout << "item_width = " << item_width << "\n";
    item->setBounds(
      used_width,
      BREADCRUMBS_BORDER_WIDTH,
      item_width,
      height
    );

    used_width += item_width - half_height; // overlap

    ++it;
  }
}