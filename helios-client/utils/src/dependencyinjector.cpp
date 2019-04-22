#include "dependencyinjector.h"

DependencyInjector::~DependencyInjector()
{
    unregisterAllInstances();
}

void DependencyInjector::unregisterAllInstances()
{
    m_instances.clear();
}
