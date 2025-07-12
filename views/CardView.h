#pragma once
#include "cocos2d.h"
#include "../models/CardModel.h"


class GameView;
/**
 * @brief ������ͼ��
 *
 * �����ſ��Ƶ��Ӿ����֣�������Ⱦ�������͵������
 * ʹ�ó�������Ϸ�����п��Ƶ��Ӿ����֡�
 */
class CardView : public cocos2d::Sprite {
public:
    /**
     * @brief ����������ͼ
     *
     * @param model ����ģ��
     * @return CardView* ������ͼʵ��
     */
    static CardView* create(const CardModel& model);
    /**
     * @brief ��ʼ��������ͼ
     *
     * @param model ����ģ��
     * @return bool ��ʼ���Ƿ�ɹ�
     */
    virtual bool init(const CardModel& model);

    /**
     * @brief ���¿�����ͼ״̬
     *
     * @param model ����ģ��
     */
    void updateView(const CardModel& model);

    /**
     * @brief �ƶ����Ƶ�ָ��λ��
     *
     * @param position Ŀ��λ��
     * @param duration ����ʱ��
     * @param callback ������ɻص�
     */
    void moveToPosition(const cocos2d::Vec2& position, float duration, const std::function<void()>& callback = nullptr);

    /**
     * @brief ���õ���ص�
     *
     * @param callback ����ص�����
     */
    void setClickCallback(const std::function<void(int)>& callback); // Re-enable the method

	CC_SYNTHESIZE(int, _cardId, CardId);            // ����ID

    /**
     * @brief ��������������ͼ
     *
     * @param face ���Ƶ���
     * @param suit ���ƻ�ɫ
     * @param position ��ʼλ��
     */
    void createCardFace(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position);

    /**
     * @brief �����Ƿ�ɵ��
     *
     * @param clickable �Ƿ�ɵ��
     */
    void setClickable(bool clickable) { _isClickable = clickable; }
    /**
     * @brief ��ȡ�Ƿ�ɵ��
     *
     * @return bool �Ƿ�ɵ��
     */
    bool isClickable() const { return _isClickable; }
private:

	std::function<void(int)> _clickCallback;            // ����ص�����

	bool _isClickable = true;                           // �Ƿ�ɵ��
    /**
    * @brief ��ȡ������������
    *
    * @param face ���Ƶ���
    * @param suit ���ƻ�ɫ
    * @return std::string ������Դ·��
    */
	std::string getTextureName(CardFaceType face, CardSuitType suit);               
    
    
    
};