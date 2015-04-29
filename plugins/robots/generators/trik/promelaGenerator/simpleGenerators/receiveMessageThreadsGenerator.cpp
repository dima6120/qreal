#include "receiveMessageThreadsGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include <generatorBase/parts/subprograms.h>
#include <generatorBase/parts/variables.h>
#include <qrtext/core/types/typeExpression.h>
#include <qrtext/lua/types/table.h>
#include <qrtext/lua/types/string.h>

using namespace generatorBase::simple;
using namespace trik::promela::simple;
using namespace qrtext::lua::types;

ReceiveMessageThreadsGenerator::ReceiveMessageThreadsGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "threads/" + templateSelection(repo, customizer, id)
			, { Binding::createConverting("@@VARIABLE@@"
					, "Variable"
					, customizer.factory()->functionBlockConverter(id, "Variable"))
				, Binding::createStatic("@@RECEIVER@@"
						, customizer.factory()->subprograms()->currentControlFlow()->threadId())
				, Binding::createStatic("@@SENDER@@", repo.stringProperty(repo.outgoingLinks(id).at(0)
						, "Guard"))
				}
			, parent)
{
}

QString ReceiveMessageThreadsGenerator::templateSelection(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer, const qReal::Id &id)
{
	QString const variable = repo.property(id, "Variable").toString();
	qrtext::core::types::TypeExpression *type = customizer.factory()->variables()->expressionType(variable).data();

	if (dynamic_cast<Table *>(type) != nullptr) {
		return "receiveMessage.t";
	} else if (dynamic_cast<String *>(type) != nullptr) {
		return "receiveMessageString.t";
	} else {
		return "receiveMessageInt.t";
	}
}
