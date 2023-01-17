#pragma once

namespace drft
{

	class Grid
	{
	private:
		class SubGrid
		{
		private:
			class Cell
			{
			public:
				void placeEntity(const unsigned int id, int depth)
				{
					_entities[depth].push_back(id);
				}
				void removeEntity(const unsigned int id, int depth)
				{
					assert(_entities.contains(depth));
					auto &vec = _entities[depth];
					auto it = std::find(vec.begin(), vec.end(), id);
					if (it == vec.end()) return;
					vec.erase(it);
					if (vec.empty())
					{
						_entities.erase(depth);
					}
				}
				std::vector<unsigned int>& getEntities(int depth)
				{
					assert(_entities.contains(depth));

					return _entities.at(depth);
				}
				bool empty()
				{
					return _entities.empty();
				}

			private:
				std::map<int, std::vector<unsigned int> > _entities; // map<depth, entityList>
			};

		public:
			SubGrid(sf::Vector2i worldCoordinate, sf::Vector2i dimensions)
				: _worldCoordinate(worldCoordinate), _dimensions(dimensions)
			{
				_cells.resize(_dimensions.x * _dimensions.y);
			}
			void placeEntity(const unsigned int id, sf::Vector2i localPosition, int depth)
			{
				assert(localPosition.x < _dimensions.x);
				assert(localPosition.y < _dimensions.y);

				_cells.at((localPosition.y * _dimensions.x) + localPosition.x).placeEntity(id, depth);
			}
			void removeEntity(const unsigned int id, sf::Vector2i localPosition, int depth)
			{
				assert(localPosition.x < _dimensions.x);
				assert(localPosition.y < _dimensions.y);

				_cells.at((localPosition.y * _dimensions.x) + localPosition.x).removeEntity(id, depth);
			}
			std::vector<unsigned int>& getEntities(sf::Vector2i localPosition, int depth)
			{
				assert(localPosition.x < _dimensions.x);
				assert(localPosition.y < _dimensions.y);

				return _cells.at((localPosition.y * _dimensions.x) + localPosition.x).getEntities(depth);
			}
			sf::Vector2i getCoordinates()
			{
				return _worldCoordinate;
			}

		private:
			sf::Vector2i _worldCoordinate;
			sf::Vector2i _dimensions;
			std::vector<Cell> _cells;
		};

	public:
		// Creates a x * y Grid, containing (x*y) sub grids with x' * y' cells
		
	};

} // namespace drft



