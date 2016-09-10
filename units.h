#ifndef UNITS_H
#define UNITS_H

#include<QWidget>
#include<QLabel>
#include<QDialog>
#include<QLineEdit>
#include<QApplication>
#include<QPushButton>
#include<QMainWindow>
#include<QMessageBox>
#include<QDebug>
#include<QtGui>
#include<QLayout>
#include<QCheckBox>

#define NotSet -1
//#define Android

#ifndef Android
#include<QDesktopWidget>
#endif

struct unit
{
    int start;
    int end;
    int dayOfWeek;
    QString name;
    int code;
    QString master;
};


//**********************************************************

enum week
{
    sat,
    sun,
    mon,
    tus,
    wen,
};

//**********************************************************

struct element
{
    QLineEdit * text;
    QLabel * label;
};

//**********************************************************

struct myTime {
    int hour;
    int min;
};

//**********************************************************

struct dayUnit
{
    unit lesson;
    bool isFull;
    bool alocated;
    myTime dayTime;
    QLabel* label;
};

//**********************************************************

class startDialog;

class units: public QMainWindow
{
    Q_OBJECT

    dayUnit** dayTime;
    QPushButton* addButton;
    QPushButton* exit;
    QHBoxLayout** labelLayout;
    QRect* screenSize;
    bool theme;
public:
    friend void clearMyDirtyWork(int, int, int , units*);
    units(startDialog *start , QWidget * parent = nullptr);
    void build();
private slots:
    void help(bool start);
    void add(QString,QString,myTime,myTime,int,int);
    void addClicked();
    void update(int = -1);
signals:
    void mainWindowStarted();
    void itemAdded(int = 0);
    void addClick();
};

//**********************************************************

class AddDialog : public QDialog
{
    Q_OBJECT
    element name;
    element master;
    element startTime;
    element endTime;
    element code;
    element firstDay;
    element lastDay;
    QLabel * hint;
    QPushButton * ok;
    QPushButton * cancel;
public:
    AddDialog(QWidget *parent = nullptr);
    friend int detectDay(QString , AddDialog*);
signals:
    void add(const QString, const QString, myTime, myTime ,int, int);
private slots:
    void addMe();
    void activeButton(const QString);
};

//**********************************************************

class startDialog : public QDialog
{
    Q_OBJECT
    QCheckBox* classic;
    QCheckBox* modern;
    QLabel* label;
    QPushButton* done;
    QPushButton* cancel;
    friend class units;
public:
    startDialog(QApplication* parent = nullptr);
    bool isdone;
private slots:
    void enableButton(bool);
    void selected();
signals:
    void select(bool);
};

#endif // UNITS_H
