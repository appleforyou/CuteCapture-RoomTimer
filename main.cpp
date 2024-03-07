/*
 * Copyright 2014-2020 Alina Saalfeld
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 *         limitations under the License.
 */
/*
 * (Above is the original CuteCapture license)
 */

#include <stdio.h>
#include <unistd.h>
extern "C" {
#ifdef DS
    #include "dscapture.h"
#else
    #include "3dscapture.h"
#endif
}
#include "favicon.h"
#include "helpers.h"
#include <SFML/Graphics.hpp>
#include <map>


static sf::Clock m_time;
static uint frames;
static bool split;
#ifndef DS
static bool ds_crop_mode;
#endif
static bool init;
static std::map<int, int> keycode_zoom_map {
    { sf::Keyboard::Num0, 10},
    { sf::Keyboard::Num1, 1},
    { sf::Keyboard::Num2, 2},
    { sf::Keyboard::Num3, 3},
    { sf::Keyboard::Num4, 4},
    { sf::Keyboard::Num5, 5},
    { sf::Keyboard::Num6, 6},
    { sf::Keyboard::Num7, 7},
    { sf::Keyboard::Num8, 8},
    { sf::Keyboard::Num9, 9},
    { sf::Keyboard::Numpad0, 10},
    { sf::Keyboard::Numpad1, 1},
    { sf::Keyboard::Numpad2, 2},
    { sf::Keyboard::Numpad3, 3},
    { sf::Keyboard::Numpad4, 4},
    { sf::Keyboard::Numpad5, 5},
    { sf::Keyboard::Numpad6, 6},
    { sf::Keyboard::Numpad7, 7},
    { sf::Keyboard::Numpad8, 8},
    { sf::Keyboard::Numpad9, 9}
};

