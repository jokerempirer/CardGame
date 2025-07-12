#pragma once
#include <deque>
#include <unordered_map>
#include "CardModel.h"

/**
 * @brief ��Ϸģ����
 *
 * ������Ϸ״̬���ݣ���������λ�á����򻮷ֺ���Ϸ����״̬��
 */
class GameModel {
public:
	GameModel() : stacktop(cocos2d::Vec2(800, 300)) {}          // ��ʼ������������λ��

	std::deque<std::unique_ptr<CardModel>> playfieldCards;      // ��Ϸ������
	std::deque<std::unique_ptr<CardModel>> stackCards;          // ����������
	std::unordered_map<int, CardModel*> cardMap;                // ����ID�����ƶ����ӳ��

	cocos2d::Vec2 stacktop;                                     // ����������λ��
	
    /**
    * @brief ��ӿ��Ƶ�ģ��
    *
    * @param card Ҫ��ӵĿ��ƶ���
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
     * @brief ��ȡ������
     *
     * @return CardModel* ������ָ�룬���Ʒ���nullptr
     */
	CardModel* getTopStackCard() {
		if (stackCards.empty()) return nullptr;
        return stackCards.back().get();
	}

    /**
     * @brief ͨ��ID��ȡ����
     *
     * @param id ����ID
     * @return CardModel* ����ָ�룬δ�ҵ�����nullptr
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
     * @brief �ƶ����Ƶ���������
     *
     * @param cardId ����ID
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
    * @brief �ƶ����Ƶ�������
    *
    * @param cardId ����ID
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