xiaoming-weather
================

注意：xiaoming-weather使用的和风天气API key已升级为认证个人开发者，可以获取实时天气，7天天气预报，逐小时天气预报，空气质量和生活指数。该应用旨在为linux用户以及linux爱好者提供一个编写天气插件的模板，欢迎fork，但请注意key的使用，fork时请替换为自己申请的key，谢谢！

Debian
=======
dh_make --native

Internationalization
=======
lupdate xiaoming-weather.pro
linguist xiaoming-weather_zh_CN.ts
lrelease xiaoming-weather.pro

Depends
=======
debhelper (>= 9),qtbase5-dev,qt5-qmake,qtscript5-dev,qttools5-dev-tools,pkg-config

GitHub
=======
https://github.com/eightplus/xiaoming-weather


### Json API

和风天气api文档
https://www.heweather.com/documents/api/

sojson的十五天天气预报
http://t.weather.sojson.com/api/weather/city/101250101


天气API的七天天气预报
https://www.tianqiapi.com/?action=v10

https://www.tianqiapi.com/api/?version=v1 (https://www.tianqiapi.com/   https://blog.csdn.net/qq_38832501/article/details/88384395)

https://www.tianqiapi.com/api/?version=v1&cityid=101250101


逐小时天气预报
https://free-api.heweather.com/s6/weather/hourly?location=CN101250101&key=40cc3ec6bbbf4de6a04029e207c986fd


空气质量实况
https://free-api.heweather.net/s6/air/now?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a


生活指数
https://free-api.heweather.net/s6/weather/lifestyle?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a


常规天气数据集合
https://free-api.heweather.net/s6/weather?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a


7天天气预报
https://free-api.heweather.net/s6/weather/forecast?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a


实况天气
https://free-api.heweather.net/s6/weather/now?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a



### 

qdbusxml2cpp com.xiaoming.weather.xml -a dbusadaptor -c DbusAdaptor -l MainWindow

https://www.easyicon.net/
