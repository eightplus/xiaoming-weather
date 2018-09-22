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

https://free-api.heweather.com/s6/weather/hourly?location=CN101250101&key=40cc3ec6bbbf4de6a04029e207c986fd

### 

qdbusxml2cpp com.kylin.weather.xml -a dbusadaptor -c DbusAdaptor -l MainWindow

https://www.easyicon.net/
