#pragma once

#include "../semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class IfBlockRuleBase : public SemanticTransformationRule
{
public:
	IfBlockRuleBase(SemanticTree *tree, Id const &id
			, LinkInfo const &thenLink, LinkInfo const &elseLink);

protected:
	LinkInfo const &mThenLink;
	LinkInfo const &mElseLink;
};

}
}
}
}
