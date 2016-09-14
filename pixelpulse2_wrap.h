/// simple abstraction providing for the spawning of Pixelpulse2 when an M1K is attached

#ifndef PP2_H
#define PP2_H

#include <QDebug>
#include <QProcess>
#include <iostream>
#include <QObject>
#include <QTextStream>

class PP2Wrapper: public QObject
{
    Q_OBJECT

    bool isDisabled() { return disabled; }

public slots:

    void setDisabled(bool state) { disabled = state; }

    QString getPP2Name() {
    #ifdef Q_OS_LINUX
        return "pixelpulse2";
    #elif defined(Q_OS_WIN32)
        return "pixelpulse2.exe";
    #elif defined(Q_OS_MAC)
        return "Pixelpulse2";
    #else
        #error "We don't support this platform yet."
    #endif
    }

    bool getPP2Status() {
        QProcess tasklist;
        QStringList arguments;
    #ifdef Q_OS_LINUX
        arguments << "--no-header" << "-C" << getPP2Name();
        tasklist.start("ps", arguments);
        tasklist.waitForFinished();
        QString output = tasklist.readAllStandardOutput();
        return output.contains(getPP2Name());
    #elif defined(Q_OS_MAC)
        arguments << "-i" << "-l" << getPP2Name();
        tasklist.start("pgrep", arguments);
        tasklist.waitForFinished();
        QString output = tasklist.readAllStandardOutput();
        return output.contains(getPP2Name());
    #elif defined(Q_OS_WIN32)
        arguments << "/NH" << "/FO" << "CSV" << "/FI" << QString("IMAGENAME eq %1").arg(getPP2Name());
        tasklist.start("tasklist", arguments);
        tasklist.waitForFinished();
        QString output = tasklist.readAllStandardOutput();
        return output.startsWith(QString("\"%1").arg(getPP2Name()));
    #else
        #error "We don't support this platform yet."
    #endif
    }

    bool launchPixelpulse2IfNotRunning() {
        if (!getPP2Status())
            return launchPixelpulse2();
        else
            return false;
    }

    bool launchPixelpulse2() {
        QProcess pp2Thread;
        QString program = getPP2Name();
        QStringList arguments;

        if (disabled)
		return false;

        pp2Thread.startDetached(program, arguments);
        return pp2Thread.waitForStarted(5000);
    }

public:
    PP2Wrapper():
        disabled(false) {}

private:
    bool disabled;
};

#endif // PP2_H
