/*
 * Copyright (C) 2018 ~ 2020 kobe24_lixiang@126.com
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

#include "citydelegate.h"
#include "citymodel.h"
#include "citybtngroups.h"

#include <QPainter>
#include <QDebug>

CityDelegate::CityDelegate(QObject *parent) : QStyledItemDelegate(parent)
  , m_hoverBackground(QColor("#2bb6ea"))
  , m_defaultBackground(QColor("#fbfbfb"))
  , m_default2Background(QColor("#ffffff"))
{

}

void CityDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //qDebug() << "option.rect=" << option.rect;
    if (!index.isValid()) {
        return;
    }
    //auto listview = qobject_cast<CityView *>(const_cast<QWidget *>(option.widget));

    const QString &strId = index.data(CityModel::IdRole).toString();
    const QString &strName = index.data(CityModel::NameRole).toString();
    const QString &strIcon = index.data(CityModel::IconRole).toString();
    const QString &strTemperature = index.data(CityModel::TemperatureRole).toString();
    bool isActive = index.data(CityModel::ActiveRole).toBool();
    bool isRemove = index.data(CityModel::RemoveRole).toBool();
    bool isHover = index.data(CityModel::HoverRole).toBool();
    int xOffset = index.data(CityModel::XOffsetRole).toInt();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);

    QColor background = (index.row() % 2) == 0 ? m_defaultBackground : m_default2Background;
    if (option.state & QStyle::State_Selected) {
        background = m_hoverBackground;//painter->setPen(option.palette.highlightedText().color());
    }
    if (option.state & QStyle::State_HasFocus) {
//        background = Qt::red;//painter->setPen(option.palette.foreground().color());
    }
    if (isHover) {
        background = QColor(43,182,234,130);//"#2bb6ea"
    }
    painter->fillRect(option.rect, background);//painter->fillRect(option.rect, option.palette.background());
    painter->setBrush(option.palette.foreground());

    QRect displayRect = option.rect;
    //qDebug() << "displayRect=" << displayRect;

    if (isRemove) {
        displayRect.setX(displayRect.x() + xOffset);
    }

    // draw weather icon
    const int iconSize = 48;
    QRect iconRect = QRect(displayRect.x() + 10, displayRect.y() + ((displayRect.height() - iconSize) / 2), iconSize, iconSize);
    QPixmap pixmap = QPixmap(strIcon);
    if (!pixmap.isNull())
        pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter->drawPixmap(iconRect, pixmap);

    // draw city name
    QFont font = painter->font();//option.font;
    font.setPixelSize(12);
    painter->setFont(font);
    QRect nameRect = option.rect;
    nameRect.setLeft(iconRect.right() + 5);
    nameRect.setTop(displayRect.top() + 5);
    nameRect.setRight(200);
    nameRect.setHeight(displayRect.height() - 2*5);
    painter->drawText(nameRect, Qt::AlignVCenter | Qt::ElideRight, isActive ? QString(tr("%1 [Default]")).arg(strName) : strName);

    /*if (isActive) {
        QString activeStr = tr("[Default]");
        QFontMetrics fm(font);
        //qDebug() << fm.boundingRect(activeStr).width() << fm.boundingRect(activeStr).height();
        QRect activeRect = QRect(nameRect.right(), nameRect.top(), fm.boundingRect(activeStr).width() + 10, nameRect.height());
        painter->drawText(activeRect, Qt::AlignVCenter, activeStr);
    }*/

    if (!strTemperature.isEmpty()) {
        QRect tempRect = option.rect;
        tempRect.setLeft(option.rect.right() - 200);
        tempRect.setTop(nameRect.top());
        tempRect.setRight(option.rect.right() - 40);
        tempRect.setHeight(nameRect.height());
        QFont tempFont(option.font);
        tempFont.setPixelSize(11);
        painter->setFont(tempFont);
        QFontMetrics tempFm(tempFont);
        QString tempText = tempFm.elidedText(strTemperature + "°C", Qt::ElideRight, tempRect.width());
        painter->drawText(tempRect, Qt::AlignVCenter | Qt::ElideRight, tempText);
    }

    painter->restore();
}

/*QSize CityDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    qDebug() << "index.data(Qt::SizeHintRole).toSize()=" << index.data(Qt::SizeHintRole).toSize();
    return index.data(Qt::SizeHintRole).toSize();

    //QSize size = QStyledItemDelegate::sizeHint(option, index);
    //returen size;

    //QSize baseSize = QStyledItemDelegate::sizeHint(option, index);
    //return QSize(baseSize.width() / 5, baseSize.height());
}*/

void CityDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    option->state = option->state /*& ~QStyle::State_HasFocus*/;
}

QWidget *CityDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    CityBtnGroups *btnGroups = new CityBtnGroups(parent);
    connect(btnGroups, &CityBtnGroups::defaultBtnClicked, this, &CityDelegate::defaultCityButtonClicked);
    connect(btnGroups, &CityBtnGroups::removeBtnClicked, this, &CityDelegate::removeCityButtonClicked);

    return btnGroups;
}

void CityDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    bool isActive = index.data(CityModel::ActiveRole).toBool();

    CityBtnGroups *btnGroups = static_cast<CityBtnGroups *>(editor);
    btnGroups->setFixedSize(QSize(120, 28));//设置按钮组的尺寸为(120, 28)，按钮组包括两个按钮：设置为默认按钮、删除按钮
    if (isActive) {
        btnGroups->setActive(true);
    }
    else {
        btnGroups->setActive(false);
    }
    QRect displayRect = option.rect;
    editor->setGeometry(displayRect.x() + displayRect.width() - btnGroups->width(), displayRect.y() + ((displayRect.height() - btnGroups->height()) / 2), btnGroups->width(), btnGroups->height());
}
