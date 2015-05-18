#include "strings.h"

#include <qrtext/lua/types/string.h>

using namespace trik::promela::parts;


Strings::Strings(const QString &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
	, mStringNumber(0)
{
}

QString Strings::addString(const QString &string)
{
	QString number = QString::number(mStringNumber);
	QString const name = "CONST_STRING" + number;

	if (mStrings.contains(string)) {
		return mNames.key(string);
	}

	mStrings.insert(string);
	mNames.insert(name, string);

	mStringNumber++;

	return name;
}

void Strings::setChannelType(const QString &channel, bool string)
{
	mStringChannels.insert(channel, mStringChannels.value(channel, false) || string);
}

bool Strings::stringChannel(const QString &channel)
{
	return mStringChannels.value(channel, false);
}

void Strings::processCode(QString &code)
{
	code.replace("@@CONST_STRINGS@@", declarations()).replace("@@CONST_STRINGS_INIT@@", initialization());
}

QString Strings::declarations()
{
	QString declarations = "";

	for (QString const &name : mNames.keys()) {
		declarations += readTemplate("variables/variableDeclaration.t")
				.replace("@@TYPE@@", "string")
				.replace("@@NAME@@", name);
	}

	return declarations;
}

QString Strings::initialization()
{
	QString init = "";

	for (QString const &name : mNames.keys()) {
		QString chars = "";
		QString const str = mNames.value(name);

		for (int i = 0; i < str.size(); i++) {
			chars += readTemplate("variables/oneCharInit.t")
					.replace("@@NAME@@", name)
					.replace("@@INDEX@@", QString::number(i))
					.replace("@@VALUE@@", QString::number(str[i].unicode()));
		}

		init += readTemplate("variables/constStringInit.t")
				.replace("@@STRING@@", str)
				.replace("@@NAME@@", name)
				.replace("@@SIZE@@", QString::number(str.size()))
				.replace("@@INIT@@", chars);
	}

	return init.isEmpty() ? "skip;" : readTemplate("variables/constStringsInit.t").replace("@@STRINGS@@", init);
}
