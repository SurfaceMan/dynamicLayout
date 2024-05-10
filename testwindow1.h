#pragma once

#include "dynamicwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class TestWindow1 : public DynamicWindow {
public:
    explicit TestWindow1(QWidget *parent = nullptr)
        : DynamicWindow(parent) {

        auto *label = new QLabel("aabbcc", this);

        auto *layout = new QHBoxLayout(this);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(label);

        setLayout(layout);

        toolBar()->addAction("test1");
    }
};

class TestWindow2 : public DynamicWindow {
public:
    explicit TestWindow2(QWidget *parent = nullptr)
        : DynamicWindow(parent) {

        auto *btn = new QPushButton("aabbcc", this);

        auto *layout = new QHBoxLayout(this);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(btn);

        setLayout(layout);

        toolBar()->addAction("test2");
    }
};
