#include "dependencyinjector.h"

template <typename T>
bool DependencyInjector::registerInstance(const std::shared_ptr<T>& instance)
{
    return m_instances.emplace(std::type_index(typeid(T)), instance).second;
}

template <typename T>
std::shared_ptr<T> DependencyInjector::getInstance()
{
    auto instance = m_instances.find(std::type_index(typeid(T)));
    return instance != m_instances.end() ? std::static_pointer_cast<T>(instance->second) : std::shared_ptr<T>();
}
