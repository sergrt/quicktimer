#include "QuickTimer.h"
#include "utils.h"

#include <QMessageBox>
#include <QDesktopWidget>

static constexpr int internalTimerInterval = 1000;

QuickTimer::QuickTimer(QWidget *parent)
    : QMainWindow(parent) {
    ui_.setupUi(this);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    trayIcon_.setIcon(QIcon(":/quicktimer/quicktimer.ico"));
    trayIcon_.show();

    fillTimerMenu();

    applySettings();
    connectSignals();

    run();
}

QuickTimer::~QuickTimer() {
    settings_.save();
}

void QuickTimer::applySettings() {
    timeLeft_ = settings_.timerInterval();

    if (settings_.alwaysOnTop())
        setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    resize(settings_.width(), settings_.height());
    move(settings_.x(), settings_.y());

    setStyleSheet(QString::fromStdString(settings_.backgroundCss()));
    ui_.timerLabel->setStyleSheet(QString::fromStdString(settings_.timerLabelCss()));
    ui_.closeButton->setStyleSheet(QString::fromStdString(settings_.closeButtonCss()));
}

void QuickTimer::run() {
    ui_.timerLabel->setTime(timeLeft_);
    timer_.start(internalTimerInterval);
}

void QuickTimer::connectSignals() {
    connect(ui_.closeButton, &QToolButton::clicked, this, [&]() {
        QApplication::quit();
    });

    connect(ui_.timerLabel, &TimerLabel::doubleClick, this, [&]() {
        if (timer_.isActive()) {
            timer_.stop();
        } else {
            timeLeft_ = settings_.timerInterval();
            run();
        }
    });

    connect(ui_.timerLabel, &TimerLabel::updateMainWindowPos, this, [&](const int x, const int y) {
        move(x, y);
        settings_.setX(x);
        settings_.setY(y);
    });

    connect(&timer_, &QTimer::timeout, this, [&]() {
        timeLeft_ -= std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(internalTimerInterval));
        ui_.timerLabel->setTime(timeLeft_);

        if (timeLeft_ <= std::chrono::seconds(0)) {
            timer_.stop();
            execFinishAction();
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

    connect(ui_.timerLabel, &TimerLabel::rightClick, this, [&](const QPoint& p) {
        timerMenu_.exec(p);
    });
}

void QuickTimer::execFinishAction() {
    auto* m = new QMessageBox(QDesktopWidget().screen());
    m->setAttribute(Qt::WA_DeleteOnClose, true);
    m->setWindowFlags(m->windowFlags() | Qt::WindowStaysOnTopHint);
    m->setWindowTitle("Timeout");
    m->setText("Timer timed out");
    m->show();
}

void QuickTimer::fillTimerMenu() {
    const auto intervals = settings_.menuIntervals();
    for (const auto& i : intervals) {
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