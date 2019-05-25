#include "randomfactoryimpl.h"
#include "isaac/isaac64wrapper.h"

std::shared_ptr<Random> RandomFactoryImpl::isaac64() const
{
    return std::make_shared<Isaac64Wrappwer>();
}
