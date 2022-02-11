# loggingLibrary

## Библиотека логирования работы программы
    Библиотека предоставляет возможность записи в файл или печати в консоль в зависимости
    от типа события.


### Методы доступа к записи в один из потоков вывода (файл или консоль):
    -- InfoLog_T_(const QString& message)
    -- DebugLog_T_(const QString& message)
    -- FatalLog_T_(const QString& message)
    -- WarningLog_T_(const QString& message)
    -- CriticalLog_T_(const QString& message)
    -- SystemLog_T_(const QString& message)
    где _T_ - File, message будет записано в файл,
            - Console, message будет выведено сообщение в консоль.