int main()
{
    init = capture_init();
    split = false;
#ifndef DS
    ds_crop_mode = false;
#endif
    frames = 0;
    m_time = sf::Clock();
#ifdef DS
    auto window_title = "Cute DS Capture";
#else
    auto window_title = "Cute 3DS Capture";
#endif
    char bottom_screen_title[40];
    sprintf(bottom_screen_title, "Bottom Screen %s", window_title);

#ifdef DS
    sf::RenderWindow window(sf::VideoMode(FRAMEWIDTH, FRAMEHEIGHT*2), window_title);
#else
    sf::RenderWindow window(sf::VideoMode(400, 480), window_title);
#endif
    window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
    sf::RenderWindow bottom_window;

#ifdef DS
    sf::RectangleShape top_screen(sf::Vector2f(FRAMEWIDTH, FRAMEHEIGHT));
    sf::RectangleShape bottom_screen(sf::Vector2f(FRAMEWIDTH, FRAMEHEIGHT));
    bottom_screen.setPosition(0,FRAMEHEIGHT);

    sf::View total = sf::View(sf::FloatRect(0, 0, FRAMEWIDTH, FRAMEHEIGHT*2));
    sf::View top = sf::View(sf::FloatRect(0, 0, FRAMEWIDTH, FRAMEHEIGHT));
    sf::View bottom = sf::View(sf::FloatRect(0, FRAMEHEIGHT, FRAMEWIDTH, FRAMEHEIGHT));
#else
    sf::RectangleShape top_screen(sf::Vector2f(240,400));
    top_screen.rotate(-90);
    top_screen.setPosition(0,240);
    sf::RectangleShape bottom_screen(sf::Vector2f(240,320));
    bottom_screen.rotate(-90);
    bottom_screen.setPosition(40,480);

    sf::View total = sf::View(sf::FloatRect(0, 0, 400, 480));
    sf::View top = sf::View(sf::FloatRect(0, 0, 400, 240));
    sf::View bottom = sf::View(sf::FloatRect(40, 240, 320, 240));
    sf::View ds_crop_combined = sf::View(sf::FloatRect(72, 48, 256, 384));
    sf::View ds_crop_top = sf::View(sf::FloatRect(72, 48, 256, 192));
    sf::View ds_crop_bottom = sf::View(sf::FloatRect(72, 240, 256, 192));
#endif


    sf::Texture texture;
    uint8_t tex[] = {0,0,0,255};
#ifdef DS
    texture.create(int(FRAMEWIDTH),int(FRAMEHEIGHT) * 2);
#else
    texture.create(int(FRAMEWIDTH),int(FRAMEHEIGHT));
#endif
    texture.update(tex,1,1,0,0);

#ifdef DS
    top_screen.setTexture(&texture);
    top_screen.setTextureRect(sf::IntRect(0, 0, FRAMEWIDTH, FRAMEHEIGHT));
    bottom_screen.setTexture(&texture);
    bottom_screen.setTextureRect(sf::IntRect(0, FRAMEHEIGHT, FRAMEWIDTH, FRAMEHEIGHT));
#else
    top_screen.setTexture(&texture);
    top_screen.setTextureRect(sf::IntRect(0,0,240,400));
    bottom_screen.setTexture(&texture);
    bottom_screen.setTextureRect(sf::IntRect(0,400,240,320));

    sf::Clock roomtime;
    const uint TOPSIZE = 96000; //400*240
    const uint BOTTOMSIZE = 76800; //320*240
#endif
    sf::Font font;
    sf::Text text;
    font.loadFromFile("Font/URWGothic-Book.ttf");
    text.setFont(font);
    text.setString("0`0");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setOutlineThickness(2);

    uint roomframes = 0;
    bool blackframe = true;
    bool timing_bottom = true;
    bool text_bottom = false;
    bool text_visible = true;



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
            case sf::Event::Closed:
                if (bottom_window.isOpen()) {
                    bottom_window.close();
                }
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code) {
                    case sf::Keyboard::Num1:
                    case sf::Keyboard::Num2:
                    case sf::Keyboard::Num3:
                    case sf::Keyboard::Num4:
                    case sf::Keyboard::Num5:
                    case sf::Keyboard::Num6:
                    case sf::Keyboard::Num7:
                    case sf::Keyboard::Num8:
                    case sf::Keyboard::Num9:
                    case sf::Keyboard::Num0:
                    case sf::Keyboard::Numpad1:
                    case sf::Keyboard::Numpad2:
                    case sf::Keyboard::Numpad3:
                    case sf::Keyboard::Numpad4:
                    case sf::Keyboard::Numpad5:
                    case sf::Keyboard::Numpad6:
                    case sf::Keyboard::Numpad7:
                    case sf::Keyboard::Numpad8:
                    case sf::Keyboard::Numpad9:
                    case sf::Keyboard::Numpad0:
                    // Zoom adjustment when top or main window is focused
#ifndef DS
                        if (!ds_crop_mode) {
                            window.setSize(sf::Vector2u(
                                (200 * keycode_zoom_map[event.key.code] + 200),
                                (3 * (200 * keycode_zoom_map[event.key.code] + 200) / 5) * (2-int(split))
                            ));
                        } else {
#endif
                            window.setSize(sf::Vector2u(
                                (128 * keycode_zoom_map[event.key.code] + 128),
                                (3 * (128 * keycode_zoom_map[event.key.code] + 128) / 4) * (2-int(split))
                            ));
#ifndef DS
                        }
#endif

                        break;
                case sf::Keyboard::C:
                // Switch to/from crop mode
#ifndef DS
                if (!ds_crop_mode) {
                    if (!split) {
                        window.setView(ds_crop_combined);
                        window.setSize(sf::Vector2u(256, 384));
                    } else {
                        window.setView(ds_crop_top);
                        window.setSize(sf::Vector2u(256, 192));
                        bottom_window.create(sf::VideoMode(256, 192), bottom_screen_title);
                        bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                        bottom_window.setView(ds_crop_bottom);
                    }
                    ds_crop_mode = true;
                    if (!text_bottom) {
                        text.setPosition(72, 48);
                    } else {
                        text.setPosition(72, 240);
                    }
                } else {
                    if (!split) {
                        window.setView(total);
                        window.setSize(sf::Vector2u(400, 480));
                    } else {
                        window.setView(top);
                        window.setSize(sf::Vector2u(400, 240));
                        bottom_window.create(sf::VideoMode(320, 240), bottom_screen_title);
                        bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                        bottom_window.setView(bottom);
                    }
                    ds_crop_mode = false;
                    if (!text_bottom) {
                        text.setPosition(0, 0);
                    } else {
                        text.setPosition(40, 240);
                    }
                }
#endif
                    break;
                 case sf::Keyboard::Space:
                    if (!split) {
#ifndef DS
                        if (!ds_crop_mode) {
                            window.setView(top);
                            window.setSize(sf::Vector2u(400, 240));
                            bottom_window.create(sf::VideoMode(320, 240), bottom_screen_title);
                            bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                            bottom_window.setView(bottom);
                        } else {
                            window.setView(ds_crop_top);
                            window.setSize(sf::Vector2u(256, 192));
                            bottom_window.create(sf::VideoMode(256, 192), bottom_screen_title);
                            bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                            bottom_window.setView(ds_crop_bottom);
                        }
#else
                        window.setView(top);
                        window.setSize(sf::Vector2u(FRAMEWIDTH, FRAMEHEIGHT));
                        bottom_window.create(sf::VideoMode(FRAMEWIDTH, FRAMEHEIGHT), "Bottom Screen Cute DS Capture");
                        bottom_window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
                        bottom_window.setView(bottom);
#endif
                        split = true;
                    } else {
#ifndef DS
                        if (!ds_crop_mode) {
                            window.setView(total);
                            window.setSize(sf::Vector2u(400, 480));
                        } else {
                            window.setView(ds_crop_combined);
                            window.setSize(sf::Vector2u(256, 384));
                        }
#else
                        window.setView(total);
                        window.setSize(sf::Vector2u(FRAMEWIDTH, FRAMEHEIGHT * 2));
#endif
                        bottom_window.close();
                        split = false;
                    }
                    break;
#ifdef DS
                case sf::Keyboard::Up:
                    text.setPosition(0, 0);
                    text_bottom = false;
                    break;
                case sf::Keyboard::Down:
                    text.setPosition(0, FRAMEHEIGHT);
                    text_bottom = true;
                    break;
#else
                case sf::Keyboard::Up:
                    if (!ds_crop_mode) {
                        text.setPosition(0,0);
                    } else {
                        text.setPosition(72,48);
                    }
                    text_bottom = false;
                    break;
                case sf::Keyboard::Down:
                    if (!ds_crop_mode) {
                        text.setPosition(40, 240);
                    } else {
                        text.setPosition(72, 240);
                    }
                    text_bottom = true;
                    break;
#endif
                case sf::Keyboard::PageUp:
                    timing_bottom = false;
                    text.setString("Checking frames on\ntop screen.");
                    break;
                case sf::Keyboard::PageDown:
                    timing_bottom = true;
                    text.setString("Checking frames on\nbottom screen.");
                    break;
                case sf::Keyboard::End:
                    text_visible = !text_visible;
                    break;
                default:
                    break;
                }

            default:
                break;
            }

        }

        if (bottom_window.isOpen()) {
            while (bottom_window.pollEvent(event))
            {
                switch(event.type) {
                case sf::Event::Closed:
                    bottom_window.close();
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code) {
                    case sf::Keyboard::Num1:
                    case sf::Keyboard::Num2:
                    case sf::Keyboard::Num3:
                    case sf::Keyboard::Num4:
                    case sf::Keyboard::Num5:
                    case sf::Keyboard::Num6:
                    case sf::Keyboard::Num7:
                    case sf::Keyboard::Num8:
                    case sf::Keyboard::Num9:
                    case sf::Keyboard::Num0:
                    case sf::Keyboard::Numpad1:
                    case sf::Keyboard::Numpad2:
                    case sf::Keyboard::Numpad3:
                    case sf::Keyboard::Numpad4:
                    case sf::Keyboard::Numpad5:
                    case sf::Keyboard::Numpad6:
                    case sf::Keyboard::Numpad7:
                    case sf::Keyboard::Numpad8:
                    case sf::Keyboard::Numpad9:
                    case sf::Keyboard::Numpad0:
                    // Zoom adjustment when bottom window is focused
#ifndef DS
                        if (!ds_crop_mode) {
                            bottom_window.setSize(sf::Vector2u(
                                (160 * keycode_zoom_map[event.key.code] + 160),
                                (3 * (160 * keycode_zoom_map[event.key.code] + 160) / 4)
                            ));
                        } else {
#endif
                            bottom_window.setSize(sf::Vector2u(
                                (128 * keycode_zoom_map[event.key.code] + 128),
                                (3 * (128 * keycode_zoom_map[event.key.code] + 128) / 4) * (2-int(split))
                            ));
#ifndef DS
                        }
#endif

                        break;
#ifndef DS
                    case sf::Keyboard::C:
                        if (!ds_crop_mode) {
                            if (!split) {
                                window.setView(ds_crop_combined);
                                window.setSize(sf::Vector2u(256, 384));
                            } else {
                                window.setView(ds_crop_top);
                                window.setSize(sf::Vector2u(256, 192));
                                bottom_window.create(sf::VideoMode(256, 192), bottom_screen_title);
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(ds_crop_bottom);
                            }
                            ds_crop_mode = true;
                            if (!text_bottom) {
                                text.setPosition(72, 48);
                            } else {
                                text.setPosition(72, 240);
                            }
                        } else {
                            if (!split) {
                                window.setView(total);
                                window.setSize(sf::Vector2u(400, 480));
                            } else {
                                window.setView(top);
                                window.setSize(sf::Vector2u(400, 240));
                                bottom_window.create(sf::VideoMode(320, 240), bottom_screen_title);
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(bottom);
                            }
                            ds_crop_mode = false;
                            if (!text_bottom) {
                                text.setPosition(0, 0);
                            } else {
                                text.setPosition(40, 240);
                            }
                        }
                        break;
#endif
                    case sf::Keyboard::Space:
                        if (!split) {
#ifndef DS
                            if (!ds_crop_mode) {
                                window.setView(top);
                                window.setSize(sf::Vector2u(400, 240));
                                bottom_window.create(sf::VideoMode(320, 240), bottom_screen_title);
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(bottom);
                            } else {
                                window.setView(ds_crop_top);
                                window.setSize(sf::Vector2u(256, 192));
                                bottom_window.create(sf::VideoMode(256, 192), bottom_screen_title);
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(ds_crop_bottom);
                            }
#else
                            window.setView(top);
                            window.setSize(sf::Vector2u(FRAMEWIDTH, FRAMEHEIGHT));
                            bottom_window.create(sf::VideoMode(FRAMEWIDTH, FRAMEHEIGHT), "Bottom Screen Cute DS Capture");
                            bottom_window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
                            bottom_window.setView(bottom);
#endif
                            split = true;
                        } else {
#ifndef DS
                            if (!ds_crop_mode) {
                                window.setView(total);
                                window.setSize(sf::Vector2u(400, 480));
                            } else {
                                window.setView(ds_crop_combined);
                                window.setSize(sf::Vector2u(256, 384));
                            }
#else
                            window.setView(total);
                            window.setSize(sf::Vector2u(FRAMEWIDTH, FRAMEHEIGHT * 2));
#endif
                            bottom_window.close();
                            split = false;
                        }
                        break;
#ifdef DS
                    case sf::Keyboard::Up:
                        text.setPosition(0, 0);
                        text_bottom = false;
                        break;
                    case sf::Keyboard::Down:
                        text.setPosition(0, FRAMEHEIGHT);
                        text_bottom = true;
                        break;
#else
                    case sf::Keyboard::Up:
                        if (!ds_crop_mode) {
                            text.setPosition(0,0);
                        } else {
                            text.setPosition(72,48);
                        }
                        text_bottom = false;
                        break;
                    case sf::Keyboard::Down:
                        if (!ds_crop_mode) {
                            text.setPosition(40, 240);
                        } else {
                            text.setPosition(72, 240);
                        }
                        text_bottom = true;
                        break;
#endif
                    case sf::Keyboard::PageUp:
                        timing_bottom = false;
                        text.setString("Checking frames on\ntop screen.");
                        break;
                    case sf::Keyboard::PageDown:
                        timing_bottom = true;
                        text.setString("Checking frames on\nbottom screen.");
                        break;
                    case sf::Keyboard::End:
                        text_visible = !text_visible;
                        break;
                    default:
                        break;
                    }

                default:
                    break;
                }

            }
        }

        frames++;
        if (m_time.getElapsedTime().asMilliseconds() > 500) {
            char title[50];
            if (!split) {
                sprintf(title, "%s (%.2f FPS)", window_title, float(frames)/(m_time.getElapsedTime().asSeconds()));
            } else {
                sprintf(title,"Top Screen %s (%.2f FPS)", window_title, float(frames)/(m_time.getElapsedTime().asSeconds()));
            }
            m_time.restart();
            frames = 0;
            window.setTitle(title);
        }

        window.clear();
        if(split) {
            bottom_window.clear();
        }

        if(!init) {
            usleep(1000*20); //20 milliseconds
            init = capture_init();
        }

