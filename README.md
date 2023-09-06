# automatic-fish-feeder

The design of an automatic aquarium feeder using a microcontroller. It is based on:
https://www.hackster.io/coderscafe/aquassist-diy-automatic-fish-feeder-with-companion-app-d8e4be

# What you need

Hardware components:

- ESP32 Board [Heading link](https://sklep.msalamon.pl/produkt/zestaw-esp32-esp-wroom-32u-z-antena-i-przewodem-usb/)
- Breadboard [Heading link](https://www.amazon.com/BB400-Solderless-Plug-BreadBoard-tie-points/dp/B0040Z1ERO)
- USB 5V 2A Wall Adapter [Heading link](https://thinkrobotics.com/products/power-supply-5v-2-4a)
- Feetech FS90R Servo [Heading link](https://botland.com.pl/serwa-praca-ciagla-360/4689-serwo-feetech-fs90r-micro-praca-ciagla-360-stopni-5904422305888.html)
- 3d models of the needed parts (in the repo on git you can find STL codes for your own use)

Software apps and online services:

- Firebase [Heading link](https://console.firebase.google.com/)
- Arduino Ide [Heading link](https://www.arduino.cc/en/software)
- Visual Studio Code [Heading link](https://code.visualstudio.com/)

# Configuration and launch

1. Install all the necessary software. In order for ESP32 to work with Arduino IDE you need to configure it properly [Heading link](https://www.youtube.com/watch?v=UuuqnmJIjR0).
2. Register on Firebase and create your project, select Realtime Database and copy the following values:
<p align="center">
  <img src="images\au_firebase.png" width="350" title="hover text">
  <img src="your_relative_path_here_number_2_large_name" width="350" alt="accessibility text">
</p>
   ![](images\url_firebase.png).
3. Go to the project settings tab and create your own application (choose the option with hosting and follow the commands one by one in the browser and in Visual Studio Code).
   ![](images\app_firebase.png).
4. In the folder you prepared in the previous step, place the Web_interface.html code in index.html and add to it the Firebase Config which you will find in your apps tab in project settings. Save it and deploy it.
5. Check that the website and database are working.
6. Place the ESP_program.ino code in the Arduino IDE adding the previously copied Au and URL along with your Wifi network data.
7. Check everything is working.
8. Glue 3d parts together and enjoy your own fish feeder. :smile:

# Author

Filip Gnojek
