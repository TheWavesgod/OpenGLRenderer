#pragma once
#include <glm.hpp>
#include <list>
#include <functional>
#include "CollisionDetection.h"

template<typename T>
class Octree;

template<typename T>
struct OctreeEntry
{
	glm::vec3 pos;
	glm::vec3 size;
	T object;

	OctreeEntry(T obj, const glm::vec3& newPos, const glm::vec3& newSize) : pos(newPos), size(newSize)
	{
		object = obj;
	}
};

template<typename T>
class OctreeNode
{
public:
	using OctreeFunc = std::function<void(std::list<OctreeEntry<T>>&)>;
	friend class Octree<T>;
protected:
	OctreeNode() {}

	OctreeNode(glm::vec3 pos, glm::vec3 halfSize) : position(pos), halfSize(halfSize)
	{
		children = nullptr;
	}

	~OctreeNode()
	{
		delete[] children;
	}

	void Insert(T& object, const glm::vec3& objPos, const glm::vec3& objSize, int depthLeft, int maxSize)
	{
		if (!CollisionDetection::AABBTest(objPos, position, objSize, halfSize)) return;

		if (children == nullptr)
		{
			for (int i = 0; i < 4; ++i)
			{
				children[i]->Insert(object, objPos, objSize, depthLeft - 1, maxSize);
			}
		}
		else
		{
			contents.emplace_back(OctreeEntry<T>(object, objPos, objSize));
			if (contents.size() > maxSize && depthLeft > 0)
			{
				SplitNode();								// Current Node Full, Split Node
				for (const auto& entry : contents)
				{
					for (size_t i = 0; i < 8; ++i)
					{
						children[i].Insert(entry.obj, entry.pos, entry.size, depthLeft - 1, maxSize);
					}
				}
				contents.clear();
			}
		}
	}

	void SplitNode()
	{
		glm::vec3 newSize = halfSize * 0.5f;
		children = new OctreeNode<T>[8];
		children[0] = new OctreeNode<T>(position + glm::vec3(newSize.x, newSize.y, newSize.z), newSize);
		children[1] = new OctreeNode<T>(position + glm::vec3(newSize.x, newSize.y, -newSize.z), newSize);
		children[2] = new OctreeNode<T>(position + glm::vec3(newSize.x, -newSize.y, newSize.z), newSize);
		children[3] = new OctreeNode<T>(position + glm::vec3(newSize.x, -newSize.y, -newSize.z), newSize);
		children[4] = new OctreeNode<T>(position + glm::vec3(-newSize.x, newSize.y, newSize.z), newSize);
		children[5] = new OctreeNode<T>(position + glm::vec3(-newSize.x, newSize.y, -newSize.z), newSize);
		children[6] = new OctreeNode<T>(position + glm::vec3(-newSize.x, -newSize.y, newSize.z), newSize);
		children[7] = new OctreeNode<T>(position + glm::vec3(-newSize.x, -newSize.y, -newSize.z), newSize);
	}

	void OperateOnContents(OctreeFunc& func)
	{
		if (children != nullptr)
		{
			for (auto& i : children)
			{
				i.OperateOnContents(func);
			}
		}
		else
		{
			if (!contents.empty())
			{
				func(contents);
			}
		}
	}

protected:
	std::list<OctreeEntry<T>> contents;

	glm::vec3 position;
	glm::vec3 halfSize;

	OctreeNode<T>* children;
};

template<typename T>
class Octree
{
public:
	Octree(glm::vec3 size, int maxDepth = 6, int maxSize = 5) : maxDepth(maxDepth), maxSize(maxSize)
	{
		root = OctreeNode<T>(glm::vec3(0.0f), size);
	}

	~Octree() {}

	void Insert(T object, const glm::vec3& pos, const glm::vec3& size)
	{
		root.Insert(object, pos, size, maxDepth, maxSize);
	}

	void OperateOnContents(OctreeNode<T>::OctreeFunc func)
	{
		root.OperateOnContents(func);
	}

protected:
	OctreeNode<T> root;
	int maxDepth;
	int maxSize;
};