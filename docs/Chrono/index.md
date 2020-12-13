# Chrono 
a complex but intricate complication.

![chrono](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Chrono/chrono.png "chrono")

#### Source code:
[chrono.cpp](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Chrono/chrono.cpp)

[chrono.h](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Chrono/chrono.h)

[earth.h](https://raw.githubusercontent.com/ZephyrLabs/Watchfaces/gh-pages/docs/Chrono/earth.h)

### Documentation:
chrono is a watch complication much similar to the pinealog watch complication, as in it is also an analog complication,
but this complication is unique from that as in, it has a stopwatch that is accurate and functional, as well as a functioning atlas.

to use this watchface, you need to copy the contents of chrono.cpp and chrono.h to displayapp/Clock.cpp and displayapp/Clock.h of your infinitime repository.
as for earth.h place it in the displayapp/ directory with the Clock.cpp and Clock.h and all the other app files, 

then add the file in src/CMakeList.txt:
 ```
set(INCLUDE_FILES
        BootloaderVersion.h
        logging/Logger.h
        logging/NrfLogger.h
        displayapp/DisplayApp.h
        displayapp/TouchEvents.h
        displayapp/screens/Screen.h
        displayapp/screens/Clock.h
+      displayapp/screens/earth.h // the file called earth.h is added here along with all other app header files
        displayapp/screens/Tile.h
        displayapp/screens/Meter.h
        displayapp/screens/Gauge.h
        displayapp/screens/InfiniPaint.h
        displayapp/screens/DropDownDemo.h
```

once these steps are done, you can compile the code and obtain your flashable binary.

more documentation coming later...
