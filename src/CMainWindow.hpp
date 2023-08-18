#ifndef C_MAIN_WINDOW_H
#define C_MAIN_WINDOW_H

#include "ui_CMainWindow.h"

/**
 * Class that represents the main window of our GUI application.
 */
class CMainWindow : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param aParent Parent widget in hierarchy.
     */
    CMainWindow(QWidget* aParent = 0);

  private:
    Ui::CMainWindow mUi; //!< Represents the Ui of this form.
};

#endif // C_MAIN_WINDOW_H
