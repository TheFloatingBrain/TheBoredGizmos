#include <LibBored/Config.hpp>
#ifndef BORED_GAMES_LIB_BORED_ECS_HEADER_HPP_INCLUDE_GUARD
#define BORED_GAMES_LIB_BORED_ECS_HEADER_HPP_INCLUDE_GUARD
namespace Bored
{
	template<typename T>
	using CleanType = std::remove_pointer_t<std::remove_reference_t<std::decay_t<T>>>;

	using TypeHash = uintptr_t;

	using EntityID = TypeHash;

	enum class GAME_NAME { GAME_NAME };

	template<typename Element_PT, size_t ComponentID = 0, typename Game_PT = GAME_NAME >
	inline std::flat_map<EntityID, Element_PT> components; /* This could be put into an class that
	would be instantiated, but for the scale of these games this is probably fine. doing otherwise
	would require dynamic shenanigans, and unessisary complexity. */


	template<typename>
	constexpr inline const TypeHash typeHashTarget = 0;

	template<typename T>
	constexpr inline const TypeHash* typeHash = &typeHashTarget<T>;

	template<typename T>
	inline const TypeHash runtimeTypeHash = static_cast<TypeHash>(typeHash);

	constexpr inline const TypeHash rollingTypeHashPrime = 768614336404564651;
	const TypeHash power = 1;
	const TypeHash tableSize = std::numeric_limits<uintptr_t>::max();

	template<typename T, typename... Ts>
	constexpr inline TypeHash rollingTypeHashImpl(
		const TypeHash previousHash, 
		const TypeHash prime, 
		const TypeHash power, 
		const TypeHash tableSize
	)
	{
		const TypeHash currentHash = (previousHash + runtimeTypeHash<T> * power) % tableSize;
		return rollingTypeHash<Ts...>(currentHash, prime, (power * prime) % tableSize, tableSize);
	}

	template<typename... Ts>
	constexpr inline TypeHash rollingTypeHash(
		const TypeHash prime = rollingTypeHashPrime,
		const TypeHash power,
		const TypeHash tableSize
	) { return rollingTypeHashImpl(0, prime, power, tableSize); }

	template<typename T> struct ComponentDeleter;

	template<> struct ComponentDeleter<std::nullptr_t>
	{
		std::unique_ptr<ComponentDeleter<std::nullptr_t>> next;
		constexpr virtual void deleteComponent(EntityID id) const = 0;
		constexpr virtual TypeHash typeHash() const {
			return runtimeTypeHash<std::nullptr_t>();
		}
		void append(std::unique_ptr<ComponentDeleter<std::nullptr_t>>&& next_)
		{
			if(next == nullptr)
				next = std::move(next_);
			else
				next->append(std::move(next_));
		}
	};

	using BaseComponentDeleter = ComponentDeleter<std::nullptr_t>;

	using RuntimeComponentDeleter = std::unique_ptr<BaseTypeNode>;

	template<typename... Ts> struct TypeList {};

	template<typename T>
	struct ComponentDeleter : public BaseComponentDeleter
	{
		constexpr virtual void deleteComponent(EntityID id) const final {
			components<T, GAME_NAME>.erase(components<T, GAME_NAME>.begin() + id);
			if(next != nullptr)
				next->deleteComponent(id);
		}
		constexpr virtual TypeHash typeHash() const final {
			return runtimeTypeHash<T>();
		}
	};

	template<auto Game_PC>
	struct Registry
	{
		template<typename... Component_PTs>
		const EntityID addEntity(Component_PTs&&... components)
		{
			EntityID id = 0;
			if(recentlyDeleted.size() > 0) {
				id = recentlyDeleted.back();
				recentlyDeleted.pop();
			}
		}
		void addComponent
		protected: 
			std::vector<bool> alive;
			std::flat_map<TypeHash, RuntimeComponentDeleter> componentDeleters;
			std::queue<EntityID> recentlyDeleted;
	};
}
#endif // BORED_GAMES_LIB_BORED_ECS_HEADER_HPP_INCLUDE_GUARD

