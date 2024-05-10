#include "renamedialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

RenameDialog::RenameDialog(QWidget *parent, Qt::WindowFlags flag)
    : QDialog(parent, flag) {
    setFixedHeight(120);

    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *label = new QLabel(tr("名字:"), this);
    m_editor    = new QLineEdit(this);
    auto *button =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    button->setCenterButtons(true);
    connect(button, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(label);
    layout->addWidget(m_editor);
    layout->addWidget(button);

    setLayout(layout);
}

RenameDialog::~RenameDialog() = default;

QString RenameDialog::name() const {
    return m_editor->text();
}

QString RenameDialog::getName(const QString &title, QWidget *parent, Qt::WindowFlags flag) {
    RenameDialog dialog(parent, flag);
    dialog.setWindowTitle(title);

    return QDialog::Rejected == dialog.exec() ? QString{} : dialog.name();
}
