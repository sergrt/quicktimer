#include "TimerLabel.h"
#include "utils.h"

#include <QMouseEvent>

TimerLabel::TimerLabel(QWidget* parent)
    : QLabel(parent) {
}

void TimerLabel::mouseDoubleClickEvent(QMouseEvent* evt) {
    emit doubleClick();
}

void TimerLabel::mousePressEvent(QMouseEvent *evt) {
    if (evt->button() == Qt::LeftButton) {
        QWidget* mainWindow = this->parentWidget()->parentWidget();
        localMousePos_ = evt->globalPos() - mainWindow->pos();
    } else if (evt->button() == Qt::RightButton) {
        emit rightClick(evt->globalPos());
    }
}
void TimerLabel::mouseMoveEvent(QMouseEvent *evt) {
    if (evt->buttons() & Qt::LeftButton) {
        const auto newPos = evt->globalPos();
        emit updateMainWindowPos(newPos.x() - localMousePos_.x(), newPos.y() - localMousePos_.y());
    }
}

void TimerLabel::setTime(std::chrono::seconds seconds) {
    setText(toHumanTime(seconds).c_str());
}