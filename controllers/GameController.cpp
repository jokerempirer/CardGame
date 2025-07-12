#include "GameController.h"

USING_NS_CC;

GameController* GameController::create(const std::string& levelPath) {
    GameController* pRet = new(std::nothrow) GameController();
    if (pRet && pRet->initWithLevel(levelPath)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GameController::initWithLevel(const std::string& levelPath) {
    if (!Node::init()) {
        return false;
    }
	CCLOG("GameController::initWithLevel - Level Path: %s", levelPath.c_str());
    // 1. 加载关卡配置
    LevelConfig levelConfig = LevelConfigLoader::loadFromFile(levelPath);

    // 3. 转换为游戏模型
    _gameModel = GameConfigService::convertToGameModel(levelConfig);

    // 3. 创建游戏视图
    _gameView = GameView::create();
    _gameView->setupWithGameModel(_gameModel);

    // 4. 设置卡牌点击回调
    _gameView->setCardClickCallback([this](int cardId) {
		CCLOG("1.Card clicked: %d", cardId);
        this->handleCardClick(cardId);
        });

    CCLOG("Setting undo callback");
    // 5. 设置回退按钮回调
    _gameView->setUndoCallback([this]() {
        CCLOG("Undo callback triggered");
        this->handleUndoClick();
        });

    // 6. 添加到当前节点
    this->addChild(_gameView);
    _gameView->setPosition(Vec2::ZERO); // 确保在原点

    // 7. 初始化回退管理器
    _undoManager = std::make_unique<UndoManager>();

    // 8. 设置手牌区顶部牌
    if (auto topCard = _gameModel.getTopStackCard()) {
        topCard->isTopCard = true;
        _gameView->updateCardView(*topCard);
    }

    return true;
}

void GameController::startGame()
{
    CCLOG("GameController::startGame - Starting game with model state:");
    // 打印游戏模型状态
    logModelState("Game started");

	CCLOG("GameController::startGame - Game started successfully");
}

void GameController::handleCardClick(int cardId) {
    
	CCLOG("2.Card clicked: %d", cardId);
    
    for (auto& pair : _gameModel.cardMap) {
        CCLOG("Card in Model: ID=%d, Pos=(%.1f, %.1f), isTop=%d",
            pair.first, pair.second->position.x, pair.second->position.y, pair.second->isTopCard);
    }
    
    CardModel* card = _gameModel.getCardById(cardId);
    if (!card || card->id < 0) { // 添加有效性检查
        CCLOG("ERROR: Invalid card pointer or ID: %d", cardId);
        return;
    }

    // 根据卡牌区域处理点击
    if (!card->isInPlayfield) {
        handleHandCardClick(card);  // 手牌区卡牌
    }
    else {
        handlePlayfieldCardClick(card); // 桌面牌区卡牌
    }
}

void GameController::handleHandCardClick(CardModel* card) {
    // 用手牌区卡片替换顶部牌
	CCLOG("3.Card clicked: %d", card->id);
    replaceTrayWithHandCard(card);
}

void GameController::handlePlayfieldCardClick(CardModel* card) {
	CCLOG("3.Card clicked: %d", card->id);
    // 检查卡片是否可以移动
    if (!canMoveCard(card)) {
        // 不匹配效果：抖动提示
        auto cardView = _gameView->getCardViewById(card->id);
        if (cardView) {
            auto shake = Sequence::create(
                MoveBy::create(0.05f, Vec2(10, 0)),
                MoveBy::create(0.05f, Vec2(-20, 0)),
                MoveBy::create(0.05f, Vec2(10, 0)),
                nullptr
            );
            cardView->runAction(shake);
        }
        return;
    }

    // 用桌面卡片替换手牌区顶部牌
    replaceTrayWithPlayFieldCard(card);
}

bool GameController::canMoveCard(CardModel* card) {
    // 1. 卡片必须在桌面上
    if (!card->isInPlayfield) return false;

    // 2. 手牌区必须有顶部牌
    CardModel* topCard = _gameModel.getTopStackCard();
    if (!topCard) return false;

    // 3. 点数差为1
    return GameService::isMatch(*card, *topCard);
}

void GameController::replaceTrayWithPlayFieldCard(CardModel* card) {

    CCLOG("replaceTrayWithPlayFieldCard - Card ID: %d, Address: %p", card->id, card);
    logModelState("Before playfield card replacement");

    CCLOG("4. Processing playfield card click for ID: %d", card->id);

    // 1. 创建回退记录
    UndoRecord record = createUndoRecord(card, UndoActionType::PLAYFIELD_CARD_CLICK);

    // 2. 记录操作
    _undoManager->recordAction(record);

    // 3. 更新原顶部牌状态
    if (auto oldTopCard = _gameModel.getTopStackCard()) {
        oldTopCard->isTopCard = false;
        _gameView->updateCardView(*oldTopCard);
    }

    int movedCardId = card->id;
    CardFaceType face = card->face;
    CardSuitType suit = card->suit;

    // 4. 从桌面区移除当前卡片
    auto& playfieldCards = _gameModel.playfieldCards;
    playfieldCards.erase(
        std::remove_if(playfieldCards.begin(), playfieldCards.end(),
            [card](const auto& c) { return c->id == card->id; }),
        playfieldCards.end()
    );

    // 5. 创建新卡牌对象
    auto newCard = std::make_unique<CardModel>(
		movedCardId, face, suit,
        getTopStackPosition(), false
    );
    newCard->isTopCard = true;

    // 6. 添加到手牌区顶部
    _gameModel.stackCards.push_back(std::move(newCard));
    CardModel* newCardPtr = _gameModel.stackCards.back().get();

    CCLOG("5. New top card ID: %d at position: (%.1f, %.1f)",
        newCardPtr->id, newCardPtr->position.x, newCardPtr->position.y);

    // 7. 更新卡牌映射
    _gameModel.cardMap[movedCardId] = newCardPtr;

    // 8. 调用视图执行动画
    _gameView->playCardMoveAnimation(newCardPtr->id, newCardPtr->position, [this, newCardPtr]() {
        _gameView->updateCardView(*newCardPtr);
        });

    logModelState("After playfield card replacement");
}

void GameController::replaceTrayWithHandCard(CardModel* card) {

    logModelState("Before hand card replacement");

	CCLOG("4.Card clicked: %d", card->id);
    // 1. 创建回退记录
    UndoRecord record = createUndoRecord(card, UndoActionType::HAND_CARD_CLICK);

    // 2. 记录操作
    _undoManager->recordAction(record);

    // 3. 更新原顶部牌状态
    if (auto oldTopCard = _gameModel.getTopStackCard()) {
        oldTopCard->isTopCard = false;
        _gameView->updateCardView(*oldTopCard);
    }   
    
    // 在移除前保存卡牌信息
    int movedCardId = card->id;
    CardFaceType face = card->face;
    CardSuitType suit = card->suit;

    // 4. 从原区域移除当前卡片
    if (card->isInPlayfield) {
        auto& playfieldCards = _gameModel.playfieldCards;
        playfieldCards.erase(
            std::remove_if(playfieldCards.begin(), playfieldCards.end(),
                [card](const auto& c) { return c->id == card->id; }), // 使用->
            playfieldCards.end()
        );
    }
    else {
        auto& stackCards = _gameModel.stackCards;
        stackCards.erase(
            std::remove_if(stackCards.begin(), stackCards.end(),
                [card](const auto& c) { return c->id == card->id; }), // 使用->
            stackCards.end()
        );
    }

    // 5. 创建新卡牌对象
    auto newCard = std::make_unique<CardModel>(
        movedCardId, face, suit,
        getTopStackPosition(), false
    );
    newCard->isTopCard = true;
    
    // 6. 添加到手牌区顶部
    _gameModel.stackCards.push_back(std::move(newCard));
    CardModel* newCardPtr = _gameModel.stackCards.back().get();

    CCLOG("5. New top card ID: %d at position: (%.1f, %.1f)",
        newCardPtr->id, newCardPtr->position.x, newCardPtr->position.y);

    // 7. 更新卡牌映射
    _gameModel.cardMap[card->id] = newCardPtr;

    // 8. 调用视图执行动画
    _gameView->playCardMoveAnimation(newCardPtr->id, newCardPtr->position, [this, newCardPtr]() {
        _gameView->updateCardView(*newCardPtr);
        });

    logModelState("After hand card replacement");
}

void GameController::handleUndoClick() {
    if (!_undoManager || !_undoManager->canUndo()) {
        cocos2d::log("No undo actions available");
        return;
    }

    // 1. 获取回退记录
    UndoRecord record = _undoManager->popUndoRecord();
    CCLOG("Undo action for card ID: %d, type: %d",
        record.movedCardId, static_cast<int>(record.actionType));

    // 2. 获取移动的卡牌（当前在堆顶）
    CardModel* movedCard = _gameModel.getCardById(record.movedCardId);
    if (!movedCard) {
        return;
    }

    // 3. 获取被替换的卡牌（原顶部牌）
    CardModel* replacedCard = _gameModel.getCardById(record.replacedCardId);
    if (!replacedCard) {
        return;
    }

    // 4. 从当前区域移除移动的卡牌
    if (movedCard->isInPlayfield) {
        auto& playfieldCards = _gameModel.playfieldCards;
        playfieldCards.erase(
            std::remove_if(playfieldCards.begin(), playfieldCards.end(),
                [movedCard](const auto& c) { return c->id == movedCard->id; }),
            playfieldCards.end()
        );
    }
    else {
        auto& stackCards = _gameModel.stackCards;
        stackCards.erase(
            std::remove_if(stackCards.begin(), stackCards.end(),
                [movedCard](const auto& c) { return c->id == movedCard->id; }),
            stackCards.end()
        );
    }

    // 5. 恢复原顶部牌
    replacedCard->isTopCard = true;
    _gameView->updateCardView(*replacedCard);

    // 6. 创建恢复的卡牌对象
    auto restoredCard = std::make_unique<CardModel>(
        record.movedCardId,
        movedCard->face,
        movedCard->suit,
        record.originalPosition,
        (record.actionType == UndoActionType::PLAYFIELD_CARD_CLICK)
    );

    // 7. 根据操作类型恢复到相应区域
    switch (record.actionType) {
    case UndoActionType::HAND_CARD_CLICK:
        // 恢复到手牌区
        _gameModel.stackCards.push_back(std::move(restoredCard));
        break;

    case UndoActionType::PLAYFIELD_CARD_CLICK:
        // 恢复到桌面区
        _gameModel.playfieldCards.push_back(std::move(restoredCard));
        break;
    }

    // 8. 更新卡牌映射
    CardModel* restoredCardPtr = (record.actionType == UndoActionType::HAND_CARD_CLICK) ?
        _gameModel.stackCards.back().get() :
        _gameModel.playfieldCards.back().get();

    _gameModel.cardMap[record.movedCardId] = restoredCardPtr;

    // 9. 播放回退动画
    _gameView->playUndoAnimation(
        restoredCardPtr->id,
        restoredCardPtr->position,
        [this, restoredCardPtr, replacedCard]() {
            _gameView->updateCardView(*restoredCardPtr);
            _gameView->updateCardView(*replacedCard);
        }
    );
}

cocos2d::Vec2 GameController::getTopStackPosition() const {
    // 获取游戏模型中的顶部牌位置
    return _gameModel.stacktop;
}

UndoRecord GameController::createUndoRecord(CardModel* movedCard, UndoActionType actionType) {
    UndoRecord record;
    record.actionType = actionType;
    record.movedCardId = movedCard->id;
    record.originalPosition = movedCard->position;

    if (auto topCard = _gameModel.getTopStackCard()) {
        record.replacedCardId = topCard->id;
        CCLOG("Created undo record: moved=%d, replaced=%d, pos=(%.1f,%.1f)",
            record.movedCardId, record.replacedCardId,
            record.originalPosition.x, record.originalPosition.y);
    }
    else {
        record.replacedCardId = -1; // 没有可替换的卡牌
        CCLOG("Created undo record: moved=%d, no replaced card", record.movedCardId);
    }

    return record;
}