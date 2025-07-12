#pragma once
#include "json/document.h"
/**
 * @brief JSON��������
 *
 * �ṩJSON�ļ��������ܡ�
 * ʹ�ó����������ļ����غͽ�����
 */
class JsonParser {
public:
    /**
     * @brief ����JSON�ļ�
     *
     * @param filename JSON�ļ�·��
     * @return rapidjson::Document �������JSON�ĵ�����
     */
    static rapidjson::Document parseFile(const std::string& filename) {
        std::string jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
        rapidjson::Document doc;
        doc.Parse(jsonStr.c_str());
        return doc;
    }
};