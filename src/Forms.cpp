#include "Forms.h"


bool Form::IsLightMod() const
{
	return _isLightMod;
}


UInt32 Form::RawFormID() const
{
	return _rawFormID;
}


UInt32 Form::GetLoadedFormID()
{
	if (_loadedFormID == kInvalid) {
		TESForm* form = GetForm<TESForm>(_rawFormID, _pluginName.c_str(), _isLightMod);
		_loadedFormID = form ? form->formID : kInvalid;
	}

	return _loadedFormID;
}


const char* Form::PluginName() const
{
	return _pluginName.c_str();
}


Form::Form(UInt32 a_rawFormID, std::string a_pluginName, bool a_isLightMod) :
	_rawFormID(a_rawFormID),
	_loadedFormID(kInvalid),
	_pluginName(a_pluginName),
	_isLightMod(a_isLightMod)
{}


void Form::ClearLoadedFormID()
{
	_loadedFormID = kInvalid;
}


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


Form* FormFactory::CreateForm(UInt32 a_rawFormID, std::string a_pluginName, bool a_isLightMod)
{
	Form* form = new Form(a_rawFormID, a_pluginName, a_isLightMod);
	_forms.push_back(form);
	return form;
}


void FormFactory::ClearLoadedFormIDs()
{
	for (auto& form : _forms) {
		form->ClearLoadedFormID();
	}
}


FormFactory::FormFactory()
{}


FormFactory::~FormFactory()
{
	for (auto& form : _forms) {
		delete form;
	}
}


FormFactory* FormFactory::_singleton = 0;
