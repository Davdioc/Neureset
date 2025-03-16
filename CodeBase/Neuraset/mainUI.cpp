#include "mainUI.h"
#include "ui_mainUI.h"

MainUI::MainUI(QWidget* parent, DatabaseManager* db): QMainWindow(nullptr){

    database = db;

    //Get directory of the Assets folder in Neuraset
    AssetDir = QDir(QCoreApplication::applicationDirPath()).filePath("../Neuraset/Assets/");

    ui = new Ui::MainUI;
    ui->setupUi(this);

    poweredOn = false;

    initUI();
    connectSignals();

    //Dev Functions
    loadSessionLog();
}

MainUI::~MainUI(){
    delete ui;
}

void MainUI::initUI(){
    this->setWindowTitle("Main Control Panel");

    //SETTING MEMBER POINTERS *FOR READABILITY PURPOSES
    //Set pointers to buttons
    playButton = ui->startButton;
    pauseButton = ui->pauseButton;
    stopButton = ui->stopButton;
    powerButton = ui->powerButton;
    menuButton = ui->menuButton;
    menuUpButton = ui->menuNavUpButton;
    menuDownButton = ui->menuNavDownButton;

    //Buttons on screen
    dateTimeButton = ui->setTimeButton;

    //Set pointers to lights
    contactLight = ui->contactLight;
    lostContactLight = ui->lostContactLight;
    treatmentLight = ui->treatmentLight;

    //Set Pointers to ui Display elements
    treatmentBar = ui->treatmentBar;
    chargeBar = ui->chargeBar;
    lcdDisplay = ui->timeDisplay;

    //Set pointers to session log elements
    sessionLogList = ui->sessionListWidget;
    sessionLogList->setStyleSheet("background-color: #ffffe6");
    ui->SessionLabel->setStyleSheet("background-color: white;");

    //Init screen widgets and stacked widget indexing
    displayWidget = ui->displayWidget;//widget used to store and swap between display pages

    powerOffPage = ui->deviceOffPage;
    sessionDisplayPage = ui->sessionPage;
    menuDisplayPage = ui->menuPage;
    sessionLogPage = ui->sessionLogPage;
    setDateAndTimePage = ui->dateAndTimePage;

    //Add pointers to appropriate QLists
    //Display Pages added to page list
    displayPages.push_back(sessionDisplayPage);
    displayPages.push_back(sessionLogPage);
    displayPages.push_back(setDateAndTimePage);

    //menu frames
    menuFrames.push_back(ui->newSessionFrame);
    menuFrames.push_back(ui->sessionLogFrame);
    menuFrames.push_back(ui->timeAndDateFrame);

    //light frames
    lightFrames.push_back(contactLight);
    lightFrames.push_back(lostContactLight);
    lightFrames.push_back(treatmentLight);

    //Set UI Icons
    playButton->setIcon(QIcon(AssetDir+"play.png"));
    pauseButton->setIcon(QIcon(AssetDir+"pause.png"));
    stopButton->setIcon(QIcon(AssetDir+"stop.png"));
    powerButton->setIcon(QIcon(AssetDir+"power.png"));
    menuButton->setIcon(QIcon(AssetDir+"menu.png"));

    ui->chargeIcon->setPixmap(QPixmap(AssetDir+"charge.png"));
    ui->chargeIcon->setScaledContents(true);

    //Adjust SyleSheets
    treatmentBar->setStyleSheet("background-color: #ffffe6");
    lcdDisplay->setStyleSheet("background-color: #ffffe6");

    //Init Stylesheet presets
    litFrameStylesheet = "QFrame { background-color: #ffffb3 }";
    unlitFrameStylesheet = "QFrame { background-color: white }";

    screenOnStylesheet = "QFrame { background-color: #e6ffff }";
    screenOffStylesheet = "QFrame { background-color: gray }";

    chargeHighStylesheet =  "QProgressBar { "
                                "border: 2px solid black;"
                                "border-radius: 5px; background-color: white; }"
                            "QProgressBar::chunk { "
                                "background-color: limeGreen; }";

    chargeMedStylesheet = "QProgressBar { "
                            "border: 2px solid black;"
                            "border-radius: 5px; background-color: white; }"
                          "QProgressBar::chunk { "
                             "background-color: orange; }";

    chargeLowStylesheet = "QProgressBar { "
                            "border: 2px solid black;"
                            "border-radius: 5px; background-color: white; }"
                          "QProgressBar::chunk { "
                             "background-color: orangeRed; }";

    chargeOffStylesheet = "QProgressBar { "
                            "border: 2px solid black;"
                            "border-radius: 5px; background-color: gray; }"
                          "QProgressBar::chunk { "
                             "background-color: gray; }";

    //Set UI elements to starting state
    powerOff();
}

