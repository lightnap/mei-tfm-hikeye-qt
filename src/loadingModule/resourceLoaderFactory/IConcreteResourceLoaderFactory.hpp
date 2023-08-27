#ifndef I_CONCRETE_RESOURCE_LOADER_FACTORY_H
#define I_CONCRETE_RESOURCE_LOADER_FACTORY_H

#include "Types.hpp"

class CResourceLoader;

/**
 * @brief Interface of the Resource Loader factory.
 */
class IConcreteResourceLoaderFactory
{
  public:
    /**
     * Destructor.
     */
    virtual ~IConcreteResourceLoaderFactory() = default;

    /**
     * @brief Fabricates the resource loader.
     * @param aResource: Type of the resource loader.
     * @return The fabricated resource loader.
     */
    virtual CResourceLoader* MakeResourceLoader(Types::eResource aResource) = 0;
};

#endif // I_CONCRETE_RESOURCE_LOADER_FACTORY_H
