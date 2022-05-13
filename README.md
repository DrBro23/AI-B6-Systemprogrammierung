# AI-B6-Systemprogrammierung
von Robin Prillwitz und Sven Menzel 

<br/>

### Motivation
Kühlung eines Temperatursensors mittels PWM Lüfter. Die Geschwindigkeit des Lüfters wird durch die Erwärmung des Temperatursensors geregelt.  

<br/>

### Materialien
Es wird ein Noctua (schlagmichtot) 4 Pin PWM Lüfter und ein TMP102 (not sure either) verwendet.

<br/>

### Vorgehen
Treiber verwendet File Operations und IO Control für die Kommunikation.
I2C wird für die Kommunikation mit dem Lüfter und dem Temperatursensor verwendet.