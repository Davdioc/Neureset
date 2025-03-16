#include "devpanel.h"

DevPanel::DevPanel(MainUI* mainUI, pcUI* pcUi)
{
    this-> mainUI = mainUI;
    this-> pcUi = pcUi;
    initUI();
}

void DevPanel::initUI(){
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setWindowTitle("Dev Panel");

    //Init buttons
    powerResetBtn = new QPushButton(this);
    powerResetBtn->setText("Recharge Battery");

    headsetConnectionBtn = new QPushButton(this);
    headsetConnectionBtn->setText("Toggle Headset Connection");

    openPcUiBtn = new QPushButton(this);
    openPcUiBtn->setText("Open PC UI");


    //add buttons to layout
    mainLayout->addWidget(powerResetBtn);
    mainLayout->addWidget(headsetConnectionBtn);
    mainLayout->addWidget(openPcUiBtn);

    //connect signals
    connect(openPcUiBtn, &QPushButton::clicked, this, &DevPanel::openPCUI);
    connect(powerResetBtn, &QPushButton::clicked, this, &DevPanel::powerResetButtonPressed);
    connect(headsetConnectionBtn, &QPushButton::clicked, this, &DevPanel::headsetButtonPressed);
}
