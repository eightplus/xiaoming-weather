/*
 * Copyright (C) 2018 kobe24_lixiang@126.com
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

#include "indexitemwidget.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>

IndexItemWidget::IndexItemWidget(const QString &name, const QString &iconPath, QWidget *parent)
    : QWidget(parent)
    , m_mouseHover(false)
    , m_iconPath(iconPath)
    , m_brf("-")
    , m_txt("-")
    , m_titleColor(QColor("#cfcfcf"))
    , m_txtColor(QColor("#808080"))
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if (name == "comf") {
        m_indexName = QString(tr("comf"));
    }
    else if (name == "drsg") {
        m_indexName = QString(tr("drsg"));
    }
    else if (name == "flu") {
        m_indexName = QString(tr("flu"));
    }
    else if (name == "sport") {
        m_indexName = QString(tr("sport"));
    }
    else if (name == "trav") {
        m_indexName = QString(tr("trav"));
    }
    else if (name == "uv") {
        m_indexName = QString(tr("uv"));
    }
    else if (name == "cw") {
        m_indexName = QString(tr("cw"));
    }
    else if (name == "air") {
        m_indexName = QString(tr("air"));
    }
    else {
        m_indexName = name;
    }
}

void IndexItemWidget::refreshLifeStyle(const QString &brf, const QString &txt)
{
    this->m_brf = brf;
    this->m_txt = txt;

    update();
}

void IndexItemWidget::setDayStyleSheets()
{
    m_titleColor = QColor("#cfcfcf");
    m_txtColor = QColor("#808080");
    update();
}

void IndexItemWidget::setNightStyleSheets()
{
    m_titleColor = QColor("#808080");
    m_txtColor = QColor("#cdcdcd");
    update();
}

void IndexItemWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    this->m_mouseHover = true;
    emit this->requestShowMsg(m_indexName);
    update();
}

void IndexItemWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    this->m_mouseHover = false;
    update();
}

void IndexItemWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, m_mouseHover ? 20 : 0));

    const qreal ratio = qApp->devicePixelRatio();
    const QPointF center = this->rect().center();

    painter.setRenderHint(QPainter::Antialiasing, true);

    // Attention: 20 + icon.width()/ratio/2 = 30可当作TextTip的箭头的指向坐标的x
    QPixmap icon = QPixmap(this->m_iconPath);
    QRect iconRect(20, center.y() - icon.height()/ratio/2, icon.width()/ratio, icon.height()/ratio);
    painter.drawPixmap(iconRect, icon);

    painter.setRenderHint(QPainter::Antialiasing, false);

    QFont font = painter.font();
    font.setPixelSize(12);
    QFontMetrics fm(font);
    QPen pen(Qt::white);
    painter.setPen(pen);
    painter.setFont(font);

    QString text = QString("%1:%2").arg(m_indexName).arg(this->m_brf);
    QRect textRect(iconRect.right() + 5, center.y() - 20/2, fm.width(text), 20);
    painter.setPen(QPen(m_txtColor));
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
}
