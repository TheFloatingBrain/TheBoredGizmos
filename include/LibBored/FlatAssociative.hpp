#include <LibBored/Config.hpp>
#ifndef BORED_GAMES_LIB_BORED_FLAT_ASSOCIATIVE_HEADER_HPP_INCLUDE_GUARD
#define BORED_GAMES_LIB_BORED_FLAT_ASSOCIATIVE_HEADER_HPP_INCLUDE_GUARD
#include <atomic>
namespace Bored
{
	template<
		typename Element_PT, 
		std::unsigned_integral Key_PT = size_t, 
		template<typename> typename Container_PTT = std::vector
	>
	struct ModMap
	{
		using Element = Element_PT;
		using Key = Key_PT;
		using Container = Container_PTT<Element_PT>;
		void remove(Key_PT position) {
		}
		void at(Key_PT position) {

		}
		Key_PT insert(Key_PT position, Element_PT&& value)
		{
			while(locked.compare_exchange_strong(false, true));
			const auto modPosition = position % container.size();
			if(alive[modPosition] == true)
			{
				container.push_back(std::move(value));
				alive.push_back(true);
				return (alive.size() - 1);
			}
			container[modPosition] = std::move(element);
			alive[modPosition] = true;
			locked = false;
			return modPosition;
		}
		protected: 
			Container_PTT<Element_PT> container;
			Container_PTT<bool> alive;
			std::atomic<bool> locked = false;
	};

}
#endif // BORED_GAMES_LIB_BORED_FLAT_ASSOCIATIVE_HEADER_HPP_INCLUDE_GUARD
