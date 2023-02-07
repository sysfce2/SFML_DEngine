#include "pch.h"
#include "CppUnitTest.h"
#include "Factory/EntityFactory.h"
#include "Components/Components.h"
#include "Components/Meta.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			// ToString specializations here //

			template<>
			static std::wstring inline ToString(const sf::Vector2f& vec)
			{

				std::string str("Vector: " + std::to_string(vec.x) + ", " + std::to_string(vec.y));
				std::wstring wstr;
				std::copy(str.begin(), str.end(), std::back_inserter(wstr));

				return wstr;
			}

			template<>
			static std::wstring inline ToString(const component::Info& info)
			{

				std::string str("Info: " + info.name);
				std::wstring wstr;
				std::copy(str.begin(), str.end(), std::back_inserter(wstr));

				return wstr;
			}

			template<>
			static std::wstring inline ToString(const sf::Color& color)
			{

				std::string str("Color: " + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.r));
				std::wstring wstr;
				std::copy(str.begin(), str.end(), std::back_inserter(wstr));

				return wstr;
			}

		}
	}
}


namespace UnitTests
{
	TEST_CLASS(EntityFactoryTests)
	{
	public:

		TEST_METHOD(NewFactory_LoadJSONFile_ReturnsTrue)
		{
			entt::registry registry;
			drft::EntityFactory factory;
			
			auto success = factory.loadPrototypes("prototypes.json", registry);

			Assert::IsTrue(success);
		}

		TEST_METHOD(NewFactory_LoadJSONFile_PrototypesMapPopulated)
		{
			entt::registry registry;
			drft::EntityFactory factory;

			factory.loadPrototypes("prototypes.json", registry);

			auto e = factory.build("Tree", registry);

			Assert::IsTrue(registry.all_of<component::Info>(e));
			Assert::IsTrue(registry.all_of<component::Position>(e));
			Assert::IsTrue(registry.all_of<component::Render>(e));
		}

		TEST_METHOD(NewFactory_BuildEntity_HasAllExpectedValues)
		{
			entt::registry registry;
			drft::EntityFactory factory;
			factory.loadPrototypes("prototypes.json", registry);
			auto e = factory.build("Tree", registry);

			auto pos = registry.get<component::Position>(e);
			auto info = registry.get<component::Info>(e);
			auto ren = registry.get<component::Render>(e);

			Assert::AreEqual(sf::Vector2f(10.f, 10.f), pos.position);
			Assert::AreEqual(2, pos.depth);
			Assert::AreEqual(std::string("Tree"), info.name);
			Assert::AreEqual(5u, ren.sprite);
			Assert::AreEqual(sf::Color(80, 100, 80), ren.color);
		}
	};

}