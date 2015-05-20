#pragma once

#include <qrkernel/ids.h>

#include "qrgui/textEditor/textEditorDeclSpec.h"

namespace qReal {

class QRGUI_TEXT_EDITOR_EXPORT CodeBlockManagerInterface
{
public:
	virtual ~CodeBlockManagerInterface() {}

	virtual QPair<int, int> intervalById(QString const &path, Id const &id) = 0;
	virtual QList<Id> IdsByLineNumber(QString const &path, int lineNumber) = 0;
};

}
