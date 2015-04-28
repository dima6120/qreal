#include "strings.h"

using namespace trik::promela::parts;


Strings::Strings(const QString &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
	, mStringNumber(0)
{
}

QString Strings::addString(const QString &string)
{
	QString number = QString::number(mStringNumber);
	QString const stub = "@@STRING" + number + "@@";
	QString const name = "CONST_STRING" + number;

	if (mStrings.contains(string)) {
		return mStubs.key(string);
	}

	mStrings.insert(string);
	mStubs.insert(stub, string);
	mNames.insert(stub, name);

	mStringNumber++;

	return stub;
}

void Strings::processCode(QString &code)
{
	code.replace("@@CONST_STRINGS@@", declarations()).replace("@@CONST_STRINGS_INIT@@", initialization());
	replaceStubs(code);
}

QString Strings::declarations()
{
	QString declarations = "";

	for (QString const &name : mNames.values()) {
		declarations += readTemplate("variables/variableDeclaration.t")
				.replace("@@TYPE@@", "string")
				.replace("@@NAME@@", name) + "\n";
	}

	return declarations;
}

QString Strings::initialization()
{
	QString init = "";

	for (QString const &stub : mStubs.keys()) {
		QString chars = "";
		QString const str = mStubs.value(stub);
		QString const name = mNames.value(stub);

		for (int i = 0; i < str.size(); i++) {
			chars += readTemplate("variables/oneCharInit.t")
					.replace("@@NAME@@", name)
					.replace("@@INDEX@@", QString::number(i))
					.replace("@@VALUE@@", QString::number(str[i].unicode())) + "\n";
		}

		init += readTemplate("variables/constStringInit.t")
				.replace("@@STRING@@", str)
				.replace("@@NAME@@", name)
				.replace("@@SIZE@@", QString::number(str.size()))
				.replace("@@INIT@@", chars) + "\n";
	}

	return readTemplate("variables/constStringsInit.t").replace("@@STRINGS@@", init);
}

void Strings::replaceStubs(QString &code)
{
	for (QString const &stub : mStubs.keys()) {
		code.replace(stub, mNames.value(stub));
	}
}

