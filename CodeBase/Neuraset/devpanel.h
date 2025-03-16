#ifndef DEVPANEL_H
#define DEVPANEL_H

#include "defs.h"
#include "mainUI.h"
#include "pcui.h"
#include <QWidget>

class DevPanel : public QWidget{

    Q_OBJECT

public:
    DevPanel(MainUI*, pcUI*);


private:
    MainUI* mainUI;
    pcUI* pcUi;

    void initUI();
    inline void openPCUI(){pcUi->show();}

    //UI Element pointers
    QPushButton* powerResetBtn;
    QPushButton* headsetConnectionBtn;
    QPushButton* openPcUiBtn;

signals:
    void powerResetButtonPressed();
    void headsetButtonPressed();

};

#endif // DEVPANEL_H
