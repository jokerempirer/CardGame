#pragma once
#include "cocos2d.h"
#include "CardView.h"
#include <unordered_map>
#include "../models/GameModel.h"
#include "ui/CocosGUI.h"
#include "../services/GameService.h"

/**
 * @brief ��Ϸ��ͼ��
 *
 * ������Ϸ�������Ӿ����֣�����������Ⱦ��UIԪ�غͶ���Ч����
 * ʹ�ó�������Ϸ��������Ӿ����֡�
 */
class GameView : public cocos2d::Layer {
public:
	using UndoCallback = std::function<void()>;             // ���˲����Ļص�����
   
    /**
     * @brief ������Ϸ��ͼ
     *
     * @return GameView* ��Ϸ��ͼʵ��
     */
    static GameView* create();
    
    /**
     * @brief ��ʼ����Ϸ��ͼ
     *
     * @return bool ��ʼ���Ƿ�ɹ�
     */
    virtual bool init();

    /**
     * @brief ʹ����Ϸģ��������ͼ
     *
     * @param model ��Ϸģ��
     */
    void setupWithGameModel(const GameModel& model);

    /**
     * @brief ��ӿ�����ͼ
     *
     * @param model ����ģ��
     */
    void addCardView(const CardModel& model);

    /**
     * @brief ���¿�����ͼ
     *
     * @param model ����ģ��
     */
    void updateCardView(const CardModel& model); 
   
    /**
     * @brief ���ſ����ƶ�����
     *
     * @param cardId ����ID
     * @param targetPosition Ŀ��λ��
     * @param callback ������ɻص�
     */
    void playCardMoveAnimation(int cardId, const cocos2d::Vec2& targetPosition, const std::function<void()>& callback = nullptr);

    /**
     * @brief ���ų�������
     *
     * @param cardId ����ID
     * @param originalPosition ԭʼλ��
     * @param callback ������ɻص�
     */
    void playUndoAnimation(int cardId, const cocos2d::Vec2& originalPosition, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief ���ÿ��Ƶ���ص�
     *
     * @param callback ����ص�����
     */
    void setCardClickCallback(const std::function<void(int cardId)>& callback) {
        _cardClickCallback = callback;
    }

    /**
     * @brief ���ó����ص�
     *
     * @param callback �����ص�����
     */
    void setUndoCallback(const UndoCallback& callback) {
        _undoCallback = callback;
    }
    
    /**
     * @brief ͨ��ID��ȡ������ͼ
     *
     * @param cardId ����ID
     * @return CardView* ������ͼָ�룬δ�ҵ�����nullptr
     */
    CardView* getCardViewById(int cardId);

private:
    std::unordered_map<int, CardView*> _cardViews;                  // ����ID��������ͼ��ӳ��
	std::function<void(int cardId)> _cardClickCallback;             // ���Ƶ���ص�
    UndoCallback _undoCallback;                                     // ���˻ص�

    /**
     * @brief ��ӳ�����ť
     */
    void addUndoButton();
};