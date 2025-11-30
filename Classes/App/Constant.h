//常量设定 公用
#pragma once
#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <map>
#include <string>
#include "Scenes/SettingScene.h"
#include "Scenes/MenuScene.h"
constexpr int DESIGN_RESOLUTION_WIDTH = 1280;                               // 设计分辨率宽度
constexpr int DESIGN_RESOLUTION_HEIGHT = 720;                               // 设计分辨率高度

constexpr int SMALL_RESOLUTION_WIDTH = 960;                                 // 小分辨率宽度
constexpr int SMALL_RESOLUTION_HEIGHT = 540;                                // 小分辨率高度
constexpr int MEDIUM_RESOLUTION_WIDTH = 1280;                               // 中分辨率宽度
constexpr int MEDIUM_RESOLUTION_HEIGHT = 720;                               // 中分辨率高度
constexpr int LARGE_RESOLUTION_WIDTH = 1920;                                // 大分辨率宽度
constexpr int LARGE_RESOLUTION_HEIGHT = 1080;                               // 大分辨率高度
constexpr float FRAME_RATE = 60.0f;                                         // 应用程序帧率
const std::string GAME_TITLE = "StarDewValley";                             // 应用程序标题

//Loading场景
const int LOADING_SCENE_LOADINGBAR_OFFSET_Y = -241;                     // 启动场景加载进度条位置 Y 偏移量
const int LOADING_SCENE_LOADING_LABEL_OFFSET_X = -30;                   // 启动场景加载进度条标签位置 X 偏移量
const int LOADING_SCENE_FONT_SIZE = 20;

const int LOADING_SCENE_DURATION_TIME = 2.0f;

const int SCENE_TRANSITION_DURATION = 1.0f; //场景过渡时间

/*
//menu场景
const  int INITIAL_SCENE_BUTTON_OFFSET_Y1 = -241;
const  int INITIAL_SCENE_BUTTON_OFFSET_Y2 = -241;
const  int INITIAL_SCENE_BUTTON_OFFSET_Y3 = -241;
const  int INITIAL_SCENE_BUTTON_OFFSET_X1 = -100;
const  int INITIAL_SCENE_BUTTON_OFFSET_X2 = -30;
const  int INITIAL_SCENE_BUTTON_OFFSET_X3 = -40;
*/

//Game scene场景

const int MAP_WIDTH = 105 * 16;
const int MAP_HEIGHT = 65* 16;
const int MAPBLOCK_WIDTH = 105 * 16/2;
const int MAPBLOCK_HEIGHT = 65 * 16/2;


#endif
