#ifndef NOTE_MODEL_H
#define NOTE_MODEL_H

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

signals:

public slots:
    void emitAllDataChanged();

private:

private:
    const NoteDbAgent *m_dbAgent = nullptr;
};

#endif // NOTE_MODEL_H
