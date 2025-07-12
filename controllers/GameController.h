#pragma once
#include "cocos2d.h"
#include <string>
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../managers/UndoManager.h"
#include "../services/GameService.h"
#include "../services/GameConfigService.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../views/CardView.h"

/**
 * @brief 游戏控制器类
 *
 * 负责游戏逻辑控制，协调模型和视图的交互，处理用户输入和游戏状态管理。
 * 使用场景：游戏主循环中处理用户操作和游戏逻辑。
 */
class GameController : public cocos2d::Node {
public:
	
    /**
    * @brief 创建游戏控制器实例
    *
    * @param levelPath 关卡配置文件路径
    * @return GameController* 控制器实例指针
    */
    static GameController* create(const std::string& levelPath);

    /**
     * @brief 使用指定关卡初始化控制器
     *
     * @param levelPath 关卡配置文件路径
     * @return bool 初始化是否成功
     */
    bool initWithLevel(const std::string& levelPath);

    /**
     * @brief 开始游戏
     */
    void startGame();

    /**
     * @brief 处理卡牌点击事件
     *
     * @param cardId 被点击的卡牌ID
     */
    void handleCardClick(int cardId);

    /**
     * @brief 处理撤销按钮点击事件
     */
    void handleUndoClick();
    
private:
    GameModel _gameModel;               // 游戏数据模型
    GameView* _gameView = nullptr;    // 游戏视图    
    std::unique_ptr<UndoManager> _undoManager; // 回退管理器

    /**
      * @brief 处理手牌区卡牌点击
      *
      * @param card 被点击的卡牌模型
      */
    void handleHandCardClick(CardModel* card);

    /**
     * @brief 处理桌面牌区卡牌点击
     *
     * @param card 被点击的卡牌模型
     */
    void handlePlayfieldCardClick(CardModel* card);

    /**
     * @brief 用桌面牌替换顶部牌
     *
     * @param card 要替换的桌面牌
     */
    void replaceTrayWithPlayFieldCard(CardModel* card);

    /**
     * @brief 用手牌区卡片替换顶部牌
     *
     * @param card 要替换的手牌
     */
    void replaceTrayWithHandCard(CardModel* card);

    /**
     * @brief 创建撤销记录
     *
     * @param movedCard 被移动的卡牌
     * @param actionType 操作类型
     * @return UndoRecord 撤销记录对象
     */
    UndoRecord createUndoRecord(CardModel* movedCard, UndoActionType actionType);

    /**
     * @brief 检查卡牌是否可移动
     *
     * @param card 要检查的卡牌
     * @return bool 是否可移动
     */
    bool canMoveCard(CardModel* card);

    /**
     * @brief 获取顶部牌位置
     *
     * @return cocos2d::Vec2 顶部牌位置
     */
    cocos2d::Vec2 getTopStackPosition() const;

    /**
     * @brief 记录当前游戏模型的状态
     *
     * @param context 上下文信息，用于日志输出
     */
    void logModelState(const std::string& context) {
        CCLOG("===== MODEL STATE (%s) =====", context.c_str());
        CCLOG("Playfield cards: %zu", _gameModel.playfieldCards.size());
        for (const auto& card : _gameModel.playfieldCards) {
            CCLOG("  ID: %d, Pos: (%.1f, %.1f), isTop: %d",
                card->id, card->position.x, card->position.y, card->isTopCard);
        }

        CCLOG("Stack cards: %zu", _gameModel.stackCards.size());
        for (const auto& card : _gameModel.stackCards) {
            CCLOG("  ID: %d, Pos: (%.1f, %.1f), isTop: %d",
                card->id, card->position.x, card->position.y, card->isTopCard);
        }
        CCLOG("============================");
    }
};