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

#include "weathernowcategorywidget.h"
#include "categorybutton.h"
#include "indexitemwidget.h"
#include "texttip.h"
#include "tipmodule.h"

#include <QDebug>
#include <QStackedLayout>
#include <QPainter>
#include <QGridLayout>
#include <QPropertyAnimation>

namespace {

const int BtnWidgetHeight = 28;
const int TextHeight = 20;
const int TemperatureDotSize = 3;
const QStringList LIFESTYLE = {"comf", "drsg", "flu", "sport", "trav", "uv", "cw", "air"};
const QStringList LIFESTYLEICON = {":/res/lifestyle/comf_index.png", ":/res/lifestyle/clothe_index.png", ":/res/lifestyle/flu_index.png", ":/res/lifestyle/sport_index.png", ":/res/lifestyle/trav_index.png", ":/res/lifestyle/ultraviolet_rays_index.png", ":/res/lifestyle/cash_wash_index.png", ":/res/lifestyle/air_index.png"};

}

WeatherNowCategoryWidget::WeatherNowCategoryWidget(QWidget *parent)
    : QFrame(parent)
    , m_stackedLayout(new QStackedLayout)
    , m_tipModule(new TipModule)
{
//    this->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(Qt::red));
//    this->setPalette(palette);


    m_stackedLayout->setMargin(0);
    m_stackedLayout->setSpacing(0);
    this->setLayout(m_stackedLayout);

    m_lifeIndexList = LIFESTYLE;
    m_lifeIndexIconList = LIFESTYLEICON;
    m_btnTextLists << tr("Temperature") << tr("Pop") << tr("Wind") << tr("Lifestyle");

    m_lifeItems.clear();
    m_tips.clear();

    m_temperatureCurveLabel = new QLabel;
    m_temperatureCurveLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //m_temperatureCurveLabel->setFixedSize(413, 105);
    m_temperatureCurveLabel->installEventFilter(this);

    //test background
    m_temperatureCurveLabel->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::gray));
    m_temperatureCurveLabel->setPalette(palette);
    const int idx = m_stackedLayout->addWidget(m_temperatureCurveLabel);
    m_stackedLayout->setCurrentIndex(idx);

    m_label2 = new QLabel;
    m_label2->setText("Pop");
    m_stackedLayout->addWidget(m_label2);

    m_label3 = new QLabel;
    m_label3->setText("Wind");
    m_stackedLayout->addWidget(m_label3);

    this->initIndexWidget();
    m_stackedLayout->addWidget(m_lifestyleWidget);

    m_btnsWidget = new QFrame(this);
    m_btnsWidget->setFixedHeight(BtnWidgetHeight);
    m_btnsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_layout->setSpacing(0);
    btn_layout->setMargin(0);

    for(int i = 0; i < m_btnTextLists.count(); i++) {
        CategoryButton *btn = new CategoryButton(this);
        btn->setFixedSize(QSize(120, m_btnsWidget->height()));
        btn->setIconAndText(QPixmap(":/res/ip.png"), m_btnTextLists.at(i));
        btn_layout->addWidget(btn);
        connect(btn, SIGNAL(requestChangeButtonStatus(CategoryButton*)), this, SLOT(changeCurrentPage(CategoryButton*)));
        m_btnArray[i] = btn;
    }
    m_btnsWidget->move(0, this->height() - m_btnsWidget->height());
    m_btnArray[0]->setMouseEnterPress(true);
    btn_layout->addStretch();
    m_btnsWidget->setLayout(btn_layout);


    /*m_tempAnimation = new QPropertyAnimation(m_temperatureCurveLabel, "opacity");
    m_tempAnimation->setDuration(200);
    m_tempAnimation->setStartValue(0);
    m_tempAnimation->setKeyValueAt(0.25, 1);
    m_tempAnimation->setKeyValueAt(0.5, 1);
    m_tempAnimation->setKeyValueAt(0.75, 1);
    m_tempAnimation->setEndValue(1);
    connect(m_tempAnimation, &QPropertyAnimation::finished, this, [=] {
        m_stackedLayout->setCurrentIndex(0);
        m_btnsWidget->raise();
//        m_tempAnimation->deleteLater();
    });*/
}