#ifdef DS
        uint16_t frameBuf[FRAMEHEIGHT*FRAMEWIDTH*2];
        uint8_t rgbaBuf[FRAMEHEIGHT*FRAMEWIDTH*2*4];
#else
        uint8_t frameBuf[FRAMEBUFSIZE];
        uint8_t rgbaBuf[FRAMESIZE*4/3];
#endif

        if(init){
#ifdef DS
            if(capture_grabFrame(frameBuf)) {
                toRGBA(rgbaBuf,frameBuf);
                texture.update(rgbaBuf,int(FRAMEWIDTH),int(FRAMEHEIGHT*2),0,0);

                uint matchcount = 0;
                for (uint i = FRAMEHEIGHT*FRAMEWIDTH*timing_bottom; i < FRAMEHEIGHT*FRAMEWIDTH*(timing_bottom+1); ++i) {
                    if (frameBuf[i] == 0) {
                        ++matchcount;
                    }
                }
                if (matchcount > FRAMEHEIGHT*FRAMEWIDTH/2) {
                    if (!blackframe) {
                        char modframes[3];
                        snprintf (modframes, 3, "%02d", roomframes%60);
                        text.setString(std::to_string(roomframes/60)+"`"+modframes);
                        blackframe = true;
                        roomframes = 0;
                    }
                } else {
                    ++roomframes;
                    blackframe = false;
                }
            } else {
                capture_deinit();
                init = false;
            }
#else
            switch(capture_grabFrame(frameBuf)) {
            case CAPTURE_OK:
            {
                toRGBA(frameBuf,rgbaBuf);
                texture.update(rgbaBuf,int(FRAMEWIDTH),int(FRAMEHEIGHT),0,0);

                uint matchcount = 0;
                if (!ds_crop_mode) {
                    uint screen_begin = TOPSIZE*timing_bottom;
                    uint screen_end = TOPSIZE + (BOTTOMSIZE*timing_bottom);
                    uint targetcount = (timing_bottom) ? BOTTOMSIZE / 2 : TOPSIZE / 2;
                    for (uint i = screen_begin; i < screen_end; ++i) {
                        if ((frameBuf[i*3] == 0) && (frameBuf[i*3+1] == 0) && (frameBuf[i*3+2]) == 0) {
                            ++matchcount;
                        }
                    }
                    if (matchcount > targetcount) {
                        if (!blackframe) {
                            sf::Int32 roomms = roomtime.getElapsedTime().asMilliseconds();
                            char modms[3];
                            snprintf (modms, 3, "%02d", roomms%1000);
                            text.setString(std::to_string(roomms/1000)+"."+modms);
                            blackframe = true;
                        }
                    } else {
                        if (blackframe) {
                            roomtime.restart();
                            blackframe = false;
                        }
                    }
                } else {
                    //DS screens are internally offset from the actual outsides of the 3DS maximum screen size.
                    //Both screens are stored "horizontally" in frameBuf so the "x" value is used to get to the bottom screen
                    uint dscrop_offset_x = 360*timing_bottom; //400 (3DS top screen width) - 72 (top screen left margin) + 32 (bottom screen left margin)
                    uint dscrop_offset_y = 48*timing_bottom; //240 (3DS screen height) - 192 (DS screen height)
                    for (uint x = 72 + dscrop_offset_x; x < 328 + dscrop_offset_x; ++x) {
                        for (uint y = 0 + dscrop_offset_y; y < 192 + dscrop_offset_y; ++y) {
                            uint i = x * 240 + y;
                            if ((frameBuf[i*3] == 0) && (frameBuf[i*3+1] == 0) && (frameBuf[i*3+2]) == 0) {
                                ++matchcount;
                            }
                        }
                    }
                    if (matchcount > FRAMEHEIGHT*FRAMEWIDTH/2) {
                        if (!blackframe) {
                            char modframes[3];
                            snprintf (modframes, 3, "%02d", roomframes%60);
                            text.setString(std::to_string(roomframes/60)+"`"+modframes);
                            blackframe = true;
                            roomframes = 0;
                        }
                    } else {
                        ++roomframes;
                        blackframe = false;
                    }
                }
                break;
            }
            case CAPTURE_ERROR:
                capture_deinit();
                init = false;
                break;
            default:
                break;
            }
#endif
        }

        window.draw(top_screen);

        if (!split) {
            window.draw(bottom_screen);
            if (text_visible) {
                window.draw(text);
            }
        } else {
            bottom_window.draw(bottom_screen);
            if (text_visible) {
                if (text_bottom) {
                    bottom_window.draw(text);
                } else {
                    window.draw(text);
                }
            }
            bottom_window.display();
        }

        window.display();
    }
    capture_deinit();
    return 0;
}
