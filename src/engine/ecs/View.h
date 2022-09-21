#ifndef OPENGL_RENDERER_VIEW_H
#define OPENGL_RENDERER_VIEW_H

#include "Entity.h"
#include "SparseSet.h"

/**
 * A view of entities that contain the given components.
 *
 * @tparam Cs the components each entity in the view has
 */
template<typename... Cs>
class View {
public:
    explicit View(std::tuple<SparseSet<Cs>&...> sets) : m_sets(sets) {}

    /**
     * Calls the given function for each entity in the view.
     *
     * @param func the function to call per entity
     */
    void forEach(std::function<void(Entity entity, Cs&...)> func) {
        const std::vector<Entity>& entities = std::get<0>(m_sets).entities();

        for (auto entity : entities) {
            if (contains(entity)) {
                func(entity, get<Cs>(entity)...);
            }
        }
    }

    /**
     * Checks whether the view contains a specific entity.
     *
     * @param entity the entity to check if the view has
     * @returns true if the view contains the element, false otherwise
     */
    bool contains(Entity entity) {
        // contains all in case with no component constraints
        bool contains = true;

        // each sparse set must contain the component
        std::array hasComponents = {std::get<SparseSet<Cs>&>(m_sets).contains(entity)...};
        for (auto hasComponent : hasComponents) {
            contains = contains && hasComponent;
        }

        return contains;
    }

    /**
     * Get a specific component of an entity in the view. Exhibits undefined behavior if
     * the entity is not in the view.
     *
     * @tparam C the component to get
     * @param entity the entity to get the component of
     * @returns a reference to the component
     */
    template<typename C>
    C& get(Entity entity) {
        SparseSet<C>& set = std::get<SparseSet<C>&>(m_sets);
        // TODO: consider asserting that it contains
        return set.get(set.indexOf(entity));
    }

    /**
     * Get the components of an entity in the view, grouped as a tuple. Exhibits undefined
     * behavior is the entity is not in the view.
     *
     * @param entity the entity to get the component off
     * @return a tuple of references to the component
     */
    std::tuple<Cs&...> get(Entity entity) {
        return std::make_tuple(get<Cs>(entity)...);
    }

private:
    std::tuple<SparseSet<Cs>&...> m_sets;
};


#endif //OPENGL_RENDERER_VIEW_H
