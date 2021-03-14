#pragma once

#include "Util/Misc.h"
#include "Util/Util.h"
#include "Papyrus/PapyrusCamera.h"
#include "Papyrus/PapyrusDatabase.h"
#include "Papyrus/PapyrusObject.h"
#include "Papyrus/PapyrusUtil.h"

namespace Papyrus
{
	bool Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();
		if (!papyrus) {
			logger::critical("Papyrus: Couldn't get Interface");
			return false;
		}

		papyrus->Register(Camera::Register);
		papyrus->Register(Database::Register);
		papyrus->Register(Object::Register);
		papyrus->Register(Util::Register);

		return true;
	}
}
