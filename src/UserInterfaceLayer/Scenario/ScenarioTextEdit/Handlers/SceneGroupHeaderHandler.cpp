#include "SceneGroupHeaderHandler.h"

#include "../ScenarioTextEdit.h"

#include <QKeyEvent>
#include <QTextBlock>

using namespace KeyProcessingLayer;
using namespace BusinessLogic;


SceneGroupHeaderHandler::SceneGroupHeaderHandler(ScenarioTextEdit* _editor) :
	StandardKeyHandler(_editor)
{
}

void SceneGroupHeaderHandler::handleEnter(QKeyEvent*)
{
	//
	// Получим необходимые значения
	//
	// ... курсор в текущем положении
	QTextCursor cursor = editor()->textCursor();
	// ... блок текста в котором находится курсор
	QTextBlock currentBlock = cursor.block();
	// ... текст до курсора
	QString cursorBackwardText = currentBlock.text().left(cursor.positionInBlock());
	// ... текст после курсора
	QString cursorForwardText = currentBlock.text().mid(cursor.positionInBlock());


	//
	// Обработка
	//
	if (editor()->isCompleterVisible()) {
		//! Если открыт подстановщик

		//
		// Ни чего не делаем
		//
	} else {
		//! Подстановщик закрыт

		if (cursor.hasSelection()) {
			//! Есть выделение

			//
			// Ни чего не делаем
			//
		} else {
			//! Нет выделения

			if (cursorBackwardText.isEmpty()
				&& cursorForwardText.isEmpty()) {
				//! Текст пуст

				//
				// Ни чего не делаем
				//
			} else {
				//! Текст не пуст

				if (cursorBackwardText.isEmpty()) {
					//! В начале блока

					//
					// Вставить блок время и место перед группой
					//
					cursor.insertBlock();
					cursor.movePosition(QTextCursor::Left);
					cursor.setBlockFormat(QTextBlockFormat());
					editor()->setTextCursor(cursor);
					editor()->changeScenarioBlockType(ScenarioTextBlockStyle::TimeAndPlace);
					editor()->moveCursor(QTextCursor::Right);
				} else if (cursorForwardText.isEmpty()) {
					//! В конце блока

					//
					// Вставить блок время и место
					//
					editor()->addScenarioBlock(ScenarioTextBlockStyle::TimeAndPlace);
				} else {
					//! Внутри блока

					//
					// Вставить блок время и место
					//
					editor()->addScenarioBlock(ScenarioTextBlockStyle::TimeAndPlace);
				}
			}
		}
	}
}

void SceneGroupHeaderHandler::handleTab(QKeyEvent*)
{
	//
	// Получим необходимые значения
	//
	// ... курсор в текущем положении
	QTextCursor cursor = editor()->textCursor();
	// ... блок текста в котором находится курсор
	QTextBlock currentBlock = cursor.block();
	// ... текст до курсора
	QString cursorBackwardText = currentBlock.text().left(cursor.positionInBlock());
	// ... текст после курсора
	QString cursorForwardText = currentBlock.text().mid(cursor.positionInBlock());


	//
	// Обработка
	//
	if (editor()->isCompleterVisible()) {
		//! Если открыт подстановщик

		//
		// Ни чего не делаем
		//
	} else {
		//! Подстановщик закрыт

		if (cursor.hasSelection()) {
			//! Есть выделение

			//
			// Ни чего не делаем
			//
		} else {
			//! Нет выделения

			if (cursorBackwardText.isEmpty()
				&& cursorForwardText.isEmpty()) {
				//! Текст пуст

				//
				// Ни чего не делаем
				//
			} else {
				//! Текст не пуст

				if (cursorBackwardText.isEmpty()) {
					//! В начале блока

					//
					// Ни чего не делаем
					//
				} else if (cursorForwardText.isEmpty()) {
					//! В конце блока

					//
					// Как ENTER
					//
					handleEnter();
				} else {
					//! Внутри блока

					//
					// Ни чего не делаем
					//
				}
			}
		}
	}
}

void SceneGroupHeaderHandler::handleOther(QKeyEvent*)
{
	//
	// Найти закрывающий блок и обновить его текст
	//

	QTextCursor cursor = editor()->textCursor();

	//
	// Если редактируется заголовок группы
	//
	if (editor()->scenarioBlockType() == ScenarioTextBlockStyle::SceneGroupHeader) {

		cursor.movePosition(QTextCursor::NextBlock);

		// ... открытые группы на пути поиска необходимого для обновления блока
		int openedGroups = 0;
		bool isFooterUpdated = false;
		do {
			ScenarioTextBlockStyle::Type currentType =
					editor()->scenarioBlockType(cursor.block());

			if (currentType == ScenarioTextBlockStyle::SceneGroupFooter) {
				if (openedGroups == 0) {
					cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
					cursor.insertText(QObject::tr("END OF", "SceneGroupHeader"));
					cursor.insertText(" ");
					cursor.insertText(editor()->textCursor().block().text());
					isFooterUpdated = true;
				} else {
					--openedGroups;
				}
			} else if (currentType == ScenarioTextBlockStyle::SceneGroupHeader) {
				// ... встретилась новая группа
				++openedGroups;
			}

			cursor.movePosition(QTextCursor::NextBlock);
		} while (!isFooterUpdated && !cursor.atEnd());
	}
}