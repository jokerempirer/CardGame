#pragma once
#include "cocos2d.h"

/**
 * @brief 卡牌花色枚举
 */
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};
/**
 * @brief 卡牌点数枚举
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
 * @brief 单张卡牌配置结构
 */
struct CardConfig {
    CardFaceType face;
    CardSuitType suit;
    cocos2d::Vec2 position;
};

/**
 * @brief 关卡配置结构
 */
struct LevelConfig {
    std::vector<CardConfig> playfield; ///< 牌区的卡牌配置
    std::vector<CardConfig> stack; ///< 手牌区的卡牌配置
    int levelNumber; ///< 关卡序号
    std::string levelName; ///< 关卡名称
};