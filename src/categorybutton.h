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

#ifndef CATEGORYBUTTON_H
#define CATEGORYBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

class CategoryButton : public QWidget
{
    Q_OBJECT

public:
    explicit CategoryButton(QWidget *parent = 0);
    ~CategoryButton();

    void setIconAndText(const QPixmap &pixmap, const QString &txt);

    void setMouseEnterPress(bool);
    bool getMouseEnterPress() { return m_mouseEnterPress; }

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
    void requestChangeButtonStatus(CategoryButton *btn);

private:
    bool m_mouseEnterPress;
    QHBoxLayout *m_hLayout = nullptr;
    QLabel *m_icon = nullptr;
    QLabel *m_text = nullptr;
};

#endif //CATEGORYBUTTON_H
