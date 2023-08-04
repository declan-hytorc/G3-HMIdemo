# F15 HMI Basic Peripherals Demo Introduction

F15 will use a HMI called the G3 provided by Reach Technologies as the main control board. Reach Technologies has created their own linux image with Yacto to run on the G3. Using Qt we can load applications onto the G3 which can access peripherals such as serial communications, a touch screen, and bluetooth. The purpose of this application is to demonstrate how the necessary peripherals for F15 can be accessed using Qt.

# Development and Toolchain
**Note:** At this time, the G3 Development kit and the development environment provided by Reach Reachnologies are still considered to be in beta. Documentation for both is incomplete.

## Description
Reach Technologies provides a linux virtual machine that has the Qt Creator IDE set up on it. Qt Creator uses an SSH connection with the G3 to load applications onto it. It is also possible to debug, launch, and stop applications all from the Qt Creator environment on Reach Technologies' machine. This has worked very well for me for testing the capabilities of the G3, but it is also possible to setup our own environment if needed.
## Set Up Instructions 
1. Find the developer VM image [here](https://urldefense.com/v3/__https:/reach-technology-g3-manuals.readthedocs-hosted.com/en/latest/search.html?q=images&check_keywords=yes&area=default__;!!I312wgvi!V9INi0zScRXdQtNWfy_FdTDhEv_sC-ht94mb0bDoED4kych1OGhERtPyemgfBhtDvYkNLbVrY1UpR7PIpCT2JA$). Download NXP Qt5 Dev VM 1.1.1 link under i.MX6DL Developer VM Images.
2. Next install virtual box and setup Reach Technologies VM on it. Instructions for that are [here](https://reach-technology-g3-manuals.readthedocs-hosted.com/en/latest/dev/install/windoze.html)
3. Configure settings for virtual machine. Instructions [here](https://reach-technology-g3-manuals.readthedocs-hosted.com/en/latest/dev/config/windoze.html#win-dev-env-config)

## Serial Connections
The virtual machine provided by Reach Technologies has aliases to access USB RS232 connections for interacting with the G3 device. If you open a terminal in the VM and type `com0` or `com1` the terminal will be able to read and write data to serial connections. The first RS232 connection will be `com0` and the second will be `com1`.
### Serial Debugger
The G3 has a debug port that can connect to the virtual machine over USB. Connecting to it with a is similar to a SSH connection. This is helpful for identifying the IP address of the G3, looking at boot messages, and changing configuration files.
### Serial Communication
It is also possible to set up a RS232 connection with the G3. This is used for sending and recieving messages to applications.

# Peripheral Demo
## User Interface
The QML file used is derived from the Qt Hello example. 
### Torque Display
At the top of the screen there is a torque value displayed. Pressing the buttons for incrementing and decrmenting torque will update the value. This will also log those changes in a txt file at the root of the file system on the G3
### Console and Bluetooth
There is a console that displays "Hello World" on start up. When a serial message is written to the G3 it will also be displayed here. When the "Dump Log File" is pressed, messages will be sent to the console. Also, the G3 will search for a reach development machine over bluetooth. If one is available, it will start a bluetooth client and attempt to send the contents of a log file to a server on the development machine.

## Serial Communications
The RS232 connection on the G3 is utilized to send and recieve messages. When buttons are pressed on the user interface, serial messages are sent. It is also possible to update the torque value displayed on the console using the command `torque:<torqueValue>`.

## Bluetooth
The bluetooth functionality for this demo is based on the btchat example provided by Qt. When the "Dump Log File" button is pressed, the device goes into service discovery. If the desired server name is found, the G3 starts a client and sends the contents of a log file to the server. There is also a `btserver` module in this demo. That is unused at the moment.
### To do
1. I initially wanted to have the server running on the G3. I was able to setup a connection with a client on my computer, and send messages from the client to the G3, but I could not send messages from the G3 to the client. I eventually gave up, and flipped the roles to get this working.
2. The service discovery module is unable to identify the correct UUID for the server running on my computer. I am confused about this because it can identify other UUIDs coming from my computer, just no the server I am using for testing. To get this demo working, I defined the UUID the server uses in code so that the client could connect to it, but it would be better for the client to identify the UUID on its own.
3. Currently, the MAC address of the bluetooth module for the G3 is defined as a variable in code. Similar to the above task, it would be better for the G3 to identify its own mac address in case we ever need to switch out the bluetooth dongle.