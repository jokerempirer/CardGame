#pragma once
#include "json/document.h"
/**
 * @brief JSON解析器类
 *
 * 提供JSON文件解析功能。
 * 使用场景：配置文件加载和解析。
 */
class JsonParser {
public:
    /**
     * @brief 解析JSON文件
     *
     * @param filename JSON文件路径
     * @return rapidjson::Document 解析后的JSON文档对象
     */
    static rapidjson::Document parseFile(const std::string& filename) {
        std::string jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
        rapidjson::Document doc;
        doc.Parse(jsonStr.c_str());
        return doc;
    }
};