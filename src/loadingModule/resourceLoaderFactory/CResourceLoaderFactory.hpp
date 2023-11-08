#ifndef C_RESOURCE_LOADER_FACTORY_H
#define C_RESOURCE_LOADER_FACTORY_H

#include "common/Types.hpp"
#include "loadingModule/resourceLoaderFactory/IConcreteResourceLoaderFactory.hpp"

#include <memory>
#include <unordered_map>

class CDataManager;
class CResourceLoader;

/**
 * Resource loader factory.
 */
class CResourceLoaderFactory
{
  public:
    using tConcreteFactoryUnique = std::unique_ptr<IConcreteResourceLoaderFactory>; //!< Type for pointers to concrete resource loader factories.

  public:
    /**
     * @brief Creates an instance of the indicated resource loader.
     * @param aResourceType: Type of the resource loader we want to create.
     * @param aDataManager: Reference to the dataManager.
     * @return The created resource loader.
     */
    static CResourceLoader* Create(Types::eResource aResourceType, CDataManager& aDataManager);

    /**
     * Registers a factory for creating a new type of resource loader.
     * @param aResourceType Type of the resource we want to create.
     * @param apResourceLoaderFactory Factory we will use the create the resource loader.
     */
    static bool Register(Types::eResource aResourceType, tConcreteFactoryUnique apResourceLoaderFactory);

  private:
    using tResourceLoaderFactories = std::unordered_map<Types::eResource, tConcreteFactoryUnique>; //!< Type for avilable factories map.

    /**
     * @brief Obtain availavle factories.
     * @return Available factories.
     */
    static tResourceLoaderFactories& GetFactories();
};

/**
 * @brief Template for concrete factories.
 * @tparam tResourceLoader Type of the loader to fabricate.
 */
template<typename tResourceLoader>
class CConcreteResourceLoaderFactory : public IConcreteResourceLoaderFactory
{
  public:
    /**
     * @brief Registers the concrete factory to the resource loader facory.
     * @param aResource Type of the resource we want the factory to create.
     * @return True if correctly registered, false otherwise.
     */
    static bool Register(Types::eResource aResource) noexcept
    {
        return CResourceLoaderFactory::Register(aResource, std::make_unique<CConcreteResourceLoaderFactory<tResourceLoader>>());
    };

    /**
     * @brief Fabricates the action node.
     * @param aResource: Type of the resource loader.
     * @param aDataMager: Reference to the dataManager.
     * @return Pointer to the fabricated node.
     */
    CResourceLoader* MakeResourceLoader(Types::eResource aResource, CDataManager& aDataManager) override { return new tResourceLoader(aResource, aDataManager); }
};

#endif // C_RESOURCE_LOADER_FACTORY_H
