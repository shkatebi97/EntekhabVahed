#include "units.h"

//**********************************************************

inline char * QStringToChar(QString);

inline int timeCompare(myTime , myTime);// -1 when first is more little

inline void clearMyDirtyWork(int, int, int , units *);

myTime toTime(QString);

int detectDay(QString , AddDialog*);

//**********************************************************

units::units(startDialog* start, QWidget *parent ):QMainWindow(parent)
{
    connect(start , SIGNAL(select(bool)) , this , SLOT(help(bool)));
}

void units::help(bool start )
{
    theme = start;
    build();
    this->show();
}

void units::build()
{
#ifndef Android
    screenSize = new QRect(QApplication::desktop()->screenGeometry());
#else
    screenSize = new QRect(0, 0, 640, 480) ;
#endif
    if(theme)
    {
        QWidget* parent = parentWidget();
        this->setStyleSheet("background-color : black");
        dayTime = new dayUnit*[5];
        QHBoxLayout* structureHLayout;
        structureHLayout = new QHBoxLayout(parent);
        structureHLayout->setSpacing(2);
        for(int i=0;i<19;i++)
        {
            QLabel* label;
            QString temp;
            if (i != 0)
            {
                temp += QString::number(8+((i-1)/2));
                temp += ":";
                (!((i)%2)) ? (temp += "30") : (temp += "00");
                temp += " - ";
                temp += QString::number(8+(i/2));
                temp += ":";
                (((i)%2)) ? (temp += "30") : (temp += "00");
                label = new QLabel(temp, parent);
                label->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
                label->setContentsMargins(3,3,3,3);
            }
            else
            {
                temp += "روز\n\\\\\nساعت";
                label = new QLabel(temp, parent);
                label->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
                label->setContentsMargins(1,1,1,1);
            }
            label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            label->setStyleSheet("background-color : white");
            label->setWordWrap(true);
            structureHLayout->addWidget(label);
        }
        QVBoxLayout * allLabelVLayout = new QVBoxLayout(parent);
        allLabelVLayout->setSpacing(2);
        allLabelVLayout->addLayout(structureHLayout);
        labelLayout = new QHBoxLayout*[5];
        for (int i = 0; i < 5; i++)
        {

            labelLayout[i] = new QHBoxLayout(parent);
            labelLayout[i]->setSpacing(2);
            QLabel* rightLabel;
            if (i == sat)
            {
                rightLabel = new QLabel("شنبه ", parent);
                rightLabel->setContentsMargins(1,1,1,1);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
                rightLabel->setStyleSheet("background-color : white");
            }
            else if(i == sun)
            {
                rightLabel = new QLabel("یکشنبه ", parent);
                rightLabel->setContentsMargins(1,1,1,1);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
                rightLabel->setStyleSheet("background-color : white");
            }
            else if(i == mon)
            {
                rightLabel = new QLabel("دوشنبه ", parent);
                rightLabel->setContentsMargins(1,1,1,1);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
                rightLabel->setStyleSheet("background-color : white");
            }
            else if(i == tus)
            {
                rightLabel = new QLabel("سه شنبه ", parent);
                rightLabel->setContentsMargins(1,1,1,1);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
                rightLabel->setStyleSheet("background-color : white");
            }
            else if(i == wen)
            {
                rightLabel = new QLabel("چهار شنبه ", parent);
                rightLabel->setContentsMargins(1,1,1,1);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
                rightLabel->setStyleSheet("background-color : white");
            }
            labelLayout[i]->addWidget(rightLabel);
            dayTime[i] = new dayUnit[18];
            for(int j=0;j<18;j++)
            {
                dayTime[i][j].dayTime.hour = 8+(j/2);
                dayTime[i][j].dayTime.min = 30*(j%2);
                dayTime[i][j].isFull = false;
                dayTime[i][j].alocated = false;
                //dayTime[i][j].label = new QLabel(parent);
                //dayTime[i][j].label->setVisible(false);
                //labelLayout[i]->addWidget(dayTime[i][j].label);
            }
            allLabelVLayout->addLayout(labelLayout[i]);
            allLabelVLayout->setAlignment(labelLayout[i] , Qt::AlignLeft);
        }
        QVBoxLayout* buttonLayout = new QVBoxLayout(parent);
        addButton = new QPushButton("اضافه کردن\nدرس" , parent);
        addButton->setDefault(true);
        addButton->setStyleSheet("background-color : white");
        addButton->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
        about = new QPushButton("درباره سازنده" , parent);
        about->setStyleSheet("background-color : white");
        about->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
        exit = new QPushButton("خروج");
        exit->setStyleSheet("background-color : white");
        exit->setFixedSize(screenSize->width() / 25 , screenSize->height() / 10);
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(about);
        buttonLayout->addWidget(exit);
        buttonLayout->addStretch();
        QHBoxLayout* mainLayout = new QHBoxLayout(parent);
        mainLayout->addLayout(allLabelVLayout);
        mainLayout->addLayout(buttonLayout);
        mainLayout->setAlignment(buttonLayout , Qt::AlignHCenter | Qt::AlignVCenter);
        mainLayout->setSpacing(2);
        QWidget* window = new QWidget;
        window->setLayout(mainLayout);
        setCentralWidget(window);
        connect(exit  , SIGNAL(clicked(bool)) , this , SLOT(close()));
        connect(addButton , SIGNAL(clicked(bool)) , this , SLOT(addClicked()));
        connect(about , SIGNAL(clicked(bool)) , this , SLOT(aboutF()));
        connect(this , SIGNAL(itemAdded(int)) , this , SLOT(update(int)));
        connect(this , SIGNAL(mainWindowStarted()) , this , SLOT(update()));
        emit mainWindowStarted();
    }
    else
    {
        QWidget* parent = parentWidget();
        this->setStyleSheet("background-color : black");
        dayTime = new dayUnit*[5];
        QHBoxLayout* structureHLayout;
        structureHLayout = new QHBoxLayout(parent);
        for(int i=0;i<19;i++)
        {
            QLabel* label;
            QString temp;
            if (i != 0)
            {
                temp += QString::number(8+((i-1)/2));
                temp += ":";
                (!((i)%2)) ? (temp += "30") : (temp += "00");
                temp += " - ";
                temp += QString::number(8+(i/2));
                temp += ":";
                (((i)%2)) ? (temp += "30") : (temp += "00");
                label = new QLabel(temp, parent);
                label->setFixedSize(80,100);
                label->setContentsMargins(1,1,1,1);
            }
            else
            {
                temp += "روز\n\\\\\nساعت";
                label = new QLabel(temp, parent);
                label->setFixedSize(80,100);
                label->setContentsMargins(5,5,5,5);
            }
            label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            label->setStyleSheet("background-color : white");
            label->setWordWrap(true);
            structureHLayout->addWidget(label);
        }
        QVBoxLayout * allLabelVLayout = new QVBoxLayout(parent);
        allLabelVLayout->addLayout(structureHLayout);
        QHBoxLayout** labelLayout = new QHBoxLayout*[5];
        for (int i = 0; i < 5; i++)
        {

            labelLayout[i] = new QHBoxLayout(parent);
            QLabel* rightLabel;
            if (i == sat)
            {
                rightLabel = new QLabel("شنبه ", parent);
                rightLabel->setContentsMargins(5,5,5,5);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(80,100);
                rightLabel->setStyleSheet("background-color : white");
            }
            else if(i == sun)
            {
                rightLabel = new QLabel("یکشنبه ", parent);
                rightLabel->setContentsMargins(5,5,5,5);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(80,100);
                rightLabel->setStyleSheet("background-color : white");
            }
            else if(i == mon)
            {
                rightLabel = new QLabel("دوشنبه ", parent);
                rightLabel->setContentsMargins(5,5,5,5);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(80,100);
                rightLabel->setStyleSheet("background-color : white");
            }
            else if(i == tus)
            {
                rightLabel = new QLabel("سه شنبه ", parent);
                rightLabel->setContentsMargins(5,5,5,5);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(80,100);
                rightLabel->setStyleSheet("background-color : white");
            }
            else if(i == wen)
            {
                rightLabel = new QLabel("چهار شنبه ", parent);
                rightLabel->setContentsMargins(5,5,5,5);
                rightLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                rightLabel->setFixedSize(80,100);
                rightLabel->setStyleSheet("background-color : white");
            }
            labelLayout[i]->addWidget(rightLabel);
            dayTime[i] = new dayUnit[18];
            for(int j=0;j<18;j++)
            {
                dayTime[i][j].dayTime.hour = 8+(j/2);
                dayTime[i][j].dayTime.min = 30*(j%2);
                dayTime[i][j].isFull = false;
                dayTime[i][j].label = new QLabel("Empty",parent);
                dayTime[i][j].label->setContentsMargins(1,1,1,1);
                dayTime[i][j].label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                dayTime[i][j].label->setFixedSize(80,100);
                dayTime[i][j].label->setStyleSheet("background-color : white");
                dayTime[i][j].label->setWordWrap(true);
                labelLayout[i]->addWidget(dayTime[i][j].label);
            }
            allLabelVLayout->addLayout(labelLayout[i]);
        }
        QVBoxLayout* buttonLayout = new QVBoxLayout(parent);
        addButton = new QPushButton("اضافه کردن درس" , parent);
        addButton->setDefault(true);
        addButton->setStyleSheet("background-color : white");
        about = new QPushButton("درباره سازنده" , parent);
        about->setStyleSheet("background-color : white");
        exit = new QPushButton("خروج");
        exit->setStyleSheet("background-color : white");
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(about);
        buttonLayout->addWidget(exit);
        buttonLayout->addStretch();
        QHBoxLayout* mainLayout = new QHBoxLayout(parent);
        mainLayout->addLayout(allLabelVLayout);
        mainLayout->addLayout(buttonLayout);
        QWidget* window = new QWidget;
        window->setLayout(mainLayout);
        setCentralWidget(window);
        connect(exit  , SIGNAL(clicked(bool)) , this , SLOT(close()));
        connect(addButton , SIGNAL(clicked(bool)) , this , SLOT(addClicked()));
        connect(about , SIGNAL(clicked(bool)) , this , SLOT(aboutF()));
        connect(this , SIGNAL(itemAdded()) , this , SLOT(update()));
        connect(this , SIGNAL(mainWindowStarted()) , this , SLOT(update()));
        emit mainWindowStarted();
    }
}

