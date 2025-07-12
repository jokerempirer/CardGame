#pragma once
#include "../models/CardModel.h"
/**
 * @brief ��Ϸ������
 *
 * ������Ϸ������߼��жϷ�����
 * ʹ�ó�������Ϸ�߼��жϺ͹�����֤��
 */
class GameService {
public:
    /**
     * @brief ��������ƺͶ������Ƿ�ƥ��
     *
     * @param tableCard ������
     * @param topCard ������
     * @return bool �Ƿ�ƥ�䣨������Ϊ1��
     */
    static bool isMatch(const CardModel& tableCard, const CardModel& topCard) {
        int diff = abs(tableCard.getValue() - topCard.getValue());
        return diff == 1;
    }
};