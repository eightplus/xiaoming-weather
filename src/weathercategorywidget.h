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

#ifndef WEATHER_CATEGORY_WIDGET_H
#define WEATHER_CATEGORY_WIDGET_H

#include <QFrame>

#include "data.h"

class QStackedLayout;
class CategoryButton;
class QLabel;
class QGridLayout;
class IndexItemWidget;
class LifestyleTip;
class TipModule;
class WindTooltip;
class QPropertyAnimation;
class QParallelAnimationGroup;

class WeatherCategoryWidget : public QFrame
{
    Q_OBJECT

public:
    explicit WeatherCategoryWidget(QWidget *parent = 0);
    ~ WeatherCategoryWidget();

    void initTemperatureWidget();
    void initWindWidget();
    void initPopWidget();
    void initLifeStyleWidget();
    void refershLifeIndexGridLayout();

    void paintTemperatureCurve();
    void paintWindCurve();
    void paintPopWidget();

    void setDayStyleSheets();
    void setNightStyleSheets();

    void refreshLifestyleData();
    LifestyleTip *setTipWidget(QWidget *w, const QString &txt);

    void calculateTemperatureHoverIndex(int hoverX);
    int calculateWindHoverIndex(int hoverX);
    void calculatePopHoverIndex(int hoverX);

public slots:
    void changeCurrentPage(CategoryButton *btn);
    void showLifeStyleIndex(const QString &name);

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
//    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
//    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
//    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
//    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QStackedLayout *m_stackedLayout = nullptr;
    QStringList m_btnTextLists;
    QLabel *m_temperatureCurveLabel = nullptr;
    QLabel *m_windCurveLabel = nullptr;
    QLabel *m_popLabel = nullptr;
    QWidget *m_lifestyleWidget = nullptr;
    QGridLayout *m_indexGridLayout = nullptr;

    QWidget *m_btnsWidget = nullptr;
    CategoryButton *m_btnArray[4];

    //QPropertyAnimation *m_tempAnimation = nullptr;
//    QPropertyAnimation *m_lifeAnimation = nullptr;
    //QParallelAnimationGroup *m_animationGroup = nullptr;

    Forecast forecast[8];

    // for temperature
    int m_tempCount;
    int m_firstTempX;
    int m_tempXSpace;
    QList<int> *m_tempX;
    int m_tempHoverIndex;
    bool m_tempMouseEnterPress;

    // for wind
    int m_windCount;
    int m_firstWindX;
    int m_windXSpace;
    QList<int> *m_windX;
    int m_windHoverIndex;
    bool m_windMouseEnterPress;
    WindTooltip *m_windTip= nullptr;

    // for pop
    int m_popCount;
    int m_firstPopX;
    QList<int> *m_popX;
    int m_popHoverIndex;
    bool m_popMouseEnterPress;

    // for lifestyle
    QList<IndexItemWidget *> m_lifeItems;
    QStringList m_lifeIndexList;
    QStringList m_lifeIndexIconList;
    QList<LifestyleTip *> m_lifestyleTips;
    TipModule *m_tipModule = nullptr;

};

#endif // WEATHER_CATEGORY_WIDGET_H
