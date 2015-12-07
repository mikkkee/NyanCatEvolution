# Checklist for setting up MicroSoft Visual Studio to work with OpenCV

1. Download [OpenCV][1] and extract it to desired directory, for example `D:\opencv`.
2. Set up environment variables for Windows:
   1. `OPENCV_DIR` = `D:\opencv\build`
   2. Add `%OPENDC_DIR%\x86\vc12\bin` to `PATH`
3. Set up properties of MicroSoft Visual Studio project (*Only works for OpenCV 3,
   need to add [more files][2] in step iv for OpenCV 2*):
   1. Right click on project name in `Solution Explorer` and choose `Properties`.
   2. In `Configuration Properties -> C/C++ -> General -> Additional Include Directories`,
      add `$(OPENCV_DIR)/include` to it. This will add OpenCV header files into
      compiler's search directories.
   3. In `Configuration Properties -> Linker -> General -> Additional Library Directories`,
      add `$(OPENCV_DIR)/x86/vc12/bin` and `$(OPENCV_DIR)/x86/vc12/lib` to it.
   4. In `Configuration Properties -> Linker -> Input -> Additional Dependencies`, add
      `opencv_ts300.lib` and `opencv_world300.lib` into it (*Use `opencv_ts300d.lib`
      and `opencv_world300d.lib` for Debug*).

[1]: http://opencv.org/
[2]: http://stackoverflow.com/a/6091288/2621547