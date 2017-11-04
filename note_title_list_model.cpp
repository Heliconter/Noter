#include "note_title_list_model.h"
#include <note_db_agent.h>
#include <QDebug>
#include <QSqlError>

NoteTitleListModel::NoteTitleListModel(QObject *parent)
    : QAbstractListModel(parent)
{}

NoteTitleListModel::~NoteTitleListModel()
{}

void NoteTitleListModel::setAgent(const NoteDbAgent *dbAgent)
{
    if (m_dbAgent)
        disconnect(m_dbAgent, &NoteDbAgent::dataChanged, this, &NoteTitleListModel::emitAllDataChanged);
    m_dbAgent = dbAgent;
    connect(m_dbAgent, &NoteDbAgent::dataChanged, this, &NoteTitleListModel::emitAllDataChanged);
}

int NoteTitleListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_dbAgent->noteCount();
}

QVariant NoteTitleListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::ItemDataRole::DisplayRole && index.isValid())
    {
//        QSqlQuery result = execQuery("SELECT title FROM notes;");
//        result.seek(index.row());
//        QVariant ttle = result.value(0);
        QVariant title = m_dbAgent->getNoteTitle(m_dbAgent->getNoteId(index.row()));
        return title;
    }
    return QVariant();
}

void NoteTitleListModel::emitAllDataChanged()
{
    emit dataChanged(index(0), index(rowCount() - 1));
}
