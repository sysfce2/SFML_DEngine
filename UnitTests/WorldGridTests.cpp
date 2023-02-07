#include "pch.h"
#include "CppUnitTest.h"
#include "Spatial/WorldGrid.h"


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
	TEST_CLASS(WorldGridTests)
	{

	public:

		TEST_METHOD(EmptyWorldGrid_OnCreation_DoesNotThrow)
		{
			spatial::WorldGrid world;
		}

		TEST_METHOD(EmptyWorldGrid_OnAccessPositiveCoordinates_ReturnsEmptyVector)
		{
			spatial::WorldGrid world;

			auto r1 = world.entitiesAt({ 100,100 }, spatial::Layer::Actor);
			auto r2 = world.entitiesAt({ 0, 0 }, spatial::Layer::Item);
			auto r3 = world.entitiesAt({ 1000, 1000 }, spatial::Layer::Prop);

			Assert::IsTrue(r1.empty());
			Assert::IsTrue(r2.empty());
			Assert::IsTrue(r3.empty());
		}

		TEST_METHOD(EmptyWorldGrid_OnAccessNegativeCoordinates_ReturnsEmptyVector)
		{
			spatial::WorldGrid world;

			auto r1 = world.entitiesAt({ -100,-99 }, spatial::Layer::Actor);
			auto r2 = world.entitiesAt({ 0, 0 }, spatial::Layer::Item);
			auto r3 = world.entitiesAt({ -1000, -999 }, spatial::Layer::Prop);

			Assert::IsTrue(r1.empty());
			Assert::IsTrue(r2.empty());
			Assert::IsTrue(r3.empty());
		}

		TEST_METHOD(EmptyWorldGrid_OnRemoveNullEntity_ReturnsNullEntity)
		{
			spatial::WorldGrid world;

			auto e = world.removeEntity(entt::null, { 100, 99 }, spatial::Layer::Actor);

			Assert::IsTrue(e == entt::null);
		}

		TEST_METHOD(WorldGrid_OnPlacingEntity_CanFindUsingEntitiesAtMethod)
		{
			spatial::WorldGrid world;
			entt::registry registry;
			auto entity = registry.create();

			world.placeEntity(entity, { 64,64 }, spatial::Layer::Actor);
			auto elist = world.entitiesAt({ 64,64 }, spatial::Layer::Actor);

			bool listhasEntity = std::count(elist.begin(), elist.end(), entity) > 0;

			Assert::IsTrue(listhasEntity);
		}

		TEST_METHOD(WorldGrid_OnPlaceEntity_AbleToRemoveEntity)
		{
			spatial::WorldGrid world;
			entt::registry registry;
			auto entity1 = registry.create();
			auto entity2 = registry.create();
			auto entity3 = registry.create();

			world.placeEntity(entity1, { 1000,1000 }, spatial::Layer::Actor);
			world.placeEntity(entity2, { -1000, -1000 }, spatial::Layer::Item);
			world.placeEntity(entity3, { 0, 0 }, spatial::Layer::Prop);

			auto foundEntity1 = world.removeEntity(entity1, { 1000, 1000 }, spatial::Layer::Actor);
			auto foundEntity2 = world.removeEntity(entity2, { -1000, -1000 }, spatial::Layer::Item);
			auto foundEntity3 = world.removeEntity(entity3, { 0, 0 }, spatial::Layer::Prop);

			Assert::AreEqual(entity1, foundEntity1);
			Assert::AreEqual(entity2, foundEntity2);
			Assert::AreEqual(entity3, foundEntity3);
		}

		TEST_METHOD(WorldGrid_OnMoveEntity_InNewLocationNotInOldLocation)
		{
			spatial::WorldGrid world;
			entt::registry registry;
			auto entity = registry.create();
			sf::Vector2i position1 = { -125, 25 };
			sf::Vector2i position2 = { 25, -125 };

			world.placeEntity(entity, position1, spatial::Layer::Actor);
			world.moveEntity(entity, position1, position2, spatial::Layer::Actor);

			auto elist1 = world.entitiesAt(position1, spatial::Layer::Actor);
			auto elist2 = world.entitiesAt(position2, spatial::Layer::Actor);
			bool list1hasEntity = std::count(elist1.begin(), elist1.end(), entity) > 0;
			bool list2hasEntity = std::count(elist2.begin(), elist2.end(), entity) > 0;

			Assert::IsFalse(list1hasEntity);
			Assert::IsTrue(list2hasEntity);
		}
	};
}