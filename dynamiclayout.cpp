#include "dynamiclayout.h"
#include "dynamicpage.h"
#include "renamedialog.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QStyle>
#include <QTabBar>

DynamicLayout::DynamicLayout(QWidget *parent)
    : QWidget(parent) {

    m_tabLayout = new QTabWidget(this);
    connect(m_tabLayout, &QTabWidget::currentChanged, this, &DynamicLayout::pageChanged);
    connect(m_tabLayout, &QTabWidget::tabBarDoubleClicked, this, &DynamicLayout::renamePageRequest);

    auto *page = new DynamicPage(this);
    connect(page, &DynamicPage::nameChanged, this, &DynamicLayout::renamePage);
    m_tabLayout->addTab(page, "");
    page->setName(tr("布局#%1").arg(m_index++));
    addCloseButton(0);

    auto *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_tabLayout);

    setLayout(layout);
}

DynamicLayout::~DynamicLayout() = default;

void DynamicLayout::pageChanged(int index) {
    m_tabLayout->setCornerWidget(nullptr);

    auto count = m_tabLayout->count();
    if (index == count - 1) {
        auto *newPage = new DynamicPage(this);
        connect(newPage, &DynamicPage::nameChanged, this, &DynamicLayout::renamePage);
        m_tabLayout->addTab(newPage, "");
        newPage->setName("+");

        const auto pageName = tr("布局#%1").arg(m_index++);
        auto      *page     = dynamic_cast<DynamicPage *>(m_tabLayout->widget(index));
        if (nullptr != page) {
            page->setName(pageName);
        }

        addCloseButton(index);
    }

    auto *page = dynamic_cast<DynamicPage *>(m_tabLayout->widget(index));
    if (nullptr == page) {
        return;
    }
    auto *newTool = page->toolBar();
    m_tabLayout->setCornerWidget(newTool);
    newTool->show();
}

void DynamicLayout::closePage() {
    int index = -1;
    for (int i = 0; i < m_tabLayout->count(); i++) {
        if (sender() != m_tabLayout->tabBar()->tabButton(i, QTabBar::RightSide)) {
            continue;
        }

        index = i;
        break;
    }

    if (-1 == index) {
        return;
    }

    auto *page = dynamic_cast<DynamicPage *>(m_tabLayout->widget(index));
    if (nullptr != page) {
        page->closePage();
    }

    if (m_tabLayout->count() >= 3 && index == m_tabLayout->count() - 2) {
        m_tabLayout->setCurrentIndex(m_tabLayout->count() - 3);
    }

    m_tabLayout->removeTab(index);
}

void DynamicLayout::renamePageRequest(int index) {
    auto *page = dynamic_cast<DynamicPage *>(m_tabLayout->widget(index));
    if (nullptr == page) {
        return;
    }

    page->renamePage();
}

void DynamicLayout::renamePage(const QString &name) {
    int index = -1;
    for (int i = 0; i < m_tabLayout->count(); i++) {
        if (sender() != m_tabLayout->widget(i)) {
            continue;
        }

        index = i;
        break;
    }

    if (-1 == index) {
        return;
    }

    m_tabLayout->setTabText(index, name);
}

void DynamicLayout::addCloseButton(int index) {
    auto *closeButton = new QPushButton(this);
    closeButton->setToolTip(tr("关闭布局"));
    closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    closeButton->setFlat(true);
    closeButton->setFixedWidth(16);
    m_tabLayout->tabBar()->setTabButton(index, QTabBar::RightSide, closeButton);

    connect(closeButton, &QPushButton::clicked, this, &DynamicLayout::closePage);
}
