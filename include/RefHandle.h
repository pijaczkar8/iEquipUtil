#pragma once


namespace iEquip
{
	class ISerializableForm;


	class RefHandle
	{
	public:
		RefHandle() = delete;
		explicit RefHandle(ISerializableForm* a_form);
		~RefHandle();

		UInt32	GetHandle() const;

	protected:
		bool	_isActive;
		UInt32	_handle;
	};
}
