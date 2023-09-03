#ifndef C_RESOURCE_LOADER_H
#define C_RESOURCE_LOADER_H

#include "CDataManager.hpp"

#include "Types.hpp"

#include <QThread>

/**
 * Base class for all the classes that will load resources.
 */
class CResourceLoader : public QThread
{
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param aResource: Type of the resource this loader will load.
     * @param aDataManager: Reference to the data manager.
     */
    CResourceLoader(Types::eResource aResouce, CDataManager& aDataManager);

    /**
     * @brief Gets called when this thread is started.
     */
    void run() override;

  signals:
    /**
     * @brief Emitted when resource has finished loading.
     * @param aErrorCode: Code depicting loading result.
     */
    void ResourceLoadedSignal(int aErrorCode);

  protected:
    /**
     * @brief Loads the corresponding resource.
     * @return Code depicting the loading result.
     */
    virtual Types::eResourceLoadingError LoadResource();

  protected:
    CDataManager& mDataManager; //!< Reference to the datamanager.

  private:
    Types::eResource mResourceType; //!< Type of resource this loader loads.
};
#endif // C_RESOURCE_LOADER_H
