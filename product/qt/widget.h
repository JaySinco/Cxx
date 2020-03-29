#pragma once
#include <QWidget>

class QPushButton;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow();
signals:
    void counterReached();
private slots:
    void slotButtonClicked(bool checked);

private:
    QPushButton* btnCheck;
    int counter = 0;
};