#pragma once

#include "dynamicwindow.h"

class WindowFactory {
public:
    using WindowCreator = std::function<DynamicWindow *()>;

    static WindowFactory *Instance();
    void                  registerWindow(const QString &typeName, const WindowCreator &creator);
    DynamicWindow        *createWindow(const QString &typeName, QWidget *parent) const;
    QStringList           availableWindowTypes() const;

private:
    WindowFactory();
    ~WindowFactory();
};
