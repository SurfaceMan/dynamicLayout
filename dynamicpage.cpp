#include "dynamicpage.h"
#include "creatorwindow.h"
#include "iconfont.h"
#include "renamedialog.h"
#include "windowfactory.h"

#include <QEvent>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QSplitter>

DynamicPage::DynamicPage(QWidget *parent)
    : QWidget(parent) {
    // icon
    auto  fontId       = QFontDatabase::addApplicationFont(":/iconfont.ttf");
    auto  fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    auto  fontName     = fontFamilies[ 0 ];
    QFont font(fontName, m_iconFontSize);

    m_toolBar = new QToolBar(this);
    m_toolBar->setFont(font);
    m_toolBar->setIconSize(QSize{12, 12});
    m_separator = m_toolBar->addSeparator();
    m_horizontalSplitAction =
        m_toolBar->addAction(QChar(IconFont::H_LAYOUT), this, &DynamicPage::splitHorizontal);
    m_verticalSplitAction =
        m_toolBar->addAction(QChar(IconFont::V_LAYOUT), this, &DynamicPage::splitVertical);
    m_fullScreenAction =
        m_toolBar->addAction(QChar(IconFont::FULL_SCREEN), this, &DynamicPage::fillWindow);
    m_resetScreenAction =
        m_toolBar->addAction(QChar(IconFont::RESTORE_SCREEN), this, &DynamicPage::restoreWindow);
    m_closeAction = m_toolBar->addAction(QChar(IconFont::CLOSE), this, &DynamicPage::closeWindow);
    m_separator->setVisible(false);

    m_horizontalSplitAction->setToolTip(tr("水平分割"));
    m_verticalSplitAction->setToolTip(tr("垂直分割"));
    m_fullScreenAction->setToolTip(tr("全屏"));
    m_resetScreenAction->setToolTip(tr("恢复"));
    m_resetScreenAction->setVisible(false);
    m_closeAction->setToolTip(tr("关闭"));

    auto *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(initCreatorWindow());

    setLayout(layout);
}

DynamicPage::~DynamicPage() = default;

void DynamicPage::closePage() {
    auto windows = findChildren<DynamicWindow *>();
    for (const auto &window : windows) {
        window->closeWindow();
    }
}

QString DynamicPage::name() const {
    return m_name;
}

void DynamicPage::setName(const QString &name) {
    m_name = name;
    emit nameChanged(name);
}

QToolBar *DynamicPage::toolBar() const {
    return m_toolBar;
}

void DynamicPage::splitHorizontal() {
    auto *widget = focusWidget();
    if (nullptr == widget ||
        (!dynamic_cast<DynamicWindow *>(widget) && !dynamic_cast<CreatorWindow *>(widget))) {
        return;
    }

    auto *parentWidget  = widget->parentWidget();
    auto *childSplitter = new QSplitter(Qt::Horizontal);

    if (nullptr != dynamic_cast<QSplitter *>(parentWidget)) {
        auto *parentSplitter = qobject_cast<QSplitter *>(parentWidget);
        auto  index          = parentSplitter->indexOf(widget);
        parentSplitter->replaceWidget(index, childSplitter);
    } else {
        parentWidget->layout()->replaceWidget(widget, childSplitter);
    }

    childSplitter->addWidget(widget);
    childSplitter->addWidget(initCreatorWindow());

    childSplitter->setSizes({INT32_MAX, INT32_MAX});
}

void DynamicPage::splitVertical() {
    auto *widget = focusWidget();
    if (nullptr == widget ||
        (!dynamic_cast<DynamicWindow *>(widget) && !dynamic_cast<CreatorWindow *>(widget))) {
        return;
    }

    auto *parentWidget  = widget->parentWidget();
    auto *childSplitter = new QSplitter(Qt::Vertical);

    if (nullptr != dynamic_cast<QSplitter *>(parentWidget)) {
        auto *parentSplitter = qobject_cast<QSplitter *>(parentWidget);
        auto  index          = parentSplitter->indexOf(widget);
        parentSplitter->replaceWidget(index, childSplitter);
    } else {
        parentWidget->layout()->replaceWidget(widget, childSplitter);
    }

    childSplitter->addWidget(widget);
    childSplitter->addWidget(initCreatorWindow());

    childSplitter->setSizes({INT32_MAX, INT32_MAX});
}

