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
