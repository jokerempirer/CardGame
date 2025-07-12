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
    // 1. ���عؿ�����
    LevelConfig levelConfig = LevelConfigLoader::loadFromFile(levelPath);

    // 3. ת��Ϊ��Ϸģ��
    _gameModel = GameConfigService::convertToGameModel(levelConfig);

    // 3. ������Ϸ��ͼ
    _gameView = GameView::create();
    _gameView->setupWithGameModel(_gameModel);

    // 4. ���ÿ��Ƶ���ص�
    _gameView->setCardClickCallback([this](int cardId) {
		CCLOG("1.Card clicked: %d", cardId);
        this->handleCardClick(cardId);
        });

    CCLOG("Setting undo callback");
    // 5. ���û��˰�ť�ص�
    _gameView->setUndoCallback([this]() {
        CCLOG("Undo callback triggered");
        this->handleUndoClick();
        });

    // 6. ��ӵ���ǰ�ڵ�
    this->addChild(_gameView);
    _gameView->setPosition(Vec2::ZERO); // ȷ����ԭ��

    // 7. ��ʼ�����˹�����
    _undoManager = std::make_unique<UndoManager>();

    // 8. ����������������
    if (auto topCard = _gameModel.getTopStackCard()) {
        topCard->isTopCard = true;
        _gameView->updateCardView(*topCard);
    }

    return true;
}

void GameController::startGame()
{
    CCLOG("GameController::startGame - Starting game with model state:");
    // ��ӡ��Ϸģ��״̬
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
    if (!card || card->id < 0) { // �����Ч�Լ��
        CCLOG("ERROR: Invalid card pointer or ID: %d", cardId);
        return;
    }

    // ���ݿ�����������
    if (!card->isInPlayfield) {
        handleHandCardClick(card);  // ����������
    }
    else {
        handlePlayfieldCardClick(card); // ������������
    }
}

void GameController::handleHandCardClick(CardModel* card) {
    // ����������Ƭ�滻������
	CCLOG("3.Card clicked: %d", card->id);
    replaceTrayWithHandCard(card);
}

