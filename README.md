xiaoming-weather
================

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

https://www.sojson.com/open/api/weather/json.shtml?city=长沙

https://www.heweather.com/documents/api/

https://free-api.heweather.com/s6/weather/hourly?location=CN101250101&key=40cc3ec6bbbf4de6a04029e207c986fd


空气质量实况
https://free-api.heweather.net/s6/air/now?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a


生活指数
https://free-api.heweather.net/s6/weather/lifestyle?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a


常规天气数据集合
https://free-api.heweather.net/s6/weather?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a


3天天气预报
https://free-api.heweather.net/s6/weather/forecast?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a


实况天气
https://free-api.heweather.net/s6/weather/now?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a



### 

qdbusxml2cpp com.kylin.weather.xml -a dbusadaptor -c DbusAdaptor -l MainWindow

https://www.easyicon.net/
