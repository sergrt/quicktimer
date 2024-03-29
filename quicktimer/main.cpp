#include "QuickTimer.h"

#include <QApplication>
#include <QCryptographicHash>
#include <QSharedMemory>
#include <QSystemSemaphore>

namespace
{
    QString generateKeyHash(const QString& key, const QString& salt) {
        QByteArray data;
        data.append(key.toUtf8());
        data.append(salt.toUtf8());
        data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

        return data;
    }

    class RunGuard {
    public:
        explicit RunGuard(const QString& key)
            : key_(key), memLockKey_(generateKeyHash(key, "_memLockKey")), sharedmemKey_(generateKeyHash(key, "_sharedmemKey")),
            sharedMem_(sharedmemKey_), memLock_(memLockKey_, 1) {
            
            memLock_.acquire();
            {
                QSharedMemory fix(sharedmemKey_); // Fix for *nix: http://habrahabr.ru/post/173281/
                fix.attach();
            }
            memLock_.release();
        }
        ~RunGuard() {
            release();
        }

        bool RunGuard::tryToRun() {
            if (isAnotherRunning())   // Extra check
                return false;

            memLock_.acquire();
            const bool result = sharedMem_.create(sizeof(quint64));
            memLock_.release();
            if (!result)
            {
                release();
                return false;
            }

            return true;
        }
    private:
        bool isAnotherRunning() {
            if (sharedMem_.isAttached())
                return false;

            memLock_.acquire();
            const bool isRunning = sharedMem_.attach();
            if (isRunning)
                sharedMem_.detach();
            memLock_.release();

            return isRunning;
        }
        void RunGuard::release() {
            memLock_.acquire();
            if (sharedMem_.isAttached())
                sharedMem_.detach();
            memLock_.release();
        }

        const QString key_;
        const QString memLockKey_;
        const QString sharedmemKey_;

        QSharedMemory sharedMem_;
        QSystemSemaphore memLock_;

        Q_DISABLE_COPY(RunGuard)
    };
}

int main(int argc, char *argv[])
{
    RunGuard guard("quicktimer_single_instance_check");
    if (!guard.tryToRun())
        return 0;

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QuickTimer w;
    w.show();
    return a.exec();
}