WeatherNowCategoryWidget::~WeatherNowCategoryWidget()
{
    for(int i=0; i<m_lifeItems.count(); i++) {
        IndexItemWidget *item = m_lifeItems.at(i);
        delete item;
        item = NULL;
    }
    m_lifeItems.clear();

    for(int i=0; i<m_tips.count(); i++) {
        TextTip *tip = m_tips.at(i);
        delete tip;
        tip = NULL;
    }
    m_tips.clear();
    delete m_tipModule;

    QLayoutItem *child;
    while ((child = m_stackedLayout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
    delete m_stackedLayout;

    while (QLayoutItem *item = m_indexGridLayout->takeAt(0)) {
        item->widget()->deleteLater();
        delete item;
    }
    delete m_indexGridLayout;

    for (int i = 0; i < 4; i++) {
        delete m_btnArray[i];
    }
}

void WeatherNowCategoryWidget::changeCurrentPage(CategoryButton *label)
{
    for(int i=0; i<4; i++) {
        if(label != m_btnArray[i]) {
            m_btnArray[i]->setMouseEnterPress(false);
        }
        else {
            /*if (i == 0) {
                m_tempAnimation->start();
            }
            else {
                m_stackedLayout->setCurrentIndex(i);
            }*/
            m_stackedLayout->setCurrentIndex(i);
            m_btnsWidget->raise();
//            for (int i = 0; i < 4; i++) {
//                m_btnArray[i]->raise();
//            }
        }
    }
}

bool WeatherNowCategoryWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_temperatureCurveLabel && event->type() == QEvent::Paint) {
        paintTemperatureCurve();
    }

    return QWidget::eventFilter(obj,event);
}

