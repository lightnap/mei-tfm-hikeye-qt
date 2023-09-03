#ifndef C_DATA_MANAGER_H
#define C_DATA_MANAGER_H

#include <QMutex>
#include <QString>

/**
 * @brief Class that stores and manages all data from the various areas.
 */
class CDataManager
{
  public:
    /**
     * Constructor.
     */
    CDataManager();

    /**
     * @brief Sets the folder path from where we will store/load all area files.
     * @param aFolderPath: Path for the folder we want to set.
     */
    void SetFolderPath(QString aFolderPath);

    /**
     * @brief Gets the folder path in which we store/load all area files.
     * @return Path for the folder.
     */
    QString GetFolderPath() const;

  private:
    QString mDataFolderPath; //!< Folder path in which we store/load all area files.
};
#endif // C_DATA_MANAGER_H
