#pragma once
#include <deque>
#include <unordered_map>
#include "CardModel.h"

/**
 * @brief 游戏模型类
 *
 * 管理游戏状态数据，包括卡牌位置、区域划分和游戏规则状态。
 */
class GameModel {
public:
	GameModel() : stacktop(cocos2d::Vec2(800, 300)) {}          // 初始化手牌区顶部位置

	std::deque<std::unique_ptr<CardModel>> playfieldCards;      // 游戏区卡牌
	std::deque<std::unique_ptr<CardModel>> stackCards;          // 手牌区卡牌
	std::unordered_map<int, CardModel*> cardMap;                // 卡牌ID到卡牌对象的映射

	cocos2d::Vec2 stacktop;                                     // 手牌区顶部位置
	
    /**
    * @brief 添加卡牌到模型
    *
    * @param card 要添加的卡牌对象
    */
    void addCard(std::unique_ptr<CardModel> card) {
        int id = card->id;
        if (card->isInPlayfield) {
            playfieldCards.push_back(std::move(card));
            cardMap[id] = playfieldCards.back().get();
        }
        else {
            stackCards.push_back(std::move(card));
            cardMap[id] = stackCards.back().get();
        }
    }

    /**
     * @brief 获取顶部牌
     *
     * @return CardModel* 顶部牌指针，无牌返回nullptr
     */
	CardModel* getTopStackCard() {
		if (stackCards.empty()) return nullptr;
        return stackCards.back().get();
	}

    /**
     * @brief 通过ID获取卡牌
     *
     * @param id 卡牌ID
     * @return CardModel* 卡牌指针，未找到返回nullptr
     */
    CardModel* getCardById(int id) {
        auto it = cardMap.find(id);
        if (it != cardMap.end()) {
            CCLOG("Found card ID: %d at address: %p", id, it->second);
            return it->second;
        }
        CCLOG("WARNING: Card ID %d not found in model", id);
        return nullptr;
    }

    /**
     * @brief 移动卡牌到桌面牌区
     *
     * @param cardId 卡牌ID
     */
    void moveCardToPlayfield(int cardId) {
        auto it = std::find_if(stackCards.begin(), stackCards.end(),
            [cardId](const auto& card) { return card->id == cardId; });

        if (it != stackCards.end()) {
            auto card = std::move(*it);
            card->isInPlayfield = true;
            playfieldCards.push_back(std::move(card));
            stackCards.erase(it);
        }
    }
    /**
    * @brief 移动卡牌到手牌区
    *
    * @param cardId 卡牌ID
    */
    void moveCardToStack(int cardId) {
        auto it = std::find_if(playfieldCards.begin(), playfieldCards.end(),
            [cardId](const auto& card) { return card->id == cardId; });

        if (it != playfieldCards.end()) {
            auto card = std::move(*it);
            card->isInPlayfield = false;
            stackCards.push_back(std::move(card));
            playfieldCards.erase(it);
        }
    }
  
};