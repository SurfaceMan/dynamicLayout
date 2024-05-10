#pragma once

#include <QDialog>
#include <QLineEdit>

class RenameDialog : public QDialog {
    Q_OBJECT
public:
    explicit RenameDialog(QWidget *parent = nullptr, Qt::WindowFlags flag = Qt::WindowFlags());
    ~RenameDialog() override;

    QString name() const;

    static QString getName(const QString &title, QWidget *parent = nullptr,
                           Qt::WindowFlags flag = Qt::WindowFlags());

private:
    QLineEdit *m_editor = nullptr;
};
