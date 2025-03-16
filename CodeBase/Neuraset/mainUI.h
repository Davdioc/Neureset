#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>
#include <QtWidgets>
#include <QObject>
#include "defs.h"
#include "eegwaveformgenerator.h"
#include "databasemanager.h"

//Testing Changes

namespace Ui { class MainUI; }

class MainUI : public QMainWindow {
    Q_OBJECT
public:
    MainUI(QWidget *parent, DatabaseManager*);
    ~MainUI();

    //Ui Element Control Functions

    //Progress Bar Displays
    void setTreatmentBar(int percent);
    void setChargeBar(int percent);
    void setTimeLCD(string);

    //Light Control Functions
    void toggleContactLight();
    void toggleLostContactLight();
    void toggleTreatmentLight();

    //Boolean Returns
    inline bool contactLightLit(){return contactLight->styleSheet()=="background-color: aqua;";}
    inline bool lostContactLightLit(){return lostContactLight->styleSheet()=="background-color: lightCoral;";}
    inline bool treatmentLightLit(){return treatmentLight->styleSheet()=="background-color: lightGreen;";}

    bool isReadyForSession();
    inline bool isON(){return poweredOn;}

    //Menu Interaction Related Functions
    void navigateUp();//function called when up button pressed on UI
    void navigateDown();//function called when down button pressed on UI
    void menuButtonPressed();//respond to a menu button press

    //power on and power off
    void powerOn();
    void powerOn(int charge);
    void powerOff();

    //Value Return Functions
    inline int getCharge() { return chargeBar->value(); }
    inline int getTreatmentProgress() {return treatmentBar->value();}

    void setWaveformGenerator(EEGWaveformGenerator* generator);
    QDateTime getDateTimeInput();

private:
    Ui::MainUI *ui;
    EEGWaveformGenerator* waveformGenerator;
    DatabaseManager* database;
    QString AssetDir;//Filepath to Assets folder

    //Button Ptrs
    QToolButton* playButton;
    QToolButton* pauseButton;
    QToolButton* stopButton;
    QToolButton* powerButton;
    QToolButton* menuButton;
    QPushButton* dateTimeButton;

    QToolButton* menuUpButton;
    QToolButton* menuDownButton;

    //Light Element Ptrs
    QFrame* contactLight;
    QFrame* lostContactLight;
    QFrame* treatmentLight;

    //Ui Display Element Ptrs
    QProgressBar* treatmentBar;
    QProgressBar* chargeBar;
    QLCDNumber* lcdDisplay;
    QStackedWidget* displayWidget;
    QListWidget* sessionLogList;

    //Pages that the user can navigate to with up and down
    QWidget* powerOffPage;
    QWidget* sessionDisplayPage;
    QWidget* menuDisplayPage;
    QWidget* sessionLogPage;
    QWidget* setDateAndTimePage;

    //Pointer Lists for easy organization
    QList<QWidget*> displayPages;//list of pointers to possible display pages
    QList<QFrame*> menuFrames;//list of pointers to frames on the menu page
    QList<QFrame*> lightFrames;

    //Stylesheet presets
    QString litFrameStylesheet;
    QString unlitFrameStylesheet;

    QString screenOffStylesheet;
    QString screenOnStylesheet;

    QString chargeHighStylesheet;
    QString chargeMedStylesheet;
    QString chargeLowStylesheet;
    QString chargeOffStylesheet;

    //UI control variables/functions
    int selectedMenuFrame;//denotes the currently selected frame on the menu
    void selectFrame(int);

    //Private Functions
    void initUI();
    void connectSignals();
    void resetMenu();

    void loadSessionLog();

    //misc booleans
    bool readyForSession;
    bool poweredOn;

signals:
    void playButtonClicked();
    void pauseButtonClicked();
    void stopButtonClicked();
    void powerButtonClicked();
    void menuButtonClicked();
    void timeButtonClicked();
};
#endif // MAINUI_H
