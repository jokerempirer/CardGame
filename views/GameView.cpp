#include "GameView.h"


USING_NS_CC;

GameView* GameView::create() {
    GameView* pRet = new(std::nothrow) GameView();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GameView::init() {
    if (!Layer::init()) {
        return false;
    }


    // 设置游戏区域 - 桌面牌区（主牌区）
    auto playfield = LayerColor::create(Color4B(50, 100, 50, 100), 1080, 1500);
    playfield->setPosition(0, 580);
    this->addChild(playfield, 0);

    // 设置游戏区域 - 手牌区（堆牌区）
    auto stackArea = LayerColor::create(Color4B(100, 50, 50, 100), 1080, 580);
    stackArea->setPosition(0, 0);
    this->addChild(stackArea, 0);

    // 添加标题
    auto title = Label::createWithTTF("Card Matching Game", "fonts/Marker Felt.ttf", 60);
    title->setPosition(Vec2(540, 1950));
    title->setColor(Color3B::YELLOW);
    title->enableOutline(Color4B::BLACK, 3);
    this->addChild(title, 10);

    // 添加回退按钮
    addUndoButton();

    return true;
}

void GameView::addUndoButton() {
    // 创建回退按钮
    auto undoBtn = ui::Button::create();
    if (!undoBtn) {
        CCLOG("ERROR: Failed to create undo button");
        return;
    }
    // 使用临时颜色显示按钮
    undoBtn->setColor(Color3B::RED);
    undoBtn->setContentSize(Size(200, 80));
    undoBtn->setTitleText("UNDO");
    undoBtn->setTitleFontSize(40);
    undoBtn->setTitleColor(Color3B::WHITE);
    undoBtn->setPosition(Vec2(1000, 680));
    
    // 2. 添加点击事件监听器
    undoBtn->addClickEventListener([this](Ref* sender) {
        CCLOG("Undo button clicked");
        if (_undoCallback) {
            _undoCallback();
        }
        else {
            CCLOG("ERROR: Undo callback not set");

            // 添加视觉反馈
            auto btn = dynamic_cast<ui::Button*>(sender);
            if (btn) {
                btn->runAction(Sequence::create(
                    TintTo::create(0.1f, Color3B::RED),
                    TintTo::create(0.1f, Color3B::WHITE),
                    nullptr
                ));
            }
        }
        });

    // 添加按钮标签
    auto label = Label::createWithTTF("Undo Move", "fonts/Marker Felt.ttf", 28);
    label->setPosition(Vec2(undoBtn->getPositionX(), undoBtn->getPositionY() - 40));
    label->setColor(Color3B::WHITE);
    label->enableOutline(Color4B::BLACK, 2);
    this->addChild(label, 10);

    // 4. 添加到视图
    this->addChild(undoBtn, 1000);

}

void GameView::setupWithGameModel(const GameModel& model) {
    // 清除现有卡牌视图
    for (auto& pair : _cardViews) {
        this->removeChild(pair.second);
    }
    _cardViews.clear();

    // 创建桌面牌区的卡牌视图
    for (const auto& cardPtr: model.playfieldCards) {
        addCardView(*cardPtr);
    }

    // 创建手牌区的卡牌视图
    for (const auto& cardPtr : model.stackCards) {
        addCardView(*cardPtr);
    }
}

void GameView::addCardView(const CardModel& model) {
    CCLOG("Creating CardView for Model ID: %d, Position: (%.1f, %.1f)",
        model.id, model.position.x, model.position.y);

    auto cardView = CardView::create(model);
    if (cardView) {
        CCLOG("Created CardView with ID: %d", cardView->getCardId());

        cardView->setClickCallback([this, model](int cardId) {

            CCLOG("CardView Click Callback: View ID=%d, Model ID=%d",
                cardId, model.id);
            if (_cardClickCallback) {
                _cardClickCallback(cardId); // 传递给GameController
            }
            });
       /* CCLOG("Adding Card %d to GameView: Pos(%.1f, %.1f), Playfield: %d",
            model.id, model.position.x, model.position.y, model.isInPlayfield);*/
        
        // 添加到GameView
        this->addChild(cardView, model.isInPlayfield ? 1 : 2);

                // 存储到映射表
        _cardViews[model.id] = cardView;
    }
    else {
        CCLOG("Failed to create CardView for card %d", model.id);
    }
}

CardView* GameView::getCardViewById(int cardId) {
    auto it = _cardViews.find(cardId);
    return (it != _cardViews.end()) ? it->second : nullptr;
}

//void GameView::removeCardView(int cardId) {
//    auto it = _cardViews.find(cardId);
//    if (it != _cardViews.end()) {
//        this->removeChild(it->second);
//        _cardViews.erase(it);
//    }
//}

void GameView::updateCardView(const CardModel& model) {
    auto cardView = getCardViewById(model.id);
    if (cardView) {
        cardView->updateView(model);
    }
}

void GameView::playCardMoveAnimation(int cardId, const Vec2& targetPosition,
    const std::function<void()>& callback) {
    auto cardView = getCardViewById(cardId);
    if (!cardView) return;

    // 提升层级
    cardView->setLocalZOrder(100);
    Vec2 localPosition = this->convertToNodeSpace(targetPosition);
    
    Vec2 result = Vec2(localPosition.x 
        , localPosition.y);
    CCLOG("Playing move animation for card %d to position (%.1f, %.1f)", cardId, localPosition.x, localPosition.y);
    
    cardView->moveToPosition(localPosition, 0.3f, [=]() {
        if (callback) callback();
		/*
            手牌动画结束回调
        */
        });
}

void GameView::playUndoAnimation(int cardId, const cocos2d::Vec2& originalPosition,
    const std::function<void()>& callback) {
	
    //获取卡牌视图
    auto cardView = getCardViewById(cardId);
    if (!cardView) return;

    // 提升层级
    cardView->setLocalZOrder(100);

    // 回退特效：旋转 + 移动
    auto rotate = RotateBy::create(0.2f, 360);
    auto move = MoveTo::create(0.4f, originalPosition);

    cardView->runAction(Sequence::create(
        Spawn::create(rotate, move, nullptr),
        CallFunc::create([=]() {
            if (callback) callback();
            cardView->setLocalZOrder(cardView->getLocalZOrder() == 1 ? 1 : 2);
            cardView->setRotation(0); // 重置旋转
            }),
        nullptr
    ));
}