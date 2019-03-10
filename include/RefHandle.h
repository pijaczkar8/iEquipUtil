#pragma once

#include "ISerializableData.h"  // ISerializableData


namespace Forms
{
	class ISerializableForm;


	class RefHandle : public ISerializableData
	{
	public:
		RefHandle() = delete;
		RefHandle(bool a_isActive, ISerializableForm* a_form);
		RefHandle(const RefHandle&) = default;
		RefHandle(RefHandle&&) = default;
		~RefHandle();

		virtual const char*	ClassName() const override;
		virtual void		Clear() override;
		virtual bool		Save(json& a_save) override;
		virtual bool		Load(json& a_load) override;

		UInt32				GetHandle() const;
		bool				IsActive() const;
		void				AcquireHandle();

	protected:
		void				FreeHandle();


		ISerializableForm*	_form;
		UInt32				_handle;
		bool				_isActive;
	};
}
