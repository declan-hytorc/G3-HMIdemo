import QtQuick 2.12
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2


import "."

ApplicationWindow {
  objectName: "theWindow"
  id: theWindow

  visible: true

  property string color1: "darkBlue"
  property string color2: "black"
  property string bColor: "yellow"
  property string colorText: "white"
  property int torqueVal: 500

  color: color2

  width: screen.width
  height: screen.height
  property int val: 0;
  property bool incPressed: false;
  property bool decPressed: false;

  signal submitTextField(string text)
  signal guiUpdatedTorque(int val)
  signal startBtServer()

  function setConsole(text) {
    consoleBox.append(text);
  }

  function getSerial(cmd) {
      consoleBox.append(cmd);
  }

  function updateTorqueDisplay(torque)
  {
      torqueVal = torque
      torqueDisplay.clear();
      torqueDisplay.append("Torque:");
      torqueDisplay.append(torque);
  }

  // This function is mainly used as a slot
  // for debugging the bluetooth funcitonality.
  // C++ files can invoke this to display messages
  // on the HMI
  function printBtUpdate(msg)
  {
      consoleBox.append(msg);
  }

  Rectangle {
    objectName: "button1"
    id: button1
    width: 240
    height: 56
    color: color1
    border.color: "white"
    border.width: 1
    radius: 18
    antialiasing: true

    anchors {
      top: parent.top
      topMargin: 50
      right: parent.right
      rightMargin: 18
    }

    Rectangle {
      objectName: "buttonIncrement"
      id: buttonIncrement
      height: 40
      width: height + 18
      color: color1

      border.color: bColor
      border.width: 1

      radius: 18
      antialiasing: true
      anchors.fill: parent

      Timer {
          id: longTimerInc
          interval: 1000;
          running: false;
          repeat: false;
          onTriggered: {
              if (incPressed){
                  shortTimerInc.start()
              }
          }
      }

      Timer {
          id: shortTimerInc
          interval: 200;
          running: false;
          repeat: true;
          onTriggered: {
              if (incPressed){
                  torqueVal = torqueVal + 1;
                  updateTorqueDisplay(torqueVal);
              }
              else
              {
                  shortTimerInc.stop()
              }
          }
      }

      Text {
        id: elementTextReadButton1
        color: colorText
        text: "Increment Torque"
        font.pixelSize: 20
        anchors {
          verticalCenter: parent.verticalCenter
          horizontalCenter: parent.horizontalCenter;
        }
      }

      Accessible.role: Accessible.Button
      Accessible.onPressAction: {
        if( pressed ){
            buttonIncrement.pressed()
        }
        else
        {
            buttonIncrement.released()
        }
      }

      signal pressed
      signal released

      MouseArea {
        id: mouseAreaToggle1
        anchors.fill: buttonIncrement
        onPressed: buttonIncrement.pressed()
        onReleased: buttonIncrement.released()
      }

      onPressed: {
        incPressed = true;
        longTimerInc.start();
        torqueVal = torqueVal + 1;
        updateTorqueDisplay(torqueVal);
      }
      onReleased: {
        incPressed = false;
        if(shortTimerInc.running){
          shortTimerInc.stop();
        }
        submitTextField("Torque Changed to " + torqueVal.toString());
      }
    }
  }


  //Decrement Torque
  Rectangle {
    objectName: "decrementTorque"
    id: decrementTorque
    width: button1.width
    height: 56
    color: color1
    border.color: bColor
    border.width: 1
    radius: 18
    antialiasing: true

    anchors {
      bottom: button1.bottom
      left: statusBox.left
    }

    Timer {
        id: longTimerDec
        interval: 1000;
        running: false;
        repeat: false;
        onTriggered: {
            if (decPressed){
                shortTimerDec.start()
            }
        }
    }

    Timer {
        id: shortTimerDec
        interval: 200;
        running: false;
        repeat: true;
        onTriggered: {
            if (decPressed){
                torqueVal = torqueVal - 1;
                updateTorqueDisplay(torqueVal);
            }
            else
            {
                shortTimerDec.stop()
            }
        }
    }

    Rectangle {
      objectName: "buttonDecrement"
      id: buttonDecrement
      height: parent.height - 24
      width: parent.width - 50
      color: color1

      border.color: bColor
      border.width: 1

      radius: 18
      antialiasing: true

      anchors {
        fill: parent
      }

      Text {
        id: elementDecrementTorque
        color: colorText
        text: "Decrement Torque"
        font.pixelSize: 20
        anchors {
          verticalCenter: parent.verticalCenter
          horizontalCenter: parent.horizontalCenter;
        }
      }

      Accessible.role: Accessible.Button
      Accessible.onPressAction: {
        if(pressed){
          button.pressed()
        }
        else{
          button.released()
        }
      }

      signal pressed
      signal released
      MouseArea {
        id: mouseAreaClear
        anchors.fill: parent
        onPressed: buttonDecrement.pressed()
        onReleased: buttonDecrement.released()
      }

      onPressed: {
        decPressed = true;
        longTimerDec.start();
        torqueVal = torqueVal - 1;
        updateTorqueDisplay(torqueVal);
      }

      onReleased: {
          decPressed = false;
          if(shortTimerDec.running)
          {
              shortTimerDec.stop()
          }
          submitTextField("Torque Changed to " + torqueVal.toString());
      }
    }

  }

  Rectangle {
    objectName: "dumpLogFile"
    id: dumpLogFile
    width: button1.width
    height: button1.height
    color: color1
    border.color: bColor
    border.width: 1
    radius: 18
    antialiasing: true

    anchors {
      top: statusBox.top
      left: button1.left
    }


    Rectangle {
      objectName: "buttonLogDump"
      id: buttonLogDump
      height: parent.height - 24
      width: parent.width - 50
      color: color1

      border.color: bColor
      border.width: 1

      radius: 18
      antialiasing: true

      anchors {
        fill: parent
      }

      Text {
        id: elementDump
        color: colorText
        text: "Dump Log File"
        font.pixelSize: 20
        anchors {
          verticalCenter: parent.verticalCenter
          horizontalCenter: parent.horizontalCenter;
        }
      }

      Accessible.role: Accessible.Button
      Accessible.onPressAction: {
        button.clicked();
      }

      signal clicked
      MouseArea {
        id: mouseAreaDump
        anchors.fill: parent
        onPressed: buttonLogDump.clicked()
      }

      onClicked: {
        consoleBox.append("Dumping Logs via Bluetooth")
        startBtServer()
      }
    }

  }

  Rectangle {
    objectName: "consoleClear"
    id: consoleClear
    width: button1.width
    height: button1.height
    color: color1
    border.color: bColor
    border.width: 1
    radius: 18
    antialiasing: true

    anchors {
      top: dumpLogFile.bottom
      topMargin: 20
      left: button1.left
    }


    Rectangle {
      objectName: "clearButton"
      id: clearButton
      height: parent.height - 24
      width: parent.width - 50
      color: color1

      border.color: bColor
      border.width: 1

      radius: 18
      antialiasing: true

      anchors {
        fill: parent
      }

      Text {
        id: clearLogConsole
        color: colorText
        text: "Clear Console"
        font.pixelSize: 20
        anchors {
          verticalCenter: parent.verticalCenter
          horizontalCenter: parent.horizontalCenter;
        }
      }

      Accessible.role: Accessible.Button
      Accessible.onPressAction: {
        button.clicked();
      }

      signal clicked
      MouseArea {
        id: mouseAreaClearC
        anchors.fill: parent
        onPressed: clearButton.clicked()
      }

      onClicked: {
        consoleBox.clear()
      }
    }

  }

  //Status / Console BOX
  Rectangle {
    objectName: "statusBox"
    id: statusBox
    width: parent.width - 300
    height: parent.height - 146
    color: color1
    border.color: bColor
    border.width: 1
    radius: 5
    antialiasing: true

    anchors {
      bottom: parent.bottom
      left: parent.left
      leftMargin: 20
      bottomMargin: 20
    }

    TextEdit {
      objectName: "consoleBox"
      id: consoleBox
      width: 240
      text: "Hello, World"
      color: colorText
      leftPadding: 15
      topPadding: 15
      bottomPadding: 15
    }

  }

  //Status / Console BOX
  Rectangle {
    objectName: "torqueReading"
    id: torqueReading
    width: 200
    height: 86
    color: color1
    border.color: bColor
    border.width: 1
    radius: 5
    antialiasing: true

    anchors {
      top: parent.top
      topMargin: 20
      left: parent.left
      leftMargin: 300
      bottomMargin: 20
    }

    TextEdit {
      objectName: "torqueDisplay"
      id: torqueDisplay
      width: 170
      text: "Torque:\n"
      color: colorText
      leftPadding: 15
      topPadding: 15
      bottomPadding: 15
    }

  }

}
