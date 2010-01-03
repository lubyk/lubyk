#include "mimas.h"
#include "m_ghost_component.h"

#include "m_component.h"

MGhostComponent::MGhostComponent(MComponent *master)
    : mimas_(master->mimas()) {
  set_and_hold(&master_, master);
}


// void MGhostComponent::mouseDown(const MouseEvent &e);
// void MGhostComponent::mouseDrag(const MouseEvent &e);
// void MGhostComponent::mouseUp(const MouseEvent &e);