void GameController::handlePlayfieldCardClick(CardModel* card) {
	CCLOG("3.Card clicked: %d", card->id);
    // ��鿨Ƭ�Ƿ�����ƶ�
    if (!canMoveCard(card)) {
        // ��ƥ��Ч����������ʾ
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

    // �����濨Ƭ�滻������������
    replaceTrayWithPlayFieldCard(card);
}

bool GameController::canMoveCard(CardModel* card) {
    // 1. ��Ƭ������������
    if (!card->isInPlayfield) return false;

    // 2. �����������ж�����
    CardModel* topCard = _gameModel.getTopStackCard();
    if (!topCard) return false;

    // 3. ������Ϊ1
    return GameService::isMatch(*card, *topCard);
}

void GameController::replaceTrayWithPlayFieldCard(CardModel* card) {

    CCLOG("replaceTrayWithPlayFieldCard - Card ID: %d, Address: %p", card->id, card);
    logModelState("Before playfield card replacement");

    CCLOG("4. Processing playfield card click for ID: %d", card->id);

    // 1. �������˼�¼
    UndoRecord record = createUndoRecord(card, UndoActionType::PLAYFIELD_CARD_CLICK);

    // 2. ��¼����
    _undoManager->recordAction(record);

    // 3. ����ԭ������״̬
    if (auto oldTopCard = _gameModel.getTopStackCard()) {
        oldTopCard->isTopCard = false;
        _gameView->updateCardView(*oldTopCard);
    }

    int movedCardId = card->id;
    CardFaceType face = card->face;
    CardSuitType suit = card->suit;

    // 4. ���������Ƴ���ǰ��Ƭ
    auto& playfieldCards = _gameModel.playfieldCards;
    playfieldCards.erase(
        std::remove_if(playfieldCards.begin(), playfieldCards.end(),
            [card](const auto& c) { return c->id == card->id; }),
        playfieldCards.end()
    );

    // 5. �����¿��ƶ���
    auto newCard = std::make_unique<CardModel>(
		movedCardId, face, suit,
        getTopStackPosition(), false
    );
    newCard->isTopCard = true;

    // 6. ��ӵ�����������
    _gameModel.stackCards.push_back(std::move(newCard));
    CardModel* newCardPtr = _gameModel.stackCards.back().get();

    CCLOG("5. New top card ID: %d at position: (%.1f, %.1f)",
        newCardPtr->id, newCardPtr->position.x, newCardPtr->position.y);

    // 7. ���¿���ӳ��
    _gameModel.cardMap[movedCardId] = newCardPtr;

    // 8. ������ͼִ�ж���
    _gameView->playCardMoveAnimation(newCardPtr->id, newCardPtr->position, [this, newCardPtr]() {
        _gameView->updateCardView(*newCardPtr);
        });

    logModelState("After playfield card replacement");
}

void GameController::replaceTrayWithHandCard(CardModel* card) {

    logModelState("Before hand card replacement");

	CCLOG("4.Card clicked: %d", card->id);
    // 1. �������˼�¼
    UndoRecord record = createUndoRecord(card, UndoActionType::HAND_CARD_CLICK);

    // 2. ��¼����
    _undoManager->recordAction(record);

    // 3. ����ԭ������״̬
    if (auto oldTopCard = _gameModel.getTopStackCard()) {
        oldTopCard->isTopCard = false;
        _gameView->updateCardView(*oldTopCard);
    }   
    
    // ���Ƴ�ǰ���濨����Ϣ
    int movedCardId = card->id;
    CardFaceType face = card->face;
    CardSuitType suit = card->suit;

    // 4. ��ԭ�����Ƴ���ǰ��Ƭ
    if (card->isInPlayfield) {
        auto& playfieldCards = _gameModel.playfieldCards;
        playfieldCards.erase(
            std::remove_if(playfieldCards.begin(), playfieldCards.end(),
                [card](const auto& c) { return c->id == card->id; }), // ʹ��->
            playfieldCards.end()
        );
    }
    else {
        auto& stackCards = _gameModel.stackCards;
        stackCards.erase(
            std::remove_if(stackCards.begin(), stackCards.end(),
                [card](const auto& c) { return c->id == card->id; }), // ʹ��->
            stackCards.end()
        );
    }

    // 5. �����¿��ƶ���
    auto newCard = std::make_unique<CardModel>(
        movedCardId, face, suit,
        getTopStackPosition(), false
    );
    newCard->isTopCard = true;
    
    // 6. ��ӵ�����������
    _gameModel.stackCards.push_back(std::move(newCard));
    CardModel* newCardPtr = _gameModel.stackCards.back().get();

    CCLOG("5. New top card ID: %d at position: (%.1f, %.1f)",
        newCardPtr->id, newCardPtr->position.x, newCardPtr->position.y);

    // 7. ���¿���ӳ��
    _gameModel.cardMap[card->id] = newCardPtr;

    // 8. ������ͼִ�ж���
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

    // 1. ��ȡ���˼�¼
    UndoRecord record = _undoManager->popUndoRecord();
    CCLOG("Undo action for card ID: %d, type: %d",
        record.movedCardId, static_cast<int>(record.actionType));

    // 2. ��ȡ�ƶ��Ŀ��ƣ���ǰ�ڶѶ���
    CardModel* movedCard = _gameModel.getCardById(record.movedCardId);
    if (!movedCard) {
        return;
    }

    // 3. ��ȡ���滻�Ŀ��ƣ�ԭ�����ƣ�
    CardModel* replacedCard = _gameModel.getCardById(record.replacedCardId);
    if (!replacedCard) {
        return;
    }

    // 4. �ӵ�ǰ�����Ƴ��ƶ��Ŀ���
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

    // 5. �ָ�ԭ������
    replacedCard->isTopCard = true;
    _gameView->updateCardView(*replacedCard);

    // 6. �����ָ��Ŀ��ƶ���
    auto restoredCard = std::make_unique<CardModel>(
        record.movedCardId,
        movedCard->face,
        movedCard->suit,
        record.originalPosition,
        (record.actionType == UndoActionType::PLAYFIELD_CARD_CLICK)
    );

    // 7. ���ݲ������ͻָ�����Ӧ����
    switch (record.actionType) {
    case UndoActionType::HAND_CARD_CLICK:
        // �ָ���������
        _gameModel.stackCards.push_back(std::move(restoredCard));
        break;

    case UndoActionType::PLAYFIELD_CARD_CLICK:
        // �ָ���������
        _gameModel.playfieldCards.push_back(std::move(restoredCard));
        break;
    }

    // 8. ���¿���ӳ��
    CardModel* restoredCardPtr = (record.actionType == UndoActionType::HAND_CARD_CLICK) ?
        _gameModel.stackCards.back().get() :
        _gameModel.playfieldCards.back().get();

    _gameModel.cardMap[record.movedCardId] = restoredCardPtr;

    // 9. ���Ż��˶���
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
    // ��ȡ��Ϸģ���еĶ�����λ��
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
        record.replacedCardId = -1; // û�п��滻�Ŀ���
        CCLOG("Created undo record: moved=%d, no replaced card", record.movedCardId);
    }

    return record;
}