void WeatherNowCategoryWidget::paintTemperatureCurve()
{
    int i = 0;
    forecast[i].high = "33";

    i = 1;
    forecast[i].high = "32";

    i = 2;
    forecast[i].high = "37";

    i = 3;
    forecast[i].high = "36";

    i = 4;
    forecast[i].high = "38";

    i = 5;
    forecast[i].high = "35";

    i = 6;
    forecast[i].high = "25";

    i = 7;
    forecast[i].high = "30";

    QPainter painter(m_temperatureCurveLabel);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int tempTotal = 0;
    int temperatureArray[8] = {};
    int minValue = 0;
    int maxValue = 0;
    int maxPos = 0;

    for (int i = 0; i < 8; i++) {
        temperatureArray[i] = forecast[i].high.toInt();
        tempTotal += temperatureArray[i];

        if (i==0) {
            minValue = temperatureArray[i];
            maxValue = temperatureArray[i];
        }

        if (temperatureArray[i] > maxValue) {
            maxValue = temperatureArray[i];
            maxPos = i;
        }
        if (temperatureArray[i] < minValue) {
            minValue = temperatureArray[i];
        }
    }
    //qDebug() << "temperatureArray=" << temperatureArray[0] << temperatureArray[1] << temperatureArray[2] << temperatureArray[3] << temperatureArray[4] << temperatureArray[5] << temperatureArray[6] << temperatureArray[7];
    int tempAverage = (int)(tempTotal / 8); //当天各小时段温度的平均值
    int tempSpace = abs(maxValue - minValue);//最高温和最低温的差
    int tempMiddle = maxValue - (int)(tempSpace / 2);//最高温和最低温的中间值
    int basePosY = m_temperatureCurveLabel->height()/2;
    int spaceIndex = (m_temperatureCurveLabel->height() - 20*2)/tempSpace;//每一摄氏度占据的高度,让显示曲线顶部和底部均空出10个像素
    //qDebug() << "tempMiddle=" << tempMiddle << ",tempSpace=" << tempSpace << ",spaceIndex=" << spaceIndex << m_temperatureCurveLabel->height();
    //qDebug() << "tempAverage=" << tempAverage << ", maxPos=" << maxPos << temperature[maxPos];
    int pointX[8] = {51, 151, 251, 351, 451, 551, 651, 751};//X
    int pointHY[8] = {0};
    for (int i = 0; i < 8; i++) {
        if (temperatureArray[i] >= tempMiddle) {
            pointHY[i] = basePosY - abs(temperatureArray[i] - tempMiddle) * spaceIndex;
        }
        else {
            pointHY[i] = basePosY + abs(tempMiddle - temperatureArray[i]) * spaceIndex;
        }
    }
    //qDebug() << "pointHY=" << pointHY[0] << pointHY[1] << pointHY[2] << pointHY[3] << pointHY[4] << pointHY[5] << pointHY[6] << pointHY[7];

    QPen pen = painter.pen();
    pen.setWidth(1);
    painter.save();
    pen.setColor(QColor("#fff222"));
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(QColor("#fff222"));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawEllipse(QPoint(pointX[0], pointHY[0]), TemperatureDotSize, TemperatureDotSize);
    for (int i = 0; i < 7; i++) {
        painter.drawEllipse(QPoint(pointX[i+1], pointHY[i+1]), TemperatureDotSize, TemperatureDotSize);
        painter.drawLine(pointX[i], pointHY[i], pointX[i+1], pointHY[i+1]);
    }

    for (int i = 0; i < 8; i++) {
        QFont font = painter.font();
        font.setPixelSize(12);
        QFontMetrics fm(font);
        painter.setFont(font);
        painter.setPen(QPen(QColor("#808080")));

        QString tempStr = QString::number(temperatureArray[i]) + "°C";
        if (pointHY[i] >= basePosY) {
            QRect valueRect(pointX[i] - fm.width(tempStr)/2, pointHY[i] - 20 - 5, fm.width(tempStr), TextHeight);
            painter.drawText(valueRect, Qt::AlignCenter, tempStr);
        }
        else {
            QRect valueRect(pointX[i] - fm.width(tempStr)/2, pointHY[i] + 5, fm.width(tempStr), TextHeight);
            painter.drawText(valueRect, Qt::AlignCenter, tempStr);
        }
    }

    //平均温度线
    QPen penAve;
    penAve.setColor(Qt::red);
    penAve.setWidth(2);
    penAve.setStyle(Qt::DotLine);
    painter.setPen(penAve);
    int aveY = 0;
    if (tempAverage >= tempMiddle) {
        aveY = basePosY - abs(tempAverage - tempMiddle) * spaceIndex;
    }
    else {
        aveY = basePosY + abs(tempMiddle - tempAverage) * spaceIndex;
    }
    painter.drawLine(51, aveY, 750, aveY+1);
    QFont fontAve = painter.font();
    fontAve.setPixelSize(12);
    QFontMetrics fmAve(fontAve);
    painter.setFont(fontAve);
    painter.setPen(QPen(QColor("#808080")));

    QString aveStr = QString::number(tempAverage) + "°C";
    QRect aveRect(750 + 20, aveY - TextHeight/2, fmAve.width(aveStr), TextHeight);
    painter.drawText(aveRect, Qt::AlignCenter, aveStr);

    painter.restore();
}

void WeatherNowCategoryWidget::initIndexWidget()
{
    m_lifestyleWidget = new QWidget;
    m_lifestyleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //test background
    /*m_lifestyleWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::blue));
    m_lifestyleWidget->setPalette(palette);
    //m_lifestyleWidget->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:rgba(0,0,0,0.2);}");
    */

    m_indexGridLayout = new QGridLayout(m_lifestyleWidget);
    m_indexGridLayout->setSpacing(1);
    m_indexGridLayout->setContentsMargins(0,0,0,30);

    this->refershLifeIndexGridLayout();

//    showLifeStyleIndex(QString());
}

