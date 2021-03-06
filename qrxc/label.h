#pragma once

#include <QDomElement>
#include <QtCore/QStringList>
#include "../qrutils/scalableItem.h"

namespace utils {
	class OutFile;
}

class Label : public utils::ScalableItem
{
public:
	bool init(QDomElement const &element, int index, bool nodeLabel, int width, int height);
	void generateCodeForConstructor(utils::OutFile &out);
	void generateCodeForUpdateData(utils::OutFile &out);
	void generateCodeForFields(utils::OutFile &out);
	// Default copy constructor ok.
private:
	QString titleName() const;

	/// split the string into substrings wherever "##" occurs and return the list of those strings
	QStringList getListOfStr(QString const &strToParse) const;

	utils::ScalableCoordinate mX;
	utils::ScalableCoordinate mY;
	int mIndex;
	QString mText;
	QString mTextBinded;
	QString mReadOnly;
	QString mBackground;
	QString mCenter;
	bool mIsHard;
	bool mIsPlainText;
};
