#pragma once
#include "cocos2d.h"
#include "CardView.h"
#include <unordered_map>
#include "../models/GameModel.h"
#include "ui/CocosGUI.h"
#include "../services/GameService.h"

/**
 * @brief 游戏视图类
 *
 * 负责游戏场景的视觉呈现，包括卡牌渲染、UI元素和动画效果。
 * 使用场景：游戏主界面的视觉表现。
 */
class GameView : public cocos2d::Layer {
public:
	using UndoCallback = std::function<void()>;             // 回退操作的回调类型
   
    /**
     * @brief 创建游戏视图
     *
     * @return GameView* 游戏视图实例
     */
    static GameView* create();
    
    /**
     * @brief 初始化游戏视图
     *
     * @return bool 初始化是否成功
     */
    virtual bool init();

    /**
     * @brief 使用游戏模型设置视图
     *
     * @param model 游戏模型
     */
    void setupWithGameModel(const GameModel& model);

    /**
     * @brief 添加卡牌视图
     *
     * @param model 卡牌模型
     */
    void addCardView(const CardModel& model);

    /**
     * @brief 更新卡牌视图
     *
     * @param model 卡牌模型
     */
    void updateCardView(const CardModel& model); 
   
    /**
     * @brief 播放卡牌移动动画
     *
     * @param cardId 卡牌ID
     * @param targetPosition 目标位置
     * @param callback 动画完成回调
     */
    void playCardMoveAnimation(int cardId, const cocos2d::Vec2& targetPosition, const std::function<void()>& callback = nullptr);

    /**
     * @brief 播放撤销动画
     *
     * @param cardId 卡牌ID
     * @param originalPosition 原始位置
     * @param callback 动画完成回调
     */
    void playUndoAnimation(int cardId, const cocos2d::Vec2& originalPosition, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 设置卡牌点击回调
     *
     * @param callback 点击回调函数
     */
    void setCardClickCallback(const std::function<void(int cardId)>& callback) {
        _cardClickCallback = callback;
    }

    /**
     * @brief 设置撤销回调
     *
     * @param callback 撤销回调函数
     */
    void setUndoCallback(const UndoCallback& callback) {
        _undoCallback = callback;
    }
    
    /**
     * @brief 通过ID获取卡牌视图
     *
     * @param cardId 卡牌ID
     * @return CardView* 卡牌视图指针，未找到返回nullptr
     */
    CardView* getCardViewById(int cardId);

private:
    std::unordered_map<int, CardView*> _cardViews;                  // 卡牌ID到卡牌视图的映射
	std::function<void(int cardId)> _cardClickCallback;             // 卡牌点击回调
    UndoCallback _undoCallback;                                     // 回退回调

    /**
     * @brief 添加撤销按钮
     */
    void addUndoButton();
};