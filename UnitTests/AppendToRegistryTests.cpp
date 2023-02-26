#include "pch.h"
#include "CppUnitTest.h"
#include "Snapshot/EnTTSnapshot.h"
#include "Utility/TestEntities.h"
#include "Utility/CopyEntity.h"
#include "Factory/EntityFactory.h"
#include "Components/Meta.h"
#include "Components/Components.h"
#include "Components/Serialize.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace drft;
using namespace entt::literals;

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
	TEST_CLASS(SerializationTests)
	{

	public:

		TEST_METHOD(EmplaceComponent_CheckIfHandleContainsComponentUsingMetaFunc_HandleDoesContain)
		{
			using namespace snapshot;

			reflectComponent<component::Position, POSITION_STRING>();
			auto reg = entt::registry{};
			auto h = entt::handle{ reg, reg.create() };
			h.emplace<component::Position>(sf::Vector2f(12.f, 12.f), 4 );
			auto const_h = entt::const_handle{ h };

			auto reflComp = ComponentReflection{ Reflection{ POSITION_STRING } };
			auto& refl = reflComp.reflection();
			auto type = refl.type();
			auto func = type.func("contains"_hs);
			auto containsMeta = func.invoke(type, const_h);

			bool contains = containsMeta.cast<bool>();
			
			Assert::AreEqual(true, contains);

		}
		TEST_METHOD(NonEmptyRegistry_SerializeToCerealBinary_CanBeDeserialized)
		{
			using namespace snapshot;

			EntityFactory factory;
			entt::registry reg1;
			entt::registry reg2;
			std::stringstream ss;
			cereal::BinaryOutputArchive output(ss);
			factory.loadPrototypes("prototypes.json", reg1);

			for (int i = 0; i < 1000; ++i)
			{
				factory.build("Tree", reg1);
				factory.build("NPC", reg1);
			}

			Snapshot::save({ output }, reg1);
			cereal::BinaryInputArchive input(ss);
			SnapshotLoader::load({ input }, reg2);
			
			Assert::AreEqual(reg1.size(), reg2.size());

		}
		TEST_METHOD(TwoRegistriesOneFullOneEmpty_AppendFullToEmpty_BothAreSameSize)
		{
			using namespace snapshot;

			EntityFactory factory;
			entt::registry reg1;
			entt::registry reg2;
			std::stringstream ss;
			factory.loadPrototypes("prototypes.json", reg1);

			for (int i = 0; i < 10000; ++i)
			{
				factory.build("Tree", reg1);
				factory.build("NPC", reg1);
			}

			reg1.each([&](auto entity)
				{
					auto e = reg2.create();
					util::copyEntity(e, entity, reg2, reg1);
				}
			);

			Assert::AreEqual(reg1.size(), reg2.size());

		}
	};
}
