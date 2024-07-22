#pragma once

#include "Event.h"
#include "../util/Timestep.h"
#include "../util/Vector.h"
#include "RenderList.h"

/*
 * the main idea for placing components
 *
 * each component will have a size that it reports to its parent
 * the main parameters being:
 * 1. (internal) the status of the component being hidden or visible
 * 2. (external) the size of the components parent
 * 3. (either) whether the component is considered hidden
 *   - could be that the parent doesn't place or draw it
 *   - or could be that the component does draw on onDraw, returns size of 0 by 0
 * 4. the way i am doing this, there is no such thing as child-specific margin.
 *   - margin can be added by parents, components can provide their own external padding
 *   - could also add a margin component that has no behavior, caused some focus issues though
 *
 * another idea:
 *   - have a tree of components, which are composed in as react-like many
 *   - once the component tree is built, then the element tree can be created from there
 *   - how to expand the tree
 *   - have a root component, run the build method to replace it
 *   - have a bunch of nested components
 *
 *  the 3 methods forms the simple application loop:
 *
 *  while (true) {
 *      // handle all pending events
 *      pendingEvents = Window.poll();
 *      for (auto event : pendingEvents) {
 *          component->handleEvent(event);
 *      }
 *
 *      // handle time-based updates
 *      timestep = Timestep::now(timestep);
 *      component->update(timestep);
 *
 *      // draw the frame
 *      component->draw();
 *  }
 *
 *  here is the handshake for sizing elements
 *  elements must choose there size based on only two factors
 *      - the size of their parent
 *      - a size, in pixels that they are set to be
 *      - there will NOT be automatic sizing, where a parent sizes based on the size of the children
 *
 *  handleLayout(width, height, x, y) {
 *      myWidth, myHeight = computeSize(width, height);
 *
 *      for (child : children) {
 *          childX, childY = 0, 0;
 *          childWidth, childHeight = child->layout(myWidth, myHeight, childX, childY);
 *      }
 *
 *      return {myWidth, myHeight}
 *  }
 */

namespace ui {

struct Cursor {
    float x; // the global x position, in pixels
    float y; // the global y position, in pixels
};

class Component {
public:
    Component() : x_(0.0f), y_(0.0f), width_(0.0f), height_(0.0f) {}

    virtual ~Component() = default;

    /**
     * Handles time-triggered updates to the component which occurred since the last u_pdate.
     * This includes updating any child components by using the same timestep.
     *
     * @param timestep the timestep since the last update
     */
    virtual void update(const Timestep& timestep) {}

    /**
     * Handles a user-triggered event that affects the component.
     *
     * @param event the event to handle
     */
    virtual void handle_event(Event& event) {}

    /**
     * Returns whether the a click from the given cursor position would "hit" this component.
     * In general, a hit occurs if the cursor is within the bounding box of the component and
     * the component is not transparent.
     *
     * @param cursor_x the x screen coordinate of the cursor
     * @param cursor_y the y screen coordinate of the cursor
     * @returns true if a hit would occur, false otherwise
     */
    bool is_hit(float cursor_x, float cursor_y) const;

    /**
     * Selects a child of this node to propagate a given event to.
     *
     * @param event the event to propagate
     * @return a pointer to the child, or nullptr if none is selected
     */
    virtual Component* select_child(const Event& event) const {
        return nullptr;
    }

    /**
     * Propagates an event from the component, as the root, down a hierarchy of selected children,
     * and handles the event when bubbling back up the hierarchy (the furthest child from the root
     * runs its handler first, then its parent, proceeding up to the root last).
     *
     * @param event the event to propagate
     */
    void propagate_event(Event& event);

    /**
     * Resizes the component and its children given a bounding box. The new dimensions must fit within
     * the box, meaning that both the height and width are less then or equal to that of the bounding box.
     *
     * @param max_width the width of the bounding box
     * @param max_height the height of the bounding box
     * @return the new dimensions of the component
     */
    virtual Size resize(float max_width, float max_height) = 0;

    /**
     * Repositions the component and its children given the new global offset of the component.
     *
     * @param x the global x offset of the component from the left, in pixels
     * @param y the global y offset of the component from the top, in pixels
     */
    virtual void reposition(float x, float y) = 0;

    /**
     * Draws the component by writing the attributes required to draw the component to the given
     * render list, which is to be consumed by a 2d renderer.
     *
     * @param render_list the render list to write to render information to
     */
    virtual void draw(RenderList& render_list) const = 0;

protected:
    float x_;
    float y_;
    float width_;
    float height_;
};

} // ui