void WeatherNowCategoryWidget::refershLifeIndexGridLayout()
{
    while (QLayoutItem *item = m_indexGridLayout->takeAt(0)) {
        item->widget()->deleteLater();
        delete item;
    }

    for(int i=0; i<m_tips.count(); i++) {
        TextTip *tip = m_tips.at(i);
        delete tip;
        tip = NULL;
    }
    m_tips.clear();

    int index = 0;
    const int count = m_lifeIndexList.size();
    for (int i = 0; i != count; ++i, ++index) {
        IndexItemWidget *item = new IndexItemWidget(m_lifeIndexList[i], m_lifeIndexIconList[i]);
        connect(item, SIGNAL(requestShowMsg(QString)), this, SLOT(showLifeStyleIndex(QString)));
        m_indexGridLayout->addWidget(item, index / 3, index % 3);
        TextTip *tip = this->setTipWidget(item, "");
        m_tips.append(tip);
        m_lifeItems.append(item);
    }
}

void WeatherNowCategoryWidget::showLifeStyleIndex(const QString &name)
{

}


void WeatherNowCategoryWidget::refreshLifestyleData(const LifeStyle &data)
{
    int n = 0;
    if (m_lifeItems.count() == 8 && m_tips.count() == 8) {
        //舒适度指数
        m_lifeItems[n]->refreshLifeStyle(data.comf_brf, data.comf_txt);
        m_tips[n++]->resetTipText(data.comf_txt);

        //穿衣指数
        m_lifeItems[n]->refreshLifeStyle(data.drsg_brf, data.drsg_txt);
        m_tips[n++]->resetTipText(data.drsg_txt);

        //感冒指数
        m_lifeItems[n]->refreshLifeStyle(data.flu_brf, data.flu_txt);
        m_tips[n++]->resetTipText(data.flu_txt);

        //运动指数
        m_lifeItems[n]->refreshLifeStyle(data.sport_brf, data.sport_txt);
        m_tips[n++]->resetTipText(data.sport_txt);

        //旅游指数
        m_lifeItems[n]->refreshLifeStyle(data.trav_brf, data.trav_txt);
        m_tips[n++]->resetTipText(data.trav_txt);

        //紫外线指数
        m_lifeItems[n]->refreshLifeStyle(data.uv_brf, data.uv_txt);
        m_tips[n++]->resetTipText(data.uv_txt);

        //洗车指数
        m_lifeItems[n]->refreshLifeStyle(data.cw_brf, data.cw_txt);
        m_tips[n++]->resetTipText(data.cw_txt);

        //空气污染扩散条件指数
        m_lifeItems[n]->refreshLifeStyle(data.air_brf, data.air_txt);
        m_tips[n++]->resetTipText(data.air_txt);
    }
}

TextTip *WeatherNowCategoryWidget::setTipWidget(QWidget *w, const QString &txt)
{
    TextTip *tip = new TextTip(txt, this);
    w->setProperty("TextTipWidget", QVariant::fromValue<QWidget *>(tip));
    w->installEventFilter(m_tipModule);

    return tip;
}

void WeatherNowCategoryWidget::setDayStyleSheets()
{
    for(int i=0; i<m_lifeItems.count(); i++) {
        IndexItemWidget *item = m_lifeItems.at(i);
        item->setDayStyleSheets();
    }
}

void WeatherNowCategoryWidget::setNightStyleSheets()
{
    for(int i=0; i<m_lifeItems.count(); i++) {
        IndexItemWidget *item = m_lifeItems.at(i);
        item->setNightStyleSheets();
    }
}

void WeatherNowCategoryWidget::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    m_btnsWidget->setFixedWidth(event->size().width());
    m_btnsWidget->move(0, event->size().height() - m_btnsWidget->height());
}

/*void WeatherNowCategoryWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    m_temperatureCurveLabel->update();
}*/
