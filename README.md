# Binnenklimaat
Het binnenklimaat gaat over het meten van de status van de lucht in gebouwen en kamers.
De waarden die worden gemeten zijn: 
* temperatuur
* relatieve luchtvochtigheid
* CO2 niveau
* luchtdruk
* Vluchtige Organische Stoffen. De meer voorkomende engelse benaming: volatile organic compounds
Deze waarden, samen met de locatie data van het onderdeel "positie bepalen" worden dan verstuurd naar de back-end server die deze data verwerkt.


De chips die we hiervoor gebruiken zijn de:
* BME280 ( temperatuur, relatieve luchtvochtigheid, luchtdruk )
* SCD30 ( CO2 niveau ) 
* CCS811 ( Vluchtige Organische Stoffen )

Libraries voor C++ code ( requirements ):
* Wire
* SPI
* Adafruit_Sensor
* Adafruit_BME280
* Adafruit_SCD30
* Arduino

Libraries voor Python code ( requirements ):
* network  
* socket
* binascii
* struct
* time
* config
