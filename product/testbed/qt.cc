#include "common/utility/base.h"
#include <QApplication>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QWidget>
#include <QtPlugin>

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

class MainWindow : public QWidget {
public:
    explicit MainWindow()
    {
        setFixedSize(500, 500);

        QLabel* label = new QLabel("<h2><i>Hello</i><font color='red'>Qt!</font></h2>", this);
        label->setGeometry(10, 80, 150, 80);

        QPushButton* btnQuit = new QPushButton("Quit", this);
        btnQuit->setGeometry(400, 450, 80, 30);
        btnQuit->setToolTip("quit program");
        connect(btnQuit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

        QProgressBar* bar = new QProgressBar(this);
        bar->setRange(0, 100);
        bar->setValue(0);
        bar->setGeometry(10, 10, 180, 30);
        QSlider* slider = new QSlider(this);
        slider->setOrientation(Qt::Horizontal);
        slider->setRange(0, 100);
        slider->setValue(0);
        slider->setGeometry(10, 40, 180, 30);
        connect(slider, SIGNAL(valueChanged(int)), bar, SLOT(setValue(int)));
    }
};

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