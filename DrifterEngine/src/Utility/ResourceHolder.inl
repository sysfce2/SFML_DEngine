#include "ResourceHolder.h"
#include <stdexcept>
#include <utility>
#include <cassert>

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	std::unique_ptr<Resource> resource(std::make_unique<Resource>());
	if (!resource->loadFromFile(filename))
	{
		throw std::runtime_error("TextureHolder::load - Failed to load " + filename);
	}
	auto inserted = _resourceMap.insert({ id, std::move(resource) });
	assert(inserted.second);
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename,
												const Parameter& param)
{
	std::unique_ptr<Resource> resource(std::make_unique<Resource>());
	if (!resource->loadFromFile(filename, param))
	{
		throw std::runtime_error("TextureHolder::load - Failed to load " + filename);
	}
	auto inserted = _resourceMap.insert({ id, std::move(resource) });
	assert(inserted.second);
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = _resourceMap.find(id);
	assert(found != _resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = _resourceMap.find(id);
	assert(found != _resourceMap.end());

	return *found->second;
}

