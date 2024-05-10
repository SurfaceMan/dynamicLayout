#include "windowfactory.h"

WindowFactory *WindowFactory::Instance() {
    static WindowFactory factory;
    return &factory;
}

namespace {
QMap<QString, WindowFactory::WindowCreator> WindowCreatorMap;
}

void WindowFactory::registerWindow(const QString &typeName, const WindowCreator &creator) {
    WindowCreatorMap[ typeName ] = creator;
}

DynamicWindow *WindowFactory::createWindow(const QString &typeName, QWidget *parent) const {
    auto iter = WindowCreatorMap.find(typeName);
    if (WindowCreatorMap.end() == iter) {
        return nullptr;
    }

    auto *window = iter.value()();
    if (window) {
        window->setParent(parent);
    }

    return window;
}

QStringList WindowFactory::availableWindowTypes() const {
    return WindowCreatorMap.keys();
}

WindowFactory::WindowFactory()  = default;
WindowFactory::~WindowFactory() = default;
