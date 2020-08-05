#include "oknotablitsa.h"
#include <QApplication>
#include <QLabel>
#include <QList>
#include <QListWidget>
#include "xmlparser.h"
#include "scheduler.h"
#include <iostream>
using namespace std;
using namespace xmlParser;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    oknotablitsa w(argv);
    w.show();

    return a.exec();
}
