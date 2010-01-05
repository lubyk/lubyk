#include "mimas.h"
#include "m_breadcrumbs_item.h"
#include "m_browser.h"

MBreadcrumbsItem::MBreadcrumbsItem(MBrowser *browser, const Url &url)
    : mimas_(browser->mimas()),
      browser_(browser),
      url_(url) {}

void MBreadcrumbsItem::mouseDown(const MouseEvent &e) {
  browser_->select_container_url(url_);
}

