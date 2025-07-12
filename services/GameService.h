#pragma once
#include "../models/CardModel.h"
/**
 * @brief 游戏服务类
 *
 * 包含游戏规则和逻辑判断方法。
 * 使用场景：游戏逻辑判断和规则验证。
 */
class GameService {
public:
    /**
     * @brief 检查桌面牌和顶部牌是否匹配
     *
     * @param tableCard 桌面牌
     * @param topCard 顶部牌
     * @return bool 是否匹配（点数差为1）
     */
    static bool isMatch(const CardModel& tableCard, const CardModel& topCard) {
        int diff = abs(tableCard.getValue() - topCard.getValue());
        return diff == 1;
    }
};