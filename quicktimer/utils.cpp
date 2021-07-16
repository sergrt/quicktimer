#include "utils.h"

#include <QString>

std::string toHumanTime(std::chrono::seconds seconds) {
    int secondsCount = seconds.count();
    if (seconds <= std::chrono::seconds(0))
        secondsCount = 0;

    int hoursPart = secondsCount / 60 / 60;
    int minutesPart = (secondsCount - hoursPart * 60 * 60) / 60;
    const int secondsPart = secondsCount - hoursPart * 60 * 60 - minutesPart * 60;

    // Case - 1 hour represented as 60:00
    if (hoursPart == 1 && minutesPart == 0 && secondsPart == 0) {
        minutesPart = 60;
        hoursPart = 0;
    }

    QString text = QString("%1:%2").arg(minutesPart, 2, 10, QChar('0')).arg(secondsPart, 2, 10, QChar('0'));
    if (hoursPart > 0)
        text = QString("%1:").arg(hoursPart, 2, 10, QChar('0')) + text;

    return text.toStdString();
}