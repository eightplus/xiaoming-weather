/*
 * Copyright (C) 2018 ~ 2019 kobe24_lixiang@126.com
 *
 * Authors:
 *  lixiang    kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "titlebar.h"
#include "toolbutton.h"

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QShortcut>

TitleBar::TitleBar(QWidget *parent)
    :QFrame(parent)
{
    initWidgets();
    initMenu();
}

TitleBar::~TitleBar()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void TitleBar::initLeftContent()
{
    QWidget *w = new QWidget;
    m_lLayout = new QHBoxLayout(w);
    m_lLayout->setContentsMargins(5, 0, 0, 0);
    m_lLayout->setSpacing(0);
    m_layout->addWidget(w, 1, Qt::AlignLeft);

    QLabel *label = new QLabel;
    label->setText(tr("Xiaoming Weather"));
    label->setStyleSheet("QLabel{font-size:14px;font-style:italic;color: rgb(0, 0, 0);background-color:transparent;}");//font-weight:bold;

    m_lLayout->addWidget(label);
}

void TitleBar::initMiddleContent()
{
    QWidget *w = new QWidget;
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_mLayout = new QHBoxLayout(w);
    m_mLayout->setContentsMargins(0, 0, 0, 0);
    m_mLayout->setSpacing(0);

    m_layout->addWidget(w);
}

void TitleBar::initRightContent()
{
    QWidget *w = new QWidget;
    m_rLayout = new QHBoxLayout(w);
    m_rLayout->setContentsMargins(0, 0, 5, 0);
    m_rLayout->setSpacing(0);
    m_layout->addWidget(w, 1, Qt::AlignRight);

    ToolButton *menu_button = new ToolButton;
    ToolButton *min_button = new ToolButton;
    ToolButton *close_button = new ToolButton;
    menu_button->loadPixmap(":/res/menu_button.png");
    min_button->loadPixmap(":/res/min_button.png");
    close_button->loadPixmap(":/res/close_button.png");
    menu_button->setFocusPolicy(Qt::NoFocus);
    min_button->setFocusPolicy(Qt::NoFocus);
    close_button->setFocusPolicy(Qt::NoFocus);

    connect(menu_button, &ToolButton::clicked, this, [=] {
        if (parentWidget()) {
            const QPoint m_globalPoint = this->mapToGlobal(QPoint(0, 0));
            const QPoint m_point(m_globalPoint.x() + width() - m_menu->sizeHint().width() - 30, m_globalPoint.y() + 39 - 10);
            m_menu->popup(m_point);
        }
    });

    connect(min_button, &ToolButton::clicked, this, [=] {
        emit this->requestMinSize();
//        if (parentWidget() && parentWidget()->parentWidget()) {
//            parentWidget()->parentWidget()->showMinimized();
//        }
    });

    connect(close_button, &ToolButton::clicked, this, [=] {
        qApp->quit();
    });

    m_rLayout->addWidget(menu_button);
    m_rLayout->addWidget(min_button);
    m_rLayout->addWidget(close_button);
}

void TitleBar::initWidgets()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    this->setLayout(m_layout);

    initLeftContent();
    initMiddleContent();
    initRightContent();
}

void TitleBar::initMenu()
{
    m_menu = new QMenu(this);

    QAction *m_switchAciton = m_menu->addAction(tr("Switch"));
    m_menu->addSeparator();
    QAction *m_aboutAction = m_menu->addAction(tr("About"));
    QAction *m_quitAction = m_menu->addAction(tr("Exit"));

    connect(m_switchAciton, &QAction::triggered, this, &TitleBar::onSwitch);
    connect(m_aboutAction, &QAction::triggered, this, &TitleBar::onAbout);
    connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

    QShortcut *m_quitShortCut = new QShortcut(QKeySequence("Ctrl+Q"), this);
    connect(m_quitShortCut, SIGNAL(activated()), qApp, SLOT(quit()));
}

void TitleBar::onSwitch()
{
    emit this->switchDayOrNight();
}

void TitleBar::onAbout()
{
    emit this->requestDisplayAboutDialog();
}
