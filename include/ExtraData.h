#pragma once

#include "GameBSExtraData.h"  // ExtraDataType, BaseExtraList

#include <map>  // map
#include <utility>  // pair

#include "ISerializableData.h"  // ISerializableData


namespace Forms
{
	class IExtraDataType;


	class ExtraData : public ISerializableData
	{
	public:
		ExtraData();
		ExtraData(const ExtraData&) = default;
		ExtraData(ExtraData&&) = default;
		virtual ~ExtraData();

		virtual const char*	ClassName() const override;
		virtual void		Clear() override;
		virtual bool		Save(json& a_save) override;
		virtual bool		Load(json& a_load) override;

		int					Compare(const ExtraData& a_rhs) const;
		bool				operator<(const ExtraData& a_rhs) const;
		void				Parse(BaseExtraList* a_baseExtraList);

	protected:
		using ExtraDataMap = std::map<ExtraDataType, IExtraDataType*>;
		using GetOrCreate_Result = std::pair<ExtraDataMap::iterator, bool>;


		GetOrCreate_Result	GetOrCreate(ExtraDataType a_type, bool a_tryToGet);


		ExtraDataMap _extraList;
	};
}
