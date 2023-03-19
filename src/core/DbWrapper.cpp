#include "DbWrapper.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

#include <QDebug>

using namespace ColorLines;

DbWrapper::DbWrapper()
{
    connectToDB();
}

DbWrapper::~DbWrapper()
{
    closeDataBase();
}

int DbWrapper::getPrevScores() const
{
    if (!db_.isOpen())
    {
        return 0;
    }

    QSqlQuery query(db_);
    if (!query.exec( "SELECT " + cScoresParamName + " from " + cDbHostname ))
    {
        qDebug() << "Error getting " + cScoresParamName + " from table:\n" << query.lastError();
        return 0;
    }
    query.last();
    return query.value(0).toInt();
}

QByteArray DbWrapper::getPrevFieldState() const
{
    if (!db_.isOpen())
    {
        return {};
    }

    QSqlQuery query(db_);
    if (!query.exec("SELECT " + cFieldStateParamName + " from " + cDbHostname))
    {
        qDebug() << "Error getting " + cScoresParamName + " from table:\n" << query.lastError();
        return {};
    }
    query.last();
    return query.value(0).toByteArray();
}

void DbWrapper::saveState(int scores, const QByteArray &fieldState)
{
    if (!db_.isOpen())
    {
        return;
    }

    QSqlQuery query(db_);
    query.prepare("INSERT INTO " + cDbHostname + " (" + cScoresParamName + ", " + cFieldStateParamName + ") VALUES (:scores, :fieldState)");
    query.bindValue(":scores", scores);
    query.bindValue(":fieldState", fieldState);
    if (!query.exec())
    {
        qDebug() << "Error inserting state into table:\n" << query.lastError();
    }
}

bool DbWrapper::openDataBase()
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setHostName(cDbHostname);
    db_.setDatabaseName(getDefaultDbPath());
    return db_.open();
}

bool DbWrapper::restoreDataBase()
{
    if (this->openDataBase())
    {
        return createTable() ? true : false;
    }
    else
    {
        return false;
    }
    return false;
}

void DbWrapper::closeDataBase()
{
    db_.close();
}

bool DbWrapper::createTable()
{
    QSqlQuery query;
    if (!query.exec( "CREATE TABLE ColorLines ("\
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "\
                            + cScoresParamName + " INTEGER,"\
                            + cFieldStateParamName + " BLOB"\
                        " )"
                    ))
    {
        return false;
    }
    else
    {
        return true;
    }
    return false;
}

void DbWrapper::connectToDB()
{
    if (!QFile(getDefaultDbPath()).exists())
    {
        restoreDataBase();
    }
    else
    {
        openDataBase();
    }
}

QString DbWrapper::getDefaultDbPath() const
{
    // qDebug() << QDir::homePath() + QDir::separator() + QCoreApplication::applicationName() + QDir::separator() + cDbName;
    return cDbName;
}
