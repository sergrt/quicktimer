#pragma once

#include "ui_quicktimer.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>

#include "Settings.h"

#include <chrono>

class QuickTimer : public QMainWindow {
    Q_OBJECT

public:
    QuickTimer(QWidget *parent = Q_NULLPTR);
    ~QuickTimer();

private:
    Ui::quicktimerClass ui_;
    QSystemTrayIcon trayIcon_;
    QMenu timerMenu_;
    Settings settings_;
    QTimer timer_;
    std::chrono::seconds timeLeft_;

    void applySettings();
    void connectSignals();

    void run();

    void fillTimerMenu();
    void execFinishAction();
};
