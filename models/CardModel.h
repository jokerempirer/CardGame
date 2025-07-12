#pragma once
#include "cocos2d.h"
#include "../configs/models/LevelConfig.h"

/**
 * @brief 卡牌模型类
 *
 * 表示游戏中的一张卡牌，包含卡牌的状态和属性。
 */
class CardModel {
public:
    int id;                     // 唯一ID
	CardFaceType face;          // 卡牌点数
	CardSuitType suit;          // 卡牌花色
	cocos2d::Vec2 position;     // 卡牌位置
    bool isTopCard;             // 是否顶部牌
    bool isInPlayfield;         // 是否在桌面区

    /**
     * @brief 构造函数
     *
     * @param id 卡牌ID
     * @param f 卡牌点数
     * @param s 卡牌花色
     * @param pos 初始位置
     * @param inPlayfield 是否在桌面牌区
     */
    CardModel(int id, CardFaceType f, CardSuitType s, const cocos2d::Vec2& pos, bool inPlayfield)
        : id(id), face(f), suit(s), position(pos), isTopCard(false), isInPlayfield(inPlayfield) {
    }

    /**
     * @brief 获取卡牌数值
     *
     * @return int 卡牌数值（A=1, J=11, Q=12, K=13）
     */
    int getValue() const {
        return static_cast<int>(face);
    }
};