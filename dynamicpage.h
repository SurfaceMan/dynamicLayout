#pragma once

#include "dynamicwindow.h"

#include <QAction>
#include <QSplitter>
#include <QWidget>

class DynamicPage : public QWidget {
    Q_OBJECT
public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    explicit DynamicPage(QWidget *parent = nullptr);
    ~DynamicPage() override;

    void closePage();

    QString name() const;
    void    setName(const QString &name);

    QToolBar *toolBar() const;

signals:
    void nameChanged(const QString &name);

public slots:
    void splitHorizontal();
    void splitVertical();
    void closeWindow();
    void switchWindowSize();
    void fillWindow();
    void restoreWindow();
    void createWindow(const QString &typeName);
    bool eventFilter(QObject *watched, QEvent *event) override;
    void renamePage();

private:
    QWidget *initCreatorWindow(QWidget *parent = nullptr);

private:
    QString   m_name;
    QToolBar *m_toolBar               = nullptr;
    QAction  *m_closeAction           = nullptr;
    QAction  *m_fullScreenAction      = nullptr;
    QAction  *m_resetScreenAction     = nullptr;
    QAction  *m_verticalSplitAction   = nullptr;
    QAction  *m_horizontalSplitAction = nullptr;
    QAction  *m_separator             = nullptr;
    QAction  *m_subToolBar            = nullptr;

    QMap<QToolBar *, QAction *> m_subToolBars;

    enum Mode { FULL_SCREEN, NORMAL } m_mode = NORMAL;
    struct State {
        QWidget   *topWidget = nullptr;
        QSplitter *parent    = nullptr;
        QWidget   *current   = nullptr;
        int        index     = -1;
    } m_state;

    int       m_index        = 0;
    const int m_iconFontSize = 11;
};
