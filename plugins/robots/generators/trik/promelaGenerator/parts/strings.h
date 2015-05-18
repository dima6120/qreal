#pragma once

#include <QtCore/QMap>
#include <QtCore/QSet>

#include <generatorBase/templateParametrizedEntity.h>
#include <qrtext/core/types/typeExpression.h>

namespace trik {
namespace promela {
namespace parts {

class Strings : public generatorBase::TemplateParametrizedEntity
{
public:
	explicit Strings(const QString &pathToTemplates);

	QString addString(const QString &string);
	void setChannelType(const QString &channel, bool string);
	bool stringChannel(const QString &channel);

	void processCode(QString &code);

private:
	QString declarations();
	QString initialization();

	int mStringNumber;

	QMap<QString, bool> mStringChannels;
	QMap<QString, QString> mNames;
	QSet<QString> mStrings;
};

}
}
}
