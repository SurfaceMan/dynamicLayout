#include "dynamicwindow.h"
#include "renamedialog.h"

DynamicWindow::DynamicWindow(QWidget *parent)
    : QFrame(parent) {
    m_toolBar = new QToolBar(this);
    m_toolBar->setVisible(false);
    setFocusPolicy(Qt::ClickFocus);

    m_nameAction = m_toolBar->addAction("&" + name(), this, &DynamicWindow::renameWindow);
    connect(this, &DynamicWindow::nameChanged, this,
            [ this ](const QString &name) { m_nameAction->setText("&" + name); });

    m_cachedStyle = styleSheet();
    setObjectName("DynamicWindow");
}

DynamicWindow::~DynamicWindow() = default;

void DynamicWindow::closeWindow() {}

QString DynamicWindow::name() const {
    return m_name;
}

void DynamicWindow::setName(const QString &name) {
    m_name = name;
    emit nameChanged(name);
}

QToolBar *DynamicWindow::toolBar() const {
    return m_toolBar;
}

void DynamicWindow::focusInEvent(QFocusEvent *event) {
    // highlight current window
    setStyleSheet("#DynamicWindow{border:1px solid blue}");

    QWidget::focusInEvent(event);
}

void DynamicWindow::focusOutEvent(QFocusEvent *event) {
    setStyleSheet(m_cachedStyle);

    QWidget::focusOutEvent(event);
}

void DynamicWindow::renameWindow() {
    auto windowName = RenameDialog::getName(tr("修改名字..."), this);
    if (windowName.isEmpty()) {
        return;
    }

    setName(windowName);
}