#include "common/utility/base.h"
#include "widget.h"
#include <QtCore/QtPlugin>
#include <QtWidgets/QApplication>

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}