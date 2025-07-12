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
 * @brief ��Ϸ��������
 *
 * ������Ϸ�߼����ƣ�Э��ģ�ͺ���ͼ�Ľ����������û��������Ϸ״̬����
 * ʹ�ó�������Ϸ��ѭ���д����û���������Ϸ�߼���
 */
class GameController : public cocos2d::Node {
public:
	
    /**
    * @brief ������Ϸ������ʵ��
    *
    * @param levelPath �ؿ������ļ�·��
    * @return GameController* ������ʵ��ָ��
    */
    static GameController* create(const std::string& levelPath);

    /**
     * @brief ʹ��ָ���ؿ���ʼ��������
     *
     * @param levelPath �ؿ������ļ�·��
     * @return bool ��ʼ���Ƿ�ɹ�
     */
    bool initWithLevel(const std::string& levelPath);

    /**
     * @brief ��ʼ��Ϸ
     */
    void startGame();

    /**
     * @brief �����Ƶ���¼�
     *
     * @param cardId ������Ŀ���ID
     */
    void handleCardClick(int cardId);

    /**
     * @brief ��������ť����¼�
     */
    void handleUndoClick();
    
private:
    GameModel _gameModel;               // ��Ϸ����ģ��
    GameView* _gameView = nullptr;    // ��Ϸ��ͼ    
    std::unique_ptr<UndoManager> _undoManager; // ���˹�����

    /**
      * @brief �������������Ƶ��
      *
      * @param card ������Ŀ���ģ��
      */
    void handleHandCardClick(CardModel* card);

    /**
     * @brief ���������������Ƶ��
     *
     * @param card ������Ŀ���ģ��
     */
    void handlePlayfieldCardClick(CardModel* card);

    /**
     * @brief ���������滻������
     *
     * @param card Ҫ�滻��������
     */
    void replaceTrayWithPlayFieldCard(CardModel* card);

    /**
     * @brief ����������Ƭ�滻������
     *
     * @param card Ҫ�滻������
     */
    void replaceTrayWithHandCard(CardModel* card);

    /**
     * @brief ����������¼
     *
     * @param movedCard ���ƶ��Ŀ���
     * @param actionType ��������
     * @return UndoRecord ������¼����
     */
    UndoRecord createUndoRecord(CardModel* movedCard, UndoActionType actionType);

    /**
     * @brief ��鿨���Ƿ���ƶ�
     *
     * @param card Ҫ���Ŀ���
     * @return bool �Ƿ���ƶ�
     */
    bool canMoveCard(CardModel* card);

    /**
     * @brief ��ȡ������λ��
     *
     * @return cocos2d::Vec2 ������λ��
     */
    cocos2d::Vec2 getTopStackPosition() const;

    /**
     * @brief ��¼��ǰ��Ϸģ�͵�״̬
     *
     * @param context ��������Ϣ��������־���
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