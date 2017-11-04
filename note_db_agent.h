#ifndef NOTE_DB_AGENT_H
#define NOTE_DB_AGENT_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class NoteDbAgent : public QObject
{
    Q_OBJECT
public:
    explicit NoteDbAgent(QObject *parent = nullptr);
    ~NoteDbAgent();
    NoteDbAgent(const NoteDbAgent &) = delete;
    void operator=(const NoteDbAgent &) = delete;

    int noteCount() const;
    int getNoteId(int index) const;

    QString getNoteTitle(int id) const;
    QString getNoteContent(int id) const;

    void updateNote(int id, QString title, QString content);

    int addNote(QString title, QString content);
    void deleteNote(int id);

signals:
    void dataChanged();

private:
    QSqlQuery execQuery(const QString &query) const;

private:
    QSqlDatabase m_db;
};

#endif // NOTE_DB_AGENT_H
