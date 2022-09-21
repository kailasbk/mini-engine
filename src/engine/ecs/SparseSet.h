#ifndef OPENGL_RENDERER_SPARSESET_H
#define OPENGL_RENDERER_SPARSESET_H

#include "Entity.h"

class StorageSet {
public:
    virtual ~StorageSet() = default;

    /**
     * Adds the given entity to the storage set.
     *
     * @param entity the entity to add to the storage, must not be in the storage
     */
    virtual void add(Entity entity) = 0;

    /**
     * Removes the given entity from the storage set.
     *
     * @param entity the entity to remove from the storage, must be in the storage
     */
    virtual void remove(Entity entity) = 0;

    /**
     * Checks if the given entity is present in the storage set.
     *
     * @param entity the entity to check for
     * @returns true if the entity is present, false otherwise
     */
    virtual bool contains(Entity entity) const = 0;
};

/**
 * A sparse set of entities and components.
 */
template<typename C>
class SparseSet : public StorageSet {
public:
    SparseSet() : m_sparse{}, m_entities{}, m_components{} {}

    /**
     * Adds the given entity to the sparse set.
     *
     * @param entity the entity to add to the set, must not be in the set
     * @returns the index of the added entity in the dense vectors
     */
    void add(Entity entity) override {
        // place the entity at the end of the dense array
        uint32_t denseIndex = m_entities.size();
        m_entities.push_back(entity);
        m_components.push_back(C{});

        // sparse array but be at least big enough for the entity
        if (entity >= m_sparse.size()) {
            m_sparse.resize(entity + 1);
        }

        // place the entity index in the sparse array
        m_sparse[entity] = denseIndex;
    }

    /**
     * Removes the given entity from the sparse set. The last entity in the dense vectors is
     * moved to the index of the removed entity.
     *
     * @param entity the entity to remove from the set, must be in the set
     */
    void remove(Entity entity) override {
        // get the last entity and its component from dense vectors
        Entity lastEntity = m_entities.back();
        C lastComponent = std::move(m_components.back());

        // get the index of the removed entity
        uint32_t removedIndex = indexOf(entity);

        // move the last entity (+ component) to the index of the removed entity
        m_entities[removedIndex] = lastEntity;
        m_components[removedIndex] = std::move(lastComponent);
        m_sparse[lastEntity] = removedIndex;

        // trim the last element off the dense vectors
        m_entities.pop_back();
        m_components.pop_back();
    }

    /**
     * Checks if the given entity is present in the sparse set.
     *
     * @param entity the entity to check for
     * @returns true if the entity is present, false otherwise
     */
    bool contains(Entity entity) const override {
        // key must be within range of possible keys
        if (entity >= m_sparse.size()) {
            return false;
        }

        uint32_t denseIndex = m_sparse[entity];

        // must point to value dense array entry
        if (denseIndex >= m_entities.size()) {
            return false;
        }

        return m_entities[denseIndex] == entity;
    }

    /**
     * Gets the index of the given entity in the dense vectors.
     *
     * @param entity the entity to get the index of, must be in the set
     * @returns the index of the entity in the dense vectors
     */
    uint32_t indexOf(Entity entity) {
        return m_sparse[entity];
    }

    /**
     * Gets the component at the given index in the dense vectors.
     *
     * @param index the index of the component, must be less than size()
     * @returns a reference to the component at the index
     */
    C& get(uint32_t index) {
        return m_components[index];
    }

    /**
     * @returns the number of entities in the sparse set
     */
    size_t size() {
        return m_entities.size();
    }

    /**
     * @returns a reference to the dense vector of entities
     */
    const std::vector<Entity>& entities() {
        return m_entities;
    }

    /**
     * @returns a reference to the sparse vector of entity indices in the dense vectors
     */
    const std::vector<uint32_t>& sparse() {
        return m_sparse;
    }

private:
    std::vector<uint32_t> m_sparse;
    std::vector<Entity> m_entities;
    std::vector<C> m_components;
};


#endif //OPENGL_RENDERER_SPARSESET_H
