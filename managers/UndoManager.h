#pragma once
#include <stack>
#include "../models/UndoRecord.h"

/**
 * @brief 撤销管理器类
 *
 * 管理游戏中的撤销操作记录，支持记录、撤销和清空操作。
 * 使用场景：游戏操作历史记录和撤销功能实现。
 */
class UndoManager {
public:
    /**
    * @brief 记录操作
    *
    * @param record 要记录的撤销操作
    */
    void recordAction(const UndoRecord& record) {
        _undoStack.push(record);
    }
    /**
     * @brief 检查是否可以撤销
     *
     * @return bool 是否可以撤销
     */
    bool canUndo() const { return !_undoStack.empty(); }
    /**
     * @brief 弹出撤销记录
     *
     * @return UndoRecord 撤销记录对象
     */
    UndoRecord popUndoRecord() {
        if (_undoStack.empty()) {
            throw std::out_of_range("Undo stack is empty");
        }
        UndoRecord record = _undoStack.top();
        _undoStack.pop();
        return record;
    }
    /**
     * @brief 清空撤销记录
     */
    void clear() {
        while (!_undoStack.empty()) _undoStack.pop();
    }

private:
	std::stack<UndoRecord> _undoStack; // 撤销记录栈
};