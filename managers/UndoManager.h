#pragma once
#include <stack>
#include "../models/UndoRecord.h"

/**
 * @brief ������������
 *
 * ������Ϸ�еĳ���������¼��֧�ּ�¼����������ղ�����
 * ʹ�ó�������Ϸ������ʷ��¼�ͳ�������ʵ�֡�
 */
class UndoManager {
public:
    /**
    * @brief ��¼����
    *
    * @param record Ҫ��¼�ĳ�������
    */
    void recordAction(const UndoRecord& record) {
        _undoStack.push(record);
    }
    /**
     * @brief ����Ƿ���Գ���
     *
     * @return bool �Ƿ���Գ���
     */
    bool canUndo() const { return !_undoStack.empty(); }
    /**
     * @brief ����������¼
     *
     * @return UndoRecord ������¼����
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
     * @brief ��ճ�����¼
     */
    void clear() {
        while (!_undoStack.empty()) _undoStack.pop();
    }

private:
	std::stack<UndoRecord> _undoStack; // ������¼ջ
};