#include <QDateTime>
#include <QDebug>

#include "logger.h"

Logger* Logger::m_instance = nullptr;

const QString path("/logApp");

Logger::Logger():
    m_fileLog(QString("%1/log %2 %3.txt").arg(path)
                .arg(QDateTime::currentDateTime().date().toString(QString("dd:MM:yyyy")))
                .arg(QDateTime::currentDateTime().time().toString(QString("hh:mm:ss")))
              )
{
    openFile();
}

void Logger::setMode(LogMode mode)
{
    m_logMode = mode;
}

Logger::LogMode Logger::mode()
{
    return m_logMode;
}

Logger::~Logger()
{
    m_fileLog.close();
}

Logger* Logger::instance()
{
    if(m_instance == nullptr )
    {
        system(QString("mkdir -p %1").arg(path));
        m_instance = new Logger();
    }

    return m_instance;
}

void Logger::finish()
{
    delete m_instance;
    m_instance = nullptr;
}

void Logger::setFileName(const QString& name)
{
    m_fileLog.setFileName(name);
}

QString Logger::fileName() const
{
    return m_fileLog.fileName();
}

void Logger::setType(MsgType type)
{
    m_typeMessage = type;
}

bool Logger::openFile()
{
    if(m_fileLog.open(QIODevice::WriteOnly))
        return true;
    else return false;
}

Logger::MsgType Logger::type()
{
    return m_typeMessage;
}

void Logger::messageHandler(MsgType typeMessage, const QString& message, LogMode mode)
{
    switch(mode)
    {
        case LogMode::File:
            writeToFile(typeMessage, message);
            break;

        case LogMode::Console:
            writeToConsole(typeMessage, message);
            break;

    }
}

void Logger::writeToConsole(MsgType typeMessage, const QString& message)
{
    QString messageType;

    if(MsgType::debugMsg == typeMessage)
        messageType = "[Debug] ";
    if(MsgType::warningMsg == typeMessage)
        messageType = "[Warning] ";
    if(MsgType::infoMsg == typeMessage)
        messageType = "[Info] ";
    if(MsgType::fatalMsg == typeMessage)
        messageType = "[Fatal] ";
    if(MsgType::criticalMsg == typeMessage || MsgType::systemMsg == typeMessage)
        messageType = "[System] ";

    QString debugString = QString("[%1 %2] %3\n\t\t function name: [%4]\n\t\t description: %5")
            .arg(QDateTime::currentDateTime().date().toString(QString("dd:MM:yyyy")))
            .arg(QDateTime::currentDateTime().time().toString(QString("hh:mm:ss")))
            .arg(messageType)
            .arg(__func__)
            .arg(message);
    qDebug() << debugString;
}

void Logger::writeToFile(MsgType typeMessage, const QString& message)
{
    if(!m_fileLog.isOpen())
    {
        if(!openFile())
            return;
    }

    m_fileLog.write("[");
    m_fileLog.write(QDateTime::currentDateTime().date().toString(QString("dd:MM:yyyy")).toUtf8() + QString(" ").toUtf8());
    m_fileLog.write(QDateTime::currentDateTime().time().toString(QString("hh:mm:ss")).toUtf8());
    m_fileLog.write("] ");
    if(MsgType::debugMsg == typeMessage)
        m_fileLog.write("[Debug] ");
    if(MsgType::warningMsg == typeMessage)
        m_fileLog.write("[Warning] ");
    if(MsgType::infoMsg == typeMessage)
        m_fileLog.write("[Info] ");
    if(MsgType::fatalMsg == typeMessage)
        m_fileLog.write("[Fatal] ");
    if(MsgType::systemMsg == typeMessage)
    {
        m_fileLog.write("[System] ");
        m_fileLog.write(message.toUtf8()+"\n");
        m_fileLog.write("\n Application ABORT");
        m_fileLog.flush();
        m_fileLog.close();
        abort();
    }

    m_fileLog.write(message.toUtf8()+"\n");
}

void Logger::InfoLogFile(const QString& message)
{
    instance()->messageHandler(MsgType::infoMsg, message);
}

void Logger::DebugLogFile(const QString& message)
{
    instance()->messageHandler(MsgType::debugMsg, message);
}

void Logger::FatalLogFile(const QString& message)
{
    instance()->messageHandler(MsgType::fatalMsg, message);
}

void Logger::WarningLogFile(const QString& message)
{
    instance()->messageHandler(MsgType::warningMsg, message);
}

void Logger::CriticalLogFile(const QString& message)
{
    instance()->messageHandler(MsgType::criticalMsg, message);
}

void Logger::SystemLogFile(const QString& message)
{
    instance()->messageHandler(MsgType::systemMsg, message);
}

void Logger::InfoLogConsole(const QString& message)
{
    instance()->messageHandler(MsgType::infoMsg, message, LogMode::Console);
}

void Logger::DebugLogConsole(const QString& message)
{
    instance()->messageHandler(MsgType::debugMsg, message, LogMode::Console);
}

void Logger::FatalLogConsole(const QString& message)
{
    instance()->messageHandler(MsgType::fatalMsg, message, LogMode::Console);
}

void Logger::WarningLogConsole(const QString& message)
{
    instance()->messageHandler(MsgType::warningMsg, message, LogMode::Console);
}

void Logger::CriticalLogConsole(const QString& message)
{
    instance()->messageHandler(MsgType::criticalMsg, message, LogMode::Console);
}

void Logger::SystemLogConsole(const QString& message)
{
    instance()->messageHandler(MsgType::systemMsg, message, LogMode::Console);
}
