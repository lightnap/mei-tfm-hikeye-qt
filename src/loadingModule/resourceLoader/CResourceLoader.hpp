#ifndef C_RESOURCE_LOADER_H
#define C_RESOURCE_LOADER_H

#include "CDataManager.hpp"

#include "Types.hpp"

#include <QString>
#include <QThread>

class QFile;

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
     * @param aErrorMessage: A brief message describing the error.
     */
    void ResourceLoadedSignal(int aErrorCode, QString aErrorMessage);

  protected:
    /**
     * @brief Loads the corresponding resource.
     * @return Code depicting the loading result.
     */
    virtual Types::eLoadResult LoadResource();

    /**
     * @brief Opens a resource file, giving appropiate message if it fails.
     * @param aFileName: Name of the file we want to open.
     * @param aFile: Place where we store the opened file.
     * @return Error code depicting if loading was sucessful.
     */
    Types::eLoadResult OpenFile(QString& aFileName, QFile& aFile);

    /**
     * @brief Appends name of the file to path of the area data folder.
     * @param aFileName: The name of the file.
     * @retrun The path of the file in the area data folder.
     */
    QString GetResourceFilePath(QString& aFileName) const;

  protected:
    CDataManager& mDataManager;      //!< Reference to the datamanager.
    QString       mLoadErrorMessage; //!< Stores messages to be displayed when loading errors occur.

  private:
    Types::eResource mResourceType; //!< Type of resource this loader loads.
};
#endif // C_RESOURCE_LOADER_H
