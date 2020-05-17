![alt text](https://i.imgur.com/telFnTB.png)

# ⚡⚡⚡⚡⚡⚡⚡ Ziggy Stardust ⚡⚡⚡⚡⚡⚡⚡ </br> (An NFC bitcoin Lightning Network point of sale device) 

## *"There is life on Mars"*

A project to engage with the bitcoin Lightning Network over RFID/NFC, using an ESP32, RFID-RC522, OLED, Keypad

[![LN Slave Mod](https://i.imgur.com/aSmrQgv.png)](https://www.youtube.com/watch?v=5A0KK0k33cI)

**Wiring diagram**
![alt text](https://i.imgur.com/CtX7M1d.png)


Parts list with 2 possible sources, from: Aliexpress.com (cheaper, but slower) & Amazon.co.uk (faster, more expensive):

1.	ESP32 Wireless WiFi Bluetooth Development Board
[AliExpress](https://www.aliexpress.com/wholesale?catId=0&SearchText=ESP32+Wireless+WiFi+Bluetooth+Development+Board) / 
[Amazon](https://www.amazon.co.uk/s?k=ESP32+Wireless+WiFi+Bluetooth+Development+Board)


2.	0.96 inch OLED White Display Module 128X64 SPI 7pin Driver Chip SSD1306 for Arduino Diy Kit
[AliExpress](https://www.aliexpress.com/wholesale?catId=0&SearchText=0.96+inch+OLED+White+Display+Module+128X64) / 
[Amazon](https://www.amazon.co.uk/s?k=0.96+Inch+7+Pin+128+x+64+SPI+OLED)


3.	MFRC522 RFID Module for Arduino SPI Writer / Reader
[AliExpress](https://www.aliexpress.com/wholesale?catId=0&SearchText=MFRC-522+RFID+Module+for+Arduino+SPI) / 
[Amazon](https://www.amazon.co.uk/s?k=MFRC522+RFID+Module+for+Arduino+SPI+Writer+%2F+Reader)


4.	16 key membrane
[AliExpress](https://www.aliexpress.com/wholesale?catId=0&SearchText=16+key+membrane) / 
[Amazon](https://www.amazon.co.uk/s?k=16+key+membrane)


Assemble as per above wiring diagram.

Use USB micro cable to connect ESP32 to PC 

Install RFID library:
* Type in RFID<br>
![rfid](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/rfid.png)
* Scroll down to MFRC522 byGithubCommunity
* Click Install

**Dumpinfo example**

File -> Examples -> Dumpinfo
![dumpinfo](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/dumpinfo.png)

Change pin values on lines 39 & 40 from 9 and 10 to 22 and 5
![pins](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/pins.png)

* Uploade code

* Open Serial Monitor

* Tap NFC card upon the reader

* Serial Monitor will display contents of NFC card:<br>
![cardData](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/cardData.png)
<br>

**Set up Opennode accounts**

* Create an opennode.com account

First we generate an API Key for the Customer account:
* Click on:	Developers -> Integrations -> Add Key
* Enter Label:	NFCWITHDRAW
* Select Withdrawals from the Permissions dropdown
* Enter your 2FA
* Click on Generate<br>
![customerKey](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/customerKey.png)

* Copy the API Key to your clipboard, then paste into a text editor & remove the hypens<br>
![keyRemovedHyphens](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/keyRemovedHyphens.png)


Now we repeat for Merchant account
* Click on:	Developers -> Integrations -> Add Key
* Enter Label:	NFCINVOICE
* Select Invoices from the Permissions dropdown
Copy the API Key to your clipboard, then paste into a text editor<br>
![merchantKey](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/merchantKey.png)
<br>

**writedata**

* Copy main.ino code from Ziggy/writedata

* Paste into new Arduino file & save as NFCwrite

* Upload to ESP32

* Open Serial Monitor

* Place NFC card upon NFC writer

* Copy Customer API Key:<br>
![copyKey](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/copyKey.png)

* Paste into Serial Monitor, followed by a hash symbol # & press enter:
![pasteKey](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/pasteKey.png)


* The Serial Monitor should now display that the Customer API Key has successfully been written to the NFC Card:
![keySuccess](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/keySuccess.png)
<br>

**PoS**

* Copy main.ino code from Ziggy/writedata

* Paste into new Arduino file (& save as NFCPOS)

* Copy Merchants key into:<br>
`string apikey = "MERCHANTS-OPENNODE-INVOICE-API-KEY"`
![pasteMerchantKey](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/pasteMerchantKey.png)

* Copy apicalls.h code from Ziggy/PoS

* Paste into text editor and save as apicalls.h in the same directory as NFCPOS

* Enter you WiFI details:<br>
![wifiDeets](https://raw.githubusercontent.com/derrickr/arcbtc/master/images/wifiDeets.png)

* Upload the code

* When completed, the OLED should display "connecting", followed shortly by "connected" and then "Sats:"

* You can now use the the keypad to enter an amount of sats and press the hash key #

* The OLED should then display "..." followed by "Tap card" and finally "Paid!"

* You can now go back to opennode.com to check the transactions (remembering to refresh the page)
