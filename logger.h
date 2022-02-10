#pragma once

#include <QFile>

class Logger
{
    enum LogMode
    {
        File,
        Console
    };

    enum MsgType
    {
        debugMsg,
        warningMsg,
        criticalMsg,
        fatalMsg,
        infoMsg,
        systemMsg = criticalMsg
    };

protected:
    static Logger* instance();
    static void finish();

public:
    static void InfoLogFile(const QString& message);
    static void DebugLogFile(const QString& message);
    static void FatalLogFile(const QString& message);
    static void WarningLogFile(const QString& message);
    static void CriticalLogFile(const QString& message);
    static void SystemLogFile(const QString& message);

    static void InfoLogConsole(const QString& message);
    static void DebugLogConsole(const QString& message);
    static void FatalLogConsole(const QString& message);
    static void WarningLogConsole(const QString& message);
    static void CriticalLogConsole(const QString& message);
    static void SystemLogConsole(const QString& message);

private:
    bool openFile();
    void setFileName(const QString&);
    void messageHandler(MsgType typeMessage, const QString& message, LogMode mode = File);
    void setType(MsgType type);
    void setMode(LogMode mode);
    void writeToFile(MsgType typeMessage, const QString& message);
    void writeToConsole(MsgType typeMessage, const QString&);
private:
    LogMode mode();
    MsgType type();
    QString fileName() const;

private:
    static Logger* m_instance;
    QFile m_fileLog;
    LogMode m_logMode;
    MsgType m_typeMessage;

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};