#pragma once

#include <QtCore/QMap>
#include <QtCore/QSet>

#include <generatorBase/templateParametrizedEntity.h>

namespace trik {
namespace promela {
namespace parts {

class Strings : public generatorBase::TemplateParametrizedEntity
{
public:
	explicit Strings(const QString &pathToTemplates);

	QString addString(const QString &string);

	void processCode(QString &code);

private:
	QString declarations();
	QString initialization();
	void replaceStubs(QString &code);

	int mStringNumber;

	QMap<QString, QString> mStubs;
	QMap<QString, QString> mNames;
	QSet<QString> mStrings;
};

}
}
}