void MainUI::connectSignals(){
    connect(playButton, &QToolButton::clicked, this, &MainUI::playButtonClicked);
    connect(pauseButton, &QToolButton::clicked, this, &MainUI::pauseButtonClicked);
    connect(stopButton, &QToolButton::clicked, this, &MainUI::stopButtonClicked);
    connect(powerButton, &QToolButton::clicked, this, &MainUI::powerButtonClicked);
    connect(menuButton, &QToolButton::clicked, this, &MainUI::menuButtonPressed);
    connect(dateTimeButton, &QPushButton::clicked, this, &MainUI::timeButtonClicked);

    connect(menuUpButton, &QToolButton::clicked, this, &MainUI::navigateUp);
    connect(menuDownButton, &QToolButton::clicked, this, &MainUI::navigateDown);

}

void MainUI::loadSessionLog(){
    sessionLogList->clear();
    QList<QDateTime> data = database->getDateTimeValues();

    for(QDateTime datapoint : data){
        sessionLogList->addItem(datapoint.toString());
    }

    sessionLogList->setCurrentRow(0);
}

//=====UI INPUT CONTROL FUNCITONS=====

void MainUI::navigateUp() {
    if(!poweredOn) return;//if device not on, do nothing

    bool onDisplayPage = displayWidget->currentWidget() == menuDisplayPage;//if currently on menu screen
    bool onSessionLogPage = displayWidget->currentWidget() == sessionLogPage;

    int sessionLogCurrentRow = sessionLogList->currentRow();

    if(onDisplayPage && selectedMenuFrame == -1){//if in menu and no frame selected, select bottom frame
        selectFrame(menuFrames.size()-1);
    } else if(onDisplayPage && selectedMenuFrame > 0){//if not selecting top frame, select next frame up
        selectedMenuFrame--;
        selectFrame(selectedMenuFrame);
    } else if(onSessionLogPage && sessionLogCurrentRow > 0){
        sessionLogList->setCurrentRow(sessionLogCurrentRow-1);
    }
}

void MainUI::navigateDown() {
    if(!poweredOn) return;//if device not on, do nothing

    bool onDisplayPage = displayWidget->currentWidget() == menuDisplayPage;//if currently on menu screen
    bool onSessionLogPage = displayWidget->currentWidget() == sessionLogPage;

    int sessionLogCurrentRow = sessionLogList->currentRow();

    if(onDisplayPage && selectedMenuFrame == -1){//if in menu and no frame selected, select Top frame
        selectFrame(0);
    } else if(onDisplayPage && selectedMenuFrame < menuFrames.size()-1){//if not selecting top frame, select next frame up
        selectedMenuFrame++;
        selectFrame(selectedMenuFrame);
    } else if(onSessionLogPage && sessionLogCurrentRow < sessionLogList->count()-1){
        sessionLogList->setCurrentRow(sessionLogCurrentRow+1);
    }
}

void MainUI::selectFrame(int indx){
    if(indx < 0 || indx > menuFrames.size()){//verify that the provided index is within range and has a frame assosiated with it
        qDebug()<<"ERROR: selected frame is unavaliable or frame index is invalid";
    } else {
        selectedMenuFrame = indx;
        for(int i=0;i<menuFrames.size();i++){
            if(i == selectedMenuFrame){//if this frame is the one selected, hightlight it, otherwise do not
                menuFrames[i]->setStyleSheet(litFrameStylesheet);
            } else {
                menuFrames[i]->setStyleSheet(unlitFrameStylesheet);
            }
        }
    }
}

