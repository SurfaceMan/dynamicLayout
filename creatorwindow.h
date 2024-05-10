#pragma once

#include <QFrame>

class CreatorWindow : public QFrame {
    Q_OBJECT
public:
    explicit CreatorWindow(QWidget *parent = nullptr);
    ~CreatorWindow() override;

signals:
    void clickedWindowType(const QString &typeName);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    QString m_cachedStyle;
};
