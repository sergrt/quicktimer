#include "stdafx.h"
#include "TimerLabel.h"
TimerLabel::TimerLabel(QWidget* parent)
    : QLabel(parent) {
}

void TimerLabel::mouseDoubleClickEvent(QMouseEvent* evt) {
    emit doubleClick();
}

void TimerLabel::mousePressEvent(QMouseEvent *evt) {
    QWidget* mainWindow = this->parentWidget()->parentWidget();
    localMousePos_ = evt->globalPos() - mainWindow->pos();
}
void TimerLabel::mouseMoveEvent(QMouseEvent *evt) {
    const auto newPos = evt->globalPos();
    emit updateMainWindowPos(newPos.x() - localMousePos_.x(), newPos.y() - localMousePos_.y());
}

void TimerLabel::setTime(std::chrono::seconds seconds) {
    int secondsCount = seconds.count();
    if (seconds <= std::chrono::seconds(0))
        secondsCount = 0;

    const int hoursPart = secondsCount / 60 / 60;
    const int minutesPart = secondsCount / 60;
    const int secondsPart = secondsCount - hoursPart * 60 * 60 - minutesPart * 60;

    QString text = QString("%1:%2").arg(minutesPart, 2, 10, QChar('0')).arg(secondsPart, 2, 10, QChar('0'));
    if (hoursPart > 0)
        text = QString("%1:").arg(minutesPart, 2, 10, QChar('0')) + text;

    setText(text);
}