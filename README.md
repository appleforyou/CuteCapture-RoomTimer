# What is CuteCapture?

Go here for an explanation of the original CuteCapture:
https://github.com/Gotos/CuteCapture

# What is RoomTimer?

This is a WIP of a relatively minor modification to the original CuteCapture, which shows the time it took to complete the previous room. It's intended for speedrun practice, and it only works on DS for now because I don't have a 3DS to test with.

Text will appear on the capture window in the upper left of the top screen, showing the time in the format [SECONDS]`[FRAMES]. For example 3`42 is 3 seconds and 42 frames, or 222 total frames. For the sake of simplicity, 60 frames are equated to 1 second. It works by counting the number of frames between separate sections of completely black frames, and then displaying this number until you reach another separate set of black frames. This should work as a room timer for any game in which rooms are separated by black frames. All of the calculation is done by analyzing the video feed, so it won't interfere with the operation of the original game at all.

# Warnings

1. This only works on regular DS capture for now (not 3DS.) It should be easy to implement for 3DS, but I won't do that until I can get test results from a 3DS owner. If you have one, you're open to implement it yourself (as a pull request or a fork, what have you.)
2. This is only tested on linux with libsfml version 2.5. That is also what the zip version in Releases is for. Check the original CuteCapture for notes on building from source.
3. The text appears on the top screen. This is convenient for the main game I use it for, but it may not be best for all games. I may add text placement as a config option in the future.
4. It will of course not be useful if the room is not separated by black frames, such as when the screen fades to white.
5. A frame is seen as a black frame if over half of the pixels on the bottom screen are completely black. In the future I may add config options to make this work for the top screen instead, or to change the ratio of black pixels required.
6. If your computer can't keep up with the fps of the game, the results may not be useful. It assumes that your capture program processes the capture feed at 60fps. It also assumes all games are 60fps, and that may not be true for all games.
7. A separate counter for the black frames between rooms may be added in the future.
