#pragma once

#include <QTabWidget>
#include <QWidget>

class DynamicLayout : public QWidget {
    Q_OBJECT
public:
    explicit DynamicLayout(QWidget *parent = nullptr);
    ~DynamicLayout() override;

private slots:
    void pageChanged(int index);
    void closePage();
    void renamePageRequest(int index);
    void renamePage(const QString &name);
    void addCloseButton(int index);

private:
    QTabWidget *m_tabLayout = nullptr;

    int m_index = 0;
};
