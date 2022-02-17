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
        system(QString("mkdir -p %1").arg(path).toUtf8().data());
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
    return (m_fileLog.open(QIODevice::WriteOnly));
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

QString Logger::makeMessage(MsgType typeMessage, const QString& message) const
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
    if((MsgType::criticalMsg == typeMessage) || (MsgType::systemMsg == typeMessage))
        messageType = "[System] ";
    
    return QString("[%1 %2] %3\n\t\t function name: [%4]\n\t\t description: %5\n")
            .arg(QDateTime::currentDateTime().date().toString(QString("dd:MM:yyyy")))
            .arg(QDateTime::currentDateTime().time().toString(QString("hh:mm:ss")))
            .arg(messageType)
            .arg(__func__)
            .arg(message);
}

void Logger::writeToConsole(MsgType typeMessage, const QString& message)
{
    qDebug() << makeMessage(typeMessage, message);
    if((MsgType::fatalMsg || MsgType::criticalMsg == typeMessage || MsgType::systemMsg == typeMessage) == typeMessage)
        throw "Application aborted!!!";
}

void Logger::writeToFile(MsgType typeMessage, const QString& message)
{
    if(!m_fileLog.isOpen())
    {
        if(!openFile())
            return;
    }
    m_fileLog.write(makeMessage(typeMessage, message).toUtf8());
    if((MsgType::fatalMsg || MsgType::criticalMsg == typeMessage || MsgType::systemMsg == typeMessage) == typeMessage)
    {
        m_fileLog.flush();
        m_fileLog.close();
        throw "Application aborted!!!";
    }
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