void units::add(QString Name, QString Master, myTime StartTime, myTime EndTime, int Day, int code = NotSet)
{
    unit temp;
    temp.name = Name;
    temp.master = Master;
    temp.dayOfWeek = Day;
    temp.code = code;
    bool isItChange = false;
    int buffSec = -1;                                                                  //for saving the number of start piont
    if (StartTime.hour < 8 || StartTime.hour > 15 || EndTime.hour < 9 || EndTime.hour > 17 || StartTime.min < 0 || StartTime.min > 59 || EndTime.min < 0 || EndTime.min > 59)
    {
        QApplication::beep();
        return;
    }
    else
    {
        for(int z = 0 ; z < 18 ; z++)
        {
            if ((timeCompare(dayTime[Day][z].dayTime , StartTime) == 1
                    || timeCompare(dayTime[Day][z].dayTime , StartTime) == 0)
                    && timeCompare(dayTime[Day][z].dayTime , EndTime) ==  -1)
            {
                if(!dayTime[Day][z].isFull)
                {
                    StartTime.min < 30 ?(temp.start = (StartTime.hour - 8) * 2) :(temp.start = (StartTime.hour - 8) * 2 + 1);
                    EndTime.min < 30 ? (temp.end = (EndTime.hour - 8) * 2) : (temp.end = (EndTime.hour - 8) * 2 + 1);
                    dayTime[Day][z].lesson = temp;
                    dayTime[Day][z].isFull = true;
                    if(!isItChange)
                    {
                        buffSec = z;
                    }
                    isItChange =true;
                }
                else
                {
                    QMessageBox* alert;
                    QString tt = QString("A Collision Happend.\n");
                    tt += dayTime[Day][z].lesson.name;
                    this->setStyleSheet("background-color : red");
                    alert = new QMessageBox(QMessageBox::Warning,tr("Conflict"), tt , QMessageBox::warning(this ,"Collision Error" , tt));
                    this->setStyleSheet("background-color : black");
                    clearMyDirtyWork(buffSec , z , Day , this);
                    delete alert;
                    return;
                }
            }
        }
        if(!isItChange)
        {
            QMessageBox* alert;
            QString tt = "No Such time.";
            this->setStyleSheet("background-color : red");
            alert = new QMessageBox(QMessageBox::Warning,tr("Time"), tt , QMessageBox::warning(this ,"Time Error" , tt));
            this->setStyleSheet("background-color : black");
            update();
            delete alert;
            return;
        }
        else
        {

            QMessageBox* alert;
            QString tt = "This Lesson Added to the Chart:\n";
            tt += Name;
            tt +="\n";
            tt += Master;
            this->setStyleSheet("background-color : green");
            alert = new QMessageBox(QMessageBox::Information,tr("Added"), tt , QMessageBox::information(this ,"Item Added" , tt));
            this->setStyleSheet("background-color : black");
            delete alert;
            if(theme)
            {
            emit itemAdded(Day);
            }
            else
            {
            emit itemAdded();
            }
        }
    }
}

