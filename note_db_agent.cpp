#include "note_db_agent.h"
#include <QStringList>
#include <QDebug>
#include <QSqlError>

NoteDbAgent::NoteDbAgent(QObject *parent)
    : QObject(parent)
{
    QString dbDriverName = "QSQLITE";
    QString connectionName = "noteDbConnection";
    if (QSqlDatabase::isDriverAvailable(dbDriverName))
    {
        if (!QSqlDatabase::connectionNames().contains(connectionName))
        {
            m_db = QSqlDatabase::addDatabase(dbDriverName, connectionName);
            m_db.setDatabaseName("notes.sqlite");
            if (m_db.open())
            {
                if (!m_db.tables().contains("notes"))
                    execQuery("CREATE TABLE notes(id INTEGER PRIMARY KEY, title TEXT, content TEXT);");
                if (!m_db.tables().contains("labels"))
                    execQuery("CREATE TABLE labels(id INTEGER PRIMARY KEY, name TEXT);");
                m_db.transaction(); //execQuery("BEGIN;");
            }
            else
                qWarning() << "error: opening of database failed: " << m_db.lastError();
        }
        else
            qWarning() << "error: " + connectionName + "already exist.";
    }
    else
        qWarning() << "error: " + dbDriverName + " driver is unavailable.";
}

NoteDbAgent::~NoteDbAgent()
{
    m_db.commit(); //execQuery("COMMIT;");
    m_db.close();
    QSqlDatabase::removeDatabase(m_db.connectionName());
}

int NoteDbAgent::noteCount() const
{
    QSqlQuery result = execQuery("SELECT COUNT(id) FROM notes;");
    result.first();
    int count = result.value(0).toInt();
    return count;
}

int NoteDbAgent::getNoteId(int index) const
{
    QSqlQuery result = execQuery("SELECT id FROM notes;");
    result.seek(index);
    int noteId = result.value(0).toInt();
    return noteId;
}

QString NoteDbAgent::getNoteTitle(int id) const
{
    QSqlQuery result = execQuery(QString("SELECT title FROM notes WHERE id = %1;").arg(id));
    result.first();
    return result.value(0).toString();
}

QString NoteDbAgent::getNoteContent(int id) const
{
    QSqlQuery result = execQuery(QString("SELECT content FROM notes WHERE id = %1;").arg(id));
    result.first();
    return result.value(0).toString();
}

void NoteDbAgent::updateNote(int id, QString title, QString content)
{
    execQuery(QString("UPDATE notes SET title = '%1' WHERE id = %2;").arg(title).arg(id));
    execQuery(QString("UPDATE notes SET content = '%1' WHERE id = %2;").arg(content).arg(id));
    emit dataChanged();
}

int NoteDbAgent::appendNote(QString title, QString content)
{
    execQuery(QString("INSERT INTO notes (title, content) VALUES ('%1', '%2');").arg(title).arg(content));
    QSqlQuery result = execQuery("SELECT LAST_INSERT_ROWID();");
    result.first();
    int id = result.value(0).toInt();
    emit dataChanged();
    return id;
}

void NoteDbAgent::deleteNote(int id)
{
    execQuery(QString("DELETE FROM notes WHERE id = %1;").arg(id));
    emit dataChanged();
}

QSqlQuery NoteDbAgent::execQuery(const QString &query) const
{
    QSqlQuery q(m_db);
    if (!q.exec(query))
        qWarning() << "error: query error: " << q.lastError();
    return q;
}
