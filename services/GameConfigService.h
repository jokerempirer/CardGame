#pragma once
#include "../models/GameModel.h"
#include "../configs/models/LevelConfig.h"

/**
 * @brief ��Ϸ���÷�����
 *
 * ���𽫹ؿ�����ת��Ϊ��Ϸģ�͡�
 * ʹ�ó�������Ϸ��ʼ��ʱ�������ò�������Ϸģ�͡�
 */
class GameConfigService {
public:
    /**
    * @brief ���ؿ�����ת��Ϊ��Ϸģ��
    *
    * @param config �ؿ�����
    * @return GameModel ��Ϸģ�Ͷ���
    */
    static GameModel convertToGameModel(const LevelConfig& config) {
        GameModel gameModel;
        int id = 0;

        for (const auto& cardConfig : config.playfield) {
            gameModel.addCard(std::make_unique<CardModel>(
                id++, cardConfig.face, cardConfig.suit,
                cardConfig.position, true
            ));
        }

        for (const auto& cardConfig : config.stack) {
            gameModel.addCard(std::make_unique<CardModel>(
                id++, cardConfig.face, cardConfig.suit,
                cardConfig.position, false
            ));
        }

        return gameModel;
    }
};