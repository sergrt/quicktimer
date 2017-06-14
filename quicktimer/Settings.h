#pragma once
#include <string>
#include <chrono>

class Settings {
public:
    Settings();
    void save() const;
private:
    static const std::string fileName;
    
    std::chrono::seconds timerInterval_;
    std::string backgroundCss_;
    std::string timerLabelCss_;
    std::string closeButtonCss_;

    bool alwaysOnTop_;
    int width_;
    int height_;

    int x_;
    int y_;
public:
    auto timerInterval() const->decltype(timerInterval_);
    auto backgroundCss() const->decltype(backgroundCss_);
    auto timerLabelCss() const->decltype(timerLabelCss_);
    auto closeButtonCss() const->decltype(closeButtonCss_);

    auto alwaysOnTop() const->decltype(alwaysOnTop_);
    auto width() const->decltype(width_);
    auto height() const->decltype(height_);
    auto x() const->decltype(x_);
    auto y() const->decltype(y_);

    void setX(decltype(x_) x);
    void setY(decltype(y_) y);
};
