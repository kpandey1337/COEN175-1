
#include "Scope.h"
#include "checker.h"
#include "Type.h"
#include <sstream>

static std::ostream& operator <<(std::ostream& strm, Expression* expr);

void generateGlobals(Scope* toplevel);

