#include "sendMessageThreadsGenerator.h"

#include "promelaGeneratorFactory.h"

#include <generatorBase/generatorCustomizer.h>
#include <generatorBase/parts/subprograms.h>
#include <generatorBase/parts/variables.h>
#include <qrtext/core/types/typeExpression.h>
#include <qrtext/lua/types/table.h>
#include <qrtext/lua/types/string.h>
#include <qrtext/lua/types/integer.h>

using namespace generatorBase::simple;
using namespace trik::promela::simple;
using namespace trik::promela;
using namespace qrtext::lua::types;

SendMessageThreadsGenerator::SendMessageThreadsGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "threads/" + templateSelection(repo, customizer, id)
			, { Binding::createStatic("@@SENDER@@"
					, customizer.factory()->subprograms()->currentControlFlow()->threadId())
				, Binding::createConverting("@@MESSAGE@@"
					, "Message"
					, customizer.factory()->functionBlockConverter(id, "Message"))
				, Binding::createDirect("@@RECEIVER@@", "Thread")
				, Binding::createStatic("@@VARIABLE@@"
					, isInt(repo, customizer, id) ? "temp"
					: customizer.factory()->subprograms()->currentControlFlow()->threadId() + "proc_strings")
				, Binding::createStatic("@@SIZE@@"
					, QString::number(repo.property(id, "Message").toString().count(",") + 1))}
			, parent)
{
}

QString SendMessageThreadsGenerator::templateSelection(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer, const qReal::Id &id)
{
	QString const message = repo.property(id, "Message").toString();
	QSharedPointer<qrtext::core::types::TypeExpression> type = customizer.factory()->variables()
			->expressionType(message);
	PromelaGeneratorFactory *factory = dynamic_cast<PromelaGeneratorFactory *>(customizer.factory());

	if (type.data()->is<Table>()) {
		QSharedPointer<Table> table = type.dynamicCast<Table>();
		factory->strings().setChannelType(repo.property(id, "Thread").toString()
				, table.data()->elementType().data()->is<String>());

		if (message.contains("{")) {
			return "sendMessageTableConstructor.t";
		} else {
			return "sendMessage.t";
		}
	} else if (type.data()->is<String>()) {
		factory->strings().setChannelType(repo.property(id, "Thread").toString(), true);
		return "sendMessageString.t";
	} else {
		factory->strings().setChannelType(repo.property(id, "Thread").toString(), false);
		return "sendMessageInt.t";
	}
}

bool SendMessageThreadsGenerator::isInt(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer, const qReal::Id &id)
{
	QString const message = repo.property(id, "Message").toString();
	QSharedPointer<qrtext::core::types::TypeExpression> type = customizer.factory()->variables()
			->expressionType(message);
	bool const isInteger = type.data()->is<Integer>();
	QSharedPointer<Table> table = type.dynamicCast<Table>();
	bool const isIntTable = !table.isNull() && table.data()->elementType().data()->is<Integer>();

	return isInteger || isIntTable;
}
