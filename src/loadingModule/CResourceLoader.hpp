#ifndef C_RESOURCE_LOADER_H
#define C_RESOURCE_LOADER_H

#include <QThread>

/**
 * Base class for all the classes that will load resources.
 */
class CResourceLoader : public QThread
{
    Q_OBJECT

    void run() override;

  protected:
    virtual void LoadResource() = 0;
};

#endif // C_RESOURCE_LOADER_H
