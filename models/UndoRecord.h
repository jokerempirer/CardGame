#pragma once
#include "CardModel.h"

/**
 * @brief 撤销操作类型枚举
 */
enum class UndoActionType {
    HAND_CARD_CLICK,                     // 点击手牌区的牌
    PLAYFIELD_CARD_CLICK                 // 点击桌面牌区的牌
};

/**
 * @brief 撤销记录结构
 */
struct UndoRecord {
	UndoActionType actionType;           // 动作类型
    int movedCardId;                     // 被移动的卡牌ID
    cocos2d::Vec2 originalPosition;      // 移动前的位置
    int replacedCardId;                  // 被替换的卡牌ID（原顶部牌）
                     
};