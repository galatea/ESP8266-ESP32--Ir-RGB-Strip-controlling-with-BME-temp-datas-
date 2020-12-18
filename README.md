# IrRGBFromBME

Sziasztok

Segítség lehet azoknak, akiknek nem wifis az RGB led vezérlőjük. Nincs szükség másra csak egy ESP8266-ra, egy 220Ohm-os ellenállás, 1 vagy 2 BME280 szenzorra (én kettőt használok, egyéb projektbő maradt meg, így mindkettőt beüzemelve hagytam), illetve egy IR ledre (én spec kitörtem a távirányítóból és uccu neki).
A kód segítségével lehetőség van lokálisan beállítani a led színét, fényerejét, ki/be kapcsolását.
Továbbá van egy gom hogy hőmérséklettől függő legyen a led.
Ekkor adott intervallumokon belül más és más színeket fog produkálni a szalag. (Panelos vagyok, így 20+ állítódig a fény, de nem egy bonyolult átállítani).
Amint lesz időm egyre több funkciót csempészek bele. 

Bekötés:
 BME280
  VCC- VCC
  GND - GND
  SCL - D1
  SDA - D2
 
 IR Dioda
  D6 - > Dióda - > 220ohm - >  GND

Kiindulási alap ez volt:
https://www.instructables.com/ESP8266-RGB-LED-STRIP-WIFI.../
