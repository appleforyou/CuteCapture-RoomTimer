# What is CuteCapture?

CuteCapture is a program written by Gotos, which displays the video feed from capture devices from https://3dscapture.com.
Go here for an explanation of the original CuteCapture:
https://github.com/Gotos/CuteCapture

# What is RoomTimer?

This is a WIP of a modification to the original CuteCapture, which shows the time it took to complete the previous room. It's intended for speedrun practice. It works on DS, and (tentatively) it should work on 3DS (not New3DSXL) now as well, although I don't own a 3DS so I can't test this myself. Please report any issues you have that do not happen in the regular version of CuteCapture.

Text will appear on the capture window by default in the upper left of the top screen. It works by counting the number of frames between separate sections of completely black frames, and then displaying this number until you reach another separate set of black frames. This should work as a room timer for any game in which rooms are separated by black frames. All of the calculation is done by analyzing the video feed, so it won't interfere with the operation of the original game at all.

For 3DS games it will simply show time in seconds (for example 3.25 seconds) since it seems framerate in 3DS games is not as consistent as DS games are. Be aware that the time can only be as precise as the framerate, so it won't actually be accurate to 0.01 seconds.
For DS games (including when running on 3DS) it is assumed that the game runs at a consistent "60 fps." Time is shown in the format (SECONDS)\`(FRAMES). For example 3\`42 is 3 seconds and 42 frames, or 222 total frames. For the sake of simplicity, 60 frames are equated to 1 second.

In the future I may implement controls to choose timing method (i.e. real time vs. seconds+frames at 60fps vs. 30fps)

# Controls

All keyboard controls from the original CuteCapture are retained. New controls have been added:
- Press the up or down arrow key to move the timer to the top or bottom screen. This defaults to the top screen.
- Press the PageUp or PageDown key to change which screen is currently being checked for black frames. This defaults to the bottom screen.
- Press the End key to toggle whether the timer is displayed or hidden.

# Warnings

- The 3DS implementation is experimental and requires testing from other people to make sure it works, as I do not own one.
- This is only tested on linux with libsfml version 2.5. That is also what the zip version in Releases is for. Check the original CuteCapture for notes on building from source.
- It will of course not be useful if the room is not separated by black frames, such as when the screen fades to white.
- A frame is seen as a black frame if over half of the pixels on the chosen screen are completely black.
- If your computer can't keep up with the fps of the video feed, the results may not be useful. The timer can only be as accurate as the framerate.
- For DS games specifically it is assumed that they run at 60fps, but there are a few DS games that don't.
- A separate counter for the black frames between rooms may be added in the future.

# Note on the timing method

As mentioned above, the timer simplifies to 60fps, but most DS games are actually 59.8261 fps. There's no way to cleanly convert this to seconds + extra frames, so readability was prioritized. As a speedrun practice tool the most important function is to see the difference in frames. If you want to do the real-time math for whatever reason, you can convert seconds to 60 frames each before dividing by the real frame rate.

Unfortunately it seems that there are very few 3DS games that can be relied on to run at 60fps, so they are set to simply display real time at the moment. This may become a controllable option in the future.
