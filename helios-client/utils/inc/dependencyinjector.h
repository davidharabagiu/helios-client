#ifndef DEPENDENCYINJECTOR_H
#define DEPENDENCYINJECTOR_H

#include <memory>
#include <map>
#include <typeinfo>
#include <typeindex>

/**
 * @class DependencyInjector
 * @brief Register objects to be injected as dependencies
 */
class DependencyInjector
{
public:
    /**
     * @brief Constructor
     */
    DependencyInjector() = default;

    /**
     * @brief Destructor
     */
    ~DependencyInjector();

    /**
     * @brief Register an instance. Only one instance of a certain type may be registered.
     * @param instance - Instance to be registered
     * @tparam T - type of the instance
     * @return true if the instance was registered successfully, false if there is already an instance of type T
     */
    template <typename T>
    bool registerInstance(const std::shared_ptr<T>& instance);

    /**
     * @brief Replace an existing instance with a new one.
     * @param instance - Instance to be replaced
     * @tparam T - type of the instance
     * @return true if the instance was replaced successfully, false if there is no instance of type T
     */
    template <typename T>
    bool replaceInstance(const std::shared_ptr<T>& instance);

    /**
     * @brief Returns a registered instance.
     * @tparam T - type of the instance
     * @return The instance of the T. If there is no such instance, returns an invalid pointer.
     */
    template <typename T>
    std::shared_ptr<T> getInstance();

    /**
     * @brief Unregister all existing instances.
     */
    void unregisterAllInstances();

private:
    /**
     * @brief Collection of all instances
     */
    std::map<std::type_index, std::shared_ptr<void>> m_instances;
};

#include "dependencyinjector.tpp"

#endif  // DEPENDENCYINJECTOR_H
