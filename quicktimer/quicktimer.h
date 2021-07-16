#pragma once

#include "ui_quicktimer.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>

#include "Settings.h"

#include <chrono>

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

    void fillTimerMenu();
    QMenu timerMenu_;
};
