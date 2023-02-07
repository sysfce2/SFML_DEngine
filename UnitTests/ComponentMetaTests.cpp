#include "pch.h"
#include "CppUnitTest.h"
#include "Components/Components.h"
#include "Components/Meta.h"
#include "../deps/EnTT/core/hashed_string.hpp"


namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			// ToString specializations here //

			template<>
			static std::wstring inline ToString(const type_info& typeInfo)
			{

				std::string str("TypeInfo: " + std::string(typeInfo.name()));
				std::wstring wstr;
				std::copy(str.begin(), str.end(), std::back_inserter(wstr));

				return wstr;
			}

			template<>
			static std::wstring inline ToString(const sf::Vector2f& vec)
			{

				std::string str("Vector: " + std::to_string(vec.x) + ", " + std::to_string(vec.y));
				std::wstring wstr;
				std::copy(str.begin(), str.end(), std::back_inserter(wstr));

				return wstr;
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(ComponentMetaTests)
	{
	public:

		TEST_METHOD(MetaInitialized_GetComponentFromString_AllDataCastsToProperType)
		{
			using namespace entt::literals;
			component::Meta::initialize();
			entt::registry registry;
			auto entity = registry.create();

			auto metaPos = entt::resolve("Position"_hs);
			auto pos = metaPos.construct(entt::forward_as_meta(registry), entity);
			pos.type().data("position"_hs).set(pos, sf::Vector2f(10.f, 10.f));
			pos.type().data("depth"_hs).set(pos, 11);

			auto actualPos = registry.get<component::Position>(entity);

			Assert::IsTrue(registry.all_of<component::Position>(entity));
			Assert::AreEqual(sf::Vector2f(10.f, 10.f), actualPos.position);
			Assert::AreEqual(11, actualPos.depth);
		}
	};
}