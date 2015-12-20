# NyanCatEvolution

![NycanCat Animation][1]
![NycanCat Original][2]
![NycanCat Mockup][3]

NyanCatEvolution is inspired by [EvoLisa][4], it implements a genetic algorithm
to mock up target image by ploting opaque polygons on a canvas.

## Features
* Implemented in C++ with OpenCV 2.4.
* Uses opaque polygons instead of semi-transparent polygons to speed up mutation
  step (Semi-transparent polygon support to be added).
* Generic algorithm paremeters are optimized to favor many small changes over
  single large changes. This helps the mock up image converge better to target
  image when they are close.
* Singleton mutations are used instead of cross mutations. This again helps
  improve the speed of convergence.
* User can choose to resize the target image or not based on their preference on
  speed or image quality by setting `MaxHeight` and `MaxWidth` parameters in
  `settings.h`.
* Useful tools for printing canvas and log convergence details.

## Install
1. Install OpenCV 2.4 and set up environmental variables as [MSVS.Setup.md][5]
   documented if you are using Windows.
2. Install [CMake][6].
3. Run the following commands in `bash` or `cmd`. You'll have either an executable
   or a Visual Studio Project.
```bash
git clone https://github.com/mikkkee/NyanCatEvolution.git
cd NyanCatEvolution
mkdir build
cd build
cmake ..
```

[1]: http://i.imgur.com/RSfGBLn.gif
[2]: http://i.imgur.com/klT9PqQ.png
[3]: http://i.imgur.com/USYuNne.png
[4]: http://rogeralsing.com/2008/12/07/genetic-programming-evolution-of-mona-lisa/
[5]: MSVS.Setup.md
[6]: https://cmake.org/
