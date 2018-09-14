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

#ifndef WEATHER_NOW_CATEGORY_WIDGET_H
#define WEATHER_NOW_CATEGORY_WIDGET_H

#include <QFrame>

#include "data.h"

class QStackedLayout;
class CategoryButton;
class QLabel;
class QGridLayout;
class IndexItemWidget;
class TextTip;
class TipModule;
class QPropertyAnimation;

class WeatherNowCategoryWidget : public QFrame
{
    Q_OBJECT
public:
    explicit WeatherNowCategoryWidget(QWidget *parent = 0);
    ~ WeatherNowCategoryWidget();

    void paintTemperatureCurve();
    void initIndexWidget();
    void refershLifeIndexGridLayout();

    void setDayStyleSheets();
    void setNightStyleSheets();

    void refreshLifestyleData(const LifeStyle &data);
    TextTip *setTipWidget(QWidget *w, const QString &txt);

public slots:
    void changeCurrentPage(CategoryButton *btn);
    void showLifeStyleIndex(const QString &name);

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
//    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    QStackedLayout *m_stackedLayout = nullptr;
    QStringList m_btnTextLists;
    QLabel *m_temperatureCurveLabel = nullptr;
    QWidget *m_lifestyleWidget = nullptr;
    QGridLayout *m_indexGridLayout = nullptr;

    QLabel *m_label2 = nullptr;
    QLabel *m_label3 = nullptr;
    QWidget *m_btnsWidget = nullptr;
    CategoryButton *m_btnArray[4];
    QList<IndexItemWidget *> m_lifeItems;
    QStringList m_lifeIndexList;
    QStringList m_lifeIndexIconList;

    //QPropertyAnimation *m_tempAnimation = nullptr;
    Forecast forecast[8];

    QList<TextTip *> m_tips;
    TipModule *m_tipModule = nullptr;
};

#endif // WEATHER_NOW_CATEGORY_WIDGET_H
