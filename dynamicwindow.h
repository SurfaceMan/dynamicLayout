#pragma once

#include <QFrame>
#include <QToolBar>

class DynamicWindow : public QFrame {
    Q_OBJECT
public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    explicit DynamicWindow(QWidget *parent = nullptr);
    ~DynamicWindow() override;

    virtual void closeWindow();

    QString name() const;
    void    setName(const QString &name);

    QToolBar *toolBar() const;

signals:
    void nameChanged(const QString &name);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    void renameWindow();

private:
    QString   m_name       = "window";
    QToolBar *m_toolBar    = nullptr;
    QAction  *m_nameAction = nullptr;

    QString m_cachedStyle;
};
