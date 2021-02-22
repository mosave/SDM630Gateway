# Eastron SDM630-Modbus MQTT gateway

## MQTT интерфейс на шину Modbus на примере электросчетчика Eastron SDM630

Проект построен на основе [AE' ESP8266 IoT Framework] с использованием следующих библиотек:
 * [PubSubClient by Nick O'Leary](https://github.com/knolleary/pubsubclient): MQTT клиент
 * [modbus-esp8266 by Alexander Emelianov](https://github.com/emelianov/modbus-esp8266): Взаимодействие с Modbus шиной
 * Другие библиотеки в составе Arduino ESP8266 SDK
 
### Использованное железо

 * [WeMos D1 ESP8266 module ($12.3/5шт)](https://aliexpress.ru/item/32649549788.html) или аналогичный
 * [RS485 to Serial UART converter module ($1.5)](https://aliexpress.ru/item/32813370341.html) или аналогичный
 * [5V блок питания ($14/10шт)](https://aliexpress.ru/item/32727708839.html)
 * [Eastron SDM630 V2 MID ($88)](https://aliexpress.ru/item/32755125115.html)
 * [Различные](https://www.aliexpress.com/item/32829408859.html) [колодки](https://aliexpress.ru/item/32828321217.html), 
 [выключатели](https://www.chipdip.ru/product/smrs-101-1c2), [втулки](https://aliexpress.ru/item/32877279906.html) россыпью и 3D принтер для печати корпуса :)

Header1 | Description2
--------|-------------
![Схема](https://github.com/mosave/SDM630Gateway/blob/main/photos/diagram.jpg) | Description


### Схема подключения

