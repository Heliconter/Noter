#ifndef NOTE_TITLE_LIST_MODEL_H
#define NOTE_TITLE_LIST_MODEL_H

#include <QAbstractListModel>

class NoteDbAgent;

class NoteTitleListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit NoteTitleListModel(QObject *parent = nullptr);
    ~NoteTitleListModel();

    void setAgent(const NoteDbAgent *dbAgent);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
    void emitAllDataChanged();

private:
    const NoteDbAgent *m_dbAgent = nullptr;
};

#endif // NOTE_TITLE_LIST_MODEL_H
