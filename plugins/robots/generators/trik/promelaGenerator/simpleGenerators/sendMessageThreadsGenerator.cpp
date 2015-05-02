#include "sendMessageThreadsGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include <generatorBase/parts/subprograms.h>
#include <generatorBase/parts/variables.h>
#include <qrtext/core/types/typeExpression.h>
#include <qrtext/lua/types/table.h>
#include <qrtext/lua/types/string.h>
#include <qrtext/lua/types/integer.h>

using namespace generatorBase::simple;
using namespace trik::promela::simple;
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
					, QString::number(repo.property(id, "Message").toString().count(",") + 1))
				, Binding::createStatic("@@I@@", "")
				, Binding::createStatic("@@S@@", "")}
			, parent)
{
}

QString SendMessageThreadsGenerator::templateSelection(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer, const qReal::Id &id)
{
	QString const message = repo.property(id, "Message").toString();
	qrtext::core::types::TypeExpression *type = customizer.factory()->variables()->expressionType(message).data();

	if (dynamic_cast<Table *>(type) != nullptr) {
		if (message.contains("{")) {
			return "sendMessageTableConstructor.t";
		} else {
			return "sendMessage.t";
		}
	} else if (dynamic_cast<String *>(type) != nullptr) {
		return "sendMessageString.t";
	} else {
		return "sendMessageInt.t";
	}
}

bool SendMessageThreadsGenerator::isInt(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer, const qReal::Id &id)
{
	QString const message = repo.property(id, "Message").toString();
	qrtext::core::types::TypeExpression *type = customizer.factory()->variables()->expressionType(message).data();
	bool const isInteger = dynamic_cast<Integer *>(type) != nullptr;
	Table *table = dynamic_cast<Table *>(type);
	bool const isIntTable = table != nullptr && !table->elementType().dynamicCast<Integer>().isNull();

	return isInteger || isIntTable;
}
