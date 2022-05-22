# BLE-Smart-Thermal-Flask-by-M5Stack

![alt text](https://github.com/akarsh98/BLE-Smart-Thermal-Flask-by-M5Stack/blob/main/BLE%20Flask/18.JPG)

In this tutorial, we are going to make a Smart Thermal Flask with the help of M5Stick C module, M5Stamp module and LM35 Temperature Sensor. The main concept behind this project is of BLE Communication. Both the modules, M5Stick C and M5Stamp pico use BLE Technology to communicate with each other. In this project, we are going to transmit Temperature data from one end to the other with the help of these modules. Using the modules from M5Stack, you can make a number of different projects like the one that we are discussing here. The Smart Thermal Flask is a device which will continuously keep a check on its inside temperature and display it to its user. On the basis of the Temperature data received, the user will decide what to do with the things inside the Flask.

![alt text](https://github.com/akarsh98/BLE-Smart-Thermal-Flask-by-M5Stack/blob/main/BLE%20Flask/7.JPG)

Now we have understood that what the Flask will do, let's see how it will perform this task and what things are required to be done to make this thing work. First of all, we will make a Transmitter node that will be installed on the Flask. The Transmitter node will be made with M5Stamp pico module. To this module, an LM35 Temperature Sensor will be connected. The Connections to be done are as mentioned below:-
Connect the LM35's output pin to M5Stamp's 36th pin and LM35's power to 5 V and GND to GND of the PICO.

VOUT -> G36
GND -> GND
VCC -> 5V

Once the connections are done, the board needs to be programmed. So to program the board, the "Transmitter.ino" code present in this Github repository will be uploaded to the module using Arduino IDE. Once the code is uploaded, the Transmitter Node will start working. The Temperature Sensor will start collecting Temperature data and will pass on the data to the M5Stamp module to be transmitted.

![alt text](https://github.com/akarsh98/BLE-Smart-Thermal-Flask-by-M5Stack/blob/main/BLE%20Flask/19.JPG)

Now the Transmitter node is ready, its time to make the receiver node and test the system. For the receiver node, we will require a M5Stick C module. We do not need to attach any additional hardware to this module. We just need to program the module and we are good to go. The code that needs to be uploaded in this module is the "Receiver.ino" code in this Github repository. Once the code is uploaded, our Receiver will be ready to work. It will start receiving messages from the Transmitter at a difference of 10 seconds and according to the Temperature data received, it will display some mesaages on the inbuilt TFT Screen that it has. So once the receiver is ready, you can test it. For that, you need to connect the Transmitter node to the Computer and open the Arduino IDE Serial monitor. On the Serial monitor, the data that is being transmitted will appear and the similar data will appear on the TFT Screen of the Receiver node. As soon as you are satisfied that the connection is working properly, you can install the Tranmitter node to you Flask. Place the Temperature Sensor on the inside of the lid of the Flask and paste the M5Stamp module on the outside of the Flask. Attach a small LiPo Battery to power the Transmitter system and that's it. Your Smart Flask is ready to use. You will receive the Temperature data from your Flask and accordingly you can decide what to do with the inside components.Not only this but you can make several different fun and useful projects using these components.

![alt text](https://github.com/akarsh98/WebServers-on-ESP32-Codes/raw/master/Webservers%20on%20ESP32/pcbway.JPG)

You must check out [PCBWAY](https://www.pcbway.com/) for ordering PCBs online for cheap!

You get 10 good-quality PCBs manufactured and shipped to your doorstep for cheap. You will also get a discount on shipping on your first order. Upload your Gerber files onto PCBWAY to get them manufactured with good quality and quick turnaround time. [PCBWay](https://www.pcbway.com/) now could provide a complete product solution, from design to enclosure production. Check out their online Gerber viewer function. With reward points, you can get free stuff from their gift shop.
