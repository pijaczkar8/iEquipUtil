#include "SerializableFormFactory.h"

#include "GameForms.h"  // FormType

#include <memory>  // make_shared

#include "Armor.h"  // Armor


namespace Forms
{
	FormFactory* FormFactory::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new FormFactory();
		}
		return _singleton;
	}


	void FormFactory::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	SerializableFormPtr FormFactory::GetForm(FormType a_type, bool a_useHandle)
	{
		SerializableFormPtr ptr;
		switch (a_type) {
		case FormType::kFormType_Armor:
			ptr = std::make_shared<Armor>(a_useHandle);
			break;
		default:
			_WARNING("[WARNING] Encountered type with no case (%u)", a_type);
			break;
		}
		return ptr;
	}


	SerializableFormPtr FormFactory::GetForm(UInt32 a_type, bool a_useHandle)
	{
		return GetForm(static_cast<FormType>(a_type), a_useHandle);
	}


	FormFactory::FormFactory()
	{}


	FormFactory::~FormFactory()
	{}


	FormFactory* FormFactory::_singleton = 0;
}
