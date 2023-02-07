#include "pch.h"
#include "CppUnitTest.h"
#include "Factory/EntityFactory.h"
#include "Components/Components.h"


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

		}
	}
}


namespace UnitTests
{
	TEST_CLASS(EntityFactoryTests)
	{
	public:

		TEST_METHOD(TwoEntities_WhenCopyEntityOne_EntityTwoHasSameComponents)
		{
			entt::registry registry;
			drft::EntityFactory factory(registry);
			auto e1 = registry.create();
			registry.emplace<component::Info>(e1, "Entity1", "", "");
			registry.emplace<component::Position>(e1, sf::Vector2f{ 0.f,0.f }, 0);
			registry.emplace<component::Render>(e1, 1u, sf::Color::Blue);
			auto e2 = registry.create();

			factory.copyEntity(e1, e2);

			Assert::IsTrue(registry.all_of<component::Info,component::Position, component::Render>(e2));
		}

		TEST_METHOD(TwoEntities_WhenCopyEntityOne_EntityTwoHasSameComponentValues)
		{
			entt::registry registry;
			drft::EntityFactory factory(registry);
			auto e1 = registry.create();
			registry.emplace<component::Info>(e1, "Entity1", "", "");
			registry.emplace<component::Position>(e1, sf::Vector2f{ 0.f,0.f }, 0);
			registry.emplace<component::Render>(e1, 1u, sf::Color::Blue);
			auto e2 = registry.create();

			factory.copyEntity(e1, e2);

			auto& info1 = registry.get<component::Info>(e1);
			auto& info2 = registry.get<component::Info>(e2);
			auto& pos1 = registry.get<component::Position>(e1);
			auto& pos2 = registry.get<component::Position>(e2);
			auto& ren1 = registry.get<component::Render>(e1);
			auto& ren2 = registry.get<component::Render>(e2);

			Assert::AreNotSame(info1, info2);
			Assert::AreEqual(info1.name, info2.name);
			Assert::AreEqual(pos1.position, pos2.position);
			Assert::AreEqual(ren1.sprite, ren2.sprite);
		}

		TEST_METHOD(TwoEntities_WhenCopyEntityOneAndAlterEntityTwosValues_EntityTwoHasDifferentComponentValues)
		{
			entt::registry registry;
			drft::EntityFactory factory(registry);
			auto e1 = registry.create();
			registry.emplace<component::Info>(e1, "Entity1", "", "");
			registry.emplace<component::Position>(e1, sf::Vector2f{ 0.f,0.f }, 0);
			registry.emplace<component::Render>(e1, 1u, sf::Color::Blue);
			auto e2 = registry.create();

			factory.copyEntity(e1, e2);

			auto& info1 = registry.get<component::Info>(e1);
			auto& info2 = registry.get<component::Info>(e2);
			auto& pos1 = registry.get<component::Position>(e1);
			auto& pos2 = registry.get<component::Position>(e2);
			auto& ren1 = registry.get<component::Render>(e1);
			auto& ren2 = registry.get<component::Render>(e2);

			info2.name = "Entity2";
			pos2.position = { 10.0f, 10.0f };
			ren2.sprite = 5u;

			Assert::AreNotSame(info1, info2);
			Assert::AreNotEqual(info1.name, info2.name);
			Assert::AreNotEqual(pos1.position, pos2.position);
			Assert::AreNotEqual(ren1.sprite, ren2.sprite);
		}

		TEST_METHOD(NewFactory_LoadJSONFile_ReturnsTrue)
		{
			entt::registry registry;
			drft::EntityFactory factory(registry);

			Logger::WriteMessage(std::filesystem::current_path().c_str());

			auto success = factory.loadPrototypes("prototypes.json");

			Assert::IsTrue(success);
		}
	};

}