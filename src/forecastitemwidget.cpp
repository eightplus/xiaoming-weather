#include "forecastitemwidget.h"

#include <QDebug>
#include <QEvent>
#include <QCursor>
#include <QPainter>

inline QString covertDateToWeek(QString dateStr)
{
    //星期使用基姆拉尔森计算公式
    if (dateStr.contains(QChar('-'))) {
        QStringList dateList= dateStr.split(QChar('-'));
        if (dateList.length() == 3) {
            bool ok;
            int year = dateList.at(0).toInt(&ok, 10);
            int month = dateList.at(1).toInt(&ok, 10);
            int day = dateList.at(2).toInt(&ok, 10);

            int week = (day + 2*month + 3*(month+1)/5 + year + year/4 - year/100 + year/400)%7;
            switch (week) {
            case 0:
                return QString(QObject::tr("Monday"));//星期一
                break;
            case 1:
                return QString(QObject::tr("Tuesday"));//星期二
                break;
            case 2:
                return QString(QObject::tr("Wednesday"));//星期三
                break;
            case 3:
                return QString(QObject::tr("Thursday"));//星期四
                break;
            case 4:
                return QString(QObject::tr("Friday"));//星期五
                break;
            case 5:
                return QString(QObject::tr("Saturday"));//星期六
                break;
            case 6:
                return QString(QObject::tr("Sunday"));//星期日
                break;
            default:
                break;
            }
        }
    }

    return "--";
}

ForecastItemWidget::ForecastItemWidget(QWidget *parent) :
    QWidget(parent)
    , m_isHover(false)
{
    this->setFixedSize(100, 140);//140:initForecastWidget's height - 2*space = 160- 10*2
    this->setStyleSheet("QWidget{border-radius: 0px;color:rgb(250,250,250);background-color:rgba(0,0,0,0.2)}");
//    setAttribute(Qt::WA_TransparentForMouseEvents);

    m_weekLabel = new QLabel(this);
    m_dateLabel = new QLabel(this);
    m_weatherLabel = new QLabel(this);
    m_iconLabel = new QLabel(this);
    m_tempLabel = new QLabel(this);
    m_iconLabel->setFixedSize(48, 48);
    m_iconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(5);
    m_layout->addWidget(m_weekLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    m_layout->addWidget(m_dateLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_weatherLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_iconLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_tempLabel, 0, Qt::AlignBottom | Qt::AlignHCenter);

    this->setDayStyleSheets();
    this->setDefaultData();
}

void ForecastItemWidget::resetForecastData(const ForecastWeather &data, int index)
{
    if (index == 0) {
        m_weekLabel->setText(tr("Today"));
    }
    else {
        m_weekLabel->setText(covertDateToWeek(data.forcast_date));
    }

    m_dateLabel->setText(data.forcast_date);
    m_weatherLabel->setText(data.cond_txt_d);

    //darkgrey or lightgrey
    QPixmap pixmap;
    pixmap = QPixmap(":/res/101.png");
//    if (m_isDayOrNight) {
//        pixmap = QPixmap(QString(":/res/weather_icons/darkgrey/%1.png").arg(data.cond_code_d));
//    }
//    else {
//        pixmap = QPixmap(QString(":/res/weather_icons/lightgrey/%1.png").arg(data.cond_code_d));
//    }
    pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
    m_tempLabel->setText(QString("%1°C~%2°C").arg(data.tmp_min).arg(data.tmp_max));
}

void ForecastItemWidget::setDayStyleSheets()
{
    m_isDayOrNight = true;

    m_weekLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_dateLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_weatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
}

void ForecastItemWidget::setNightStyleSheets()
{
    m_isDayOrNight = false;

    m_weekLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#d9d9d9;font-size:12px;}");
    m_dateLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#d9d9d9;font-size:12px;}");
    m_weatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#d9d9d9;font-size:12px;}");
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#d9d9d9;font-size:12px;}");
}

void ForecastItemWidget::setTextData()
{
//    m_weekLabel->setText("星期五");
}

void ForecastItemWidget::setDefaultData()
{
    m_weekLabel->setText("今天");
    m_dateLabel->setText("20180919");
    m_weatherLabel->setText("晴");
    QPixmap pixmap = QPixmap(":/res/101.png");
    pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
//    m_tempLabel->setText("-°C");
    m_tempLabel->setText("27°C");
}

void ForecastItemWidget::setLabelText(const QString &name, const QString &desc)
{
//    QFont ft;
//    QFontMetrics fm(ft);
//    QString elided_text = fm.elidedText(name, Qt::ElideRight, 260);
//    name_label->setText(elided_text);
//    elided_text = fm.elidedText(desc, Qt::ElideRight, 260);
//    description_label->setText(elided_text);
//    if(elided_text.endsWith("…"))
//        description_label->setToolTip(desc);
}

bool ForecastItemWidget::event(QEvent *event)
{
//    if (event->type() == QEvent::ToolTip) {
//    }
//    else if (event->type() == QEvent::Leave) {
//    }
    if (event->type() == QEvent::Enter) {
        m_isHover = true;
    }
    else if (event->type() == QEvent::Leave) {
        m_isHover = false;
    }

    return QWidget::event(event);
}


void ForecastItemWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (m_isHover) {
        QPen pen(Qt::NoBrush, 1);
        painter.setPen(pen);

        QLinearGradient lineargradient(this->rect().topLeft(), this->rect().bottomLeft());
        //lineargradient.setColorAt(0, QColor(255, 255, 255, 40));
        lineargradient.setColorAt(0, QColor(230, 230, 230, 150));
        lineargradient.setColorAt(0.5, QColor(230, 230, 230, 100));
        lineargradient.setColorAt(1, QColor(230, 230, 230, 0));
        QBrush brush(lineargradient);
        painter.setBrush(brush);//painter.setBrush(lineargradient);
        painter.drawRect(rect());//painter.drawRoundedRect(this->rect(), 1, 1);
    }

    QWidget::paintEvent(event);
}
