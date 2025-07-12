#pragma once
#include "cocos2d.h"
#include "../configs/models/LevelConfig.h"

/**
 * @brief ����ģ����
 *
 * ��ʾ��Ϸ�е�һ�ſ��ƣ��������Ƶ�״̬�����ԡ�
 */
class CardModel {
public:
    int id;                     // ΨһID
	CardFaceType face;          // ���Ƶ���
	CardSuitType suit;          // ���ƻ�ɫ
	cocos2d::Vec2 position;     // ����λ��
    bool isTopCard;             // �Ƿ񶥲���
    bool isInPlayfield;         // �Ƿ���������

    /**
     * @brief ���캯��
     *
     * @param id ����ID
     * @param f ���Ƶ���
     * @param s ���ƻ�ɫ
     * @param pos ��ʼλ��
     * @param inPlayfield �Ƿ�����������
     */
    CardModel(int id, CardFaceType f, CardSuitType s, const cocos2d::Vec2& pos, bool inPlayfield)
        : id(id), face(f), suit(s), position(pos), isTopCard(false), isInPlayfield(inPlayfield) {
    }

    /**
     * @brief ��ȡ������ֵ
     *
     * @return int ������ֵ��A=1, J=11, Q=12, K=13��
     */
    int getValue() const {
        return static_cast<int>(face);
    }
};