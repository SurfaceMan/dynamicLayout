#include "creatorwindow.h"
#include "windowfactory.h"

#include <QFocusFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>

CreatorWindow::CreatorWindow(QWidget *parent)
    : QFrame(parent) {
    setFocusPolicy(Qt::ClickFocus);

    auto *hLayout = new QHBoxLayout(this);
    hLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));

    auto *vLayout = new QVBoxLayout();
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    auto types = WindowFactory::Instance()->availableWindowTypes();
    for (auto &type : types) {
        auto *btn = new QPushButton(type, this);
        vLayout->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, [ btn, this ]() {
            auto clickedTypeName = btn->text();
            emit clickedWindowType(clickedTypeName);
        });
    }

    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    hLayout->addLayout(vLayout);
    hLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
    setLayout(hLayout);

    m_cachedStyle = styleSheet();
    setObjectName("CreatorWindow");
}

CreatorWindow::~CreatorWindow() = default;

void CreatorWindow::focusInEvent(QFocusEvent *event) {
    // highlight current window
    setStyleSheet("#CreatorWindow{border:1px solid blue}");

    QWidget::focusInEvent(event);
}

void CreatorWindow::focusOutEvent(QFocusEvent *event) {
    setStyleSheet(m_cachedStyle);

    QWidget::focusOutEvent(event);
}
