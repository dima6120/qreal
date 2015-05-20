#pragma once

#include <QtCore/QMap>

#include "qrgui/textEditor/codeBlockManagerInterface.h"
#include <qrkernel/ids.h>

namespace qReal {
namespace text {

class QRGUI_TEXT_EDITOR_EXPORT CodeBlockManager : public CodeBlockManagerInterface
{
public:
	CodeBlockManager();
	void addNewCode(QString const &path);
	void removeCode(QString const &path);
	QPair<int, int> intervalById(QString const &path, Id const &id);
	QList<Id> IdsByLineNumber(QString const &path, int lineNumber);

private:
	void readDbgFile(QString const &path);

	QMap<QString, QMap<Id, QPair<int, int> > > mBlockToCode;
	QMap<QString, QMap<int, QList<Id> > > mCodeToBlock;
};

}
}
