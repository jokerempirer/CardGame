#pragma once
#include "../models/LevelConfig.h"
#include "utils/JsonParser.h"

/**
 * @brief 关卡配置加载器类
 *
 * 负责从JSON文件加载关卡配置数据，将配置信息解析为LevelConfig对象。
 * 使用场景：游戏初始化和关卡切换时加载配置。
 */
class LevelConfigLoader {
public:
    /**
     * @brief 从文件加载关卡配置
     *
     * @param filename JSON配置文件的路径
     * @return LevelConfig 解析后的关卡配置对象
     */
    static LevelConfig loadFromFile(const std::string& filename) {
        LevelConfig config;
        auto json = JsonParser::parseFile(filename);

        // 解析桌面牌区
        const auto& playfield = json["Playfield"];
        for (const auto& card : playfield.GetArray()) {
            CardConfig cardConfig;
            cardConfig.face = static_cast<CardFaceType>(card["CardFace"].GetInt());
            cardConfig.suit = static_cast<CardSuitType>(card["CardSuit"].GetInt());
            const auto& pos = card["Position"];
            cardConfig.position = cocos2d::Vec2(pos["x"].GetFloat(), pos["y"].GetFloat());
            config.playfield.push_back(cardConfig);
        }

        // 解析手牌区
        const auto& stack = json["Stack"];
        for (const auto& card : stack.GetArray()) {
            CardConfig cardConfig;
            cardConfig.face = static_cast<CardFaceType>(card["CardFace"].GetInt());
            cardConfig.suit = static_cast<CardSuitType>(card["CardSuit"].GetInt());
            const auto& pos = card["Position"];
            cardConfig.position = cocos2d::Vec2(pos["x"].GetFloat(), pos["y"].GetFloat());
            config.stack.push_back(cardConfig);
        }

        return config;
    }
};