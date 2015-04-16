# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	trikGeneratorBase \
	trikQtsGenerator \
	trikFSharpGenerator \
	trikRuntimeUploaderPlugin \
	nxtGeneratorBase \
	nxtOsekCGenerator \
	nxtRussianCGenerator \
	promelaGenerator \

trikGeneratorBase.subdir = $$PWD/trik/trikGeneratorBase
trikQtsGenerator.subdir = $$PWD/trik/trikQtsGenerator
trikFSharpGenerator.subdir = $$PWD/trik/trikFSharpGenerator
trikRuntimeUploaderPlugin.subdir = $$PWD/trik/trikRuntimeUploaderPlugin
nxtGeneratorBase.subdir = $$PWD/nxt/nxtGeneratorBase
nxtOsekCGenerator.subdir = $$PWD/nxt/nxtOsekCGenerator
nxtRussianCGenerator.subdir = $$PWD/nxt/nxtRussianCGenerator
promelaGenerator.subdir = $$PWD/trik/promelaGenerator

trikGeneratorBase.depends = generatorBase
trikQtsGenerator.depends = trikGeneratorBase
trikFSharpGenerator.depends = trikGeneratorBase
trikRuntimeUploaderPlugin.depends = trikGeneratorBase
nxtGeneratorBase.depends = generatorBase
nxtOsekCGenerator.depends = nxtGeneratorBase
nxtRussianCGenerator.depends = nxtGeneratorBase
promelaGenerator.depends = trikGeneratorBase