void units::update(int Day)
{
    if(theme)
    {
        for(int i=0;i<5;i++)
        {
            for(int j=0;j<18;j++)
            {
                if(dayTime[i][j].isFull)
                {
                    if(!(dayTime[i][j].alocated))
                    {
                        int distance = dayTime[i][j].lesson.end - dayTime[i][j].lesson.start;
                        QString temp = dayTime[i][j].lesson.name;
                        temp += "\n";
                        temp += dayTime[i][j].lesson.master;
                        QLabel* tLabel = new QLabel(temp , this);
                        tLabel->setContentsMargins(1,1,1,1);
                        tLabel->setStyleSheet("background-color : white");
                        tLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                        tLabel->setWordWrap(true);
                        tLabel->setGeometry((screenSize->width() / 25 + 2) * (j+1) + 11 , (screenSize->height() / 10 +2) * (i+1) + 11 ,  0 , 0);
                        tLabel->setFixedSize((screenSize->width() / 25) * (distance) + 2*(distance -1), (screenSize->height() / 10));
                        tLabel->show();
                        for(int k = 0 ; k  < distance ; k++)
                        {
                            dayTime[i][j + k].label = tLabel;
                            dayTime[i][j + k].alocated = true;
                        }
                    }
                }
            }
        }
    }
    else
    {
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<18;j++)
        {
            if(dayTime[i][j].isFull)
            {
                QString temp = dayTime[i][j].lesson.name;
                temp += "\n";
                temp += dayTime[i][j].lesson.master;
                dayTime[i][j].label->setText(temp);
            }
            else
            {
                 dayTime[i][j].label->clear();
            }
        }
    }
    }
}

