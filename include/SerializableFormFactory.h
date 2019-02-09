#pragma once

#undef GetForm

#include "GameForms.h"  // FormType

#include <memory>  // shared_ptr

#include "Types.h"  // SerializableFormPtr


namespace iEquip
{
	namespace Serializable
	{
		class FormFactory
		{
		public:
			static FormFactory*	GetSingleton();
			static void			Free();
			SerializableFormPtr	GetForm(FormType a_type, bool a_useHandle);
			SerializableFormPtr	GetForm(UInt32 a_type, bool a_useHandle);

		private:
			FormFactory();
			FormFactory(const FormFactory&) = delete;
			FormFactory(FormFactory&&) = delete;
			~FormFactory();

			FormFactory&	operator=(const FormFactory&) = delete;
			FormFactory&	operator=(FormFactory&&) = delete;


			static FormFactory* _singleton;
		};
	}
}
