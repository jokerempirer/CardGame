#pragma once
#include "cocos2d.h"
#include "../models/CardModel.h"


class GameView;
/**
 * @brief 卡牌视图类
 *
 * 负责单张卡牌的视觉表现，包括渲染、动画和点击处理。
 * 使用场景：游戏场景中卡牌的视觉呈现。
 */
class CardView : public cocos2d::Sprite {
public:
    /**
     * @brief 创建卡牌视图
     *
     * @param model 卡牌模型
     * @return CardView* 卡牌视图实例
     */
    static CardView* create(const CardModel& model);
    /**
     * @brief 初始化卡牌视图
     *
     * @param model 卡牌模型
     * @return bool 初始化是否成功
     */
    virtual bool init(const CardModel& model);

    /**
     * @brief 更新卡牌视图状态
     *
     * @param model 卡牌模型
     */
    void updateView(const CardModel& model);

    /**
     * @brief 移动卡牌到指定位置
     *
     * @param position 目标位置
     * @param duration 动画时长
     * @param callback 动画完成回调
     */
    void moveToPosition(const cocos2d::Vec2& position, float duration, const std::function<void()>& callback = nullptr);

    /**
     * @brief 设置点击回调
     *
     * @param callback 点击回调函数
     */
    void setClickCallback(const std::function<void(int)>& callback); // Re-enable the method

	CC_SYNTHESIZE(int, _cardId, CardId);            // 卡牌ID

    /**
     * @brief 创建卡牌正面视图
     *
     * @param face 卡牌点数
     * @param suit 卡牌花色
     * @param position 初始位置
     */
    void createCardFace(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position);

    /**
     * @brief 设置是否可点击
     *
     * @param clickable 是否可点击
     */
    void setClickable(bool clickable) { _isClickable = clickable; }
    /**
     * @brief 获取是否可点击
     *
     * @return bool 是否可点击
     */
    bool isClickable() const { return _isClickable; }
private:

	std::function<void(int)> _clickCallback;            // 点击回调函数

	bool _isClickable = true;                           // 是否可点击
    /**
    * @brief 获取卡牌纹理名称
    *
    * @param face 卡牌点数
    * @param suit 卡牌花色
    * @return std::string 纹理资源路径
    */
	std::string getTextureName(CardFaceType face, CardSuitType suit);               
    
    
    
};