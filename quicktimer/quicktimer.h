#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quicktimer.h"
#include <chrono>
#include "Settings.h"

class quicktimer : public QMainWindow {
    Q_OBJECT

public:
    quicktimer(QWidget *parent = Q_NULLPTR);
    ~quicktimer();
private:
    Ui::quicktimerClass ui;
    QSystemTrayIcon trayIcon_;

    Settings settings_;
    QTimer timer_;
    std::chrono::seconds timeLeft_;

    void applySettings();
    void connectSignals();

    void run();
};
