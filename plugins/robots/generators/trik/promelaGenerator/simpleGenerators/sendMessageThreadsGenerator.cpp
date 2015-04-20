#include "sendMessageThreadsGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include <generatorBase/parts/threads.h>

using namespace generatorBase::simple;
using namespace trik::promela::simple;

SendMessageThreadsGenerator::SendMessageThreadsGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "threads/sendMessage.t"
			, { Binding::createStatic("@@SENDER@@", customizer.factory()->threads().blockThreadId(id))
				, Binding::createConverting("@@MESSAGE@@"
					, "Message"
					, customizer.factory()->functionBlockConverter(id, "Message"))
				, Binding::createDirect("@@RECEIVER@@", "Thread") }
			, parent)
{
}
