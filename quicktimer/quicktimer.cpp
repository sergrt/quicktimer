#include "quicktimer.h"
#include "utils.h"

#include <QMessageBox>
#include <QDesktopWidget>

const int internalTimerInterval = 1000;

quicktimer::quicktimer(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    trayIcon_.setIcon(QIcon(":/quicktimer/quicktimer.ico"));
    trayIcon_.show();

    fillTimerMenu();
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    applySettings();
    connectSignals();
    run();
}
quicktimer::~quicktimer() {
    settings_.save();
}
void quicktimer::applySettings() {
    timeLeft_ = settings_.timerInterval();

    if (settings_.alwaysOnTop())
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    this->resize(settings_.width(), settings_.height());
    this->move(settings_.x(), settings_.y());

    this->setStyleSheet(QString::fromStdString(settings_.backgroundCss()));
    ui.timerLabel->setStyleSheet(QString::fromStdString(settings_.timerLabelCss()));
    ui.closeButton->setStyleSheet(QString::fromStdString(settings_.closeButtonCss()));
}
void quicktimer::run() {
    ui.timerLabel->setTime(timeLeft_);
    timer_.start(internalTimerInterval);
}
void quicktimer::connectSignals() {
    connect(ui.closeButton, &QToolButton::clicked, this, [&]() {
        QApplication::quit();
    });

    connect(ui.timerLabel, &TimerLabel::doubleClick, this, [&]() {
        if (timer_.isActive()) {
            timer_.stop();
        } else {
            timeLeft_ = settings_.timerInterval();
            run();
        }
    });

    connect(ui.timerLabel, &TimerLabel::updateMainWindowPos, this, [&](const int x, const int y) {
        move(x, y);
        settings_.setX(x);
        settings_.setY(y);
    });

    connect(&timer_, &QTimer::timeout, this, [&]() {
        timeLeft_ -= std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(internalTimerInterval));
        ui.timerLabel->setTime(timeLeft_);

        if (timeLeft_ <= std::chrono::seconds(0)) {
            timer_.stop();
            QMessageBox* m = new QMessageBox(QDesktopWidget().screen());
            m->setAttribute(Qt::WA_DeleteOnClose, true);
            m->setWindowFlags(m->windowFlags() | Qt::WindowStaysOnTopHint);
            m->setWindowTitle("Timeout");
            m->setText("Timer timed out");
            m->show();
        }
    });

    connect(&trayIcon_, &QSystemTrayIcon::activated, this, [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            if (!isVisible())
                show();
            else
                hide();
        }
    });

    connect(ui.timerLabel, &TimerLabel::rightClick, this, [&](const QPoint& p) {
        timerMenu_.exec(p);
    });
}

void quicktimer::fillTimerMenu() {
    auto intervals = settings_.menuIntervals();
    for (const auto i : intervals) {
        if (i.count() > 0) {
            auto f = std::bind([&](const std::chrono::seconds x) {
                if (timer_.isActive())
                    timer_.stop();
                
                timeLeft_ = x;
                run();
            }, i);
            timerMenu_.addAction(toHumanTime(i).c_str(), this, f);
        }
    }
}