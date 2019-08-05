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

#ifndef TRANLUCENTLABEL_H
#define TRANLUCENTLABEL_H

#include <QLabel>

class TranslucentLabel : public QLabel {
    Q_OBJECT
public:
    explicit TranslucentLabel(bool showTip = false, QWidget *parent = 0);

    void setLabelIcon(const QString &iconPath);
    void setLabelText(const QString &text);

signals:
    void clicked();

protected:
//    virtual void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
//    virtual void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    //bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel *m_icon = nullptr;
    QLabel *m_text = nullptr;
    bool m_showTip;
};
#endif // TRANLUCENTLABEL_H
