#ifndef NOTER_H
#define NOTER_H

#include <QMainWindow>

class NoteDbAgent;
class NoteTitleListModel;

namespace Ui
{
class Noter;
}

class Noter : public QMainWindow
{
    Q_OBJECT
public:
    explicit Noter(QWidget *parent = nullptr);
    ~Noter();

private slots:
    void closeOpenedNote();
    void openNote(int id);

    void addNote();
    void deleteSelectedNote();

private:
    Ui::Noter *m_ui;

    NoteDbAgent *m_agent;
    NoteTitleListModel *m_model;

    bool m_hasOpenedNote = false;
    int m_openedNoteId;
};

#endif // NOTER_H
