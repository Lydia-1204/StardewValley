//�����趨 ����
#pragma once
#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <map>
#include <string>
#include"SettingScene.h"
#include"MenuScene.h"
constexpr int DESIGN_RESOLUTION_WIDTH = 1280;                               // ��Ʒֱ��ʿ��
constexpr int DESIGN_RESOLUTION_HEIGHT = 720;                               // ��Ʒֱ��ʸ߶�

constexpr int SMALL_RESOLUTION_WIDTH = 960;                                 // С�ֱ��ʿ��
constexpr int SMALL_RESOLUTION_HEIGHT = 540;                                // С�ֱ��ʸ߶�
constexpr int MEDIUM_RESOLUTION_WIDTH = 1280;                               // �зֱ��ʿ��
constexpr int MEDIUM_RESOLUTION_HEIGHT = 720;                               // �зֱ��ʸ߶�
constexpr int LARGE_RESOLUTION_WIDTH = 1920;                                // ��ֱ��ʿ��
constexpr int LARGE_RESOLUTION_HEIGHT = 1080;                               // ��ֱ��ʸ߶�
constexpr float FRAME_RATE = 60.0f;                                         // Ӧ�ó���֡��
const std::string GAME_TITLE = "StarDewValley";                             // Ӧ�ó������

//Loading����
const int LOADING_SCENE_LOADINGBAR_OFFSET_Y = -241;                     // �����������ؽ�����λ�� Y ƫ����
const int LOADING_SCENE_LOADING_LABEL_OFFSET_X = -30;                   // �����������ؽ�������ǩλ�� X ƫ����
const int LOADING_SCENE_FONT_SIZE = 20;

const int LOADING_SCENE_DURATION_TIME = 2.0f;

const int SCENE_TRANSITION_DURATION = 1.0f; //��������ʱ��

/*
//menu����
const  int INITIAL_SCENE_BUTTON_OFFSET_Y1 = -241;
const  int INITIAL_SCENE_BUTTON_OFFSET_Y2 = -241;
const  int INITIAL_SCENE_BUTTON_OFFSET_Y3 = -241;
const  int INITIAL_SCENE_BUTTON_OFFSET_X1 = -100;
const  int INITIAL_SCENE_BUTTON_OFFSET_X2 = -30;
const  int INITIAL_SCENE_BUTTON_OFFSET_X3 = -40;
*/

//Game scene����

const int MAP_WIDTH = 105 * 16;
const int MAP_HEIGHT = 65* 16;
const int MAPBLOCK_WIDTH = 105 * 16/2;
const int MAPBLOCK_HEIGHT = 65 * 16/2;


#endif