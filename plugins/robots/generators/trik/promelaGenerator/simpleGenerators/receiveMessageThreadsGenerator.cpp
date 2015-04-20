#include "receiveMessageThreadsGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include <generatorBase/parts/threads.h>
#include <qrkernel/ids.h>


using namespace generatorBase::simple;
using namespace trik::promela::simple;

ReceiveMessageThreadsGenerator::ReceiveMessageThreadsGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "threads/receiveMessage.t"
			, { Binding::createConverting("@@VARIABLE@@"
					, "Variable"
					, customizer.factory()->functionBlockConverter(id, "Variable"))
				, Binding::createStatic("@@RECEIVER@@"
						, customizer.factory()->threads().blockThreadId(id))
				, Binding::createStatic("@@SENDER@@", repo.stringProperty(repo.outgoingLinks(id).at(0)
						, "Guard"))
				}
			, parent)
{
}
