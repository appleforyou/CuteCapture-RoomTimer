# What is CuteCapture?

Go here for an explanation of the original CuteCapture:
https://github.com/Gotos/CuteCapture

# What is RoomTimer?

This is a WIP of a relatively minor modification to the original CuteCapture, which shows the time it took to complete the previous room. It's intended for speedrun practice, and it only works on DS for now because I don't have a 3DS to test with.

Text will appear on the capture window by default in the upper left of the top screen, showing the time in the format (SECONDS)\`(FRAMES). For example 3\`42 is 3 seconds and 42 frames, or 222 total frames. For the sake of simplicity, 60 frames are equated to 1 second. It works by counting the number of frames between separate sections of completely black frames, and then displaying this number until you reach another separate set of black frames. This should work as a room timer for any game in which rooms are separated by black frames. All of the calculation is done by analyzing the video feed, so it won't interfere with the operation of the original game at all.

# Controls

All keyboard controls from the original CuteCapture are retained. New controls have been added:
Press the up or down arrow key to move the timer to the top or bottom screen. This defaults to the top screen.
Press the PageUp or PageDown key to change which screen is currently being checked for black frames. This defaults to the bottom screen.
Press the End key to toggle whether the timer is displayed or hidden.

# Warnings

1. This only works on regular DS capture for now (not 3DS.) It should be easy to implement for 3DS, but I won't do that until I can get test results from a 3DS owner. If you have one, you're open to implement it yourself (as a pull request or a fork, what have you.)
2. This is only tested on linux with libsfml version 2.5. That is also what the zip version in Releases is for. Check the original CuteCapture for notes on building from source.
3. It will of course not be useful if the room is not separated by black frames, such as when the screen fades to white.
4. A frame is seen as a black frame if over half of the pixels on the chosen screen are completely black.
5. If your computer can't keep up with the fps of the game, the results may not be useful. It assumes that your capture program processes the capture feed at 60fps. It also assumes all games are 60fps, and that may not be true for all games.
6. A separate counter for the black frames between rooms may be added in the future.

# Note on the timing method

As mentioned above, the timer represents frames and not real time. It assumes 60fps which is slightly different from the real framerate of "60fps" games, so your results will not equate exactly to real time. This is done for readability's sake, and shouldn't change the results of which strategy is faster than another, as long as you're not attempting to compare times between this program and a real-time timer. If you want to do that, you will get a more accurate number by adding up the total frames and then dividing it by the true framerate (probably 59.8261 fps, but not all games may match this.)
Another option would be to show a raw framecount without converting to seconds. I could add this, or even real-time timing as an option if requested, but for me the current approach is a good enough tradeoff between readability and truthfulness of the numbers.
