#pragma once
#include <QLabel>
#include <chrono>

class TimerLabel : public QLabel {
    Q_OBJECT

public:
    TimerLabel(QWidget* parent);
    void mouseDoubleClickEvent(QMouseEvent* evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseMoveEvent(QMouseEvent *evt) override;
    void setTime(std::chrono::seconds seconds);
private:
    QPoint localMousePos_; // position inside window to calculate offset
signals:
    void doubleClick();
    void updateMainWindowPos(const int x, const int y);
    void rightClick(const QPoint&);
};

