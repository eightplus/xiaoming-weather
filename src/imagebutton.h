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

#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class ImageButton : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString normalPic READ getNormalPic WRITE setNormalPic DESIGNABLE true)
    Q_PROPERTY(QString hoverPic READ getHoverPic WRITE setHoverPic DESIGNABLE true)
    Q_PROPERTY(QString pressPic READ getPressPic WRITE setPressPic DESIGNABLE true)

public:
    explicit ImageButton(QWidget * parent=0);
    ~ImageButton();

    enum BtnState {Normal, Hover, Press};
    void setBtnState(BtnState state);

    void updateIconImage();

    void setNormalPic(const QString &pixmap);
    const QString getNormalPic() const { return m_normalPic; }

    void setHoverPic(const QString &pixmap);
    const QString getHoverPic() const { return m_hoverPic; }

    void setPressPic(const QString &pixmap);
    const QString getPressPic() const { return m_pressPic; }

signals:
    void clicked();

protected:
    void enterEvent(QEvent * event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

private:
    BtnState m_state = Normal;
    QString m_normalPic;
    QString m_hoverPic;
    QString m_pressPic;
};

#endif // IMAGEBUTTON_H
