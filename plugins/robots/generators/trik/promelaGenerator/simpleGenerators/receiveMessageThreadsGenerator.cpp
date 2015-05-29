#include "receiveMessageThreadsGenerator.h"

#include "promelaGeneratorFactory.h"

#include <generatorBase/generatorCustomizer.h>
#include <generatorBase/parts/subprograms.h>
#include <generatorBase/parts/variables.h>
#include <qrtext/core/types/typeExpression.h>
#include <qrtext/lua/types/table.h>
#include <qrtext/lua/types/string.h>

using namespace generatorBase::simple;
using namespace trik::promela::simple;
using namespace trik::promela;
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
				, Binding::createStatic("@@ASSIGNMENT@@", assignment(repo, customizer, id))
				}
			, parent)
{
}

QString ReceiveMessageThreadsGenerator::templateSelection(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer, const qReal::Id &id)
{
	QString const variable = repo.property(id, "Variable").toString();
	QSharedPointer<qrtext::core::types::TypeExpression> type = customizer.factory()->variables()
			->expressionType(variable);
	PromelaGeneratorFactory *factory = dynamic_cast<PromelaGeneratorFactory *>(customizer.factory());

	if (type.data()->is<Table>()) {
		QSharedPointer<Table> table = type.dynamicCast<Table>();
		factory->strings().setChannelType(customizer.factory()->subprograms()->currentControlFlow()->threadId()
				, table.data()->elementType().data()->is<String>());

		return "receiveMessage.t";
	} else if (type.data()->is<String>()) {
		factory->strings().setChannelType(customizer.factory()->subprograms()->currentControlFlow()->threadId()
				, true);
		return "receiveMessageString.t";
	} else {
		factory->strings().setChannelType(customizer.factory()->subprograms()->currentControlFlow()->threadId()
				, false);
		return "receiveMessageInt.t";
	}
}

QString ReceiveMessageThreadsGenerator::assignment(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer, const qReal::Id &id)
{
	QString const variable = repo.property(id, "Variable").toString();
	QSharedPointer<qrtext::core::types::TypeExpression> type = customizer.factory()->variables()
			->expressionType(variable);
	QSharedPointer<Table> table = type.dynamicCast<Table>();

	if (!table.isNull()) {
		if (table.data()->elementType().data()->is<String>()) {
			return QString("copyStr(@@VARIABLE@@, @@VALUE@@)")
					.replace("@@VARIABLE@@", variable + ".a[receive_i]")
					.replace("@@VALUE@@",
						repo.stringProperty(repo.outgoingLinks(id).at(0), "Guard") + "proc_strings[receive_i]");
		} else {
			return QString("@@VARIABLE@@ = @@VALUE@@")
					.replace("@@VARIABLE@@", variable + ".a[receive_i]")
					.replace("@@VALUE@@", "temp.a[receive_i]");
		}
	}

	return QString();
}
