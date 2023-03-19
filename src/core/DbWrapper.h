#pragma once

#include <QSql>
#include <QSqlDatabase>

namespace ColorLines
{

class DbWrapper
{
public:
    DbWrapper();
    ~DbWrapper();

    int getPrevScores() const;
    QByteArray getPrevFieldState() const;

    void saveState(int scores, const QByteArray& fieldState);

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

    void connectToDB();

    QString getDefaultDbPath() const;

private:
    const QLatin1Literal cDbName{ "ColorLines.db" };
    const QLatin1Literal cDbHostname{ "ColorLines" };
    const QLatin1Literal cScoresParamName{ "Scores" };
    const QLatin1Literal cFieldStateParamName{ "FieldState" };

    QSqlDatabase db_;
};

}
