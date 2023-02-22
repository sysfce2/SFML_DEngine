#include "pch.h"
#include "CppUnitTest.h"
#include "Utility/SerializeEntity.h"
#include "Utility/TestEntities.h"
#include "Factory/EntityFactory.h"
#include "Components/Components.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace drft;


namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			// ToString specializations here //

			template<>
			static std::wstring inline ToString(const entt::entity& entity)
			{
				std::uint32_t id = static_cast<std::uint32_t>(entity);

				std::string str("Entity ID: ");
				str += (std::uint32_t)id;
				std::wstring wstr;
				std::copy(str.begin(), str.end(), std::back_inserter(wstr));

				return wstr;
			}


		}
	}
}



namespace UnitTests
{
	TEST_CLASS(AppendToRegistryTests)
	{

	public:

		TEST_METHOD(TwoRegistries_AppendOneOntoOther_ContainsAllCopies)
		{
			EntityFactory factory;
			entt::registry reg1;
			entt::registry reg2;
			factory.loadPrototypes("prototypes.json", reg1);
			
			for (int i = 0; i < 20; ++i)
			{
				factory.build("tree", reg1);
				factory.build("NPC", reg1);
			}

			util::transfer(reg1, reg2);

			auto infoView = reg2.view<component::Info>();
			for (auto [entity, info] : infoView.each())
			{
				auto name = info.name;
				auto desp = info.description;
			}

			Assert::AreEqual(reg1.size(), reg2.size());
		}
	};
}
