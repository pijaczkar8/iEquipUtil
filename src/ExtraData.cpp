#include "ExtraData.h"

#include "GameBSExtraData.h"  // ExtraDataType, BSExtraData, BaseExtraList

#include <string>  // to_string, stoi
#include <algorithm>  // make_pair
#include <exception>  // exception

#include "ExtraEnchantment.h"  // ExtraEnchantment
#include "ExtraHealth.h"  // ExtraHealth
#include "ExtraPoison.h"  // ExtraPoison
#include "ExtraTextDisplayData.h"  // ExtraTextDisplayData
#include "IExtraDataType.h"  // IExtraDataType
#include "version.h"  // MAKE_STR


namespace iEquip
{
	ExtraData::ExtraData()
	{}


	ExtraData::~ExtraData()
	{
		Clear();
	}


	const char* ExtraData::ClassName() const
	{
		return MAKE_STR(ExtraData);
	}


	void ExtraData::Clear()
	{
		for (auto& xData : _extraList) {
			delete xData.second;
		}
	}


	bool ExtraData::Save(json& a_save)
	{
		try {
			json xDataSave;
			for (auto& xData : _extraList) {
				if (!xData.second->Save(xDataSave)) {
					return false;
				}
				a_save[std::to_string(xData.second->GetType())] = xDataSave;
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	bool ExtraData::Load(json& a_load)
	{
		try {
			GetOrCreate_Result result;
			for (auto& xData : a_load.items()) {
				result = GetOrCreate(static_cast<ExtraDataType>(std::stoi(xData.key())), false);
				if (result.second) {
					result.first->second->Load(xData.value());
				} else {
					_ERROR("[ERROR] Failed to insert extra data block:\n%s", xData.value().dump(4));
					return false;
				}
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] %s", e.what());
			return false;
		}

		return true;
	}


	int ExtraData::Compare(const ExtraData& a_rhs) const
	{
		int result = _extraList.size() - a_rhs._extraList.size();
		if (result != 0) {
			return result < 0 ? -1 : 1;
		} else {
			for (auto lIt = _extraList.begin(), rIt = a_rhs._extraList.begin(); lIt != _extraList.end(); ++lIt, ++rIt) {
				result = lIt->second->Compare(*rIt->second);
				if (result != 0) {
					return result < 0 ? -1 : 1;
				}
			}
		}
		return 0;
	}


	bool ExtraData::operator<(const ExtraData& a_rhs) const
	{
		return Compare(a_rhs) < 0;
	}


	void ExtraData::Parse(BaseExtraList* a_baseExtraList)
	{
		GetOrCreate_Result result;
		BSExtraData* data = a_baseExtraList->m_data;
		while (data) {
			result = GetOrCreate(static_cast<ExtraDataType>(data->GetType()), true);
			if (result.second) {
				result.first->second->Set(a_baseExtraList->GetByType(static_cast<ExtraDataType>(data->GetType())));
			}
			data = data->next;
		}
	}


	ExtraData::GetOrCreate_Result ExtraData::GetOrCreate(ExtraDataType a_type, bool a_tryToGet)
	{
		if (a_tryToGet) {
			auto it = _extraList.find(a_type);
			if (it != _extraList.end()) {
				return { it, true };
			}
		}

		GetOrCreate_Result result;
		IExtraDataType* extraData = 0;
		bool make;
		switch (a_type) {
		case ExtraDataType::kExtraData_Health:
			extraData = new ExtraHealth();
			make = true;
			break;
		case ExtraDataType::kExtraData_Poison:
			extraData = new ExtraPoison();
			make = true;
			break;
		case ExtraDataType::kExtraData_TextDisplayData:
			extraData = new ExtraTextDisplayData();
			make = true;
			break;
		case ExtraDataType::kExtraData_Enchantment:
			extraData = new ExtraEnchantment();
			make = true;
			break;
		default:
			_WARNING("[WARNING] Encountered type with no case (%u)", a_type);
			make = false;
			break;
		}

		if (make) {
			result = _extraList.insert(std::make_pair(extraData->GetType(), extraData));
		} else {
			result = { _extraList.end(), false };
		}

		if (!result.second) {
			delete extraData;
		}
		return result;
	}
}
