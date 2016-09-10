#include<QApplication>
#include"units.h"

void executeMain()
{
}

int main(int argc , char *argv[])
{
    QApplication app(argc, argv);
    startDialog* a = new startDialog(&app);
    a->show();
    units b(a);
    return app.exec();
}

