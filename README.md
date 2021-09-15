# ml-skynet-snake

Snake game clone using SDL and Machine learning for my C++ practice purposes in Windows environment with Visual Studio.

Snake game works and can be controlled with arrow keys. Space bar starts another game.

SDL Dependencies:
SDL2_image
SDL2_ttf
SDL2

Snake controlled by Ai starts by pressing 'a' key.

Machine learning with Mlpack sort of works, but its still complete retard when "Ai" is
playing the game.

Machine learning Dependency:
MlPack

Game and code structure might be a bit overcomplicated for such a simple game as a Snake
clone, but point of this project is to make it on object oriented style and using various
C++ features including some from C++ 17.

Visuals have been secondary objective within this practice project.

![snake](https://user-images.githubusercontent.com/64461167/133246765-a5c4a87e-d009-4702-8ac1-c80ab68060ce.jpg)

Snake has vision for every 45 degree angle; 8 directions in total visualized as dotted lines on the screen.
