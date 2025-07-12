#pragma once
#include "CardModel.h"

/**
 * @brief ������������ö��
 */
enum class UndoActionType {
    HAND_CARD_CLICK,                     // �������������
    PLAYFIELD_CARD_CLICK                 // ���������������
};

/**
 * @brief ������¼�ṹ
 */
struct UndoRecord {
	UndoActionType actionType;           // ��������
    int movedCardId;                     // ���ƶ��Ŀ���ID
    cocos2d::Vec2 originalPosition;      // �ƶ�ǰ��λ��
    int replacedCardId;                  // ���滻�Ŀ���ID��ԭ�����ƣ�
                     
};