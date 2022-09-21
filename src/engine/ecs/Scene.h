#ifndef OPENGL_RENDERER_SCENE_H
#define OPENGL_RENDERER_SCENE_H

#include "Entity.h"
#include "SparseSet.h"
#include "View.h"

inline uint32_t next() {
    static uint32_t next = 0;
    return next++;
}

template<typename C>
uint32_t id() {
    static uint32_t id = next();
    return id;
}

class Scene {
public:
    Scene() : m_sets{}, m_destroyed{}, nextEntity(0) {}

    /**
     * @returns a new entity with no components
     */
    Entity createEntity() {
        // first taken already used but free tags
        if (!m_destroyed.empty()) {
            Entity entity = m_destroyed.back();
            m_destroyed.pop_back();
            return entity;
        }
        // make a new tag if no old ones
        else {
            return nextEntity++;
        }
    };

    /**
     * Removes all of the components belonging to the entity and then removes it.
     *
     * @param entity the entity to remove
     */
    void destroyEntity(Entity entity) {
        for (auto& set : m_sets) {
            // remove the entity from every set it is in
            if (set != nullptr && set->contains(entity)) {
                set->remove(entity);
            }
        }

        m_destroyed.push_back(entity);
    };

    /**
     * Removes the given component from the given entity if one exists.
     *
     * @tparam C the component to remove
     * @param entity the entity to remove the component from
     */
    template<typename C>
    void removeComponent(Entity entity) {
        SparseSet<C>& set = storage<C>();
        set.remove(entity);
    };

    /**
     * Adds the given component to the given entity.
     *
     * @tparam C the component to add
     * @param entity the entity to add the component to
     */
    template<typename C>
    void addComponent(Entity entity) {
        SparseSet<C>& set = storage<C>();
        set.add(entity);
    };

    /**
     * Checks whether the given entity has the specified component.
     *
     * @tparam C the component to check for
     * @param entity the entity to check
     * @returns true if the entity has the component, false otherwise
     */
    template<typename C>
    bool hasComponent(Entity entity) {
        SparseSet<C>& set = storage<C>();
        return set.contains(entity);
    };

    /**
     * Gets the entities that have the given component.
     *
     * @tparam C the component the entities have
     * @return a reference to the entities
     */
    template<typename C>
    const std::vector<Entity>& withComponent() {
        SparseSet<C>& set = storage<C>();
        return set.entities();
    }

    /**
     * Gets a component for the given entity.
     *
     * @tparam C the component to get
     * @param entity the entity to get the component of
     * @returns a reference to the component
     */
    template<typename C>
    C& getComponent(Entity entity) {
        SparseSet<C>& set = storage<C>();
        if (!set.contains(entity)) {
            throw std::out_of_range("the entity does have the specified component");
        }
        return set.get(set.indexOf(entity));
    };

    template<typename... Cs>
    View<Cs...> view() {
        auto sets = std::tuple<SparseSet<Cs>&...>(storage<Cs>()...);
        return View(sets);
    }

private:
    /**
     * Gets the sparse set used to store the given component. If the set does not exist,
     * it is created.
     *
     * @tparam C the component stored by the sparse set
     * @returns a reference to the sparse set
     */
    template<typename C>
    SparseSet<C>& storage() {
        uint32_t componentID = id<C>();

        if (componentID >= m_sets.size()) {
            m_sets.resize(componentID + 1);
        }

        std::unique_ptr<StorageSet>& set = m_sets[componentID];

        // created sparse set if none exists
        if (set == nullptr) {
            set.reset(new SparseSet<C>());
        }

        return dynamic_cast<SparseSet<C>&>(*set);
    }

    std::vector<std::unique_ptr<StorageSet>> m_sets;
    std::vector<Entity> m_destroyed;
    Entity nextEntity = 0;
};


#endif //OPENGL_RENDERER_SCENE_H
