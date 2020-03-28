#include "widget.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

MainWindow::MainWindow()
{
    setFixedSize(500, 500);

    QPushButton* btnQuit = new QPushButton("Quit", this);
    btnQuit->setGeometry(400, 450, 80, 30);
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

    btnCheck = new QPushButton("Hello!", this);
    btnCheck->setGeometry(10, 450, 80, 30);
    btnCheck->setCheckable(true);
    connect(btnCheck, SIGNAL(clicked(bool)), this, SLOT(slotButtonClicked(bool)));
}

void MainWindow::slotButtonClicked(bool checked)
{
    if (checked) {
        btnCheck->setText("Checked");
    } else {
        btnCheck->setText("Hello!");
    }
}