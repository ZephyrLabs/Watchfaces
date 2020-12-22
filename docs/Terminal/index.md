# Terminal.
It's no Commodore!

The text based presentation of info loved by tech enthusiasts (and others).
Note: One DFU contains vibration for notifications.

![Terminal](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Terminal/Terminal.jpg "Terminal")

### for Infinitime (latest - 0.9)
[terminal.cpp](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Terminal/terminal.cpp)
[terminal.h](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Terminal/terminal.h)

Source code (Vibration): [MotorController.cpp](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Terminal/motor/MotorController.cpp)
[MotorController.h](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Terminal/motor/MotorController.h)
[CmakeLists.txt](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Terminal/CMakeLists.txt)
[Notifications.cpp](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Terminal/motor/Notifications.cpp)
[Notifications.h](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Terminal/motor/Notifications.h)

### Source Install info: 
MotorController files go into (your InfiniTime folder)/src/components/motor/MotorController.cpp/.h
CMakeLists.txt goes into (your InfiniTime folder)/ , this is needed for the compiler to know to link the above files
Replace the Notifications.cpp/.h in (your InfiniTime folder)/src/displayapp/screens/ 

pre-compiled Infinitime: download [Vibration](https://github.com/ZephyrLabs/Watchfaces/raw/gh-pages/docs/Terminal/pinetime-mcuboot-app-dfu(terminal).zip)

Alt download: [No Vibration](https://github.com/ZephyrLabs/Watchfaces/raw/gh-pages/docs/Terminal/pinetime-mcuboot-app-dfu(terminalNoV).zip)