void MainUI::menuButtonPressed(){
    if(!poweredOn) return;//if device not on, do nothing

    if(displayWidget->currentWidget() != menuDisplayPage){//if not currently on menu screen navigate to it
        displayWidget->setCurrentWidget(menuDisplayPage);
        resetMenu();
    } else {//if on menu page, and a frame is selected, and the frame has a valid page, switch to the selected page
        if(selectedMenuFrame > displayPages.size()-1 || selectedMenuFrame < 0){ //verify that the selected frame has a page to navigate to
            qDebug()<<"ERROR: Selected destination does not have a page to display";
        } else {
            //if setting to session log page, load the newest date/time log
            if(displayPages[selectedMenuFrame] == sessionLogPage) loadSessionLog();
            displayWidget->setCurrentWidget(displayPages[selectedMenuFrame]); //navigate to selected page
        }
    }
}

void MainUI::resetMenu(){//reset menu to default state
    for(QFrame* frame : menuFrames){
        frame->setStyleSheet(unlitFrameStylesheet);
    }
    selectedMenuFrame = -1;
}

void MainUI::powerOn(){
    poweredOn = true;
    displayWidget->setCurrentWidget(menuDisplayPage);
    ui->screenFrame->setStyleSheet(screenOnStylesheet);
    setChargeBar(100);
    ui->chargeIcon->show();
    setTreatmentBar(0);
    setTimeLCD("00:00");
}

void MainUI::powerOn(int charge){
    poweredOn = true;
    displayWidget->setCurrentWidget(menuDisplayPage);
    ui->screenFrame->setStyleSheet(screenOnStylesheet);
    setChargeBar(charge);
    ui->chargeIcon->show();
    setTreatmentBar(0);
    setTimeLCD("00:00");
}

void MainUI::powerOff(){
    poweredOn = false;
    resetMenu();
    displayWidget->setCurrentWidget(powerOffPage);
    ui->screenFrame->setStyleSheet(screenOffStylesheet);
    setChargeBar(0);
    ui->chargeIcon->hide();
    chargeBar->setStyleSheet(chargeOffStylesheet);


    //turn off all lights if on
    for(QFrame* light : lightFrames) if(light->styleSheet() != "") light->setStyleSheet("");
}

//functions for Progres Bar control
void MainUI::setTreatmentBar(int percent){
    if(percent<=100 && percent>=0){
        treatmentBar->setValue(percent);
    } else if(SYSTEM_OUTPUTS) qDebug()<<"ERROR: Attempting to set treatment progress to invalid value";
}

void MainUI::setChargeBar(int percent){
    if(percent<=100 && percent>=0){
        chargeBar->setValue(percent);

        //Adjust color of charge bar to reflect charge of device
        if(percent <= 15) chargeBar->setStyleSheet(chargeLowStylesheet);
        else if(percent <= 30) chargeBar->setStyleSheet(chargeMedStylesheet);
        else chargeBar->setStyleSheet(chargeHighStylesheet);

    } else if(SYSTEM_OUTPUTS) qDebug()<<"ERROR: Attempting to set charge to invalid value";
}

void MainUI::setTimeLCD(string value){
    lcdDisplay->display(QString::fromStdString(value));
}

//Functions for Light Control
void MainUI::toggleContactLight(){
    if(contactLightLit()){
        contactLight->setStyleSheet("");
    } else contactLight->setStyleSheet("background-color: aqua;");
}

void MainUI::toggleLostContactLight(){
    if(lostContactLightLit()){
        lostContactLight->setStyleSheet("");
    } else lostContactLight->setStyleSheet("background-color: lightCoral;");
}

void MainUI::toggleTreatmentLight(){
    if(treatmentLightLit()){
        treatmentLight->setStyleSheet("");
    } else treatmentLight->setStyleSheet("background-color: lightGreen;");
}

//Boolean Return
bool MainUI::isReadyForSession(){
    if(displayWidget->currentWidget() == sessionDisplayPage && poweredOn) return true;
    else return false;
}

void MainUI::setWaveformGenerator(EEGWaveformGenerator* generator) {
    qDebug() << "Setting waveform generator";
    waveformGenerator = generator;
    QVBoxLayout *layout = new QVBoxLayout();
    ui->plotWidgetContainer->setLayout(layout);
    ui->plotWidgetContainer->layout()->addWidget(waveformGenerator);
}

QDateTime MainUI::getDateTimeInput(){
    return ui->dateTimeEdit->dateTime();
}
















