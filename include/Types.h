#pragma once

#include <memory>  // shared_ptr


namespace iEquip
{
	class ISerializableForm;

	typedef std::shared_ptr<ISerializableForm> SerializableFormPtr;
}
