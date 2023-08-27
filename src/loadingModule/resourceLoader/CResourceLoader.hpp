#ifndef C_RESOURCE_LOADER_H
#define C_RESOURCE_LOADER_H

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
     */
    CResourceLoader(Types::eResource aResouce);

    /**
     * Gets called when this thread is started.
     */
    void run() override;

  public slots:
    /**
     * @brief Slot to launch the load of the corresponding resource.
     * @param aResource: Type of resource we want to load.
     */
    void LoadResourceSlot(Types::eResource aResource);

  signals:
    /**
     * @brief Emitted when we want to notify errors in the loading.
     * @param aErrorCode: Code depicting errors on the loading.
     */
    void SendErrorCode(int aErrorCode);

  protected:
    /**
     * @brief Loads the corresponding resource.
     */
    virtual void LoadResource();

  private:
    Types::eResource mResourceType; //!< Type of resource this loader loads.
};
#endif // C_RESOURCE_LOADER_H