void DynamicPage::closeWindow() {
    auto *widget = focusWidget();
    if (nullptr == widget ||
        (!dynamic_cast<DynamicWindow *>(widget) && !dynamic_cast<CreatorWindow *>(widget))) {
        return;
    }

    if (nullptr == dynamic_cast<QSplitter *>(widget->parentWidget())) {
        return;
    }

    auto *splitter       = qobject_cast<QSplitter *>(widget->parentWidget());
    auto *parentSplitter = qobject_cast<QSplitter *>(widget->parentWidget()->parentWidget());
    auto *oldWidget      = splitter->replaceWidget(!splitter->indexOf(widget), new QWidget);
    if (nullptr != parentSplitter) {
        parentSplitter->replaceWidget(parentSplitter->indexOf(splitter), oldWidget);
    } else {
        widget->parentWidget()->parentWidget()->layout()->replaceWidget(splitter, oldWidget);
    }

    auto *window = dynamic_cast<DynamicWindow *>(widget);
    if (nullptr != window) {
        window->closeWindow();
        auto found = m_subToolBars.find(window->toolBar());
        if (m_subToolBars.end() != found) {
            if (m_subToolBar == found.value()) {
                m_subToolBar = nullptr;
            }

            m_toolBar->removeAction(found.value());
            m_subToolBars.remove(found.key());
        }
    }

    delete splitter;
}

void DynamicPage::switchWindowSize() {
    m_mode == NORMAL ? fillWindow() : restoreWindow();
}

void DynamicPage::fillWindow() {
    auto *widget = focusWidget();
    if (nullptr == widget ||
        (!dynamic_cast<DynamicWindow *>(widget) && !dynamic_cast<CreatorWindow *>(widget))) {
        return;
    }

    if (nullptr == dynamic_cast<QSplitter *>(widget->parentWidget()) || nullptr == layout()) {
        return;
    }

    auto *topSplitter = this->findChild<QSplitter *>();
    if (nullptr == topSplitter) {
        return;
    }

    auto *parentSplitter = qobject_cast<QSplitter *>(widget->parentWidget());
    auto  index          = parentSplitter->indexOf(widget);
    parentSplitter->replaceWidget(index, new QWidget);
    layout()->replaceWidget(topSplitter, widget);
    topSplitter->setVisible(false);

    m_mode  = FULL_SCREEN;
    m_state = {topSplitter, parentSplitter, widget, index};

    // switch toolbar
    m_closeAction->setVisible(false);
    m_fullScreenAction->setVisible(false);
    m_resetScreenAction->setVisible(true);
    m_verticalSplitAction->setVisible(false);
    m_horizontalSplitAction->setVisible(false);
}

void DynamicPage::restoreWindow() {
    if (FULL_SCREEN != m_mode) {
        return;
    }

    m_state.topWidget->setVisible(true);
    layout()->replaceWidget(m_state.current, m_state.topWidget);

    auto *empty = m_state.parent->replaceWidget(m_state.index, m_state.current);
    delete empty;

    m_mode  = NORMAL;
    m_state = {};

    // switch toolbar
    m_closeAction->setVisible(true);
    m_fullScreenAction->setVisible(true);
    m_resetScreenAction->setVisible(false);
    m_verticalSplitAction->setVisible(true);
    m_horizontalSplitAction->setVisible(true);
}

void DynamicPage::createWindow(const QString &typeName) {
    auto *window = WindowFactory::Instance()->createWindow(typeName, nullptr);
    if (nullptr == window) {
        return;
    }

    auto *widget = dynamic_cast<CreatorWindow *>(sender());
    if (nullptr == widget) {
        delete window;
        return;
    }

    const auto windowName = tr("窗口#%1").arg(m_index++);
    window->setName(windowName);

    auto *parentWidget = widget->parentWidget();
    // window->setParent(parentWidget);
    if (nullptr != dynamic_cast<QSplitter *>(parentWidget)) {
        auto *parentSplitter = qobject_cast<QSplitter *>(parentWidget);
        auto  index          = parentSplitter->indexOf(widget);
        parentSplitter->replaceWidget(index, window);
    } else {
        parentWidget->layout()->replaceWidget(widget, window);
    }

    delete widget;

    window->installEventFilter(this);
}

bool DynamicPage::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::FocusIn) {
        if (nullptr != m_subToolBar) {
            m_subToolBar->setVisible(false);
            m_separator->setVisible(false);
        }

        auto *window = dynamic_cast<DynamicWindow *>(watched);
        if (nullptr != window) {
            // set sub toolbar
            auto found = m_subToolBars.find(window->toolBar());
            if (m_subToolBars.end() == found) {
                window->toolBar()->setVisible(true);
                m_subToolBar = m_toolBar->insertWidget(m_separator, window->toolBar());
                m_subToolBars[ window->toolBar() ] = m_subToolBar;
            } else {
                m_subToolBar = found.value();
                m_subToolBar->setVisible(true);
            }

            m_separator->setVisible(true);
        }
    }

    return QWidget::eventFilter(watched, event);
}

QWidget *DynamicPage::initCreatorWindow(QWidget *parent) {
    auto *creatorWindow = new CreatorWindow(parent);
    creatorWindow->installEventFilter(this);
    connect(creatorWindow, &CreatorWindow::clickedWindowType, this, &DynamicPage::createWindow);

    return creatorWindow;
}

void DynamicPage::renamePage() {
    auto pageName = RenameDialog::getName(tr("修改名字..."), this);
    if (pageName.isEmpty()) {
        return;
    }

    setName(pageName);
}
