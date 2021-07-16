#include "Settings.h"

#include <QSettings>

const std::string Settings::fileName = "settings.ini";
const int maxMenuIntervals = 10;

static std::string intervalSettingsName(int i) {
    return "Interval" + std::to_string(i);
}

Settings::Settings() {
    QSettings s(QString::fromStdString(fileName), QSettings::IniFormat);
    s.beginGroup("Timer");
    timerInterval_ = std::chrono::seconds(s.value("Interval", 25 * 60).toInt());
    s.endGroup();

    s.beginGroup("Interface");
    backgroundCss_ = s.value("BackgroundCss", "").toString().toStdString();
    timerLabelCss_ = s.value("TimerLabelCss", "").toString().toStdString();
    closeButtonCss_ = s.value("CloseButtonCss", "").toString().toStdString();
    
    alwaysOnTop_ = s.value("AlwaysOnTop", true).toBool();

    width_ = s.value("Width", 128).toInt();
    height_ = s.value("Height", 28).toInt();

    x_ = s.value("X", 0).toInt();
    y_ = s.value("Y", 0).toInt();
    s.endGroup();

    s.beginGroup("Intervals");
    for (int i = 0; i < maxMenuIntervals; ++i)
        menuIntervals_.push_back(std::chrono::seconds(s.value(intervalSettingsName(i).c_str(), 0).toInt()));
    s.endGroup();
}

void Settings::save() const {
    QSettings s(QString::fromStdString(fileName), QSettings::IniFormat);
    s.beginGroup("Timer");
    s.setValue("Interval", QVariant::fromValue<int>(timerInterval_.count()));
    s.endGroup();

    s.beginGroup("Interface");
    s.setValue("BackgroundCss", QVariant::fromValue<QString>(QString::fromStdString(backgroundCss_)));
    s.setValue("TimerLabelCss", QVariant::fromValue<QString>(QString::fromStdString(timerLabelCss_)));
    s.setValue("CloseButtonCss", QVariant::fromValue<QString>(QString::fromStdString(closeButtonCss_)));

    s.setValue("AlwaysOnTop", alwaysOnTop_);

    s.setValue("Width", width_);
    s.setValue("Height", height_);

    s.setValue("X", x_);
    s.setValue("Y", y_);
    s.endGroup();

    s.beginGroup("Intervals");
    for (int i = 0; i < menuIntervals_.size(); ++i)
        s.setValue(intervalSettingsName(i).c_str(), QVariant::fromValue<int>(menuIntervals_[i].count()));

    if (menuIntervals_.size() < maxMenuIntervals) {
        for (int i = menuIntervals_.size(); i < maxMenuIntervals; ++i)
            s.setValue(intervalSettingsName(i).c_str(), QVariant::fromValue<int>(0));
    }
    s.endGroup();
}

auto Settings::timerInterval() const->decltype(timerInterval_) {
    return timerInterval_;
}
auto Settings::backgroundCss() const->decltype(backgroundCss_) {
    return backgroundCss_;
}
auto Settings::timerLabelCss() const->decltype(timerLabelCss_) {
    return timerLabelCss_;
}
auto Settings::closeButtonCss() const->decltype(closeButtonCss_) {
    return closeButtonCss_;
}
auto Settings::alwaysOnTop() const->decltype(alwaysOnTop_) {
    return alwaysOnTop_;
}
auto Settings::width() const->decltype(width_) {
    return width_;
}
auto Settings::height() const->decltype(height_) {
    return height_;
}
auto Settings::x() const->decltype(x_) {
    return x_;
}
auto Settings::y() const->decltype(y_) {
    return y_;
}
auto Settings::menuIntervals() const->decltype(menuIntervals_) {
    return menuIntervals_;
}
void Settings::setX(decltype(x_) x) {
    x_ = x;
}
void Settings::setY(decltype(y_) y) {
    y_ = y;
}