void units::addClicked()
{
    AddDialog * dialog = new AddDialog(this);
    dialog->show();
}

int timeCompare(myTime first , myTime second)
{
    if (first.hour < second.hour)
    {
        return -1;
    }
    else if(first.hour == second.hour)
    {
        if(first.min < second.min)
        {
            return -1;
        }
        else if(first.min == second.min)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

void clearMyDirtyWork(int first , int second , int row , units* parent)
{
    if(first < 0)
    {
        QApplication::beep();
        return;
    }
    for(int i = first ; i < second ; i++)
    {
        parent->dayTime[row][i].isFull = false;
    }
}

void units::aboutF()
{
    setStyleSheet("background-color : cyan");
    QMessageBox* aboutAlert = new QMessageBox(QMessageBox::Information ,tr("About"), tr("About") , QMessageBox::information(this ,"About me" , tr("About me:\nI'm S. Hossein Katebi.\nE-mail: s.h.katebi97@gmail.com\nTelegram ID: @shkatebi97\n Please send me your feedbacks and offers.")));
    delete aboutAlert;
    setStyleSheet("background-color : black");
}

//**********************************************************

AddDialog::AddDialog(QWidget *parent):QDialog(parent)
{
    setStyleSheet("background-color : white");

    name.label = new QLabel(tr("Name of lesson:"),parent);
    name.text = new QLineEdit(parent);
    name.label->setStyleSheet("background-color : white");
    name.text->setStyleSheet("background-color : white");

    name.label->setBuddy(name.text);

    master.label = new QLabel(tr("Name of your master") , parent);
    master.text = new QLineEdit(parent);
    master.label->setStyleSheet("background-color : white");
    master.text->setStyleSheet("background-color : white");

    master.label->setBuddy(master.text);

    startTime.label = new QLabel(tr("Time of start of the lesson") , parent);
    startTime.text = new QLineEdit(parent);
    startTime.label->setStyleSheet("background-color : white");
    startTime.text->setStyleSheet("background-color : white");

    startTime.label->setBuddy(startTime.text);

    endTime.label = new QLabel(tr("Time of end of the lesson") , parent);
    endTime.text = new QLineEdit(parent);
    endTime.label->setStyleSheet("background-color : white");
    endTime.text->setStyleSheet("background-color : white");

    endTime.label->setBuddy(endTime.text);

    code.label = new QLabel(tr("The code of the lesson(optional)") , parent);
    code.text = new QLineEdit(parent);
    code.label->setStyleSheet("background-color : white");
    code.text->setStyleSheet("background-color : white");

    code.label->setBuddy(code.text);

    firstDay.label = new QLabel(tr("First day of the lesson in week:") , parent);
    firstDay.text = new QLineEdit(parent);
    firstDay.label->setStyleSheet("background-color : white");
    firstDay.text->setStyleSheet("background-color : white");

    firstDay.label->setBuddy(firstDay.text);

    lastDay.label = new QLabel(tr("Last day of the lesson in week:\nIf the lesson is just one time in week,\nkeep it empty\nTemporary disabled") , parent);
    lastDay.text = new QLineEdit(parent);
    lastDay.label->setStyleSheet("background-color : white");
    lastDay.text->setStyleSheet("background-color : white");
    lastDay.text->setDisabled(true);

    lastDay.label->setBuddy(lastDay.text);

    hint = new QLabel(tr("Hint:"),parent);
    hint->setStyleSheet("background-color : white");


    ok = new QPushButton(tr("&OK"),parent);
    ok->setDefault(true);
    ok->setDisabled(true);
    ok->setStyleSheet("background-color : white");
    cancel = new QPushButton(tr("&Cancel") , parent);
    cancel->setStyleSheet("background-color : white");


    connect(ok,SIGNAL(clicked(bool)),this,SLOT(addMe()));
    connect(cancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(this ,SIGNAL(add(QString,QString, myTime, myTime, int, int)),parent ,SLOT(add(QString,QString, myTime, myTime, int, int)));
    connect(name.text, SIGNAL(textEdited(QString)), this, SLOT(activeButton(QString)));
    connect(master.text, SIGNAL(textEdited(QString)), this, SLOT(activeButton(QString)));
    connect(startTime.text, SIGNAL(textEdited(QString)), this, SLOT(activeButton(QString)));
    connect(endTime.text, SIGNAL(textEdited(QString)), this, SLOT(activeButton(QString)));
    connect(firstDay.text, SIGNAL(textEdited(QString)), this, SLOT(activeButton(QString)));


    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget(name.label);
    nameLayout->addWidget(name.text);

    QHBoxLayout* masterLayout = new QHBoxLayout;
    masterLayout->addWidget(master.label);
    masterLayout->addWidget(master.text);

    QHBoxLayout* startTimeLayout = new QHBoxLayout;
    startTimeLayout->addWidget(startTime.label);
    startTimeLayout->addWidget(startTime.text);

    QHBoxLayout* endTimeLayout = new QHBoxLayout;
    endTimeLayout->addWidget(endTime.label);
    endTimeLayout->addWidget(endTime.text);

    QHBoxLayout* codeLayout = new QHBoxLayout;
    codeLayout->addWidget(code.label);
    codeLayout->addWidget(code.text);

    QHBoxLayout* firstDayLayout = new QHBoxLayout;
    firstDayLayout->addWidget(firstDay.label);
    firstDayLayout->addWidget(firstDay.text);

    QHBoxLayout* lastDayLayout = new QHBoxLayout;
    lastDayLayout->addWidget(lastDay.label);
    lastDayLayout->addWidget(lastDay.text);

    QHBoxLayout* hintLayout = new QHBoxLayout;
    hintLayout->addWidget(hint);

    QVBoxLayout* labelTextLayout = new QVBoxLayout;
    labelTextLayout->addLayout(nameLayout);
    labelTextLayout->addLayout(masterLayout);
    labelTextLayout->addLayout(startTimeLayout);
    labelTextLayout->addLayout(endTimeLayout);
    labelTextLayout->addLayout(codeLayout);
    labelTextLayout->addLayout(firstDayLayout);
    labelTextLayout->addLayout(lastDayLayout);
    labelTextLayout->addLayout(hintLayout);

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttonLayout->addStretch();

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addLayout(labelTextLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Add Lesson"));
    setFixedHeight(sizeHint().height());
}

void AddDialog::addMe()
{
    bool isOk = true;
    bool hasLastDay = false;
    code.text->text().toInt(&isOk);
    if(!name.text->text().isEmpty() && !master.text->text().isEmpty() && !startTime.text->text().isEmpty() && !endTime.text->text().isEmpty() && !firstDay.text->text().isEmpty())
    {
        if(isOk || code.text->text().isEmpty())
        {
            myTime s = toTime(startTime.text->text());
            myTime e = toTime(endTime.text->text());
            int firstday = detectDay(firstDay.text->text() , this);
            if (firstday == -1)
            {
                return;
            }
            int secondday;
            if(!lastDay.text->text().isEmpty())
            {
                secondday = detectDay(lastDay.text->text() , this);
                hasLastDay = true;
            }
            else
            {
                hasLastDay = false;
                secondday = -1;
            }
            if (s.hour || s.min || e.hour || e.min)
            {
                if (!(s.hour < 8 || s.hour > 15 || e.hour < 9 || e.hour > 17 || s.min < 0 || s.min > 59 || e.min < 0 || e.min > 59))
                {
                    emit add(name.text->text() , master.text->text() , s , e , code.text->text().toInt() , firstday);
                    if(hasLastDay)
                    {
                        emit add(name.text->text() , master.text->text() , s , e , code.text->text().toInt() , secondday);
                    }
                    close();
                }
                else
                {
                    QApplication::beep();
                    hint->setText(tr("Out of range"));
                    hint->setStyleSheet("color : red");
                    return;
                }
            }
            else
            {
                QApplication::beep();
                hint->setText(tr("The Time is not correct"));
                hint->setStyleSheet("color : red");
                return;
            }
        }
        else
        {
            hint->setText(tr("You must enter a numeric code in code filed"));
            hint->setStyleSheet("color : red");
            QApplication::beep();
        }
    }
}

myTime toTime(QString ourTime)
{
    myTime t;
    int x = 0, y = 30;
    QString temp;
    for (int i=0;i<ourTime.length();i++)
    {
        char te = ourTime.at(i).toLatin1();
        if(i == ourTime.length() - 1)
        {
            temp += te;
            bool isOk = true;
            y = temp.toInt(&isOk);
            temp.clear();
            if(!isOk)
            {
                t.min=0;
                t.hour=0;
                return t;
            }
        }
        if(te == ':')
        {
            bool isOk = true;
            x = temp.toInt(&isOk);
            temp.clear();
            if(!isOk)
            {
                t.min=0;
                t.hour=0;
                return t;
            }
        }
        else
        {
            temp += te;
        }
    }
    t.hour = x;
    t.min = y;
    return t;
}

void AddDialog::activeButton(QString)
{
    if(!name.text->text().isEmpty() && !master.text->text().isEmpty() && !startTime.text->text().isEmpty() && !endTime.text->text().isEmpty() && !firstDay.text->text().isEmpty())
    {
        ok->setEnabled(true);
    }
}

char * QStringToChar(QString str)
{
    QApplication::beep();
    int len = str.length();
    char * a = new char[len];
    for (int i = 0; i != len; i++)
    {
        a[i] = str.at(i).toLatin1();
    }
    a[len] = NULL;
    return a;
}

int detectDay(QString day , AddDialog* dialog)
{
    int temp;
    if(day == QString("saturday"))
    {
        temp = 0;
    }
    else if(day == QString("sunday"))
    {
        temp = 1;
    }
    else if(day == QString("monday"))
    {
        temp = 2;
    }
    else if(day == QString("tuesday"))
    {
        temp = 3;
    }
    else if(day == QString("wednesday"))
    {
        temp= 4;
    }
    else
    {
        QApplication::beep();
        dialog->hint->setText(QString("The Day is not correct"));
        dialog->hint->setStyleSheet("color : red");
        temp = -1;
    }
    return temp;
}

//**********************************************************

startDialog::startDialog(QApplication* parent):QDialog()
{
    isdone = false;
    label = new QLabel("Please pick up a view methood: " , this);
    label->setStyleSheet("color : green , font-family : Tahoma");
    label->setContentsMargins(5,5,5,5);
    label->setWordWrap(true);
    QHBoxLayout* labelLayout = new QHBoxLayout;
    labelLayout->addWidget(label);
    labelLayout->addStretch();
    QHBoxLayout* classicOptionLayout = new QHBoxLayout;
    classic = new QCheckBox("Classic" , this);
    classicOptionLayout->addWidget(classic);
    QHBoxLayout* modernOptionLayout = new QHBoxLayout;
    modern = new QCheckBox("Modern" , this);
    modernOptionLayout->addWidget(modern);
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    done = new QPushButton("Let's go" , this);
    done->setDefault(true);
    done->setDisabled(true);
    done->setStyleSheet("background-color : red");
    buttonLayout->addWidget(done);
    cancel = new QPushButton("Cancel" , this);
    cancel->setStyleSheet("background-color : yellow");
    buttonLayout->addWidget(cancel);
    buttonLayout->addStretch();
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(labelLayout);
    mainLayout->addLayout(classicOptionLayout);
    mainLayout->addLayout(modernOptionLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    connect(cancel , SIGNAL(clicked(bool)) , parent , SLOT(closeAllWindows()));
    connect(classic , SIGNAL(clicked(bool)) , modern , SLOT(setDisabled(bool)));
    connect(modern , SIGNAL(clicked(bool)) , classic , SLOT(setDisabled(bool)));
    connect(classic , SIGNAL(clicked(bool)) , this , SLOT(enableButton(bool)));
    connect(modern , SIGNAL(clicked(bool)) , this , SLOT(enableButton(bool)));
    connect(done , SIGNAL(clicked(bool)) , this , SLOT(selected()));
}

void startDialog::enableButton(bool state)
{
    (state)?(done->setStyleSheet("background-color : green")):(done->setStyleSheet("background-color : red"));
    done->setEnabled(state);
}

void startDialog::selected()
{
    if (!classic->isChecked())
    {
        isdone = true;
    }
    emit select(isdone);
    close();
}
