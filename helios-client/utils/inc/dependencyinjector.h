#ifndef DEPENDENCYINJECTOR_H
#define DEPENDENCYINJECTOR_H

#include <memory>
#include <map>
#include <typeinfo>
#include <typeindex>

class DependencyInjector
{
public:
    DependencyInjector() = default;

    ~DependencyInjector();

    template <typename T>
    bool registerInstance(const std::shared_ptr<T>& instance);

    template <typename T>
    std::shared_ptr<T> getInstance();

    void unregisterAllInstances();

private:
    std::map<std::type_index, std::shared_ptr<void>> m_instances;
};

#include "dependencyinjector.tpp"

#endif  // DEPENDENCYINJECTOR_H
