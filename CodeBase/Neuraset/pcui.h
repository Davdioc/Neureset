#ifndef PCUI_H
#define PCUI_H

#include "defs.h"
#include "databasemanager.h"

#include <QtWidgets>

class pcUI : public QWidget
{
    Q_OBJECT
public:
    explicit pcUI(QWidget *parent, DatabaseManager*);
    ~pcUI();

    //add widget pointer to pages that can be displayed
    void addEntry(QString, QWidget*);

    void runTest();
    void refreshData();


private:
    QString assetDirectory;

    DatabaseManager* database;

    QVector<QToolButton*> buttons;

    QVector<QWidget*> dataEntries;

    //Pointers to ui elements
    QVBoxLayout* blankLayout;

    //Display Elements
    QStackedWidget* displayArea;

    QWidget* sessionDisplayPage;
    QListWidget* sessionSelectList;
    QScrollArea* sessionSelectScroller;

    QWidget* dataDisplayPage;
    QStackedWidget* dataDisplaySwitcher;

    //Interactive Elements
    QToolButton* selectButton;
    QToolButton* backButton;

    //Functions
    void initUI();
    void initSignals();

    void openEntry(int);


    //Button Funcitons
    void upButtonPress();
    void downButtonPress();
    void selectButtonPress();
    void backButtonPress();

signals:

};

#endif // PCUI_H
