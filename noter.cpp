#include "noter.h"
#include "ui_noter.h"
#include <note_db_agent.h>
#include "note_title_list_model.h"

Noter::Noter(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::Noter),
      m_agent(new NoteDbAgent(this)),
      m_model(new NoteTitleListModel(this))
{
    m_model->setAgent(m_agent);

    m_ui->setupUi(this);
    m_ui->editPanel->setDisabled(true);
    m_ui->deleteButton->setDisabled(true);
    m_ui->listView->setModel(m_model);

    QObject::connect(m_ui->listView, &QListView::clicked, [this] (const QModelIndex &index) {
        this->openNote(m_agent->getNoteId(index.row()));
    });
    QObject::connect(m_ui->closeButton, &QPushButton::clicked, this, &Noter::closeOpenedNote);
    QObject::connect(m_ui->addButton, &QPushButton::clicked, this, &Noter::addNote);
    QObject::connect(m_ui->deleteButton, &QPushButton::clicked, this, &Noter::deleteSelectedNote);
    QObject::connect(m_ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged, [this]
    {
        QItemSelectionModel *selection = this->m_ui->listView->selectionModel();
        this->m_ui->deleteButton->setEnabled(selection->hasSelection());
    });
}

Noter::~Noter()
{
    delete m_ui;
}

void Noter::closeOpenedNote()
{
    if (m_hasOpenedNote)
    {
        m_agent->updateNote(m_openedNoteId, m_ui->noteTitle->text(), m_ui->noteContent->toPlainText());
        m_hasOpenedNote = false;
    }
    m_ui->editPanel->setDisabled(true);
    m_ui->noteTitle->clear();
    m_ui->noteContent->clear();
}

void Noter::openNote(int id)
{
    closeOpenedNote();
    m_ui->editPanel->setEnabled(true);

    m_hasOpenedNote = true;
    m_openedNoteId = id;

    QString title = m_agent->getNoteTitle(m_openedNoteId);
    QString content = m_agent->getNoteContent(m_openedNoteId);

    m_ui->noteTitle->setText(title);
    m_ui->noteContent->setText(content);
}

void Noter::addNote()
{
    int id = m_agent->appendNote("Новая заметка", "");
    m_ui->listView->setCurrentIndex(m_model->index(m_agent->noteCount() - 1));
    openNote(id);
}

void Noter::deleteSelectedNote()
{
    QModelIndex selectModelIndex = m_ui->listView->currentIndex();
    if (selectModelIndex.isValid())
    {
        int selectId = m_agent->getNoteId(selectModelIndex.row());
        if (m_hasOpenedNote && selectId == m_openedNoteId)
            closeOpenedNote();
        m_agent->deleteNote(selectId);

        m_ui->listView->setCurrentIndex(m_model->index(selectModelIndex.row() - 1));
    }
}
