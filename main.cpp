#include <QApplication>
#include "noter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Noter noter;
    noter.show();

    return a.exec();
}
