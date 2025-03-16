#include "pcui.h"

pcUI::pcUI(QWidget *parent, DatabaseManager* db)
    : QWidget{parent}
{
    assetDirectory = QDir(QCoreApplication::applicationDirPath()).filePath("../Neuraset/Assets/");

    database = db;

    initUI();
    initSignals();
    refreshData();
}

pcUI::~pcUI(){
    for(QWidget* data : dataEntries){
        delete data;
    }
    dataEntries.clear();
}

void pcUI::initUI(){
    //Init Layouts
    setWindowTitle("PC");
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    //Init Buttons
    selectButton = new QToolButton(this);
    selectButton->setIcon(QIcon(assetDirectory + "select.png"));
    buttons.push_back(selectButton);

    backButton = new QToolButton(this);
    backButton->setIcon(QIcon(assetDirectory + "back.png"));
    buttons.push_back(backButton);

    //Init Session Select Screen
    sessionDisplayPage = new QWidget(this);

    sessionSelectList = new QListWidget(sessionDisplayPage);
    sessionSelectScroller = new QScrollArea(this);

    sessionSelectScroller->setWidgetResizable(true);
    sessionSelectScroller->setWidget(sessionSelectList);

    sessionDisplayPage->setLayout(new QVBoxLayout(sessionDisplayPage));
    sessionDisplayPage->layout()->addWidget(sessionSelectScroller);

    //Init Data View Screen
    dataDisplayPage = new QWidget(this);
    dataDisplaySwitcher = new QStackedWidget(dataDisplayPage);

    //Init Display
    displayArea = new QStackedWidget(this);
    displayArea->setFixedSize(400,300);

    displayArea->addWidget(sessionDisplayPage);
    displayArea->addWidget(dataDisplayPage);

    //Init button Placement
    QFrame *buttonFrame = new QFrame(this);
    buttonFrame->setFixedSize(50,200);

    QVBoxLayout* buttonLayout = new QVBoxLayout(buttonFrame);

    for(QToolButton* button : buttons){
        button->setFixedSize(40,40);
        buttonLayout->addWidget(button);
    }

    //Deploy Layouts
    mainLayout->addWidget(displayArea);
    mainLayout->addWidget(buttonFrame);

    setLayout(mainLayout);
}

void pcUI::initSignals(){
    connect(selectButton, &QToolButton::clicked, this, &pcUI::selectButtonPress);
    connect(backButton, &QToolButton::clicked, this, &pcUI::backButtonPress);
}

void pcUI::addEntry(QString displayText, QWidget* dataDisplay){
    sessionSelectList->addItem(QString(displayText));
    dataEntries.append(dataDisplay);
    dataDisplaySwitcher->addWidget(dataDisplay);
}

void pcUI::openEntry(int indx){
    dataDisplaySwitcher->setCurrentWidget(dataEntries[indx]);
    displayArea->setCurrentWidget(dataDisplayPage);
}

void pcUI::refreshData(){
    //Nuke all data
    int count = 0;
    for(QWidget* widget : dataEntries){
        dataDisplaySwitcher->removeWidget(widget);
        delete widget;
    }
    sessionSelectList->clear();
    dataEntries.clear();

    //Repopulate data
    QList<QVariantMap> newData = database->getSessionData();

    for(QVariantMap map : newData){
        QWidget* newWidget = new QWidget(nullptr);

        QString datetimeStr = map["datetime"].toDate().toString("yyyy-MM-dd"); // Convert QDate to QString
        QString sessionInfoStr = map["session_info"].toString(); // Already a QString

        QVBoxLayout* displayLayout = new QVBoxLayout(newWidget);
        displayLayout->addWidget(new QLabel("Session Date & Time: "+datetimeStr));
        displayLayout->addWidget(new QLabel("Session Data: "+sessionInfoStr));

        addEntry(datetimeStr, newWidget);
    }
}

//BUTTON PRESS FUNCTIONS

void pcUI::selectButtonPress(){
    if(displayArea->currentWidget() == sessionDisplayPage && sessionSelectList->currentRow()>=0){
        int selectedIndx = sessionSelectList->currentRow();
        openEntry(selectedIndx);
    }
}

void pcUI::backButtonPress(){
    displayArea->setCurrentWidget(sessionDisplayPage);
    refreshData();
}

void pcUI::upButtonPress(){
    if(displayArea->currentWidget() == sessionDisplayPage){

    }
}

void pcUI::downButtonPress(){
    if(displayArea->currentWidget() == sessionDisplayPage){

    }
}


void pcUI::runTest(){
    for(int i=0;i<30;i++){
        QWidget* widget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(widget);
        layout->addWidget(new QLabel(QString("This widget is displaying data for entry %1").arg(i)));
        addEntry(QString("Entry %1").arg(i), widget);
    }
}



