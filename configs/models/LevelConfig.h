#pragma once
#include "cocos2d.h"

/**
 * @brief ���ƻ�ɫö��
 */
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      // ÷��
    CST_DIAMONDS,   // ����
    CST_HEARTS,     // ����
    CST_SPADES,     // ����
    CST_NUM_CARD_SUIT_TYPES
};
/**
 * @brief ���Ƶ���ö��
 */
enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};
/**
 * @brief ���ſ������ýṹ
 */
struct CardConfig {
    CardFaceType face;
    CardSuitType suit;
    cocos2d::Vec2 position;
};

/**
 * @brief �ؿ����ýṹ
 */
struct LevelConfig {
    std::vector<CardConfig> playfield; ///< �����Ŀ�������
    std::vector<CardConfig> stack; ///< �������Ŀ�������
    int levelNumber; ///< �ؿ����
    std::string levelName; ///< �ؿ�����
};