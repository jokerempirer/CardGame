#include "CardView.h"
#include <string>
#include <iostream>

using namespace std;
USING_NS_CC;

CardView* CardView::create(const CardModel& model) {
    CardView* pRet = new(std::nothrow) CardView();
    if (pRet && pRet->init(model)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool CardView::init(const CardModel& model) {

    _cardId = model.id; // Assign card ID from the model  
	
    /*this->setPosition(model.position);*/
    this->setPosition(model.position);

    createCardFace(model.face, model.suit, Vec2::ZERO); // 创建卡牌面

    return true;
}

std::string CardView::getTextureName(CardFaceType face, CardSuitType suit) {
    // 牌面值名称映射
    static const std::string faceNames[] = {
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
    };

    // 花色名称映射
    static const std::string suitNames[] = {
        "clubs",    // 梅花
        "diamonds", // 方块
        "hearts",   // 红桃
        "spades"    // 黑桃
    };

    // 确保索引在有效范围内
    int faceIndex = static_cast<int>(face);
    int suitIndex = static_cast<int>(suit);

    if (faceIndex < 0 || faceIndex >= CFT_NUM_CARD_FACE_TYPES ||
        suitIndex < 0 || suitIndex >= CST_NUM_CARD_SUIT_TYPES) {
        return "card_default.png"; // 无效卡牌类型，返回默认纹理
    }

    // 组合纹理名称：suit_face.png，例如 "hearts_A.png"
    return suitNames[suitIndex] + "_" + faceNames[faceIndex] + ".png";
}

void CardView::moveToPosition(const Vec2& position, float duration, const std::function<void()>& callback) {
    
    auto move = MoveTo::create(duration, position);

    if (callback) {
        this->runAction(Sequence::create(move, CallFunc::create(callback), nullptr));
    }
    else {
        this->runAction(move);
    }
}

void CardView::setClickCallback(const std::function<void(int)>& callback) {
    _clickCallback = callback;
}
void CardView::createCardFace(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position)
{
    const bool isRed = (suit == CST_HEARTS || suit == CST_DIAMONDS);
    std::string color = isRed ? "red" : "black";

    string Face[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    string Suit[4] = { "club", "diamond", "heart", "spade" };

    // 1. 创建底图
    Sprite* cardSprite = Sprite::create("res/res/card_general.png");
    if (!cardSprite) {
        CCLOG("Failed to create cardSprite");
    }

    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) -> bool {
        // 获取当前卡牌精灵
        if (this->getChildrenCount() == 0) return false;
        auto cardSprite = this->getChildren().at(0);

        // 将触摸点转换到当前节点坐标系
        Vec2 touchLocation = this->convertTouchToNodeSpace(touch);

        // 使用精灵的实际边界框
        if (cardSprite->getBoundingBox().containsPoint(touchLocation)) {
            CCLOG("CardView::onTouchBegan: Card %d clicked at (%.1f, %.1f)",
                _cardId, touchLocation.x, touchLocation.y);

            this->runAction(Sequence::create(
                ScaleTo::create(0.1f, 0.9f),
                ScaleTo::create(0.1f, 0.8f),
                CallFunc::create([this]() {
                    if (_clickCallback) {
                        _clickCallback(_cardId);
                    }
                }),
                nullptr
            ));
            return true;
        }
        return false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // 2. 小号点数（左上角，放大到0.7）
    auto smallNumSprite = Sprite::create("res/res/number/small_" + color + "_" + Face[face] + ".png");
    if (smallNumSprite) {
        smallNumSprite->setAnchorPoint(Vec2(0, 1));
        smallNumSprite->setPosition(Vec2(10, cardSprite->getContentSize().height - 20));
        smallNumSprite->setScale(0.7f);
        cardSprite->addChild(smallNumSprite);
    }

    // 3. 小花色（右上角，放大到0.7）
    auto smallSuitSprite = Sprite::create("res/res/suits/" + Suit[suit] + ".png");
    if (smallSuitSprite) {
        smallSuitSprite->setAnchorPoint(Vec2(1, 1));
        smallSuitSprite->setPosition(Vec2(cardSprite->getContentSize().width - 10, cardSprite->getContentSize().height - 20));
        smallSuitSprite->setScale(0.7f);
        cardSprite->addChild(smallSuitSprite);
    }

    // 4. 大号点数（中间）
    auto bigNumSprite = Sprite::create("res/res/number/big_" + color + "_" + Face[face] + ".png");
    if (bigNumSprite) {
        bigNumSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        bigNumSprite->setPosition(Vec2(cardSprite->getContentSize().width / 2, cardSprite->getContentSize().height / 2));
        bigNumSprite->setScale(0.9f);
        cardSprite->addChild(bigNumSprite);
    }

    cardSprite->setPosition(position);
    
    this->addChild(cardSprite);
    
    this->setScale(0.8f); // 适当缩放卡牌
}

void CardView::updateView(const CardModel& model) {
    // 更新位置
    this->setPosition(model.position);

}