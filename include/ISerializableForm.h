#pragma once

#undef SetForm

#include "GameForms.h"  // FormType

#include <string>  // string

#include "ISerializableData.h"  // ISerializableData
#include "json.hpp"  // json
#include "RefHandle.h"  // RefHandle

class BaseExtraList;
class InventoryEntryData;
class TESForm;
struct ModInfo;


namespace iEquip
{
	enum
	{
		kInvalid = static_cast<UInt32>(-1)
	};


	class ISerializableForm :
		public ISerializableData,
		public RefHandle
	{
	protected:
		using json = nlohmann::json;

	public:
		ISerializableForm() = delete;
		explicit ISerializableForm(bool a_useHandle);
		ISerializableForm(const ISerializableForm&) = default;
		ISerializableForm(ISerializableForm&&) = default;
		virtual ~ISerializableForm();

		virtual void		Clear() override;
		virtual bool		Save(json& a_save) override;
		virtual bool		Load(json& a_load) override;
		virtual FormType	GetType() const = 0;
		virtual void		Set(TESForm* a_form, BaseExtraList* a_extraList) = 0;

		bool				operator<(const ISerializableForm& a_rhs) const;
		int					Compare(const ISerializableForm& a_rhs) const;
		void				SetForm(UInt32 a_formID);
		UInt32				GetRawFormID() const;
		UInt32				GetLoadedFormID() const;

	protected:
		virtual int			Comp(const ISerializableForm* a_rhs) const = 0;

		const ModInfo*		LookupLoadedModByIndex(UInt8 a_index);
		const ModInfo*		LookupLoadedLightModByIndex(UInt16 a_index);


		UInt32		_rawFormID;
		UInt32		_loadedFormID;
		std::string	_pluginName;
		bool		_isLightMod;
		bool		_isGeneratedID;
	};
}
