#pragma once

#include "json.hpp"  // json


namespace Forms
{
	class ISerializableData
	{
	protected:
		using json = nlohmann::json;

	public:
		ISerializableData();
		ISerializableData(const ISerializableData&) = default;
		ISerializableData(ISerializableData&&) = default;
		virtual ~ISerializableData();

		virtual const char*	ClassName() const = 0;
		virtual void		Clear() = 0;
		virtual bool		Save(json& a_save) = 0;
		virtual bool		Load(json& a_load) = 0;
	};


	template <typename T>
	bool loadJsonObj(nlohmann::json& a_load, const char* a_name, T& a_val)
	{
		auto it = a_load.find(a_name);
		if (it != a_load.end()) {
			a_val = it.value();
			return true;
		} else {
			_ERROR("[ERROR] Could not find (%s) in serialized save data!\n", a_name);
			return false;
		}
	}
}
