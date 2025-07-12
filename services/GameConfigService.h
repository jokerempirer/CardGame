#pragma once
#include "../models/GameModel.h"
#include "../configs/models/LevelConfig.h"

/**
 * @brief 游戏配置服务类
 *
 * 负责将关卡配置转换为游戏模型。
 * 使用场景：游戏初始化时加载配置并创建游戏模型。
 */
class GameConfigService {
public:
    /**
    * @brief 将关卡配置转换为游戏模型
    *
    * @param config 关卡配置
    * @return GameModel 游戏模型对象
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