#include <QCoreApplication>
#include "UCalculateParser.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Q_UNUSED(argc);
    Q_UNUSED(argv);
    Q_UNUSED(app);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    QString command = "0.5 ^ 2";
    qDebug() << command << "="
             << UCalculateParser().calculate(command);

    return 0;
//    return app.exec();
}
