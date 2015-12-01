# NyanCatEvolution

![NycanCat Animation][1] 
![NycanCat Original][2]
![NycanCat Mockup][3]

NyanCatEvolution is inspired by [EvoLisa][4], it implements a genetic algorithm
to mock up target image by ploting semi-transparent or opaque polygons on a canvas.

## Features
1. Implemented in C++/CLI with OpenCV.
2. Generic algorithm paremeters are optimized to favor more small changes over large
   changes. This helps the mock up image converge better to target image when they
   are close.
3. User can choose to resize the target image or not based on their preference on 
   speed or image quality.

[1]: http://i.imgur.com/RSfGBLn.gif
[2]: http://i.imgur.com/klT9PqQ.png
[3]: http://i.imgur.com/USYuNne.png
[4]: http://rogeralsing.com/2008/12/07/genetic-programming-evolution-of-mona-lisa/