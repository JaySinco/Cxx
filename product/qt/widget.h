#pragma once
#include <QtWidgets/QWidget>

class QPushButton;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow();
private slots:
    void slotButtonClicked(bool checked);

private:
    QPushButton* btnCheck;
};